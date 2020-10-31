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
	this->shaderProgram = shaderProgram;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	ballVelX = 2;
	ballVelY = -2;
	this->currentRoom = 1;
	this->crossingRoom = 0;
	this->getAllMoney = false;
	this->currentMoney = 0;
	this->currentPoints = 0;
}

void Ball::update(int deltaTime, glm::vec2 posPlayer, int currentRoom)
{
	sprite->update(deltaTime);
	if (map->getTotalMoney() == 0) {
		this->getAllMoney = true;
	}
	this->currentMoney = map->getMoney();
	this->currentPoints = map->getPoints();
	if (!this->getStuck()) {
		posBall.y += ballVelY;
		if (map->collisionMoveUp(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.y -= ballVelY;
			ballVelY = abs(ballVelY);
		}
		else if (map->collisionMoveDown(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.y -= ballVelY;
			ballVelY = -abs(ballVelY);
		}

		if (collisionPlayer(posPlayer) && ballVelY > 0) {
			posBall.y -= ballVelY;
			ballVelY = -abs(ballVelY);
		}
		posBall.x += ballVelX;

		if (map->collisionMoveRight(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.x -= ballVelX;
			ballVelX = -abs(ballVelX);

		}
		else if (map->collisionMoveLeft(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.x -= ballVelX;
			ballVelX = abs(ballVelX);
		}
		if (collisionPlayer(posPlayer) && ballVelX > 0 && ballVelY > 0) {
			posBall.x -= ballVelX;
			ballVelX = -abs(ballVelX);
		}
		else if (collisionPlayer(posPlayer) && ballVelX < 0 && ballVelY > 0) {
			posBall.x -= ballVelX;
			ballVelX = abs(ballVelX);
		}
	}
	if (posBall.y != 0 && posBall.x != 0 && this->currentRoom != 0) {
		if (posBall.y > 752.0 - (24 * 8 * (this->currentRoom - 1))) {
			this->crossingRoom = -1;
			this->currentRoom -= 1;
		}
		else if (posBall.y < 752.0 - (24 * 8 * (this->currentRoom))) {
			this->currentRoom += 1;
			this->crossingRoom = 1;
		}

	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}
bool Ball::getGetAllMoney() {
	return this->getAllMoney;
}
int Ball::getCurrentMoney() {
	return this->currentMoney;
}
int Ball::getCurrentPoints() {
	return this->currentPoints;
}

int Ball::getCurrentRoom() {
	return this->currentRoom;
}

void Ball::setCurrentRoom(int c) {
	this->currentRoom = c;
}

int Ball::getCrossingRoom() {
	return this->crossingRoom;
}

void Ball::setCrossingRoom(int c) {
	this->crossingRoom = c;
}


bool Ball::collisionPlayer(glm::vec2& posPlayer) {
	bool collisionX = posPlayer.x + 19 >= posBall.x && posBall.x + 9 >= posPlayer.x;
	// collision y-axis?
	bool collisionY = posPlayer.y + 8 >= posBall.y && posBall.y + 10 >= posPlayer.y;
	// collision only if on both axes
	return collisionX && collisionY;
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
