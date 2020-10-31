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
	ballVelX = 1;
	ballVelY = -1;
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
		if (map->collisionMoveUp(posBall, glm::ivec2(12, 12), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.y -= ballVelY;
			ballVelY = abs(ballVelY);
		}
		else if (map->collisionMoveDown(posBall, glm::ivec2(12, 12), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.y -= ballVelY;
			ballVelY = -abs(ballVelY);
		}

		if (collisionPlayerUp(posPlayer) && ballVelY > 0) {
			posBall.y -= ballVelY;
			ballVelY = -abs(ballVelY);
		}
		posBall.x += ballVelX;

		if (map->collisionMoveRight(posBall, glm::ivec2(12, 12), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.x -= ballVelX;
			ballVelX = -abs(ballVelX);

		}
		else if (map->collisionMoveLeft(posBall, glm::ivec2(12, 12), tileMapDispl, shaderProgram, currentRoom)) {
			posBall.x -= ballVelX;
			ballVelX = abs(ballVelX);
		}
		if (collisionPlayerRight(posPlayer) && ballVelX > 0) {
			posBall.x -= ballVelX;
			ballVelX = -abs(ballVelX);
		}
		else if (collisionPlayerLeft(posPlayer) && ballVelX < 0) {
			posBall.x -= ballVelX;
			ballVelX = abs(ballVelX);
		}
	}
	if (posBall.y != 0 && posBall.x != 0 && this->currentRoom != 0) {
		if (posBall.y > 756.0 - (24 * 8 * (this->currentRoom - 1))) {
			this->crossingRoom = -1;
			this->currentRoom -= 1;
		}
		else if (posBall.y < 756.0 - (24 * 8 * (this->currentRoom))) {
			this->currentRoom += 1;
			this->crossingRoom = 1;
		}

	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
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

bool Ball::getGetAllMoney() {
	return this->getAllMoney;
}
int Ball::getCurrentMoney() {
	return this->currentMoney;
}
int Ball::getCurrentPoints() {
	return this->currentPoints;
}

void Ball::setCrossingRoom(int c) {
	this->crossingRoom = c;
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
