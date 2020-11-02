#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Thief.h"
#include "Game.h"


/*enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};*/


void Thief::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/thief.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(19, 26), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posThief.x), float(tileMapDispl.y + posThief.y)));
	thiefVelY =-2;
	thiefVelX = 4;
	stage = 2;
}

void Thief::update(int deltaTime, int currentRoom)
{
	sprite->update(deltaTime);
	if (stage == 1) {
		posThief.x += thiefVelX;
		if (posThief.x > 166) {
			posThief.x -= thiefVelX;
			thiefVelX = -abs(thiefVelX);
		}
		else if (posThief.x < 8) {
			posThief.x -= thiefVelX;
			thiefVelX = abs(thiefVelX);
		}
	}
	else if (stage == 2) {//Y=-3,X=1
		posThief.y += thiefVelY;
		if (posThief.y > 100 + 192 * (4 - currentRoom)) {
			posThief.y -= thiefVelY;
			thiefVelY = -abs(thiefVelY);
		}
		if (posThief.y < 0 + 192 * (4 - currentRoom)) {
			posThief.y -= thiefVelY;
			thiefVelY = abs(thiefVelY);
		}
		posThief.x += thiefVelX;
		if (posThief.x > 166) {
			posThief.x -= thiefVelX;
			thiefVelX = -abs(thiefVelX);
			
		}
		if (posThief.x < 8) {
			posThief.x -= thiefVelX;
			thiefVelX = abs(thiefVelX);
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posThief.x), float(tileMapDispl.y + posThief.y)));
}

void Thief::setVelocityX(int vx) {
	this->thiefVelX = vx;
}
void Thief::setVelocityY(int vy) {
	this->thiefVelY = vy;
}
void Thief::setStage(int s) {
	this->stage = s;
}
void Thief::render()
{
	sprite->render();
}

void Thief::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Thief::setPosition(const glm::vec2& pos)
{
	posThief = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posThief.x), float(tileMapDispl.y + posThief.y)));
}

glm::vec2 Thief::getPosition()
{
	return this->posThief;
}



