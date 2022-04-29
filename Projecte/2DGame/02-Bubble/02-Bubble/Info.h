#ifndef _INFO_INCLUDE
#define _INFO_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Info
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
private:
	glm::ivec2 tileMapDispl, posInfo;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};

#endif // _INFO_INCLUDE


