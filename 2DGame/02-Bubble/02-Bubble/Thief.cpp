#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Thief.h"
#include "Game.h"


enum PlayerAnims
{
	STAGE1, STAGE2, STAGE3, STAGE4, STAGE_DEAD
};


void Thief::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/thief.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(19, 26), glm::vec2(1/5.0, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);
	sprite->setAnimationSpeed(STAGE1, 8);
	sprite->addKeyframe(STAGE1, glm::vec2(0.f, 0.f));
	sprite->setAnimationSpeed(STAGE2, 8);
	sprite->addKeyframe(STAGE2, glm::vec2(1 / 5.f, 0.f));
	sprite->setAnimationSpeed(STAGE3, 8);
	sprite->addKeyframe(STAGE3, glm::vec2(2 / 5.f, 0.f));
	sprite->setAnimationSpeed(STAGE4, 8);
	sprite->addKeyframe(STAGE4, glm::vec2(3 / 5.f, 0.f));
	sprite->setAnimationSpeed(STAGE_DEAD, 8);
	sprite->addKeyframe(STAGE_DEAD, glm::vec2(4 / 5.f, 0.f));

	sprite->changeAnimation(STAGE1);
	this->shaderProgram = shaderProgram;
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posThief.x), float(tileMapDispl.y + posThief.y)));
	thiefVelY = 1;
	thiefVelX = 1;
	stage = 1;
	this->lives = 3;
	destroyed1Stage = false;
	destroyed2Stage = false;
	destroyed3Stage = false;
	destroyed4Stage = false;
	this->thiefBeaten = false;
	activeCollision = true;
}

void Thief::update(int deltaTime, int currentRoom)
{
	sprite->update(deltaTime);

	if (stage == 1) {
		if (sprite->animation() != STAGE1) {
			sprite->changeAnimation(STAGE1);
		}
		if (lives == 0) {
			activeCollision = false;
			if (!destroyed1Stage) {
				map->destroyTop(tileMapDispl, shaderProgram, stage);
				destroyed1Stage = true;
			}
			if (posThief.x < 87) {
				posThief.x += 1;
			}
			else if (posThief.x > 87) {
				posThief.x -= 1;
			}
			else {
				posThief.y -= 4;
				if (posThief.y < 450) {
					activeCollision = true;
					this->lives = 3;
					this->stage = 2;
					thiefVelX = 2;
				}
			}
		}
		if (lives == 2) {
			if (posThief.x > 20) {
				activeCollision = false;
				posThief.x -= 3;
			}
			else activeCollision = true;
		}
		if (lives == 1) {
			if (posThief.x < 87) {
				activeCollision = false;
				posThief.x += 3;
			}
			else activeCollision = true;
		}
	}
	if (stage == 2) {
		if (sprite->animation() != STAGE2) {
			sprite->changeAnimation(STAGE2);
		}
		if (lives == 0) {
			activeCollision = false;
			if (!destroyed2Stage) {
				map->destroyTop(tileMapDispl, shaderProgram, stage);
				destroyed2Stage = true;
			}
			if (posThief.x < 80) posThief.x += 4;
			else if (posThief.x > 90) posThief.x -= 4;
			else {
				posThief.y -= 4;
				if (posThief.y < 250) {
					activeCollision = true;
					lives = 3;
					stage = 3;
					thiefVelX = 3;
				}
			}
		}
		if (lives == 1) {
			posThief.x += thiefVelX;
			if (posThief.x > 166) {
				posThief.x -= thiefVelX;
				thiefVelX = -abs(thiefVelX);
			}
			else if (posThief.x < 8) {
				posThief.x -= thiefVelX;
				thiefVelX = abs(thiefVelX);
			}
			posThief.y += thiefVelY;
			if (posThief.y > 450) {
				posThief.y -= thiefVelY;
				thiefVelY = -abs(thiefVelY);
			}
			else if (posThief.y < 400) {
				posThief.y -= thiefVelY;
				thiefVelY = abs(thiefVelY);
			}
		}
		if (lives == 3) {
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
		if (lives == 2) {
			if (posThief.y > 400) {
				activeCollision = false;
				posThief.y -= 3;
			}
			else {
				activeCollision = true;
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
	}
	else if (stage == 3) {
		if (sprite->animation() != STAGE3) {
			sprite->changeAnimation(STAGE3);
		}
		if (lives == 3) {
			if (posThief.x < 155) posThief.x += 4;
		}
		if (lives == 0) {
			activeCollision = false;
			if (!destroyed3Stage) {
				map->destroyTop(tileMapDispl, shaderProgram, stage);
				destroyed3Stage = true;
			}
			if (posThief.x < 87) {
				posThief.x += 1;
			}
			else if (posThief.x > 87) {
				posThief.x -= 1;
			}
			else {
				posThief.y -= 4;
				if (posThief.y < 90) {
					activeCollision = true;
					this->lives = 3;
					this->stage = 4;
					thiefVelX = 3;
				}
			}
		}
		if (lives == 2) {
			if (posThief.x > 20) {
				activeCollision = false;
				posThief.x -= 3;
			}
			else activeCollision = true;
		}
		if (lives == 1) {
			if (posThief.x < 87) {
				activeCollision = false;
				posThief.x += 3;
			}
			else activeCollision = true;
		}
	}
	if (stage == 4) {
		if (sprite->animation() != STAGE4) {
			sprite->changeAnimation(STAGE4);
		}
		if (lives == 0) {
			if (sprite->animation() != STAGE_DEAD) {
				sprite->changeAnimation(STAGE_DEAD);
			}
			activeCollision = false;
			if (!destroyed4Stage) {
				map->destroyTop(tileMapDispl, shaderProgram, stage);
				destroyed4Stage = true;
			}
			if (posThief.x < 80) posThief.x += 1;
			else if (posThief.x > 90) posThief.x -= 1;
			else {
				posThief.y -= 1;
				if (posThief.y < 10) {
					this->thiefBeaten = true;
				}
			}
		}
		if (lives == 1) {
			posThief.x += thiefVelX;
			if (posThief.x > 166) {
				posThief.x -= thiefVelX;
				thiefVelX = -abs(thiefVelX);
			}
			else if (posThief.x < 8) {
				posThief.x -= thiefVelX;
				thiefVelX = abs(thiefVelX);
			}
			posThief.y += thiefVelY;
			if (posThief.y >= 66) {
				posThief.y -= thiefVelY;
				thiefVelY = -abs(thiefVelY);
			}
			else if (posThief.y <= 16) {
				posThief.y -= thiefVelY;
				thiefVelY = abs(thiefVelY);
			}
		}
		if (lives == 3) {
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
		if (lives == 2) {
			if (posThief.y > 16) {
				activeCollision = false;
				posThief.y -= 3;
			}
			else {
				activeCollision = true;
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

bool Thief::getThiefBeaten() {
	return this->thiefBeaten;
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
bool Thief::getCollisionActive() {
	return this->activeCollision;
}