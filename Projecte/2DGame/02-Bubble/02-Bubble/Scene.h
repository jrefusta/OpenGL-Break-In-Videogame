#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"

class Scene
{

public:
	void virtual init(int level);
	void virtual init(int level, int p, int m, int l);
	void virtual init(int p, int m, int l);
	void virtual init();
	void virtual update(int deltaTime);
	void virtual render();
protected:
	void initShaders();

protected:
	Sprite* background;
	Texture spritesheet;
	ShaderProgram texProgram;
	glm::mat4 projection;

	float currentTime;
	float currentTurnTime;



};