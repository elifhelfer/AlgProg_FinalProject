# Space Spelunker
"Space Spelunker" is a simple 2D top-down view game, made using C and the library Raylib in 2023 as a final project for my Algorithms and Programming class. The premise is about an astrounaut
exploring another planet, gathering emeralds and gold, and slaying the local evil mole population. It showcases the skills in C I learned in this class, such as how to use structs, how to read/write to files and how to use pointers.

# How to play it
Use arrow keys to move, and press G to shoot. Your shots can kill moles and break brown walls. ESC exits the game, and TAB pauses it.

## Making your own levels
You can make your own levels! Following this guide on characters, you can type up your own map. The game looks for files named "mapa[number]", with the number refering to the current level. If you add a "mapa4"
text file to the folder, for example, after the player collects all emeralds from level 3, your 4th level will load up, and the game will end after you beat it. You can make as many new levels as you want, as long
as their respective numbers are sequential, one after another (mapa1, mapa2, mapa3, mapa4, mapa5...)
### Map making guide:
\# = Unbreakable wall

J = Player spawn

E = Emerald

S = Breakable wall

O = Gold

T = Mole

A = Flashlight powerup

# How to run it
To run this game, you need to have Raylib installed on your machine. For that, you can find all you need here: https://www.raylib.com/
After installing Raylib, you can simply run the .exe file, but it has to be in the same folder as the other files in the repo.

(Obs: The saving feature may not work properly in Linux based systems.)

