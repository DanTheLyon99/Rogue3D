## Rogue3D
<br />
<p align="center">
    <img src="gifs/Roaming.gif" alt="Roaming" width="300" height="250">
    <img src="gifs/IntoCave.gif" alt="IntoCave" width="300" height="250">
    <img src="gifs/IntoDungeon.gif" alt="IntoDungeon" width="300" height="250">
</p>
Rogue3D is a revisioning  of the original 1980s dungeon crawler Rogue.
This project was to seek out and amplify the elements that made the revolutionary game so entertaining all those years ago and 
add a modern *twist* I hope you all enjoy!

## Built With

* [C](https://www.cprogramming.com/tutorial/c-tutorial.html)
* [OpenGL](https://www.opengl.org/)
* [Linux](https://www.linux.org/)

## Getting Started 
To get Rogue3D up and running properly, here's what you'll need to do.

### Prerequisites

Firstly, you will need to download [XLaunch](https://sourceforge.net/projects/vcxsrv/)
(I have also left the installer in the XLaunch folder inside the repo, should the sourceforge go down)

### Installation and Usage

Once installed, you will need to Open XLaunch, and follow these instructions:<br>
1.) choose multiple windows, display number:0<br>
2.) Start no client<br>
3.) check all boxes in extra settings<br>
4.) finish

Once XLaunch is up and running you're halfway there!

### Console Prep

To run the program you will need to compile it with the makefile, which also uses gcc.

on linux these require the ``build-esstential`` pakages
if you don't already have them the command to get them is
``sudo apt update`` and then ``sudo apt install build-essential``

## Console Commands
To set up the display type `export DISPLAY=localhost:0`<br>
To compile the program type `make` and then `./a1` to run it<br>
*BOOM*, the game should be up and running now on XLaunch.

if ever you want to re-format the world files, a1 executible or dungeon structs just type:
``make clean`` to remove everything. To run it fresh just  ``make`` and then ``./a1`` again :smile:
(the levels are saved in a binary filed called 'world[level_number]' and the dungeon struct with all mob info in 'dungeon[level_number]')

### In Game Commands
the 'f' key toggles fly mode and removes gravity and collision (no clip)

the 'm' key toggles the map of the dungeon at the bottom left corner (it is set to 3 states, so you tap 'm' twice to turn it off.)


<!-- CONTACT -->
## Contact

Daniel Lyon - danlyon99@gmail.com

