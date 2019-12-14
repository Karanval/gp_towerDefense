#pragma once

#include <memory>
#include <vector>
#include <sre/SpriteAtlas.hpp>
#include <sre\RenderPass.hpp>

class Grid;
class TowerDefense;
class GameObject;

// Class for generating levels and loading tiles as GameObjects.
class LevelLoader {
public:
	void generateLevel(std::vector<std::vector<int>> tileValues, glm::vec3& tileSize, glm::vec2& offset, std::vector<std::shared_ptr<GameObject>>* gObj);
private:
	void placeTile(int tileType, glm::vec3 tileSize, glm::vec2 offset, int x, int y, std::vector<std::shared_ptr<GameObject>>* gObj);
};
