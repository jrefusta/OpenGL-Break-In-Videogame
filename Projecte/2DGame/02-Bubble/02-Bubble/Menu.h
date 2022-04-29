#pragma once
#include "Scene.h"
#include "Game.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Menu : public Scene
{
public:
	Menu();
	~Menu();

	void init(int level);
	void update(int deltaTime);
	void render();
	void loadSprites();

private:
	struct Option {
		Sprite* sprite;
		Texture spritesheet;
	};

	Option options[7];
	int option; //opcion seleccionada

};

