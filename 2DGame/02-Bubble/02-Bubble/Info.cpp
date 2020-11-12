#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Info.h"
#include "Game.h"

void Info::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/info.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(56, 192), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInfo.x), float(tileMapDispl.y + posInfo.y)));

}

void Info::update(int deltaTime)
{
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInfo.x), float(tileMapDispl.y + posInfo.y)));
}

void Info::render()
{
	sprite->render();
}

void Info::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Info::setPosition(const glm::vec2& pos)
{
	posInfo = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posInfo.x), float(tileMapDispl.y + posInfo.y)));
}

