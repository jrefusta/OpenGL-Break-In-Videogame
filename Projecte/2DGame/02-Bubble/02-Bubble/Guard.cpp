#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Guard.h"
#include "Game.h"

void Guard::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bMoving = false;
	currentTime = 0;
	spritesheet.loadFromFile("images/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(12, 8), glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);
	
	sprite->setAnimationSpeed(0, 16);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.75f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.5f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
		
	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.5f, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuard.x), float(tileMapDispl.y + posGuard.y)));
	
}

void Guard::update(int deltaTime, glm::ivec2 posPlayer)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;
	if (currentTime/1000%5 < 2) bMoving = false;
	else bMoving = true;
	if(bMoving)
	{
		if (sprite->animation() == 0) sprite->changeAnimation(1);
		posGuardAux += glm::vec2(vecPlayer.x/180.0, vecPlayer.y/180.0);
		posGuard = posGuardAux;
	}
	else
	{
		if (sprite->animation() == 1) sprite->changeAnimation(0);
		vecPlayer = posPlayer - posGuard;
		posGuardAux = posGuard;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuard.x), float(tileMapDispl.y + posGuard.y)));
}

void Guard::render()
{
	sprite->render();
}

void Guard::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}


bool Guard::getCollisionGuard(glm::vec2 posPlayer) {
	bool collisionX = posPlayer.x + 18 >= posGuard.x && posGuard.x + 12 >= posPlayer.x;
	bool collisionY = posPlayer.y + 26 >= posGuard.y && posGuard.y + 8 >= posPlayer.y;
	return collisionX && collisionY;
}

void Guard::setPosition(const glm::vec2& pos)
{
	posGuard = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuard.x), float(tileMapDispl.y + posGuard.y)));
}

glm::ivec2 Guard::getPosition()
{
	return posGuard;
}

