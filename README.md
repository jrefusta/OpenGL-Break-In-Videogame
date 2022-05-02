# Break In based in OpenGL

Pràctica de l'assignatura Llenguatges de Programació 19-20Q1. Consisteix en un bot per Telegram que constesta textualment i gràficament a preguntes relacionades amb les enquestes descrites amb el compilador descrit prèviament, així com recullir les dades d’enquestes.
University project of the subject Videogames 20-21Q1 made at FIB, UPC. It consists of a representation of the videogame Break in from 1987, based in OpenGL.

Grade: **10.0**

![](https://media.githubusercontent.com/media/jrefusta/VJ_2D/main/gifs/gif1.gif)

## How to run it

Download the project and execute the following executable file: "Break in.exe".

```
/Binari/Break in.exe
```

![](https://media.githubusercontent.com/media/jrefusta/VJ_2D/main/gifs/gif2.gif)


## Implemented functionalities

- Three levels with four screens each.
- Three types of blocks (resistance 1, 2 and 3). Resistance blocks 2 can be found on the last screen of the second level, and resistance blocks 3 can be found on the last screen of the third level.
- Mechanics of the keys to move from screen to screen
- Mechanics of the money at the last screen of each level.
- Alarm that causes the watchman to appear and, when he catches us, we lose a life.
- Basic structure of the 4 screens: Main menu, Instructions, Credits and a menu to select between the 3 levels and the extra mode (this way we can jump to any level without passwords).
- Extra mechanics:
We have created a new game mode in which we play as the Cop. Normally, using the Thief, we would reach the last screen of each level and collect all the money to win. This time, as the Cop, we will start at the final screen and we will have to avoid taking the money to win. Every time we collect 1000 units of money, we will lose a life. We go through the screens of the first level in reverse. In each screen we will find the thief, if we hit him 3 times with the ball, we will open the door of the current screen and we will be able to access the next one. The behavior of the thief varies with each hit we give him. We win when we take all the lives of the thief in the last screen and go through the door that opens when we defeat the "final boss".

![](https://media.githubusercontent.com/media/jrefusta/VJ_2D/main/gifs/gif3.gif)


## Instructions

- We will move through the menu using the arrow keys. Select an option with 'ENTER'.
- Once inside the game, launch the ball with the up arrow or the space bar. If no action is taken for 2.5 seconds, the ball will be launched automatically.
- We will move around the screen with the arrow keys.
- Each time we collide the ball with a key, we will open the door of the current screen.
- If we collide the ball with the phone, our points will be converted into money.
- Once we collect all the money from the last screen, we will see the winning animation. If we press 'SPACE', we go to the next level directly. If we are in the last level or in the extra mode, when we press 'SPACE', we will see the credits.
- Keys to skip content:
- Pressing the 'G' key, we activate God Mode. If activated, whenever the ball collides with the player, it will go up, and we won't lose lives.
- Pressing the 'U' key, we will open the door of the current screen and go to the next screen.
- Pressing the 'R' key restarts the current level.
- To return to the menu, press the ESC key. Also, it is used to exit the game when you are in the menu.

![](https://media.githubusercontent.com/media/jrefusta/VJ_2D/main/gifs/gif4.gif)
