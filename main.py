import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
from imdb import Cinemagoer
import webbrowser
import csv
import sys
import os


def validate_input():
    if len(sys.argv) == 1:
        # Case 0: No parameters - expect console input
        return runWithConsoleInput()
    elif len(sys.argv) == 2:
        # Case 1: One parameter - treat as .csv filename
        return runWithFileInput()
    else:
        raise ValueError("Output contains more than one parameter, which is not allowed.")

def get_user_preferences():
    preferences = {}
    print("Enter your preferences (movie and rating)!")
    print("Type 'end' to finish entering your preferences.")
    print("Please try to use original english titles of the movies. If the movies are from other country - e.g. polish ones - please keep the original title.")

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


def get_preferences_from_csv(file_path):
    """
    Reads user preferences from a CSV file if it exists in the same directory
    and is in CSV format. Validates the file and processes preferences.

    :param file_path: Path to the CSV file.
    :raises ValueError: If the file does not exist, is not a CSV, or has invalid content.
    :returns: A dictionary of preferences (movie names and normalized scores).
    """
    # Check if the file exists in the same directory
    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"The file '{file_path}' does not exist in the current directory.")

    # Check if the file has a .csv extension
    if not file_path.lower().endswith('.csv'):
        raise ValueError(f"The file '{file_path}' is not a CSV file.")

    preferences = {}

    try:
        # Read data from the CSV file
        with open(file_path, mode='r', encoding='utf-8') as csvfile:
            reader = csv.reader(csvfile)

            # Ensure CSV has the expected format
            header = next(reader, None)  # Read the header row
            if header is None:
                raise ValueError("The CSV file is empty or does not have a header row.")

            # Find the positions of "movie" and "rating" columns
            try:
                movie_idx = header.index("movie")
                rating_idx = header.index("rating")
            except ValueError:
                raise ValueError("The CSV file must have columns labeled 'movie' and 'rating'.")

            # Use movie_idx and rating_idx later in the function
            for row in reader:
                if len(row) <= max(movie_idx, rating_idx):  # Ensure the row has enough columns
                    continue  # Skip rows with missing data

                movie = row[movie_idx]
                score_str = row[rating_idx]
                try:
                    score = float(score_str)
                    score = max(1.0, min(10.0, score))
                    normalized_score = (score - 1.0) / 9.0
                    preferences[movie] = normalized_score
                except ValueError:
                    print(f"Invalid rating '{score_str}' for movie '{movie}'. Skipping this entry.")


    except Exception as e:
        raise ValueError(f"An error occurred while reading the CSV file: {e}")

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
    <h2>Top 5 most recommended movies:</h2>
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
    <h2>Top 5 of movies you should avoid:</h2>
    """
    for title in movie_titles[1]:  # 0-GOOD  1-BAD
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
if len(sys.argv) == 1:
    print("No input parameter detected! Fill user data first so you can get you recommendations!")
    preferences = get_user_preferences()
elif len(sys.argv) == 2:
    print("Input parameter detected! Searching for file content...")
    preferences = get_preferences_from_csv(sys.argv[1])
else:
    raise ValueError("Output contains more than one parameter, which is not allowed.")

data = pd.read_csv("movies.csv")
user_movie_matrix = data.pivot_table(index='Name', columns='Movie', values='Score')
user_movie_matrix.fillna(0, inplace=True)
user_similarity_matrix = cosine_similarity(user_movie_matrix)
user_similarity_df = pd.DataFrame(user_similarity_matrix, index=user_movie_matrix.index,
                                    columns=user_movie_matrix.index)

generate_movie_html(recommend_based_on_users(preferences))
