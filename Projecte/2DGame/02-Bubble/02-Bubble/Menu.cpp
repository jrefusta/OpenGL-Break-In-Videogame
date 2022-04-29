#include "Menu.h"
#include <string>
#include <iostream>
using namespace std;

Menu::Menu() {}
Menu:: ~Menu() {}

void Menu::init(int o) {
	Scene::init();
	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(272.0, 240.0), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0.0, 0.0));
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	loadSprites();
	this->option = o;
	Game::instance().loopMusic("music/Break_In_OST.mp3");
}

void Menu::loadSprites() {

	options[0].spritesheet.loadFromFile("images/text_play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	options[0].sprite = Sprite::createSprite(glm::ivec2(32, 8), glm::vec2(1.f, 1.f), &options[0].spritesheet, &texProgram);
	options[0].sprite->setPosition(glm::vec2(120.0, 160.0));
	options[0].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	options[1].spritesheet.loadFromFile("images/text_instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	options[1].sprite = Sprite::createSprite(glm::ivec2(96, 8), glm::vec2(1.f, 1.f), &options[1].spritesheet, &texProgram);
	options[1].sprite->setPosition(glm::vec2(88.0, 176.0));
	options[1].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	options[2].spritesheet.loadFromFile("images/text_credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	options[2].sprite = Sprite::createSprite(glm::ivec2(56, 8), glm::vec2(1.f, 1.f), &options[2].spritesheet, &texProgram);
	options[2].sprite->setPosition(glm::vec2(104.0, 192.0));
	options[2].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	currentTurnTime += deltaTime;

	if (currentTurnTime >= float(150.0f)) {
		if (Game::instance().moveDownPressed()) {
			Game::instance().playSound("music/BlockSound.mp3");
			if (option < 2) ++option;
			else {
				option = 0;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().moveUpPressed()) {
			Game::instance().playSound("music/BlockSound.mp3");
			if (option > 0) --option;
			else {
				option = 2;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().getKey(13)) {
			Game::instance().selectScene(option + 1);
			Game::instance().playSound("music/PhoneSound.mp3");
		}

		else if (Game::instance().getKey(27)) {
			Game::instance().exit();
		}
	}


}

void Menu::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render();

	for (int i = 0; i < 3; i++) {
		if (option != i || int(currentTime/200)%5 != 0) options[i].sprite->render();
	}


}