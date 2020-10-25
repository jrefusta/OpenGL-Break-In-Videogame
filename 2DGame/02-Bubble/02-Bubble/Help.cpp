#include "Help.h"

Help::Help() {

}

Help::~Help() {

}

void Help::init() {
	Scene::init();
	page = 1;
	printHelp();
	projection = glm::ortho(0.0f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.0f);

}

void Help::update(int deltaTime) {
	if (Game::instance().getKey(27)) {
		Game::instance().selectScene(0);
	}
	if (Game::instance().moveLeftPressed() && page == 2) {
		page = 1;
		printHelp();
	}
	if (Game::instance().moveRightPressed() && page == 1) {
		page = 2;
		printHelp();
	}
}

void Help::printHelp() {
	spritesheet.loadFromFile("images/help" + to_string(page) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(640, 480.0), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0.0, 0.0));
}

void Help::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render();
}
