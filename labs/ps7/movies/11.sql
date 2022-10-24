SELECT movies.title as mr_chads_success_meter
FROM movies
INNER JOIN stars
ON stars.movie_id = movies.id
INNER JOIN ratings
ON ratings.movie_id = movies.id
INNER JOIN people
ON people.id = stars.person_id
WHERE people.name = "Chadwick Boseman"
ORDER BY ratings.rating DESC LIMIT 5;