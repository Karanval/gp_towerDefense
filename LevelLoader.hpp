#pragma once

#include <memory>
#include <vector>
#include <sre/SpriteAtlas.hpp>
#include <sre\RenderPass.hpp>

class Grid;
class TowerDefense;
class GameObject;


class LevelLoader {
public:
	void generateLevel(std::vector<std::vector<int>> tileValues, sre::RenderPass& rp);
private:
	glm::vec2 tileSize;
	void placeTile(int tileType, int x, int y, sre::RenderPass& rp);
};
