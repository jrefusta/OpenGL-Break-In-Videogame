#ifndef _DIGIT_INCLUDE
#define _DIGIT_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Digit
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, int animId);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
private:
	glm::ivec2 tileMapDispl, posDigit;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
};


#endif // _DIGIT_INCLUDE


