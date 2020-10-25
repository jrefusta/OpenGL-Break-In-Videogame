#include "Credits.h"

Credits::Credits() {

}

Credits::~Credits() {

}

void Credits::init() {
	Scene::init();

	spritesheet.loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(640.0f, 480.f), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0.0f, 0.0f));

	projection = glm::ortho(0.0f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.0f);

}

void Credits::update(int deltaTime) {
	if (Game::instance().getKey(27)) {
		Game::instance().selectScene(0);
	}
}

void Credits::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render();
}
