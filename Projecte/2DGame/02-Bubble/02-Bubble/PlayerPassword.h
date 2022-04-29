#ifndef _PLAYERPASSWORD_INCLUDE
#define _PLAYERPASSWORD_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




class PlayerPassword
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
private:
	glm::ivec2 tileMapDispl, posPlayerPassword;
	glm::vec2 posPlayerPasswordAux;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYERPASSWORD_INCLUDE


