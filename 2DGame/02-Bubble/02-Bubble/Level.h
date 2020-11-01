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
	TileMap *map, *moneyMap, *pointsMap, *livesMap, *bankMap, *roomMap;
	Player* player;
	Ball* ball;
	Info* info;
	ShaderProgram texProgram;
	int currentRoom;
	int currentLevel;
	glm::mat4 projection;
	int livesNum;
	bool alarmHited;
};


#endif // _SCENE_INCLUDE

