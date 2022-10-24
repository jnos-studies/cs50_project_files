SELECT people.name
FROM people
INNER JOIN stars
ON stars.person_id = people.id
WHERE stars.movie_id = (SELECT id FROM movies WHERE title = "Toy Story");