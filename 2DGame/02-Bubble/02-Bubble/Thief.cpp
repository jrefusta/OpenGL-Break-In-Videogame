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
	this->shaderProgram = shaderProgram;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posThief.x), float(tileMapDispl.y + posThief.y)));
	thiefVelY = -1;
	thiefVelX = 1;
	stage = 1;
	this->lives = 3;
	destroyed1Stage = false;
	destroyed2Stage = false;
	destroyed3Stage = false;
	destroyed4Stage = false;
}

void Thief::update(int deltaTime, int currentRoom)
{
	sprite->update(deltaTime);

	if (stage == 1) {
		if (lives == 0) {
			if (!destroyed1Stage) {
				map->destroyTop(tileMapDispl, shaderProgram, stage);
				destroyed1Stage = true;
			}
			if (posThief.x < 87) posThief.x += 4;
			else if (posThief.x > 87) posThief.x -= 4;
			else {
				posThief.y -= 4;
				if (posThief.y < 350) {
					lives = 3;
					stage = 2;
					thiefVelX = 1;
				}
			}
		}
		if (lives == 2) {
			if (posThief.x > 20) {
				posThief.x -= 3;
			}
		}
		if (lives == 1) {
			if (posThief.x < 87) {
				posThief.x += 3;
			}
		}
	}
	if (stage == 2) {
		if (lives == 0) {
			if (!destroyed1Stage) {
				map->destroyTop(tileMapDispl, shaderProgram, stage);
				destroyed1Stage = true;
			}
			posThief.x += thiefVelX * 2;
			if (posThief.x == 87) {
				thiefVelX = 0;
				posThief.y -= 4;
				if (posThief.y < 450) {
					lives = 3;
					stage = 2;
					thiefVelX = 1;
				}
			}
			else {
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
		}
		else {
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
	}
	else if (stage == 3) {//Y=-3,X=1
		posThief.y += thiefVelY;
		if (posThief.y > 100 + 192 * (4 - stage)) {
			posThief.y -= thiefVelY;
			thiefVelY = -abs(thiefVelY);
		}
		if (posThief.y < 0 + 192 * (4 - stage)) {
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
bool Thief::collisionBall(glm::vec2 posBall)
{
	bool collisionX = posThief.x + 19 >= posBall.x && posBall.x + 9 >= posThief.x;
	bool collisionY = (posThief.y + 21) + 5 >= posBall.y && posBall.y + 10 >= (posThief.y + 21);
	return collisionX && collisionY;
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

void Thief::setLives(int l) {
	this->lives = l;
}

int Thief::getLives() {
	return this->lives;
}