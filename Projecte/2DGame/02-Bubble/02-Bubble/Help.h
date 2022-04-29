#pragma once
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Help : public Scene
{
public:
	Help();
	~Help();

	void init();
	void update(int deltaTime);
	void render();
	void printHelp();
private:
	int page;
	Texture escSpritesheet;
	Sprite* escSprite;
};

