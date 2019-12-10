#include "Grid.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>

using namespace std;

Grid::Grid() {
	tileSize = glm::vec3(32, 32, 5);
}

Grid::~Grid() {
}

void Grid::loadMap(std::string filename) {
	using namespace rapidjson;
	ifstream fis(filename);
	IStreamWrapper isw(fis);
	Document d;
	d.ParseStream(isw);
	const Value& offset = d["offset"];
	Grid::offset = glm::vec2(offset["x"].GetInt(), offset["y"].GetInt());
	const Value& tileMap = d["tileMap"];
	for (int i = 0; i < tileMap.Size(); i++) {
		vector<int> row;
		for (int j = 0; j < tileMap[i].Size(); j++) {
			row.push_back(tileMap[i][j].GetInt());
		}
		tileValues.push_back(row);
	}
}

int Grid::getTile(int x, int y) {
	return tileValues.at(y).at(x);
}

glm::vec3 Grid::getTileSize() {
	return tileSize;
}

std::vector<std::vector<int>> Grid::getTileValues() { 
	return tileValues;
}

int Grid::getWidth() {
	return tileValues[0].size() * tileSize.x;
}

int Grid::getHeight() {
	return tileValues[0].size() * tileSize.y;
}

glm::vec2 Grid::getStartingPosition() {
	return startingPosition;
}

void Grid::setStartingPosition(glm::vec2 newStartingPosition) {
	startingPosition = newStartingPosition;
}

glm::vec2 Grid::getBasePosition() {
	return basePosition;
}

void Grid::setBasePosition(glm::vec2 newBasePosition) {
	basePosition = newBasePosition;
}

bool Grid::allowsTowers(int x, int y) {
	if (x < 0 || y < 0 || x >= tileValues[0].size() || y >= tileValues.size()) return false;
	int tileValue = getTile(x, y);
	if (tileValue) return true; // not 0 (enemy path)
	else return false;
}

void Grid::changeTileType(int x, int y, int newTileType) {
	tileValues.at(x).at(y) = newTileType;
}

glm::vec2 Grid::getOffset() {
	return offset;
}