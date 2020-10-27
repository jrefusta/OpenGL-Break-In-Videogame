#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	stuck = true;
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(9, 10), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	ballVelX = -1;
	ballVelY = 1;
}

void Ball::update(int deltaTime, glm::vec2 posPlayer)
{
	sprite->update(deltaTime);

	posBall.y += ballVelY;
	if (map->collisionMoveUp(posBall, glm::ivec2(12, 12))) {
		posBall.y -= ballVelY;
		ballVelY = abs(ballVelY);
	}
	else if (map->collisionMoveDown(posBall, glm::ivec2(12, 12))) {
		posBall.y -= ballVelY;
		ballVelY = -abs(ballVelY);
	}

	if (collisionPlayerUp(posPlayer) && ballVelY > 0 ) {
		posBall.y -= ballVelY;
		ballVelY = -abs(ballVelY);
	}

	posBall.x += ballVelX;
	if (map->collisionMoveRight(posBall, glm::ivec2(12, 12))) {
		posBall.x -= ballVelX;
		ballVelX = -abs(ballVelX);
		//ballVelY = -abs(ballVelY);

	}
	else if (map->collisionMoveLeft(posBall, glm::ivec2(12, 12))) {
		posBall.x -= ballVelX;
		ballVelX = abs(ballVelX);
		//ballVelY = -abs(ballVelY);
	}
	if (collisionPlayerRight(posPlayer) && ballVelX > 0) {
		posBall.x -= ballVelX;
		ballVelX = -abs(ballVelX);
	}
	else if (collisionPlayerLeft(posPlayer) && ballVelX < 0) {
		posBall.x -= ballVelX;
		ballVelX = abs(ballVelX);
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}


bool Ball::collisionPlayerUp(glm::vec2& posPlayer) {
	int x0 = posPlayer.x - 10;
	int x1 = posPlayer.x + 16;
	int y = posPlayer.y - 8;
	for (int i = x0; i <= x1; ++i) {
		glm::vec2 pos;
		pos.x = i;
		pos.y = y;
		if (this->posBall.x == pos.x && this->posBall.y == pos.y) return true;
	}
	return false;
}
bool Ball::collisionPlayerRight(glm::vec2& posPlayer) {
	int y0 = posPlayer.y - 8;
	int y1 = posPlayer.y + 4;
	int x = posPlayer.x - 8;
	for (int i = y0; i <= y1; ++i) {
		glm::vec2 pos;
		pos.y = i;
		pos.x = x;
		if (this->posBall.x == pos.x && this->posBall.y == pos.y) return true;
	}
	return false;
}
bool Ball::collisionPlayerLeft(glm::vec2& posPlayer) {
	int y0 = posPlayer.y - 8;
	int y1 = posPlayer.y + 4;
	int x = posPlayer.x + 16;
	for (int i = y0; i <= y1; ++i) {
		glm::vec2 pos;
		pos.y = i;
		pos.x = x;
		if (this->posBall.x == pos.x && this->posBall.y == pos.y) {
			return true;

		}
	}
	return false;
}

bool Ball::getStuck() {
	return this->stuck;
}

void Ball::setStuck(bool s) {
	this->stuck = s;
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}




