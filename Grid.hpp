#pragma once

#include <string>
#include <vector>
#include "glm/glm.hpp"

// Class that controls basic functions of the Grid system.
// It allows for loading of the maps from a JSON file, alongside with keeping track of tile values.
class Grid {
public:
	Grid();
	~Grid();
	void loadMap(std::string filename);
	int getTile(int x, int y);
	glm::vec3 getTileSize();
	int getWidth();
	int getHeight();
	std::vector<std::vector<int>> getTileValues();

	glm::vec2 getStartingPosition();
	glm::vec2 getBasePosition();
	glm::vec2 getOffset();
	std::vector<glm::vec2> getEnemyPath();

	void setStartingPosition(glm::vec2 newStartingPosition);
	void setBasePosition(glm::vec2 newBasePosition);
	void changeTileType(int x, int y, int newTileType);

	bool allowsTowers(int x, int y);
	
private:
	std::vector<std::vector<int>> tileValues;
	glm::vec3 tileSize;
	glm::vec2 startingPosition;
	glm::vec2 basePosition;
	glm::vec2 offset;
	std::vector<glm::vec2> enemyPath;
};
