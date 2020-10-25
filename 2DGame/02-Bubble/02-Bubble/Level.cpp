#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 8
#define SCREEN_Y 24

#define INIT_PLAYER_X_TILES 12
#define INIT_PLAYER_Y_TILES 94

#define NUM_ROOMS 4


Level::Level()
{
	map = NULL;
	player = NULL;
	ball = NULL;
}

Level::~Level()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
}


void Level::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(player->getPosition());
	ball->setTileMap(map);
	ball->setStuck(true);
	currentTime = 0.0f;
	currentRoom = 1;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(NUM_ROOMS - currentRoom), 192.f*float(NUM_ROOMS - currentRoom));
}

void Level::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	ball->update(deltaTime);

	if (ball->getStuck()) {
		ball->setPosition(player->getPosition());
	}

	if (Game::instance().getKey('1'))
	{
		currentRoom = 1;
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(NUM_ROOMS - currentRoom), 192.f*float(NUM_ROOMS - currentRoom));
	}
	if (Game::instance().getKey('2'))
	{
		currentRoom = 2;
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(NUM_ROOMS - currentRoom), 192.f*float(NUM_ROOMS - currentRoom));
	}
	if (Game::instance().getKey('3'))
	{
		currentRoom = 3;
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(NUM_ROOMS - currentRoom), 192.f*float(NUM_ROOMS - currentRoom));
	}
	if (Game::instance().getKey('4'))
	{
		currentRoom = 4;
		projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(NUM_ROOMS - currentRoom), 192.f*float(NUM_ROOMS - currentRoom));
	}
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
	player->render();
	ball->render();
}

void Level::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



