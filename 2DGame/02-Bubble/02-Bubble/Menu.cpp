#include "Menu.h"
#include <string>
#include <iostream>
using namespace std;

Menu::Menu() {}
Menu:: ~Menu() {}

void Menu::init(int o) {
	Scene::init();
	spritesheet.loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(640.0, 480.0), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0.0, 0.0));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	loadSprites();

	this->option = o;
	
}

void Menu::loadSprites() {

	options[0].spritesheet.loadFromFile("images/play_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	options[0].sprite = Sprite::createSprite(glm::ivec2(100, 30), glm::vec2(1.f, 1.f), &options[0].spritesheet, &texProgram);
	options[0].sprite->setPosition(glm::vec2(float(SCREEN_WIDTH - 355), float(SCREEN_HEIGHT - 250)));
	options[0].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	options[1].spritesheet.loadFromFile("images/help_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	options[1].sprite = Sprite::createSprite(glm::ivec2(107, 40), glm::vec2(1.f, 1.f), &options[1].spritesheet, &texProgram);
	options[1].sprite->setPosition(glm::vec2(float(SCREEN_WIDTH - 360), float(SCREEN_HEIGHT - 190)));
	options[1].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));

	options[2].spritesheet.loadFromFile("images/credits_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	options[2].sprite = Sprite::createSprite(glm::ivec2(130, 40), glm::vec2(1.f, 1.f), &options[2].spritesheet, &texProgram);
	options[2].sprite->setPosition(glm::vec2(float(SCREEN_WIDTH - 380), float(SCREEN_HEIGHT - 100)));
	options[2].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
}

void Menu::update(int deltaTime) {
	currentTime += deltaTime;
	currentTurnTime += deltaTime;

	if (currentTurnTime >= float(150.0f)) {
		if (Game::instance().moveDownPressed()) {
			if (option < 2) ++option;
			else {
				option = 0;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().moveUpPressed()) {
			if (option > 0) --option;
			else {
				option = 2;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().getKey(13)) {
			Game::instance().selectScene(option + 1);
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
		if (option != i) {
			options[i].sprite->resize(1.25f);
			options[i].sprite->setColor(glm::vec3(0.4f, 0.0f, 0.1f));
		}
		else {
			options[i].sprite->resize(1.5f);
			options[i].sprite->setColor(glm::vec3(0.4f, 0.7f, 0.1f));
		}
	}


}