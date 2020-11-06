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
	void setCurrentMoney(int t);
	void setCurrentPoints(int t);
	int getCrossingRoom();
	int getCurrentRoom();
	bool getThiefShooted();
	void setThiefShooted(bool t);
	void setCurrentRoom(int c);
	void setGodMode(bool g);
	bool getGodMode();
	glm::vec2 getPosBall();
private:
	float ballVelX;
	float ballVelY;
	int initPoints;
	int initMoney;
	int currentRoom;
	bool stuck;
	bool alarmHit;
	float currentTime;	
	int currentMoney;
	int currentPoints;
	int currentLevel;
	bool getAllMoney;
	bool thiefShooted;
	bool godMode;
	int crossingRoom;
	glm::ivec2 tileMapDispl;
	glm::vec2 posBall;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	ShaderProgram shaderProgram;

};


#endif // _PLAYER_INCLUDE


