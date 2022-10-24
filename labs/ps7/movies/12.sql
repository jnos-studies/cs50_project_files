SELECT movies.title as johnny_and_helena
FROM movies
INNER JOIN stars
ON stars.movie_id = movies.id
INNER JOIN people
ON people.id = stars.person_id
WHERE people.name = "Johnny Depp" OR people.name = "Helena Bonham Carter"
GROUP BY movies.title
HAVING COUNT(*) > 1;