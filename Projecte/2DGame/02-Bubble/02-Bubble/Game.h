#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Credits.h"
#include "Menu.h"
#include "Play.h"
#include "Help.h"
#include "Level.h"

#include <irrKlang.h>
#include <ik_ISoundEngine.h>
using namespace irrklang;

#define SCREEN_WIDTH 272
#define SCREEN_HEIGHT 240

class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void exit();
	void runConsole();
	void mouseRelease(int button);
	bool moveUpPressed();
	bool moveDownPressed();
	bool moveRightPressed();
	bool moveLeftPressed();

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void selectScene(int ID);


	void loopMusic(char* fileName);
	void stopMusic();
	void playSound(char* fileName);

private:
	bool bPlay;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

	irrklang::ISoundEngine* engine; //para la musica del juego
	ISound* music;

};


#endif // _GAME_INCLUDE


