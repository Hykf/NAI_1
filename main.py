import pandas as pd
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.metrics.pairwise import euclidean_distances
from imdb import Cinemagoer
import webbrowser
import csv
import sys
import os


def get_user_preferences():
    """
    Collects user preferences for movies and their ratings via console input.

    This function prompts the user to input their favorite movies along with a
    rating (on a scale from 1 to 10). The process continues until the user types
    'end'. Ratings are normalized to a scale of 0.0 to 1.0 for further processing.

    **Workflow**:
    - Prompts the user to input a movie name.
    - For each movie, asks for a rating between 1 and 10.
    - Normalizes the rating to a scale of 0.0 to 1.0 using the formula:
      `(rating - 1) / 9`.
    - Adds the movie and its normalized rating to a dictionary.

    **Special Instructions**:
    - Users are encouraged to use the original English titles of the movies.
    - For foreign-language movies, the original title is preferred.

    :raises ValueError: If the rating is not a number or is outside the 1-10 range.
    :returns: A dictionary with movie titles as keys and normalized ratings as values.

    Example:
        >>> get_user_preferences()
        Enter your preferences (movie and rating)!
        Type 'end' to finish entering your preferences.
        Enter movie name: Inception
        Enter rating for the movie 'Inception' (1 - 10): 9
        Enter movie name: Parasite
        Enter rating for the movie 'Parasite' (1 - 10): 8
        Enter movie name: end

        Returns: {'Inception': 0.8888888888888888, 'Parasite': 0.7777777777777778}
    """
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


def recommend_based_on_users(preferences, top_n=5):
    """
    Recommends movies based on user preferences by calculating similarities with other users.

    The function computes both cosine and Euclidean similarities between the new user's preferences and all existing users,
    and generates recommendations by considering movies rated highly by similar users that the new user has not rated.

    :param preferences: A dictionary of movie titles and normalized ratings provided by the new user.
    :param top_n: The number of top recommended and worst recommended movies to return (default is 5).
    :returns: A tuple containing two lists:
              - top_n_best: A list of the top N recommended movies.
              - top_n_worst: A list of the top N movies the user should avoid based on recommendations.
    """
    # Create a new DataFrame for the new user's preferences
    new_user_vector = pd.DataFrame([preferences], columns=user_movie_matrix.columns).fillna(0)

    # Extend the user-movie matrix with the new user's preferences
    extended_matrix = pd.concat([user_movie_matrix, new_user_vector], ignore_index=True)

    # Calculate Cosine Similarity
    extended_cosine_similarity = cosine_similarity(extended_matrix)
    new_user_cosine_similarities = extended_cosine_similarity[-1][:-1]  # Exclude the new user's own similarity

    # Calculate Euclidean Similarity (inverse of Euclidean distance)
    extended_euclidean_distances = euclidean_distances(extended_matrix)
    new_user_euclidean_distances = extended_euclidean_distances[-1][:-1]  # Exclude the new user's own distance
    new_user_euclidean_similarities = 1 / (1 + new_user_euclidean_distances)  # Convert distance to similarity

    # Combine both similarities into separate series for comparison
    similar_users_cosine = pd.Series(new_user_cosine_similarities, index=user_movie_matrix.index).sort_values(
        ascending=False)
    similar_users_euclidean = pd.Series(new_user_euclidean_similarities, index=user_movie_matrix.index).sort_values(
        ascending=False)

    # Print both similarities for comparison
    print("Cosine Similarity between new user and other users:")
    print(similar_users_cosine)  # Display top N cosine similarities

    print("\nEuclidean Similarity between new user and other users:")
    print(similar_users_euclidean)  # Display top N euclidean similarities

    recommendations = {}

    # Generate recommendations based on cosine similarities
    for similar_user, cosine_sim_value in similar_users_cosine.items():  # Renamed variable to avoid conflict
        user_ratings = user_movie_matrix.loc[similar_user]

        for movie, score in user_ratings.items():
            if movie not in preferences and score > 0:
                recommendations[movie] = recommendations.get(movie, 0) + cosine_sim_value * score

        if len(recommendations) >= top_n * 2:
            break

    sorted_recommendations = sorted(recommendations.items(), key=lambda x: x[1], reverse=True)

    top_n_best = sorted_recommendations[:top_n]
    top_n_worst = sorted_recommendations[-top_n:]

    return (top_n_best, top_n_worst)


def get_preferences_from_csv(file_path):
    """
    Reads user preferences from a CSV file and processes them.

    The file must contain columns labeled 'movie' and 'rating', which can appear in any order.
    Ratings are normalized to a 0.0 to 1.0 scale. Invalid rows are skipped.

    :param file_path: Path to the CSV file.
    :raises FileNotFoundError: If the file does not exist.
    :raises ValueError: If the file is not a CSV or lacks the required columns.
    :returns: A dictionary with movie titles as keys and normalized ratings as values.
    """
    # Check if the file exists in the same directory
    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"The file '{file_path}' does not exist in the current directory.")

    # Check if the file has a .csv extension
    if not file_path.lower().endswith('.csv'):
        raise ValueError(f"The file '{file_path}' is not a CSV file.")

    preferences = {}
    print("Input looks like a proper file. Trying to read it...")

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

            print("File has a proper structure. Collecting data...")
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
        print("Data gathered successfully from the file!")


    except Exception as e:
        raise ValueError(f"An error occurred while reading the CSV file: {e}")

    return preferences

def generate_movie_html(movie_titles, output_file="movies.html"):
    """
    Generates an HTML file displaying details of movies grouped into two categories:
    recommended movies and movies to avoid.

    :param movie_titles: A tuple containing two lists:
                         - [0]: List of recommended movie titles (to include as "Top 5 most recommended movies").
                         - [1]: List of movies to avoid (to include as "Top 5 movies you should avoid").
    :param output_file: The name of the output HTML file (default is "movies.html").
    :raises Exception: If there is an error fetching data for a movie.
    :returns: None. Writes an HTML file and opens it in the default web browser.
    """
    html_content = """
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Movies</title>
    <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@600&display=swap" rel="stylesheet">
    <style>
        /* Back to Top Button */
        .back-to-top {
            position: fixed;
            bottom: 20px;
            right: 20px;
            background-color: #ff4500;
            color: white;
            border: none;
            border-radius: 50%;
            width: 50px;
            height: 50px;
            font-size: 20px;
            cursor: pointer;
            transition: background-color 0.3s;
        }

        .back-to-top:hover {
            background-color: #ff6347;
        }

        /* Global Body Styles */
        body {
            font-family: Arial, sans-serif;
            background-color: #2e2e2e; /* Dark background */
            margin: 0;
            padding: 20px;
            display: flex;
            flex-direction: column;
            align-items: center;
            background: linear-gradient(45deg, #2e2e2e, #333333); /* Darker gradient */
            background-size: 400% 400%;
            animation: gradientShift 10s ease infinite;
        }

        @keyframes gradientShift {
            0% { background-position: 0% 50%; }
            50% { background-position: 100% 50%; }
            100% { background-position: 0% 50%; }
        }

        /* Header Styles */
        h1, h2 {
            font-family: 'Poppins', sans-serif;
            color: #ffffff; 
            font-weight: 600;
            letter-spacing: 1px;
            text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.5);            
            text-align: center;
        }
        
        h3 {
            font-family: 'Poppins', sans-serif;
            color: #333;
            font-weight: 600;
            letter-spacing: 1px;
            text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.2);
            text-align: center;
        }

        /* Movie Container (Grid Layout) */
        .movie-container {
            display: grid;
            grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
            gap: 20px;
            padding: 20px;
        }

        /* Movie Card Styles */
        .movie {
            border: 1px solid #444;            
            border-radius: 5px;
            padding: 15px;
            margin: 10px 0;
            background-color: #fff;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            width: 80%;
            max-width: 600px;
            text-align: center;
            transition: transform 0.3s ease, box-shadow 0.3s ease;
        }

        .movie:hover {
            transform: scale(1.05);
            box-shadow: 0 8px 16px rgba(0, 0, 0, 0.4);
        }

        .movie img {
            max-width: 100%;
            height: auto;
            display: block;
            margin: 0 auto 10px;
        }

        .movie .tags {
            margin: 5px 0;
            color: #555;
        }

        /* Emoji Hover Animation */
        header emoji {
            display: inline-block;
            animation: emojiAnimation 1s infinite;
        }

        @keyframes emojiAnimation {
            0% { transform: scale(1); }
            50% { transform: scale(1.2); }
            100% { transform: scale(1); }
        }

        /* Footer Styles */
        footer {
            background-color: #222;
            color: white;
            text-align: center;
            padding: 20px;
            margin-top: 40px;
        }

        footer a {
            color: #fff;
            text-decoration: none;
        }

        footer a:hover {
            text-decoration: underline;
        }

        /* Image Scaling */
        img {
            max-height: 30vh; /* 30% of the viewport height */
            width: auto; /* Maintain aspect ratio */
            object-fit: contain; /* Ensure the image fits without distortion */
        }

    </style>
</head>
<body>
    <!-- Logo -->
    <img src="logo2.webp" alt="Logo of the app">
    
    <!-- Main Title -->
    <h1>🎬 Your Movies Wrapped 🎬</h1>

    <!-- Recommended Movies -->
    <h2>🤩 Top 5 most recommended movies for you 🤩</h2>

    <div class="movie-container">
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
                <h3>{movie_title}</h3>
                <img src="{movie_poster}" alt="{movie_title}">
                <p class="tags">Genres: {movie_genres}</p>
                <p>{movie_plot}</p>
            </div>
            """
        except Exception as e:
            print(f"Error fetching data for '{title[0]}': {e}")

    html_content += """
        </div>
     <h2>🤢 Top 5 worst movies for you 🤢</h2>

    <div class="movie-container">
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
                <h3>{movie_title}</h3>
                <img src="{movie_poster}" alt="{movie_title}">
                <p class="tags">Genres: {movie_genres}</p>
                <p>{movie_plot}</p>
            </div>
            """
        except Exception as e:
            print(f"Error fetching data for '{title[0]}': {e}")

    html_content += """
 </div>

    <!-- Footer Section -->
    <footer>
        <div class="footer-content">
            <p>Contact us at: <a href="mailto:mechlinski.pawel@gmail.com">mechlinski.pawel@gmail.com</a></p>
            <p>Follow us on <a href="https://github.com/Hykf/NAI_2024-2025/tree/NAI_3">GitHub</a></p>
        </div>
    </footer>
    </body>
    </html>
    """

    with open(output_file, "w", encoding="utf-8") as file:
        file.write(html_content)

    print(f"HTML file generated: {output_file}")
    webbrowser.open("movies.html")


####################
if len(sys.argv) == 1:
    print("No input parameter detected! Fill user data first so you can get you recommendations!")
    preferences = get_user_preferences()
elif len(sys.argv) == 2:
    print("Input parameter detected! Searching for file content...")
    preferences = get_preferences_from_csv(sys.argv[1])
else:
    raise ValueError("Output contains more than one parameter, which is not allowed.")

print("Starting data analysis based on the recommendation database...")
data = pd.read_csv("movies.csv")
user_movie_matrix = data.pivot_table(index='Name', columns='Movie', values='Score')
user_movie_matrix.fillna(0, inplace=True)
user_similarity_matrix = cosine_similarity(user_movie_matrix)
user_similarity_df = pd.DataFrame(user_similarity_matrix, index=user_movie_matrix.index,
                                    columns=user_movie_matrix.index)

print("Generating recommendations...")
generate_movie_html(recommend_based_on_users(preferences))
