#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Level.h"
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
	thief = NULL;
	guard = NULL;
	playerPassword = NULL;
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
	if (thief != NULL)
		delete thief;
	if (guard != NULL)
		delete guard;
	if (playerPassword != NULL)
		delete playerPassword;
}


void Level::init(int ID, int pointsP, int moneyP, int livesP)
{
	initShaders();
	currentLevel = ID;
	Game::instance().stopMusic();
	map = TileMap::createTileMap("levels/level0" + to_string(currentLevel) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, currentLevel);
	player->setPosition(glm::vec2(INIT_PLAYER_X, INIT_PLAYER_Y));
	player->setTileMap(map);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, currentLevel);
	ball->setTileMap(map);
	ball->setStuck(true);
	ball->setCurrentPoints(pointsP);
	ball->setCurrentMoney(moneyP);
	if (currentLevel < 3) winSpritesheet.loadFromFile("images/password.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else winSpritesheet.loadFromFile("images/password_win.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winSprite = Sprite::createSprite(glm::ivec2(272.0, 240.0), glm::vec2(1.f, 1.f), &winSpritesheet, &texProgram);
	winSprite->setPosition(glm::vec2(0.0, 576.0 - 192.0 * float(5 - 1) - 48));
	loseSpritesheet.loadFromFile("images/game_over.png", TEXTURE_PIXEL_FORMAT_RGBA);
	loseSprite = Sprite::createSprite(glm::ivec2(272.0, 240.0), glm::vec2(1.f, 1.f), &loseSpritesheet, &texProgram);
	loseSprite->setPosition(glm::vec2(0.0, 192.0 * float(5 - 1) + 48));
	frameSpritesheet.loadFromFile("images/frame.png", TEXTURE_PIXEL_FORMAT_RGBA);
	frameSprite = Sprite::createSprite(glm::ivec2(272, 240), glm::vec2(1.f, 1.f), &frameSpritesheet, &texProgram);
	frameSprite->setPosition(glm::vec2(0.0, 576.0));
	alarmSpritesheet.loadFromFile("images/alarm.png", TEXTURE_PIXEL_FORMAT_RGBA);
	alarmSprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f, 1.f), &alarmSpritesheet, &texProgram);
	if (currentLevel == 2) alarmSprite->setPosition(glm::vec2(168.0, 120.0));
	else if (currentLevel == 3) alarmSprite->setPosition(glm::vec2(104.0, 472.0));
	info = new Info();
	info->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	info->setPosition(glm::vec2(INIT_INFO_X, INIT_INFO_Y));
	info->setTileMap(map);
	if (currentLevel < 4) batmodeSpritesheet.loadFromFile("images/text_batmode_thief.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else batmodeSpritesheet.loadFromFile("images/text_batmode_cop.png", TEXTURE_PIXEL_FORMAT_RGBA);
	batmodeSprite = Sprite::createSprite(glm::ivec2(56, 16), glm::vec2(1.f, 1.f), &batmodeSpritesheet, &texProgram);
	batmodeSprite->setPosition(glm::vec2(208.0, 752.0));
	if (currentLevel == 4) {
		thief = new Thief();
		thief->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		thief->setPosition(glm::vec2(INIT_PLAYER_X+55, INIT_PLAYER_Y - 150));
		thief->setTileMap(map);
	}
	else {
		thief = new Thief();
		thief->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		thief->setPosition(glm::vec2(0, 0));
		thief->setTileMap(map);
	}
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
	playerPassword = new PlayerPassword();
	playerPassword->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	playerPassword->setPosition(glm::vec2(20, -111));
	playerPassword->setTileMap(map);
	currentTime = 0.0f;
	currentRoom = 1;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) + 192.f*float(4 - currentRoom), 192.f*float(4 - currentRoom));
	livesNum = livesP;
	pointsNum = pointsP;
	moneyNum = moneyP;
	currentTurnTime = 0.0f;
	cameraVelocity = 24.0f;
	topCamera = 192.f * float(4 - currentRoom);
	bottomCamera = float(SCREEN_HEIGHT) + 192.f * float(4 - currentRoom);
	winState = loseState = false;
	exitMenu = false;
	exitCredits = false;
	loseTransition = false;
	passwordTime = -1;
	soundYet = false;
}

void Level::update(int deltaTime)
{
	currentTime += deltaTime;
	currentTurnTime += deltaTime;
	currentRoom = ball->getCurrentRoom();
	player->update(deltaTime, currentRoom, ball->getPosBall());

	if (exitCredits) {
		Game::instance().selectScene(3);
	}
	else {
		if (!winState && !loseState && !loseTransition) {
			info->update(deltaTime);
			//player->update(deltaTime, currentRoom, ball->getPosBall());
			ball->update(deltaTime, player->getPosition(), thief->getPosition(), currentRoom);
		}
		//if (!loseTransition) deadTime = (currentTime);
		if (loseTransition) {
			if (deadTime < 0) deadTime = currentTime;
			player->setDead(true);
			if (!soundYet) {
				Game::instance().stopMusic();
				Game::instance().playSound("music/DefeatSound.mp3");
				soundYet = true;
			}
			bool done = false;
			if (currentTime - deadTime >= 1000) {
				player->setDead(false);
				if (!ball->getGodMode()) --livesNum;
				loseTransition = false;
				done = true;
				deadTime = -1.f;
				soundYet = false;
				ball->setInitVel();
			}
			if (done) {
				Game::instance().stopMusic();
				if ((currentLevel == 2 && currentRoom == 4) || (currentLevel == 3 && currentRoom == 2) || currentLevel == 4) {
					if (currentLevel == 2) guard->setPosition(glm::vec2(10, 181));
					else if (currentLevel == 3) guard->setPosition(glm::vec2(10, 565));
					if (ball->getGetAlarmHit()) {
						guard = NULL;
						ball->setAlarmHit(false);
					}
					player->setPosition(glm::vec2(INIT_PLAYER_X, INIT_PLAYER_Y - (192 * (currentRoom - 1))));
					ball->setPosition(player->getPosition() + glm::vec2(5.f, -9.f));
					ball->setStuck(true);
				}
				else {
					player->setPosition(glm::vec2(INIT_PLAYER_X, INIT_PLAYER_Y));
					ball->setPosition(player->getPosition() + glm::vec2(5.f, -9.f));
					ball->setCurrentRoom(1);
					ball->setStuck(true);
					currentRoom = ball->getCurrentRoom();
				}
			}
		}
		if (winState) {
			if (passwordTime < 0) passwordTime = currentTime;
			guard = NULL;
			if (ball->getGetAlarmHit()) ball->setAlarmHit(false);
			Game::instance().loopMusic("music/WinSong.mp3");
			playerPassword->update(deltaTime);
			if (topCamera > 192.f * float(4 - 5) - 48) {
				topCamera -= cameraVelocity;
				bottomCamera -= cameraVelocity;
				projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(bottomCamera), float(topCamera));
			}
			if (currentTurnTime >= float(300.0f)) {
				if (Game::instance().getKey('\ ') || currentTime - passwordTime >= 17500)
				{
					if (currentLevel < 3) {
						winState = false;
						init(currentLevel + 1, ball->getCurrentPoints(), ball->getCurrentMoney(), livesNum);
					}
					else {
						exitCredits = true;
					}
					winState = false;
					currentTurnTime = 0.0f;
				}
			}
		}
		if (loseState) {
			if (topCamera < 192.f * float(4) + 48) {
				topCamera += cameraVelocity;
				bottomCamera += cameraVelocity;
				projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(bottomCamera), float(topCamera));
			}
			if (currentTurnTime >= float(300.0f)) {
				if (Game::instance().getKey('\ '))
				{
					exitMenu = true;
					currentTurnTime = 0.0f;
				}
			}
		}
		int offset = (winState) ? -48 : 0;
		int aux = (currentRoom == 0) ? 1 : currentRoom;
		if (winState) aux = 5;
		frameSprite->setPosition(glm::vec2(0.0, 576.0 - 192.0 * float(aux-1) + offset));
		info->setPosition(glm::vec2(INIT_INFO_X, INIT_INFO_Y - 192.f * float(aux - 1) + offset));
		batmodeSprite->setPosition(glm::vec2(208.0, 752.0 - 192.0 * float(aux-1) + offset));
		if (currentLevel == 4) thief->update(deltaTime, currentRoom);
		for (int i = 0; i < 7; ++i) {
			int animId = ball->getCurrentMoney() / int(pow(10, 7 - 1 - i)) % 10;
			money[i]->update(deltaTime, animId);
			money[i]->setPosition(glm::vec2(208 + 8 * i, 608 - 192.f * float(aux - 1) + offset));
		}
		for (int i = 0; i < 7; ++i) {
			int animId = ball->getCurrentPoints() / int(pow(10, 7 - 1 - i)) % 10;
			points[i]->update(deltaTime, animId);
			points[i]->setPosition(glm::vec2(208 + 8 * i, 648 - 192.f * float(aux - 1) + offset));
		}
		for (int i = 0; i < 2; ++i) {
			if (livesNum >= 0) {
				int animId = livesNum / int(pow(10, 2 - 1 - i)) % 10;
				lives[i]->update(deltaTime, animId);
				lives[i]->setPosition(glm::vec2(248 + 8 * i, 696 - 192.f * float(aux - 1) + offset));
			}
		}
		for (int i = 0; i < 2; ++i) {
			int animId = currentLevel / int(pow(10, 2 - 1 - i)) % 10;
			bank[i]->update(deltaTime, animId);
			bank[i]->setPosition(glm::vec2(248 + 8 * i, 728 - 192.f * float(aux - 1) + offset));
		}
		for (int i = 0; i < 2; ++i) {
			int actualRoom = currentRoom;
			if (actualRoom == 0) actualRoom = 1;
			else if (actualRoom == 5) actualRoom = 4;
			int animId = actualRoom / int(pow(10, 2 - 1 - i)) % 10;
			room[i]->update(deltaTime, animId);
			room[i]->setPosition(glm::vec2(248 + 8 * i, 784 - 192.f * float(aux - 1) + offset));
		}
		if (currentLevel == 4) {
			if (ball->getThiefShooted()) {
				ball->setThiefShooted(false);
				if (thief->getCollisionActive()) {
					thief->setLives(thief->getLives() - 1);
				}
			}
			if (thief->getThiefBeaten() && ball->getPosBall().y < -16) {
				winState = true;
			}
			if (!ball->getGodMode()) {
				if (ball->getCurrentMoney() >= 1000 && livesNum == 4) {
					loseTransition = true;
				}
				if (ball->getCurrentMoney() >= 2000 && livesNum == 3) {
					loseTransition = true;
				}
				if (ball->getCurrentMoney() >= 3000 && livesNum == 2) {
					loseTransition = true;
				}
				if (ball->getCurrentMoney() >= 4000 && livesNum == 1) {
					loseTransition = true;
				}
				if (ball->getCurrentMoney() >= 5000 && livesNum == 0) {
					loseTransition = true;
				}
			}
		}
		if (ball->getGetAllMoney()) {
			winState = true;
		}
		if (ball->getGetAlarmHit()) {
			if ((currentLevel == 2 && currentRoom == 4) || (currentLevel == 3 && currentRoom == 2)) {
				Game::instance().loopMusic("music/AlarmSound.mp3");
				if (guard == NULL) {
					guard = new Guard();
					guard->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					if (currentLevel == 2) guard->setPosition(glm::vec2(10, 181));
					else if (currentLevel == 3) guard->setPosition(glm::vec2(10, 565));
					guard->setTileMap(map);
				}
				else {
					guard->update(deltaTime, player->getPosition());
					if (guard->getCollisionGuard(player->getPosition())) {
						loseTransition = true;
					}
				}
			}
			else {
				Game::instance().stopMusic();
				guard = NULL;
			}
		}

		if (ball->getStuck()) {
			ball->setPosition(player->getPosition() + glm::vec2(5.f, -9.f));
			if (stuckTime < 0) stuckTime = currentTime;
			if (currentTime - stuckTime >= 2500 && ball->getStuck()) {
				ball->setStuck(false);
				stuckTime = -1;
			}

		}

		if (ball->getCrossingRoom() == 1) {
			player->setPosition(player->getPosition() + glm::vec2(0, -192));
			ball->setCrossingRoom(0);
		}
		if (ball->getCrossingRoom() == -1) {
			if (ball->getCurrentRoom() != 0) {
				player->setPosition(player->getPosition() + glm::vec2(0, 192));
			}
			ball->setCrossingRoom(0);
		}

		if (livesNum == -1) {
			loseState = true;
			livesNum = 0;
		}
		if (currentRoom == 0 && !loseTransition)
		{
			loseTransition = true;
		}
		if (currentRoom <= 4 && currentRoom != 0 && !winState && !loseState && !exitMenu) {
			if (topCamera > 192.f * float(4 - currentRoom)) {
				topCamera -= cameraVelocity;
				bottomCamera -= cameraVelocity;
				projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(bottomCamera), float(topCamera));
			}
			else if (topCamera < 192.f * float(4 - currentRoom)) {
				topCamera += cameraVelocity;
				bottomCamera += cameraVelocity;
				projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(bottomCamera), float(topCamera));
			}
		}
		if (currentTurnTime >= float(300.0f)) {
			if (Game::instance().getKey('u') || Game::instance().getKey('U')) {//upper Layer
				if (currentRoom > 0 && currentRoom < 4) {
					ball->destroyedTop();
					ball->setStuck(false);
					ball->setPosition(player->getPosition() + glm::vec2(5.f, -201.f));
					currentTurnTime = 0;
				}
			}
			else if (Game::instance().getKey('\ ') || Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				stuckTime = -1;
				ball->setStuck(false);
				currentTurnTime = 0;
			}
			else if (Game::instance().getKey('g') || Game::instance().getKey('G')) {//God mode
				if (!ball->getGodMode()) {
					Game::instance().playSound("music/PhoneSound.mp3");
				}
				else {
					Game::instance().playSound("music/InvertPhoneSound.mp3");
				}
				ball->setGodMode(!ball->getGodMode());
				currentTurnTime = 0;
			}
			else if (Game::instance().getKey('r') || Game::instance().getKey('R'))
			{
				init(currentLevel, 0, 0, 4);
				currentTurnTime = 0;
			}
			else if (Game::instance().getKey(27) || exitMenu) {
				Game::instance().selectScene(1);
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
	if (ball->getGetAlarmHit() && int(currentTime / 10) % 2 == 1) alarmSprite->render();
	ball->render();
	player->render();
	if (currentLevel == 4) thief->render();
	winSprite->render();
	frameSprite->render();
	info->render();
	batmodeSprite->render();
	loseSprite->render();
	for (int i = 0; i < 7; ++i) money[i]->render();
	for (int i = 0; i < 7; ++i) points[i]->render();
	for (int i = 0; i < 2; ++i) lives[i]->render();
	for (int i = 0; i < 2; ++i) bank[i]->render();
	if (currentRoom > 1 || int(currentTime/500)%2 == 0) {
		for (int i = 0; i < 2; ++i) room[i]->render();
	}
	if (guard != NULL && ball->getGetAlarmHit()) guard->render();
	if (winState) playerPassword->render();
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



