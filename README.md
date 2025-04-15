# Space Spelunker
"Space Spelunker" is a simple 2D top-down view game, made using C and the library Raylib as a final project for my Algorithms and Programming class. The premise is about an astrounaut
exploring another planet, gathering emeralds and gold, and slaying the local evil mole population. It showcases the skills in C I learned in this class, such as:
- Structs,
- Pointers,
- File reading/writing,
- Graphics, input handling and collision (through Raylib).

# How to play it
- **Movement**: Arrow keys  
- **Shoot**: `G` key  
- **Pause**: `TAB`  
- **Exit**: `ESC`  

## Level Design  
Create custom levels by editing text files (`mapa1`, `mapa2`, etc.) with the following key:  

| Symbol | Object           |  
|--------|------------------|  
| `#`    | Unbreakable wall |  
| `J`    | Player spawn     |  
| `E`    | Emerald         |  
| `S`    | Breakable wall  |  
| `O`    | Gold            |  
| `T`    | Mole (enemy)    |  
| `A`    | Flashlight powerup |  

Levels must be sequentially numbered (e.g., `mapa1`, `mapa2`).  

# How to run it
To run this game, you need to have Raylib installed on your machine. For that, follow the plataform-specific setup instructions in [raylib.com](https://www.raylib.com/).
After installing Raylib, you can simply run the .exe file, but it has to be in the same folder as the other files in the repo.

(Obs: The saving feature may not work properly in Linux based systems.)


---
*Developed in 2023 as a learning project for C and game development fundamentals.*  
