#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Scene.h"
#include "Game.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Info.h"
#include "Thief.h"
#include "Digit.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Level : public Scene
{

public:
	Level();
	~Level();

	void init(int ID);
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	Ball* ball;
	Info* info;
	Thief* thief;
	Info *info;
	Digit *money[7], *points[7], *lives[2], *bank[2], *room[2];
	ShaderProgram texProgram;
	int currentRoom;
	int currentLevel;
	glm::mat4 projection;
	int livesNum;
};


#endif // _SCENE_INCLUDE

