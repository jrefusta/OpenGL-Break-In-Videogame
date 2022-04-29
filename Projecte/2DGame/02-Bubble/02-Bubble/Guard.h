#ifndef _GUARD_INCLUDE
#define _GUARD_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Guard
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::ivec2 posPlayer);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool getCollisionGuard(glm::vec2 posPlayer);
	glm::ivec2 getPosition();
	
private:
	bool bMoving;
	glm::ivec2 tileMapDispl, posGuard, vecPlayer;
	glm::vec2 posGuardAux;
	int currentTime;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};

#endif // _GUARD_INCLUDE


