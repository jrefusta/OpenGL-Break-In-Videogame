#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


enum PlayerAnims
{
	                    LOOK_UP_LEFT_2,     LOOK_UP_LEFT_1,     LOOK_UP_CENTER,     LOOK_UP_RIGHT_1,     LOOK_UP_RIGHT_2,
	LOOK_CENTER_LEFT_3, LOOK_CENTER_LEFT_2, LOOK_CENTER_LEFT_1, LOOK_CENTER_CENTER, LOOK_CENTER_RIGHT_1, LOOK_CENTER_RIGHT_2, LOOK_CENTER_RIGHT_3,
	LOOK_DOWN_LEFT_3,   LOOK_DOWN_LEFT_2,   LOOK_DOWN_LEFT_1,   LOOK_DOWN_CENTER,   LOOK_DOWN_RIGHT_1,   LOOK_DOWN_RIGHT_2,   LOOK_DOWN_RIGHT_3,
	CLOSE
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int ID)
{
	if (ID == 4) spritesheet.loadFromFile("images/cop.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(19, 26), glm::vec2(1/7.0, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(20);
	
		sprite->setAnimationSpeed(LOOK_UP_LEFT_2, 8);
		sprite->addKeyframe(LOOK_UP_LEFT_2, glm::vec2(1/7.f, 0.f));
		
		sprite->setAnimationSpeed(LOOK_UP_LEFT_1, 8);
		sprite->addKeyframe(LOOK_UP_LEFT_1, glm::vec2(2/7.f, 0.f));

		sprite->setAnimationSpeed(LOOK_UP_CENTER, 8);
		sprite->addKeyframe(LOOK_UP_CENTER, glm::vec2(3/7.f, 0.f));

		sprite->setAnimationSpeed(LOOK_UP_RIGHT_1, 8);
		sprite->addKeyframe(LOOK_UP_RIGHT_1, glm::vec2(4/7.f, 0.f));

		sprite->setAnimationSpeed(LOOK_UP_RIGHT_2, 8);
		sprite->addKeyframe(LOOK_UP_RIGHT_2, glm::vec2(5/7.f, 0.f));

		sprite->setAnimationSpeed(LOOK_CENTER_LEFT_3, 8);
		sprite->addKeyframe(LOOK_CENTER_LEFT_3, glm::vec2(0.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_CENTER_LEFT_2, 8);
		sprite->addKeyframe(LOOK_CENTER_LEFT_2, glm::vec2(1/7.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_CENTER_LEFT_1, 8);
		sprite->addKeyframe(LOOK_CENTER_LEFT_1, glm::vec2(2/7.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_CENTER_CENTER, 8);
		sprite->addKeyframe(LOOK_CENTER_CENTER, glm::vec2(3/7.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_CENTER_RIGHT_1, 8);
		sprite->addKeyframe(LOOK_CENTER_RIGHT_1, glm::vec2(4/7.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_CENTER_RIGHT_2, 8);
		sprite->addKeyframe(LOOK_CENTER_RIGHT_2, glm::vec2(5/7.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_CENTER_RIGHT_3, 8);
		sprite->addKeyframe(LOOK_CENTER_RIGHT_3, glm::vec2(6/7.f, 0.25f));

		sprite->setAnimationSpeed(LOOK_DOWN_LEFT_3, 8);
		sprite->addKeyframe(LOOK_DOWN_LEFT_3, glm::vec2(0.f, 0.5f));

		sprite->setAnimationSpeed(LOOK_DOWN_LEFT_2, 8);
		sprite->addKeyframe(LOOK_DOWN_LEFT_2, glm::vec2(1/7.f, 0.5f));

		sprite->setAnimationSpeed(LOOK_DOWN_LEFT_1, 8);
		sprite->addKeyframe(LOOK_DOWN_LEFT_1, glm::vec2(2/7.f, 0.5f));

		sprite->setAnimationSpeed(LOOK_DOWN_CENTER, 8);
		sprite->addKeyframe(LOOK_DOWN_CENTER, glm::vec2(3/7.f, 0.5f));

		sprite->setAnimationSpeed(LOOK_DOWN_RIGHT_1, 8);
		sprite->addKeyframe(LOOK_DOWN_RIGHT_1, glm::vec2(4/7.f, 0.5f));

		sprite->setAnimationSpeed(LOOK_DOWN_RIGHT_2, 8);
		sprite->addKeyframe(LOOK_DOWN_RIGHT_2, glm::vec2(5/7.f, 0.5f));

		sprite->setAnimationSpeed(LOOK_DOWN_RIGHT_3, 8);
		sprite->addKeyframe(LOOK_DOWN_RIGHT_3, glm::vec2(6/7.f, 0.5f));

		sprite->setAnimationSpeed(CLOSE, 8);
		sprite->addKeyframe(CLOSE, glm::vec2(0.f, 0.75f));
		sprite->addKeyframe(CLOSE, glm::vec2(1/7.f, 0.75f));
		sprite->addKeyframe(CLOSE, glm::vec2(2/7.f, 0.75f));
		sprite->addKeyframe(CLOSE, glm::vec2(3/7.f, 0.75f));
		sprite->addKeyframe(CLOSE, glm::vec2(4/7.f, 0.75f));
		sprite->addKeyframe(CLOSE, glm::vec2(5/7.f, 0.75f));
		sprite->addKeyframe(CLOSE, glm::vec2(6/7.f, 0.75f));
		
	sprite->changeAnimation(LOOK_DOWN_CENTER);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime, int currentRoom)
{
	//int currentRoom = 1;
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		/*if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);*/
		if (posPlayer.x > 8) posPlayer.x -= 4;
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		//if (sprite->animation() != MOVE_RIGHT)
			//sprite->changeAnimation(MOVE_RIGHT);
		if (posPlayer.x < 166) posPlayer.x += 4;
	}/*
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}*/

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		/*if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);*/
		if (posPlayer.y < 166 + 192*(4 - currentRoom)) posPlayer.y += 2;
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		/*if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);*/
		if (posPlayer.y > 20 + 192*(4 - currentRoom)) posPlayer.y -= 2;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition()
{
	return this->posPlayer;
}



