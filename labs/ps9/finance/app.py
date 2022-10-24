import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Importing datetime to log timestamps
from datetime import datetime as date

import re

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get the current user's stocks
    user_stonks = db.execute("SELECT symbol, shares FROM stonks WHERE user_id = ? GROUP BY name;", session["user_id"])

    # Get the curent user's cash
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]["cash"]

    # Convert owned stocks to current price to be able to see the price change of current stocks
    portfolio = [{"name": lookup(s["symbol"])["name"], "symbol":s["symbol"], "shares":s["shares"], "price":lookup(
        s["symbol"])["price"], "total": lookup(s["symbol"])["price"] * s["shares"]} for s in user_stonks]

    # Sum the owned stocks added with the user's cash
    portfolio_sum = sum([s["total"] for s in portfolio]) + user_cash

    return render_template("index.html", user_cash=user_cash, stonks=portfolio, portfolio_sum=portfolio_sum)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        try:
            symbol, shares = request.form.get("symbol"), request.form.get("shares")

            # Get user's cash
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]["cash"]

            # lookup symbol from form
            look = lookup(symbol)

            # Get the number of shares the user has decided to buy
            if int(shares) < 1:
                return apology("Bad Request!", 400)

            shares_int = int(shares)


            bought = look["price"]

            # Check if user has enough funds
            if user_cash - bought * shares_int < 0:
                return apology("Not enough funds!", 400)

            # Check if the symbol does not match or if their is a Bad Request through hacks
            if symbol == None:
                return apology("Symbol did not match!", 400)

            # See if there are any of the same stock that the user already owns
            stonks_exist = db.execute(
                "SELECT stonks.symbol, stonks.shares FROM stonks WHERE user_id = ? AND stonks.symbol = ?;", session["user_id"], look["symbol"])

            # If the user ownes stocks, update the stonks database, otherwise, insert a new addition.
            if len(stonks_exist) > 0 and stonks_exist != None:
                bought_shares = db.execute("SELECT shares FROM stonks WHERE user_id = ? AND symbol = ?",
                                           session["user_id"], look["symbol"])[0]["shares"]
                db.execute("INSERT INTO history (user_id, symbol, shares, price, timestamp) VALUES(?, ?, ?, ?, ?);",
                           session["user_id"], look["symbol"], shares, look["price"], date.now())
                db.execute("UPDATE stonks SET shares = ? WHERE symbol = ? AND user_id = ?;",
                           bought_shares + shares_int, look["symbol"], session["user_id"])
                db.execute("UPDATE users SET cash = ? WHERE id = ?;", user_cash - bought, session["user_id"])
                flash("Purchase successful!")

                return redirect("/")
            else:
                db.execute("INSERT INTO history (user_id, symbol, shares, price, timestamp) VALUES(?, ?, ?, ?, ?);",
                           session["user_id"], look["symbol"], shares, look["price"], date.now())
                db.execute("INSERT INTO stonks (user_id, symbol, shares, name) VALUES(?, ?, ?, ?);",
                           session["user_id"], look["symbol"], shares, look["name"])
                db.execute("UPDATE users SET cash = ? WHERE id = ?;", user_cash - bought * shares_int, session["user_id"])
                flash("Purchase successful!")

                return redirect("/")
        except:
            return apology("Woops! Bad Request!", 400)

    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    try:

        # Store the user's current history if a variable and then render it.
        history = db.execute("SELECT symbol, shares, price, timestamp FROM history WHERE user_id = ?;", session["user_id"])

        return render_template("history.html", history=history)
    except:

        return apology("Bad Request!")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged in successully.")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If user requests via POST look up the symbol and render the name, symbol, and price if there is a match
    if request.method == "POST":
        symbol = lookup(request.form.get("symbol"))
        if symbol == None:
            return apology("Symbol does not match!")
        else:
            return render_template("quoted.html", name=symbol["name"], symbol=symbol["symbol"], price=symbol["price"])

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # If user registers VIA POST
    if request.method == "POST":

        username, password, confirmation = request.form.get(
            "username"), request.form.get("password"), request.form.get("confirmation")

        # Check if the password matches and if the user exits.
        if password == confirmation:
            user_exists = db.execute("SELECT * FROM users WHERE username = ?;", username)

            # Force users to have a password length longer than 8 characters and have at least 1 nonalphanumeric character
            if len(password) < 8 and len(re.findall("\W+", password)) < 1:
                return apology("Password must contain at least 8 characters and 1 nonalphanumeric character!")

            if len(user_exists) == 0:
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
                return redirect("/login")

            else:
                return apology("User already exists!")

        else:
            return apology("Passwords do not match!")

    else:
        flash("Welcome!")

        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_stonks = db.execute("SELECT symbol, shares FROM stonks WHERE user_id = ?", session["user_id"])
    if request.method == "POST":
        try:
            user_cash = db.execute("SELECT cash FROM users WHERE id = ?;", session["user_id"])[0]["cash"]
            symbol, selling = request.form.get("symbol"), int(request.form.get("shares"))

            # Convert returned list from user stonks into a dictionary for key value efficiency
            owned_stocks = {}
            for s in user_stonks:
                owned_stocks[s["symbol"]] = s["shares"]

            owned_shares = owned_stocks[symbol]
            selling_price = lookup(symbol)["price"] * float(selling)

            # Check that the selling of shares is less than or equal to owned shares and that selling owned shares does not go below 0
            if selling <= owned_shares and not owned_shares - selling < 0 and not selling > owned_shares:

                # Check that the user can't sell more stocks than they own
                if owned_shares - selling == 0:

                    # Delete from the stonks table if the user no longer owns shares in that stock
                    db.execute("DELETE FROM stonks WHERE symbol = ? AND user_id = ?;", symbol, session["user_id"])

                    return redirect("/")
                else:

                    # Otherwise update amount of owned stocks in stonks table
                    db.execute("UPDATE stonks SET shares = ? WHERE symbol = ? AND user_id = ?;",
                               owned_shares - selling, symbol, session["user_id"])

                # proceed to update user's cash and add to history
                db.execute("UPDATE users SET cash = ? WHERE id = ?;", user_cash + selling_price, session["user_id"])
                db.execute("INSERT INTO history (user_id, symbol, shares, price, timestamp) VALUES(?, ?, ?, ?, ?);",
                           session["user_id"], symbol, selling * -1, selling_price, date.now())
                flash("Sale successful!")

                return redirect("/")
            else:
                return apology("Not acceptable amount of shares!", 400)

        except:
            apology("Bad Request!", 400)

    return render_template("sell.html", stonks=user_stonks)
