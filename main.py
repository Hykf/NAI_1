from imdb import Cinemagoer

# Creating an instance of the Cinemagoer class
ia = Cinemagoer()

# Getting movie by IMDb ID
movieByID = ia.get_movie('0468569')

# Getting searching movie via Title
JohnWick = ia.search_movie("John Wick 4")
print(JohnWick)

print("\n\n#######")

for category in movieByID.keys():
    print(f"{category} : {movieByID[category]}")
print("\n\n#######")

