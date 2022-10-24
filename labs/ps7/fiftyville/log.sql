-- Keep a log of any SQL queries you execute as you solve the mystery.

--Crime scene report table reports that the CS50 duck was stolen at 10:15am at Humphrey Street Bakery and that there where three witnesses who were interviewed
SELECT description, street
FROM crime_scene_reports
WHERE street = "Humphrey Street"
AND month = 7
AND day = 28;

-- Get all the transcripts which mention a bakery and fall on the date in question. This provides info on where to look, Ruth say the theif in the parking lot, Eugene saw the theif at the ATM
-- in Leggett Street withdrawing cash, and Raymond saw the theif on a call talking about taking the earliest flight out of fiftyville the day after the theft took place
SELECT id, name, transcript
FROM interviews
WHERE month = 7
AND day = 28
AND transcript LIKE "%bakery%";

--This provides a list of license plates which entered and exited the barkery parking lot around the time of the crime. next I need to match the liscence plate
SELECT id, license_plate, hour, minute
FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND hour = 10;

--columns used to figure out the culprite flights.hour, flights.minute,
--Boil all that down by comparing the flights of those who had a phone call less than a minute and showed up and the bakery on the day of crime.
SELECT people.name, people.phone_number, people.passport_number, people.license_plate, bank_accounts.account_number, airports.city, bakery_security_logs.hour, bakery_security_logs.minute, airports.id, airports.city, phone_calls.receiver, passengers.seat, flights.id
FROM atm_transactions
INNER JOIN bank_accounts
ON bank_accounts.account_number = atm_transactions.account_number
INNER JOIN people
ON people.id = bank_accounts.person_id
INNER JOIN bakery_security_logs
ON people.license_plate = bakery_security_logs.license_plate
INNER JOIN passengers
ON passengers.passport_number = people.passport_number
INNER JOIN flights
ON flights.id = passengers.flight_id
INNER JOIN airports
ON airports.id = flights.origin_airport_id
INNER JOIN phone_calls
ON phone_calls.caller = people.phone_number
WHERE atm_transactions.atm_location = "Leggett Street"
AND atm_transactions.transaction_type = "withdraw"
AND bakery_security_logs.month = 7
AND bakery_security_logs.day = 28
AND bakery_security_logs.hour BETWEEN 10 AND 10.5
AND flights.month = 7
AND flights.day = 29
AND phone_calls.duration < 60
AND phone_calls.month = 7
AND phone_calls.day = 28;

--Get the receivers name from a call by Bruce my suspect who matches the most with the datapoints and is the closest one to the time of the crime. Gives me the name robin
SELECT * FROM people WHERE phone_number = '(375) 555-8161';