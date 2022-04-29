#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap* createTileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	TileMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~TileMap();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }
	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID);
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID);
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID);
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, const glm::vec2& minCoords, ShaderProgram& program, int currentRoom, int ID);
	pair<int, int> calculateNewTiles(int x, int y, int ID);
	bool isLeftSideBlock(int pos);
	bool isRightSideBlock(int pos);
	bool isKey(int pos);
	bool isCoin(int pos);
	bool isBag(int pos);
	bool isPhone(int pos);
	bool isPearl(int pos);
	bool isAlarm(int pos);
	bool isWall(int pos);
	bool isDiamond(int pos);
	bool isFakeWall(int pos);
	bool getAlarmHited();
	void setAlarmHited(bool a);
	int getPoints();
	int getMoney();
	int getTotalMoney();
	bool checkCollision(int x, int y, int ID, int currentRoom, glm::vec2 minCoords, ShaderProgram& program);
	void destroyTop(const glm::vec2& minCoords, ShaderProgram& program, int currentRoom);
	void computeNewFourTiles(int t1, int t2, int t3, int t4, int pos, int x, int y, int ID, glm::vec2 minCoords, ShaderProgram& program);
private:
	bool loadLevel(const string& levelFile);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);
	void printTile(const glm::vec2& minCoords, ShaderProgram& program, int pos, int newTile);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int* map;
	int totalMoney;
	int money;
	int points;
	bool alarmHited;

};


#endif // _TILE_MAP_INCLUDE