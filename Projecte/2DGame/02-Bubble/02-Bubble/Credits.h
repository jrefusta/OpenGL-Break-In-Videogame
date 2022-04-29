#pragma once
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Credits : public Scene
{
public:
	Credits();
	~Credits();

	void init();
	void update(int deltaTime);
	void render();
private:
	Texture escSpritesheet;
	Sprite* escSprite;
};

