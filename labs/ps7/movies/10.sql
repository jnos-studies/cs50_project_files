SELECT name
FROM people
WHERE id IN (
    SELECT DISTINCT(people.id)
    FROM directors
    INNER JOIN people
    ON directors.person_id = people.id
    LEFT JOIN movies
    ON directors.movie_id = movies.id
    INNER JOIN ratings
    ON movies.id = ratings.movie_id
    AND ratings.rating NOT NULL
    AND ratings.rating >= 9.0
);