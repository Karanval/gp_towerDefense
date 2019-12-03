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
	void generateLevel(std::vector<std::vector<int>> tileValues, glm::vec2& tileSize, sre::RenderPass& rp);
private:
	void placeTile(int tileType, glm::vec2 tileSize, int x, int y, sre::RenderPass& rp);
};
