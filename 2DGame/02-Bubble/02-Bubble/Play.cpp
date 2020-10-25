#include "Play.h"

Play::Play() {

}

Play::~Play() {

}
void Play::init(int l) {
	Scene::init();
	spritesheet.loadFromFile("images/play.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::vec2(640.0, 480.0), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0.0, 0.0));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	loadSprites();

	this->level = l;

}

void Play::loadSprites() {

	for (int i = 0; i < 3; ++i) {
		levels[i].spritesheet.loadFromFile("images/level_" + to_string(i + 1) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
		levels[i].sprite = Sprite::createSprite(glm::ivec2(107, 40), glm::vec2(1.f, 1.f), &levels[i].spritesheet, &texProgram);
		levels[i].sprite->setPosition(glm::vec2(51.6+(i*245.6), 337));
		levels[i].sprite->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}
}

void Play::update(int deltaTime) {
	currentTime += deltaTime;
	currentTurnTime += deltaTime;

	if (currentTurnTime >= float(150.0f)) {
		if (Game::instance().moveRightPressed()) {
			if (level < 2) ++level;
			else {
				level = 0;
			}
			currentTurnTime = 0;
		}

		else if (Game::instance().moveLeftPressed()) {
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
		if (level != i) {
			levels[i].sprite->resize(1.25f);
			levels[i].sprite->setColor(glm::vec3(0.4f, 0.0f, 0.1f));
		}
		else {
			levels[i].sprite->resize(1.5f);
			levels[i].sprite->setColor(glm::vec3(0.4f, 0.7f, 0.1f));
		}
	}
}
