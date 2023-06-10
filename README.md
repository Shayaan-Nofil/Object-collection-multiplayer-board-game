# Object-collection-multiplayer-board-game
A simple multiplayer board game implemented using C++. It includes libraries like sfml, xlib, atomic etc. The goal of the game is to collect more objects (dragon balls) 
than the other player. The game was made in a linux enviroment, and to compile the game, use the following command:

 g++ -o run Game.cpp -lsfml-graphics -lsfml-window -lsfml-system -lX11
 
 This will create an executable file, by the name of "run", and to execute the file use the following command:
 
 ./run
 
 Player 1 is controlled with the wasd keys, and player 2 is controlled with the arrow keys.
 
 The board size will vary on each play through, along with the number of objects being spawned. 
 
 The multiplayer nature of the game is implemented using two detached threads, which are responsible for taking input from the users, and changing the positions of the 
 characters(while keeping inside the constraints of the board).
 
 The rendering, and collisions with objects take place in the main thread, and the sfml library is used for the keyboard inputs, and rendering.
 Along with using atomic variables to insure there are no race conditions. In this particular assignment we were not allowed to used mutexes and
 sempahores, and as such they are not present in the code.
