#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Scene.h"
#include "Game.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Level : public Scene
{

public:
	Level();
	~Level();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player* player;
	Ball* ball;
	ShaderProgram texProgram;
	float currentTime;
	int currentRoom;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

