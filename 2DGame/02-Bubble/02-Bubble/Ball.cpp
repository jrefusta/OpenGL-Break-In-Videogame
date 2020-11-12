#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int ID)
{
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(9, 10), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	this->shaderProgram = shaderProgram;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	setInitVel();
	currentRoom = 1;
	crossingRoom = 0;
	getAllMoney = false;
	currentMoney = 0;
	currentPoints = 0;
	stuck = true;
	currentLevel = ID;
	alarmHit = false;
	thiefShooted = false;
	godMode = false;
	initMoney = initPoints = 0;
}

void Ball::update(int deltaTime, glm::vec2 posPlayer, glm::vec2 posThief, int currentRoom)
{
	sprite->update(deltaTime);
	if (map->getTotalMoney() == 0) {
		this->getAllMoney = true;
	}
	this->alarmHit = map->getAlarmHited();
	this->currentMoney = map->getMoney() + initMoney;
	this->currentPoints = map->getPoints() + initPoints;
	if (!this->getStuck()) {
		posBall.y += ballVelY;
		if (map->collisionMoveUp(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom, this->currentLevel) && ballVelY < 0) {
			posBall.y -= ballVelY;
			ballVelY = abs(ballVelY);
		}
		else if (map->collisionMoveDown(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom, this->currentLevel)) {
			posBall.y -= ballVelY;
			ballVelY = -abs(ballVelY);
		}

		if (collisionPlayer(posPlayer) && ballVelY > 0) {
			Game::instance().playSound("music/PlayerSound.mp3");
			posBall.y -= ballVelY;
			if (posPlayer.x - posBall.x >= 7) {
				ballVelY = -0.5;
				ballVelX = -2;
			}
			else if (posPlayer.x - posBall.x >= 0) {
				ballVelY = -2;
				ballVelX = -2;
			}
			else if (posPlayer.x - posBall.x >= -4) {
				ballVelY = -2;
				ballVelX = -0.5;
			}
			else if (posPlayer.x - posBall.x >= -8) {
				ballVelY = -2;
				ballVelX = 0.5;
			}
			else if (posPlayer.x - posBall.x >= -14) {
				ballVelY = -2;
				ballVelX = 2;
			}
			else {
				ballVelY = -0.5;
				ballVelX = 2;
			}
		}

		if (collisionThief(posThief) && ballVelY < 0) {
			Game::instance().playSound("music/PlayerSound.mp3");
			posBall.y -= ballVelY;
			ballVelY = abs(ballVelY);
			thiefShooted = true;
		}
		posBall.x += ballVelX;

		if (map->collisionMoveRight(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom, this->currentLevel)) {
			posBall.x -= ballVelX;
			ballVelX = -abs(ballVelX);

		}
		else if (map->collisionMoveLeft(posBall, glm::ivec2(9, 10), tileMapDispl, shaderProgram, currentRoom, this->currentLevel)) {
			posBall.x -= ballVelX;
			ballVelX = abs(ballVelX);
		}
		if (!godMode) {
			if (collisionPlayer(posPlayer) && ballVelX > 0 && ballVelY > 0) {
				posBall.x -= ballVelX;
				ballVelX = -abs(ballVelX);
				Game::instance().playSound("music/PlayerSound.mp3");
			}
			else if (collisionPlayer(posPlayer) && ballVelX < 0 && ballVelY > 0) {
				posBall.x -= ballVelX;
				ballVelX = abs(ballVelX);
				Game::instance().playSound("music/PlayerSound.mp3");
			}
		}
	}
	if (posBall.y != 0 && posBall.x != 0 && this->currentRoom != 0) {
		if (posBall.y > 752.0 - (24*8*(this->currentRoom - 1))) {
			this->crossingRoom = -1;
			this->currentRoom -= 1;
		}
		else if (posBall.y < 752.0 - (24*8*(this->currentRoom))) {
			this->currentRoom += 1;
			this->crossingRoom = 1;
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::getGetAllMoney() {
	return this->getAllMoney;
}

bool Ball::getGetAlarmHit() {
	return this->alarmHit;
}

void Ball::destroyedTop() {
	map->destroyTop(tileMapDispl, shaderProgram, currentRoom);
}

int Ball::getCurrentMoney() {
	return this->currentMoney;
}

int Ball::getCurrentPoints() {
	return this->currentPoints;
}

void Ball::setCurrentMoney(int m) {
	this->initMoney = m;
}

void Ball::setCurrentPoints(int p) {
	this->initPoints = p;
}

glm::vec2 Ball::getPosBall() {
	return posBall;
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
	bool collisionX = posPlayer.x + 18 >= posBall.x && posBall.x + 9 >= posPlayer.x;
	bool collisionY = posPlayer.y + 6 >= posBall.y && posBall.y + 10 >= posPlayer.y;
	return collisionX && collisionY;
}

bool Ball::collisionThief(glm::vec2& posThief) {
	bool collisionX = posThief.x + 18 >= posBall.x && posBall.x + 9 >= posThief.x;
	bool collisionY = (posThief.y+20) + 6 >= posBall.y && posBall.y + 10 >= (posThief.y+20);
	return collisionX && collisionY;
}

bool Ball::getStuck() {
	return this->stuck;
}

void Ball::setStuck(bool s) {
	this->stuck = s;
}

bool Ball::getThiefShooted() {
	return this->thiefShooted;
}

void Ball::setThiefShooted(bool t) {
	this->thiefShooted = t;
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Ball::setInitVel()
{
	ballVelY = -2;
	ballVelX = 0.5;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::setGodMode(bool g) {
	this->godMode = g;
}
bool Ball::getGodMode() {
	return this->godMode;
}

void Ball::setAlarmHit(bool a) {
	map->setAlarmHited(a);
	alarmHit = a;
}