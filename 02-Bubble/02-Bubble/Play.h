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

	void init();
	void update(int deltaTime);
	void render();
private:
};

