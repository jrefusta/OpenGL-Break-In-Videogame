#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Game.h"

#include <utility>

using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
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

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
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
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else
				map[j*mapSize.x+i] = tile - int('0');
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();
	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.x) / tilesheetSize.y);
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
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
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


pair<int, int> TileMap::calculateNewTiles(int x, int y) {
	pair<int,int> newTile;
	if (x % 2 == 0) {
		if (y % 2 == 0) {
			newTile.first = 25;
			newTile.second = 26;
		}
		else {
			newTile.first = 26;
			newTile.second = 25;
		}
	}
	else {
		if (y % 2 == 0) {
			newTile.first = 37;
			newTile.second = 38;
		}
		else {
			newTile.first = 38;
			newTile.second = 37;
		}
	}
	return newTile;
}

bool TileMap::isRightSideBlock(int pos) {
	return map[pos] == 2 || map[pos] == 4 || map[pos] == 6 || map[pos] == 8 || map[pos] == 10 || map[pos] == 12;
}
bool TileMap::isLeftSideBlock(int pos) {
	return map[pos] == 1 || map[pos] == 3 || map[pos] == 5 || map[pos] == 7 || map[pos] == 9 || map[pos] == 11;
}

bool TileMap::isKey(int pos) {
	return map[pos] == 31 || map[pos] == 32 || map[pos] == 43 || map[pos] == 44;
}


bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom) 
{
	int x, y0, y1;
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{	
		int pos = y * mapSize.x + x;
		if (map[pos] == 13) return true;
		else if (isLeftSideBlock(pos) || isRightSideBlock(pos)) {
			pair<int,int> newTile = calculateNewTiles(x,y);
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
			return true;
		}
		else if (isKey(pos)) {
			if (map[pos] == 44) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 32) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 43) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			if (map[pos] == 31) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			for (int i = 8; i <= 15; ++i) {
				int pos = (96 - currentRoom * 24) * mapSize.x + i;
				int tile;
				if (i % 2 == 0) tile = 19;
				else tile = 20;
				printTile(minCoords, program, pos, tile);
			}
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom) 
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int pos = y * mapSize.x + x;
		if (map[pos] == 13) return true;
		else if (isLeftSideBlock(pos) || isRightSideBlock(pos)) {
			pair<int, int> newTile = calculateNewTiles(x, y);
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
			return true;
		}
		else if (isKey(pos)) {
			if (map[pos] == 44) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 32) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 43) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			if (map[pos] == 31) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			for (int i = 8; i <= 15; ++i) {
				int pos = (96 - currentRoom * 24) * mapSize.x + i;
				int tile;
				if (i % 2 == 0) tile = 19;
				else tile = 20;
				printTile(minCoords, program, pos, tile);
			}
			return true;
		}
	}
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x  + size.x - 1) / tileSize;
	y = (pos.y +size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int pos = y * mapSize.x + x;
		if (map[pos] == 13) return true;
		else if (isLeftSideBlock(pos) || isRightSideBlock(pos)) {
			pair<int, int> newTile = calculateNewTiles(x, y);
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
			return true;
		}
		else if (isKey(pos)) {
			if (map[pos] == 44) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 32) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 43) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			if (map[pos] == 31) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			for (int i = 8; i <= 15; ++i) {
				int pos = (96 - currentRoom * 24) * mapSize.x + i;
				int tile;
				if (i % 2 == 0) tile = 19;
				else tile = 20;
				printTile(minCoords, program, pos, tile);
			}
			return true;
		}
	}

	return false;
}
bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom)
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int pos = y * mapSize.x + x;
		if (map[pos] == 13) return true;
		else if (isLeftSideBlock(pos) || isRightSideBlock(pos)) {
			pair<int, int> newTile = calculateNewTiles(x, y);
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
			return true;
		}
		else if (isKey(pos)) {
			if (map[pos] == 44) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 32) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos - 1, newTile.second);
				map[pos - 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 - 1, newTile2.second);
				map[pos2 - 1] = newTile2.second;
			}
			if (map[pos] == 43) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y - 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y - 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			if (map[pos] == 31) {
				pair<int, int> newTile = calculateNewTiles(x, y);
				printTile(minCoords, program, pos, newTile.first);
				map[pos] = newTile.first;
				printTile(minCoords, program, pos + 1, newTile.second);
				map[pos + 1] = newTile.second;
				int pos2 = (y + 1) * mapSize.x + x;
				pair<int, int> newTile2 = calculateNewTiles(x, y + 1);
				printTile(minCoords, program, pos2, newTile2.first);
				map[pos2] = newTile2.first;
				printTile(minCoords, program, pos2 + 1, newTile2.second);
				map[pos2 + 1] = newTile2.second;
			}
			for (int i = 8; i <= 15; ++i) {
				int pos = (96-currentRoom*24) * mapSize.x + i;
				int tile;
				if (i % 2 == 0) tile = 19;
				else tile = 20;
				printTile(minCoords, program, pos, tile);
			}
			return true;
		}
	}

	return false;
}






























