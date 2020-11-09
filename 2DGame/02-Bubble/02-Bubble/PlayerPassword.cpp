#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "PlayerPassword.h"
#include "Game.h"






void PlayerPassword::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/player_password.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(28, 31), glm::vec2(0.25, 1.0), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.25f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.75f, 0.f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayerPassword.x), float(tileMapDispl.y + posPlayerPassword.y)));
	posPlayerPasswordAux = glm::vec2(20.0, -111.0);
	
}

void PlayerPassword::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (posPlayerPassword.x < 159) posPlayerPasswordAux.x += 1/7.5;
	posPlayerPassword = posPlayerPasswordAux;
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayerPassword.x), float(tileMapDispl.y + posPlayerPassword.y)));
}

void PlayerPassword::render()
{
	sprite->render();
}

void PlayerPassword::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void PlayerPassword::setPosition(const glm::vec2 &pos)
{
	posPlayerPassword = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayerPassword.x), float(tileMapDispl.y + posPlayerPassword.y)));
}




