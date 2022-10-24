SELECT name AS Actors_with_Bacon
FROM people
WHERE people.id
IN (
    SELECT stars.person_id
    FROM stars
    WHERE stars.movie_id IN (
        SELECT stars.movie_id
        FROM people
        INNER JOIN stars
        ON people.id = stars.person_id
        WHERE people.name = "Kevin Bacon" AND people.birth = 1958
    )
) AND people.name IS NOT "Kevin Bacon"