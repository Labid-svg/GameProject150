Space Combat Game (SFML + C++)

A 2D Space Shooting Game built with C++ and SFML

Space Combat is a real-time 2D arcade-style space shooter where the player controls a spaceship, shoots incoming enemies, dodges enemy fire, and progresses through increasingly challenging levels.
This project demonstrates core game development fundamentals using C++ and SFML, including game loops, collision detection, state management, and rendering.

◼️ Gameplay Features

• Player-controlled spaceship with smooth movement
• Bullet shooting system (player & enemies)
• Enemy spawning with increasing difficulty
• Enemy health bars
• Score, lives, and level system
• Temporary invincibility after taking damage
• Menu, gameplay, and game-over states
• Simple HUD (score, lives, level)
• Cross-platform font loading (Linux / Windows)

◼️ Controls

• Key	Action
• Arrow Keys - Move player
• Space	- Shoot/Start game
• R	- Restart after Game Over
• Esc -	Exit game

◼️ Project Structure (Key Components)

• Bullet and Enemy structs
• Game state management (MENU, PLAYING, GAME_OVER)
• Separate logic for:
• Input handling
• Enemy spawning
• Shooting mechanics
• Collision detection
• Rendering & HUD updates

◼️ Technical Highlights

• C++17
• SFML (Graphics, Window, System)
• Object-oriented design using structs
• Delta-time based movement
• Collision detection:
•Bounding box checks
• Distance-based collision
• Efficient game loop with frame limiting

◼️ Built With

• Language: C++
• Library: SFML (Simple and Fast Multimedia Library)
• Platform: Linux / Windows

◼️ Game Preview
<img width="1194" height="833" alt="Screenshot from 2026-01-18 08-51-24" src="https://github.com/user-attachments/assets/7985e527-b377-4663-8d36-2a3f59cfd4ba" />
<img width="1194" height="833" alt="Screenshot from 2026-01-18 08-51-11" src="https://github.com/user-attachments/assets/3dda8467-b4b0-4acb-ac8e-3a0a06b7cf69" />
<img width="1194" height="833" alt="Screenshot from 2026-01-18 08-51-00" src="https://github.com/user-attachments/assets/be0e01ff-f195-4259-8156-8e8bafb3f72f" />

[Screencast from 2026-01-18 08-52-24.webm](https://github.com/user-attachments/assets/e7b716f8-c2eb-4411-bbe6-d90a0f5c7825)

