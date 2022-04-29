#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Digit.h"
#include "Game.h"

void Digit::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/digits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.2, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);
	
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		
	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.2f, 0.f));

	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0.4f, 0.f));

	sprite->setAnimationSpeed(3, 8);
	sprite->addKeyframe(3, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(4, 8);
	sprite->addKeyframe(4, glm::vec2(0.8f, 0.f));

	sprite->setAnimationSpeed(5, 8);
	sprite->addKeyframe(5, glm::vec2(0.f, 0.5f));

	sprite->setAnimationSpeed(6, 8);
	sprite->addKeyframe(6, glm::vec2(0.2f, 0.5f));

	sprite->setAnimationSpeed(7, 8);
	sprite->addKeyframe(7, glm::vec2(0.4f, 0.5f));

	sprite->setAnimationSpeed(8, 8);
	sprite->addKeyframe(8, glm::vec2(0.6f, 0.5f));

	sprite->setAnimationSpeed(9, 8);
	sprite->addKeyframe(9, glm::vec2(0.8f, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDigit.x), float(tileMapDispl.y + posDigit.y)));
}

void Digit::update(int deltaTime, int animId)
{
	sprite->update(deltaTime);
	sprite->changeAnimation(animId);
		
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDigit.x), float(tileMapDispl.y + posDigit.y)));
}

void Digit::render()
{
	sprite->render();
}

void Digit::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Digit::setPosition(const glm::vec2 &pos)
{
	posDigit = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDigit.x), float(tileMapDispl.y + posDigit.y)));
}