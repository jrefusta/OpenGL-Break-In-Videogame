#pragma once
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Play : public Scene
{
public:
	Play();
	~Play();

	void init(int l, int points, int money, int lives);
	void update(int deltaTime);
	void render();
	void loadSprites();

private:
	struct Level {
		Sprite* sprite;
		Texture spritesheet;
	};

	int money, points, lives;

	Level levels[4];
	int level; //nivel seleccionado
	Texture escSpritesheet;
	Sprite* escSprite;
};

