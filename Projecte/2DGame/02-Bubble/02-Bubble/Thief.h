#ifndef _THIEF_INCLUDE
#define _THIEF_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Thief
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, int currentRoom);
	void render();
	bool collisionBall(glm::vec2 posBall);
	void setTileMap(TileMap* tileMap);
	void setVelocityX(int vx);
	void setVelocityY(int vy);
	void setStage(int s);
	void setLives(int l);
	int getLives();
	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();
	bool getCollisionActive();
	bool getThiefBeaten();
private:
	glm::ivec2 tileMapDispl, posThief;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	int thiefVelY;
	int thiefVelX;
	int stage;
	int lives;
	ShaderProgram shaderProgram;
	bool destroyed1Stage;
	bool destroyed2Stage;
	bool destroyed3Stage;
	bool destroyed4Stage;
	bool activeCollision;
	bool thiefBeaten;
};


#endif // _PLAYER_INCLUDE


