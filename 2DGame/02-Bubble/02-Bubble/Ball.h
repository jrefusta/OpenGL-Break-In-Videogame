#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::vec2 posPlayer, int currentRoom);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool collisionPlayerUp(glm::vec2& posPlayer);
	bool collisionPlayerRight(glm::vec2& posPlayer);
	bool collisionPlayerLeft(glm::vec2& posPlayer);
	void setStuck(bool s);
	void setCrossingRoom(int c);

	bool getStuck();
	bool getGetAllMoney();
	int getCrossingRoom();
	int getCurrentRoom();
	int getCurrentMoney();
	int getCurrentPoints();
	void setCurrentRoom(int c);

private:
	int ballVelX;
	int ballVelY;
	int currentRoom;
	bool stuck;
	int currentMoney;
	int currentPoints;
	bool getAllMoney;
	float currentTime;
	int crossingRoom;
	glm::ivec2 tileMapDispl, posBall;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	ShaderProgram shaderProgram;

};


#endif // _PLAYER_INCLUDE


