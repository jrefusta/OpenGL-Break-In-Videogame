#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Ball
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int currentLevel);
	void update(int deltaTime, glm::vec2 posPlayer, glm::vec2 posThief, int currentRoom);
	void update(int deltaTime, glm::vec2 posPlayer, int currentRoom);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool collisionPlayer(glm::vec2& posPlayer);
	bool collisionThief(glm::vec2& posThief);
	void setStuck(bool s);
	void setCrossingRoom(int c);
	bool getGetAllMoney();
	bool getGetAlarmHit();
	bool getStuck();	
	void destroyedTop();
	int getCurrentMoney();
	int getCurrentPoints();
	int getCrossingRoom();
	int getCurrentRoom();
	bool getThiefShooted();
	void setThiefShooted(bool t);
	void setCurrentRoom(int c);

private:
	int ballVelX;
	int ballVelY;
	int currentRoom;
	bool stuck;
	bool alarmHit;
	float currentTime;	
	int currentMoney;
	int currentPoints;
	int currentLevel;
	bool getAllMoney;
	bool thiefShooted;
	int crossingRoom;
	glm::ivec2 tileMapDispl, posBall;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	ShaderProgram shaderProgram;

};


#endif // _PLAYER_INCLUDE


