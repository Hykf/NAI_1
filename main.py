import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
from imdb import Cinemagoer
import webbrowser


def get_user_preferences():
    preferences = {}
    print("Enter your preferences (movie and rating)!")
    print("Type 'end' to finish entering your preferences.")

    while True:
        movie = input("Enter movie name: ")
        if movie.lower() == 'end':
            break

        try:
            score = float(input(f"Enter rating for the movie '{movie}' (1 - 10): "))
            score = max(1.0, min(10.0, score))
            normalized_score = (score - 1.0) / 9.0
            preferences[movie] = normalized_score
            print("")
        except ValueError:
            print("Invalid value. Please enter a number between 1 and 10.")

    return preferences


def generate_movie_html(movie_titles, output_file="movies.html"):
    html_content = """
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Movies</title>
        <style>
        
            body {
                font-family: Arial, sans-serif;
                background-color: #f4f4f9;
                margin: 0;
                padding: 20px;
                display: flex;
                flex-direction: column;
                align-items: center;
            }
            .movie {
                border: 1px solid #ccc;
                border-radius: 5px;
                padding: 15px;
                margin: 10px 0;
                background-color: #fff;
                box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
                width: 80%;
                max-width: 600px;
                text-align: center;
            }
            .movie img {
                max-width: 100%;
                height: auto;
                display: block;
                margin: 0 auto 10px;
            }
            .movie h2 {
                margin: 0 0 10px;
                font-size: 1.5em;
                color: #333;
                text-align: center;
            }
            .movie p {
                margin: 5px 0;
                color: #555;
                text-align: center;
            }
            .movie .tags {
                margin: 5px 0;
                color: #555;
                text-align: center;
            }
            
        </style>
    </head>
    <body>
    
    <h1>Movie Details</h1>
    """

    ia = Cinemagoer()
    for title in movie_titles[0]:  # 0-GOOD  1-BAD
        try:
            movie = ia.search_movie(title[0])[0]

            movie_info = ia.get_movie(movie.movieID)
            movie_title = movie_info.get('title', 'Unknown Title')
            movie_genres = ", ".join(movie_info.get('genres', []))
            movie_plot = movie_info.get('plot', ['No description available'])[0]
            movie_poster = movie_info.get('cover url', '')

            html_content += f"""
            <div class="movie">
                <h2>{movie_title}</h2>
                <img src="{movie_poster}" alt="{movie_title}">
                <p class="tags">Genres: {movie_genres}</p>
                <p>{movie_plot}</p>
            </div>
            """
        except Exception as e:
            print(f"Error fetching data for '{title[0]}': {e}")

    html_content += """
    </body>
    </html>
    """

    with open(output_file, "w", encoding="utf-8") as file:
        file.write(html_content)

    print(f"HTML file generated: {output_file}")
    webbrowser.open("movies.html")


def recommend_based_on_users(preferences, top_n=5):
    new_user_vector = pd.DataFrame([preferences], columns=user_movie_matrix.columns).fillna(0)
    extended_matrix = pd.concat([user_movie_matrix, new_user_vector], ignore_index=True)
    extended_similarity = cosine_similarity(extended_matrix)
    new_user_similarities = extended_similarity[-1][:-1]
    similar_users = pd.Series(new_user_similarities, index=user_movie_matrix.index).sort_values(ascending=False)

    recommendations = {}
    for similar_user, similarity in similar_users.items():
        user_ratings = user_movie_matrix.loc[similar_user]

        for movie, score in user_ratings.items():
            if movie not in preferences and score > 0:
                recommendations[movie] = recommendations.get(movie, 0) + similarity * score

        if len(recommendations) >= top_n * 2:
            break

    sorted_recommendations = sorted(recommendations.items(), key=lambda x: x[1], reverse=True)

    top_n_best = sorted_recommendations[:top_n]

    top_n_worst = sorted_recommendations[-top_n:]

    return (top_n_best, top_n_worst)


####################

preferences = get_user_preferences()
data = pd.read_csv("movies.csv")
user_movie_matrix = data.pivot_table(index='Name', columns='Movie', values='Score')
user_movie_matrix.fillna(0, inplace=True)
user_similarity_matrix = cosine_similarity(user_movie_matrix)
user_similarity_df = pd.DataFrame(user_similarity_matrix, index=user_movie_matrix.index,
                                  columns=user_movie_matrix.index)

generate_movie_html(recommend_based_on_users(preferences))
