#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Credits.h"
#include "Menu.h"
#include "Play.h"
#include "Help.h"
#include "Level.h"


#define SCREEN_WIDTH 272
#define SCREEN_HEIGHT 240


// Game is a singleton (a class with a single instance) that represents our whole application


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
	void mouseRelease(int button);
	void runConsole();
	bool moveUpPressed();
	bool moveDownPressed();
	bool moveRightPressed();
	bool moveLeftPressed();

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void selectScene(int ID);


private:
	bool bPlay;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

};


#endif // _GAME_INCLUDE


