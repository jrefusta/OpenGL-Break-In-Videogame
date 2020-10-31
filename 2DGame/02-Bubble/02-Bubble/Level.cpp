#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <string>
using namespace std;

#define SCREEN_X 8
#define SCREEN_Y 24

#define INIT_PLAYER_X 96
#define INIT_PLAYER_Y 742


Level::Level()
{
	map = NULL;
	player = NULL;
	ball = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
}


void Level::init(int ID)
{
	initShaders();
	map = TileMap::createTileMap("levels/level0" + to_string(ID) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X, INIT_PLAYER_Y));
	player->setTileMap(map);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setTileMap(map);
	ball->setStuck(true);
	currentTime = 0.0f;
	currentRoom = 1;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) + 192.f * float(4 - currentRoom), 192.f * float(4 - currentRoom));
	livesNum = 4;
}

void Level::update(int deltaTime)
{
	currentTime += deltaTime;
	currentTurnTime += deltaTime;
	player->update(deltaTime, currentRoom);
	ball->update(deltaTime, player->getPosition(), currentRoom);
	this->currentRoom = ball->getCurrentRoom();
	//livesText.render("Videogames!!!", glm::vec2(10, 450 - 20), 32, glm::vec4(1, 1, 1, 1));
	//livesText.render("Videogames!!!", glm::vec2(10, 450 - 20), 32, glm::vec4(1, 1, 1, 1));

	if (ball->getStuck()) {
		ball->setPosition(player->getPosition() + glm::vec2(5.f, -9.f));
	}

	if (ball->getCrossingRoom() == 1) {
		player->setPosition(player->getPosition() + glm::vec2(0, -192));
		ball->setCrossingRoom(0);
	}
	if (ball->getCrossingRoom() == -1) {
		if (this->currentRoom != 0) {
			player->setPosition(player->getPosition() + glm::vec2(0, 192));
		}
		ball->setCrossingRoom(0);
	}

	if (this->livesNum == -1) {
		Game::instance().runConsole();
		cout << "GAME OVER" << endl;
	}
	if (this->currentRoom == 0)
	{
		ball->setStuck(true);
		player->setPosition(glm::vec2(INIT_PLAYER_X, INIT_PLAYER_Y));
		ball->setPosition(player->getPosition() + glm::vec2(5.f, -9.f));
		ball->setCurrentRoom(1);
		this->currentRoom = ball->getCurrentRoom();
		--livesNum;
		Sleep(1500);
	}
	if (this->currentRoom == 1)
	{
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) + 192.f * float(4 - currentRoom), 192.f * float(4 - currentRoom));
	}
	if (this->currentRoom == 2)
	{
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) + 192.f * float(4 - currentRoom), 192.f * float(4 - currentRoom));
	}
	if (this->currentRoom == 3)
	{
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) + 192.f * float(4 - currentRoom), 192.f * float(4 - currentRoom));
	}
	if (this->currentRoom == 4)
	{
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1) + 192.f * float(4 - currentRoom), 192.f * float(4 - currentRoom));
	}
	if (Game::instance().getKey('\ ') || Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		ball->setStuck(false);
	}
	if (Game::instance().getKey('1')) currentRoom = 1;
	if (Game::instance().getKey('2')) currentRoom = 2;
	if (Game::instance().getKey('3')) currentRoom = 3;
	if (Game::instance().getKey('4')) currentRoom = 4;
}

void Level::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	ball->render();
	player->render();
}

void Level::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



