#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Game.h"

#include <utility>

using namespace std;


TileMap* TileMap::createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMap* map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	this->totalMoney = 0;
	this->money = 0;
	this->points = 0;
	this->alarmHited = false;
	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] = tile - int('0');
			if (isCoin(j * mapSize.x + i)) this->totalMoney += 0.25 * 100;
			if (isBag(j * mapSize.x + i)) this->totalMoney += 0.25 * 200;
			if (isPearl(j * mapSize.x + i)) this->totalMoney += 0.25 * 400;
			if (isDiamond(j * mapSize.x + i)) this->totalMoney += 0.25 * 1500;
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

int TileMap::getMoney() {
	return this->money;
}
int TileMap::getPoints() {
	return this->points;
}
int TileMap::getTotalMoney() {
	return this->totalMoney;
}

bool TileMap::getAlarmHited() {
	return this->alarmHited;
}
void TileMap::setAlarmHited(bool a) {
	this->alarmHited = a;
}
void TileMap::printTile(const glm::vec2& minCoords, ShaderProgram& program, int pos, int newTile) {
	int tile, nTiles = 2;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (pos == (j * mapSize.x + i)) {
				tile = newTile;
				map[pos] = newTile;
			}
			else tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


pair<int, int> TileMap::calculateNewTiles(int x, int y, int ID) {
	pair<int, int> newTile;
	switch (ID) {
	case 2:
		if (x % 2 == 0) {
			if (y % 2 == 0) {
				newTile.first = 27;
				newTile.second = 28;
			}
			else {
				newTile.first = 39;
				newTile.second = 40;
			}
		}
		else {
			if (y % 2 == 0) {
				newTile.first = 28;
				newTile.second = 27;
			}
			else {
				newTile.first = 40;
				newTile.second = 39;
			}
		}
		break;
	case 3:
		if (x % 2 == 0) {
			if (y % 2 == 0) {
				newTile.first = 29;
				newTile.second = 30;
			}
			else {
				newTile.first = 41;
				newTile.second = 42;
			}
		}
		else {
			if (y % 2 == 0) {
				newTile.first = 30;
				newTile.second = 29;
			}
			else {
				newTile.first = 42;
				newTile.second = 41;
			}
		}
		break;
	default:
		if (x % 2 == 0) {
			if (y % 2 == 0) {
				newTile.first = 25;
				newTile.second = 26;
			}
			else {
				newTile.first = 37;
				newTile.second = 38;
			}
		}
		else {
			if (y % 2 == 0) {
				newTile.first = 26;
				newTile.second = 25;
			}
			else {
				newTile.first = 38;
				newTile.second = 37;
			}
		}
		break;
	}
	return newTile;
}

bool TileMap::isWall(int pos) {
	return map[pos] == 13 || map[pos] == 15 || map[pos] == 17;
}
bool TileMap::isFakeWall(int pos) {
	return map[pos] == 14 || map[pos] == 16 || map[pos] == 18;
}

bool TileMap::isDiamond(int pos) {//1500€
	return map[pos] == 55 || map[pos] == 56 || map[pos] == 67 || map[pos] == 68;
}

bool TileMap::isRightSideBlock(int pos) {
	return map[pos] == 2 || map[pos] == 4 || map[pos] == 6 || map[pos] == 8 || map[pos] == 10 || map[pos] == 12 || map[pos] == 24;
}
bool TileMap::isLeftSideBlock(int pos) {
	return map[pos] == 1 || map[pos] == 3 || map[pos] == 5 || map[pos] == 7 || map[pos] == 9 || map[pos] == 11 || map[pos] == 23;
}

bool TileMap::isKey(int pos) {
	return map[pos] == 31 || map[pos] == 32 || map[pos] == 43 || map[pos] == 44;
}

bool TileMap::isCoin(int pos) {//dan 100€
	return map[pos] == 49 || map[pos] == 50 || map[pos] == 61 || map[pos] == 62;
}
bool TileMap::isBag(int pos) {//dan 200€
	return map[pos] == 51 || map[pos] == 52 || map[pos] == 63 || map[pos] == 64;
}
bool TileMap::isPearl(int pos) {//dan 400€
	return map[pos] == 53 || map[pos] == 54 || map[pos] == 65 || map[pos] == 66;
}
bool TileMap::isPhone(int pos) {
	return map[pos] == 57 || map[pos] == 58 || map[pos] == 69 || map[pos] == 70;
}
bool TileMap::isAlarm(int pos) {
	return map[pos] == 33 || map[pos] == 34 || map[pos] == 45 || map[pos] == 46;
}


bool TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID)
{
	int x, y0, y1;
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (checkCollision(x, y, ID, currentRoom, minCoords, program)) return true;
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID)
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (checkCollision(x, y, ID, currentRoom, minCoords, program)) return true;
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (checkCollision(x, y, ID, currentRoom, minCoords, program)) return true;
	}
	return false;
}
bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID)
{
	int x0, x1, y;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (isFakeWall(y * mapSize.x + x)) return true; 
		if (checkCollision(x, y, ID, currentRoom, minCoords, program)) return true;
	}
	return false;
}

void TileMap::destroyTop(const glm::vec2& minCoords, ShaderProgram& program, int currentRoom) {
	for (int i = 8; i <= 15; ++i) {
		int pos = (96 - currentRoom * 24) * mapSize.x + i;
		int tile;
		if (i % 2 == 0) tile = 19;
		else tile = 20;
		printTile(minCoords, program, pos, tile);
	}
}

void TileMap::computeNewFourTiles(int t1, int t2, int t3, int t4, int pos, int x, int y, int ID, glm::vec2 minCoords, ShaderProgram& program) {
	if (map[pos] == t1) {
		pair<int, int> newTile = calculateNewTiles(x, y, ID);
		printTile(minCoords, program, pos, newTile.first);
		map[pos] = newTile.first;
		printTile(minCoords, program, pos - 1, newTile.second);
		map[pos - 1] = newTile.second;
		int pos2 = (y - 1) * mapSize.x + x;
		pair<int, int> newTile2 = calculateNewTiles(x, y - 1, ID);
		printTile(minCoords, program, pos2, newTile2.first);
		map[pos2] = newTile2.first;
		printTile(minCoords, program, pos2 - 1, newTile2.second);
		map[pos2 - 1] = newTile2.second;
	}
	if (map[pos] == t2) {
		pair<int, int> newTile = calculateNewTiles(x, y, ID);
		printTile(minCoords, program, pos, newTile.first);
		map[pos] = newTile.first;
		printTile(minCoords, program, pos - 1, newTile.second);
		map[pos - 1] = newTile.second;
		int pos2 = (y + 1) * mapSize.x + x;
		pair<int, int> newTile2 = calculateNewTiles(x, y + 1, ID);
		printTile(minCoords, program, pos2, newTile2.first);
		map[pos2] = newTile2.first;
		printTile(minCoords, program, pos2 - 1, newTile2.second);
		map[pos2 - 1] = newTile2.second;
	}
	if (map[pos] == t3) {
		pair<int, int> newTile = calculateNewTiles(x, y, ID);
		printTile(minCoords, program, pos, newTile.first);
		map[pos] = newTile.first;
		printTile(minCoords, program, pos + 1, newTile.second);
		map[pos + 1] = newTile.second;
		int pos2 = (y - 1) * mapSize.x + x;
		pair<int, int> newTile2 = calculateNewTiles(x, y - 1, ID);
		printTile(minCoords, program, pos2, newTile2.first);
		map[pos2] = newTile2.first;
		printTile(minCoords, program, pos2 + 1, newTile2.second);
		map[pos2 + 1] = newTile2.second;
	}
	if (map[pos] == t4) {
		pair<int, int> newTile = calculateNewTiles(x, y, ID);
		printTile(minCoords, program, pos, newTile.first);
		map[pos] = newTile.first;
		printTile(minCoords, program, pos + 1, newTile.second);
		map[pos + 1] = newTile.second;
		int pos2 = (y + 1) * mapSize.x + x;
		pair<int, int> newTile2 = calculateNewTiles(x, y + 1, ID);
		printTile(minCoords, program, pos2, newTile2.first);
		map[pos2] = newTile2.first;
		printTile(minCoords, program, pos2 + 1, newTile2.second);
		map[pos2 + 1] = newTile2.second;
	}
}

bool TileMap::checkCollision(int x, int y, int ID, int currentRoom, glm::vec2 minCoords, ShaderProgram& program) {
	int pos = y * mapSize.x + x;
	if (isWall(pos)) {
		Game::instance().playSound("music/WallSound.mp3");
		return true;
	}
	else if (map[pos] == 21) {
		printTile(minCoords, program, pos, 23);
		map[pos] = 23;
		printTile(minCoords, program, pos + 1, 24);
		map[pos + 1] = 24;
		Game::instance().playSound("music/BlockSound.mp3");
		return true;
	}
	else if (map[pos] == 22) {
		printTile(minCoords, program, pos, 24);
		map[pos] = 24;
		printTile(minCoords, program, pos - 1, 23);
		map[pos - 1] = 23;
		Game::instance().playSound("music/BlockSound.mp3");
		return true;
	}
	else if (map[pos] == 59) {
		printTile(minCoords, program, pos, 21);
		map[pos] = 21;
		printTile(minCoords, program, pos + 1, 22);
		map[pos + 1] = 22;
		Game::instance().playSound("music/BlockSound.mp3");
		return true;
	}
	else if (map[pos] == 60) {
		printTile(minCoords, program, pos, 22);
		map[pos] = 22;
		printTile(minCoords, program, pos - 1, 21);
		map[pos - 1] = 21;
		Game::instance().playSound("music/BlockSound.mp3");
		return true;
	}
	else if (isLeftSideBlock(pos) || isRightSideBlock(pos)) {
		pair<int, int> newTile = calculateNewTiles(x, y, ID);
		if (isRightSideBlock(pos)) {
			printTile(minCoords, program, pos, newTile.first);
			map[pos] = newTile.first;
			printTile(minCoords, program, pos - 1, newTile.second);
			map[pos - 1] = newTile.second;
		}
		if (isLeftSideBlock(pos)) {
			printTile(minCoords, program, pos, newTile.first);
			map[pos] = newTile.first;
			printTile(minCoords, program, pos + 1, newTile.second);
			map[pos + 1] = newTile.second;
		}
		this->points += 100;
		Game::instance().playSound("music/BlockSound.mp3");
		return true;
	}
	else if (isKey(pos)) {
		destroyTop(minCoords, program, currentRoom);
		computeNewFourTiles(44, 32, 43, 31, pos, x, y, ID, minCoords, program);
		Game::instance().playSound("music/WallSound.mp3");
		return true;
	}
	else if (isCoin(pos)) {
		computeNewFourTiles(62, 50, 61, 49, pos, x, y, ID, minCoords, program);
		this->money += 100;
		this->totalMoney -= 100;
		Game::instance().playSound("music/WallSound.mp3");
		return true;
	}
	else if (isBag(pos)) {
		computeNewFourTiles(64, 52, 63, 51, pos, x, y, ID, minCoords, program);
		this->money += 200;
		this->totalMoney -= 200;
		Game::instance().playSound("music/BagSound.mp3");
		return true;
	}
	else if (isPhone(pos)) {
		computeNewFourTiles(70, 58, 69, 57, pos, x, y, ID, minCoords, program);
		this->money += this->points;
		this->points = 0;
		Game::instance().playSound("music/PhoneSound.mp3");
		return true;
	}
	else if (isPearl(pos)) {
		computeNewFourTiles(66, 54, 65, 53, pos, x, y, ID, minCoords, program);
		this->money += 400;
		this->totalMoney -= 400;
		Game::instance().playSound("music/BagSound.mp3");
		return true;
	}
	else if (isDiamond(pos)) {
		computeNewFourTiles(68, 56, 67, 55, pos, x, y, ID, minCoords, program);
		this->money += 1500;
		this->totalMoney -= 1500;
		Game::instance().playSound("music/BagSound.mp3");
		return true;
	}
	else if (isAlarm(pos)) {
		this->alarmHited = true;
		return true;
	}
	return false;
}