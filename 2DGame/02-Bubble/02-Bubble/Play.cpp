#include "Play.h"

Play::Play() {

}

Play::~Play() {

}
void Play::init(int l) {
	Scene::init();
	spritesheet.loadFromFile("images/play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(272.0, 240.0), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0.0, 0.0));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	loadSprites();

	this->level = l;

}

void Play::loadSprites() {

	for (int i = 0; i < 3; ++i) {
		levels[i].spritesheet.loadFromFile("images/text_bank_0" + to_string(i + 1) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
		levels[i].sprite = Sprite::createSprite(glm::ivec2(40, 16), glm::vec2(1.f, 1.f), &levels[i].spritesheet, &texProgram);
		levels[i].sprite->setPosition(glm::vec2(112.0, 88.0 + 24.0*i));
		levels[i].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

void Play::update(int deltaTime) {
	currentTime += deltaTime;
	currentTurnTime += deltaTime;

	if (currentTurnTime >= float(150.0f)) {
		if (Game::instance().moveDownPressed()) {
			if (level < 2) ++level;
			else {
				level = 0;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().moveUpPressed()) {
			if (level > 0) --level;
			else {
				level = 2;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().getKey(13)) {
			Game::instance().selectScene(level + 4);
		}

		else if (Game::instance().getKey(27)) {
			Game::instance().selectScene(0);
		}
	}


}

void Play::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render();

	for (int i = 0; i < 3; i++) {
		if (level != i || int(currentTime/500)%2 == 0) levels[i].sprite->render();
	}
}
