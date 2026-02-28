# blackbelting
The black belt project or something

So the idea is to write a game that uses SDL and might be written in C or C++. ~~No idea what it will be about. It might use pixelated art like the Minesweeper clone from earlier though. It will be a 2D game with some form of world.~~

Here I decided to start with an old idea I partially remember. This will be a game about a flying squirrel. Said squirrel can glide from tree to tree and potentially other destinations. This squirrel will be able to interact with other animals, RPG-style I suppose.

I'm not sure what the conflict of the game will be, but I have some ideas. As suggested by other students, the squirrel's habitat might be threatened by humans, and the objective of the game could be to fight them. Alternatively, the squirrel might be in search of something special.

About technical stuff: the game (you just lost) will, at least during play, consist of drawn images (like Scratch sprites but) with a camera. It will be top-down like Among Us, so the y position of objects will be a pseudo-z position. Gliding will be like using the zipline in Among Us, that is a basic movement between positions w/ animation.

I was asked to document the game engine so here it is. Basically the game (you just lost) will have various tasks. Each one has function pointers for actions like game ticks, events, and cleaning up the task. For example, a task can render a sprite on tick and handle key events with movement.

Milestones:
* ~~Write a quit handler task to test the task system~~
* ~~Render a sprite to the screen and have it move around~~
* ~~Make some basic menus with clickable buttons~~
* Create the squirrel, trees, other characters, dialogue between them...
* Now... try to make a playable game?
