#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int ID);
	void update(int deltaTime, int currentRoom, glm::vec2 posBall);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2& pos);
	void setDead(bool d);
	glm::vec2 getPosition();
	
private:
	bool dead;
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


