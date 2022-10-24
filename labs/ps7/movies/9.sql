SELECT name
FROM people
WHERE id
IN (
    SELECT DISTINCT(people.id)
    FROM stars
    INNER JOIN people
    ON stars.person_id = people.id
    LEFT JOIN movies
    ON stars.movie_id = movies.id
    WHERE movies.year = 2004
)
ORDER BY birth;