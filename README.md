# SPLFLIX
C++ program that simulates a new streaming service-SPLFLIX.SPLFLIX offers users two types of streaming content –movies and tv-episodes, which belong to a given tv-series. Like most streaming services we allow a creation of multiple users, while each user receives user-specific recommendations, according to his watch history.However, on SPLFLIX, unlike current streaming services, we give our users the ability to choose which recommendation algorithm to use,out of a number of algorithms we offer.Each user stores its own watch history, and can use a different recommendation algorithm. After the user chooses to watch a content, a recommendation can be given to the user for the next content.
At each point during the session, there is only one active user(while other users are considered as non-active), which means-only one user can watch SPLFLIX at any given point.The program will receive a configfile (json) as an input, which includes all the information about the available streaming content.
## Algorithms types:
- Length Recommender–This algorithm is based on the assumption that users prefer to watch content in a similar length. It will recommend the content whose length is closest to the average length of the content in the user's watch history, and which isn't in the user's watch history.(3-letter code–len) 
- RerunRecommender–This algorithm is intended for users who don't like new stuff.It will recommend content based on this user's watching history. Starting from the first watchable content, in a cyclic order.That is –the first watchable to be recommended is the first watchable watched by the user. Afterwards, if the last watchable that was recommended by the algorithm was at index i in the history, and the history is of length 𝑛 the next content to be recommended is at index (𝑖+1)𝑚𝑜𝑑𝑛.(3-letter code–rer) 
- Similar Genre–This algorithm will recommend content based on the most popular tag in the user's watch history.If a set of tags has the same popularity, the algorithm will use lexicographic order to pick between them. It will recommend a content which is tagged by the most popular tag in the user's history, which wasn't already watched by this user. If no such content exists, it will try with the second most popular tag, and so on.(3-letter code–gen) 
## User commands:
-Create User–Creates a new userin the current session.

Syntax: createuser<user_name> <recommendation_algorithm> Where the <recommendation_algorithm> is the 3-letter code for that algorithm.

Example: createuser john len
