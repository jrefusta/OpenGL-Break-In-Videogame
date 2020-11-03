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

#define INIT_INFO_X 200
#define INIT_INFO_Y 576


Level::Level()
{
	map = NULL;
	player = NULL;
	ball = NULL;
	info = NULL;
}

Level::~Level()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
	if (ball != NULL)
		delete ball;
	if (info != NULL)
		delete info;
}


void Level::init(int ID)
{

	initShaders();
	this->currentLevel = ID;
	map = TileMap::createTileMap("levels/level0" + to_string(ID) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, ID);
	player->setPosition(glm::vec2(INIT_PLAYER_X, INIT_PLAYER_Y));
	player->setTileMap(map);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, ID);
	ball->setTileMap(map);
	ball->setStuck(true);
	info = new Info();
	info->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	info->setPosition(glm::vec2(INIT_INFO_X, INIT_INFO_Y));
	info->setTileMap(map);
	for (int i = 0; i < 7; ++i) {
		money[i] = new Digit();
		money[i]->init(glm::ivec2(0, 0), texProgram);
		money[i]->setPosition(glm::vec2(208 + 8*i, 608));
		money[i]->setTileMap(map);
	}
	for (int i = 0; i < 7; ++i) {
		points[i] = new Digit();
		points[i]->init(glm::ivec2(0, 0), texProgram);
		points[i]->setPosition(glm::vec2(208 + 8*i, 648));
		points[i]->setTileMap(map);
	}
	for (int i = 0; i < 2; ++i) {
		lives[i] = new Digit();
		lives[i]->init(glm::ivec2(0, 0), texProgram);
		lives[i]->setPosition(glm::vec2(248 + 8*i, 696));
		lives[i]->setTileMap(map);
	}
	for (int i = 0; i < 2; ++i) {
		bank[i] = new Digit();
		bank[i]->init(glm::ivec2(0, 0), texProgram);
		bank[i]->setPosition(glm::vec2(248 + 8*i, 728));
		bank[i]->setTileMap(map);
	}
	for (int i = 0; i < 2; ++i) {
		room[i] = new Digit();
		room[i]->init(glm::ivec2(0, 0), texProgram);
		room[i]->setPosition(glm::vec2(248 + 8*i, 784));
		room[i]->setTileMap(map);
	}
	frameSpritesheet.loadFromFile("images/frame.png", TEXTURE_PIXEL_FORMAT_RGBA);
	frameSprite = Sprite::createSprite(glm::ivec2(272, 240), glm::vec2(1.f, 1.f), &frameSpritesheet, &texProgram);
	frameSprite->setPosition(glm::vec2(0.0, 576.0));
	currentTime = 0.0f;
	currentRoom = 1;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(4 - currentRoom), 192.f*float(4 - currentRoom));
	livesNum = 4;
	currentTurnTime = 0.0f;
}

void Level::update(int deltaTime)
{
	currentTime += deltaTime;
	currentTurnTime += deltaTime;
	this->currentRoom = ball->getCurrentRoom();
	player->update(deltaTime, currentRoom);
	ball->update(deltaTime, player->getPosition(), currentRoom);
	info->update(deltaTime);
	info->setPosition(glm::vec2(INIT_INFO_X, INIT_INFO_Y - 192.f*float(currentRoom - 1)));
	for (int i = 0; i < 7; ++i) {
		int animId = ball->getCurrentMoney()/int(pow(10, 7 - 1 - i))%10;
		money[i]->update(deltaTime, animId);
		money[i]->setPosition(glm::vec2(208 + 8*i, 608 - 192.f*float(currentRoom - 1)));
	}
	for (int i = 0; i < 7; ++i) {
		int animId = ball->getCurrentPoints()/int(pow(10, 7 - 1 - i))%10;
		points[i]->update(deltaTime, animId);
		points[i]->setPosition(glm::vec2(208 + 8*i, 648 - 192.f*float(currentRoom - 1)));
	}
	for (int i = 0; i < 2; ++i) {
		int animId = livesNum/int(pow(10, 2 - 1 - i))%10;
		lives[i]->update(deltaTime, animId);
		lives[i]->setPosition(glm::vec2(248 + 8*i, 696 - 192.f*float(currentRoom - 1)));
	}
	for (int i = 0; i < 2; ++i) {
		int animId = currentLevel/int(pow(10, 2 - 1 - i))%10;
		bank[i]->update(deltaTime, animId);
		bank[i]->setPosition(glm::vec2(248 + 8*i, 728 - 192.f*float(currentRoom - 1)));
	}
	for (int i = 0; i < 2; ++i) {
		int animId = currentRoom/int(pow(10, 2 - 1 - i))%10;
		room[i]->update(deltaTime, animId);
		room[i]->setPosition(glm::vec2(248 + 8*i, 784 - 192.f*float(currentRoom - 1)));
	}
	frameSprite->setPosition(glm::vec2(0.0, 576.0 - 192.0*float(currentRoom - 1)));
	if (ball->getGetAllMoney()) {
		/*Game::instance().runConsole();
		cout << "YOU WIN" << endl;*/
	}
	if (ball->getGetAlarmHit()) {
		/*Game::instance().runConsole();
		cout << "ALARM HITED" << endl;*/
	}

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
	/*if (this->currentRoom == 1)
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
	}*/
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(4 - currentRoom), 192.f*float(4 - currentRoom));

	if (Game::instance().getKey('\ ') || Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		ball->setStuck(false);
	}
	if (Game::instance().getKey('0'))
	{
		this->init(this->currentLevel);
	}
	if (Game::instance().getKey('1')) currentRoom = 1;
	if (Game::instance().getKey('2')) currentRoom = 2;
	if (Game::instance().getKey('3')) currentRoom = 3;
	if (Game::instance().getKey('4')) currentRoom = 4;
	if (currentTurnTime >= float(300.0f)) {
		if (Game::instance().getKey('u') || Game::instance().getKey('U')) {//upper Layer
			if (this->currentRoom > 0 && this->currentRoom < 4) {
				ball->setStuck(false);
				ball->setPosition(player->getPosition() + glm::vec2(5.f, -201.f));
				currentTurnTime = 0;
			}
		}
		if (Game::instance().getKey('g') || Game::instance().getKey('G')) {//God mode
			if (this->currentRoom > 0 && this->currentRoom < 4) {
				ball->setStuck(false);
				ball->setPosition(player->getPosition() + glm::vec2(5.f, -201.f));
				currentTurnTime = 0;
			}
		}
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
	ball->render();
	player->render();
	frameSprite->render();
	info->render();
	for (int i = 0; i < 7; ++i) money[i]->render();
	for (int i = 0; i < 7; ++i) points[i]->render();
	for (int i = 0; i < 2; ++i) lives[i]->render();
	for (int i = 0; i < 2; ++i) bank[i]->render();
	if (currentRoom > 1 || int(currentTime/500)%2 == 0) {
		for (int i = 0; i < 2; ++i) room[i]->render();
	}
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



