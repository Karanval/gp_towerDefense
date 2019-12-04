#define GLM_ENABLE_EXPERIMENTAL
#include "LevelLoader.hpp"
#include "Grid.hpp"
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>

void LevelLoader::generateLevel(std::vector<std::vector<int>> tileValues, glm::vec2& tileSize, sre::RenderPass& rp) {
	int x, z, tileType;
	for (int i = 0; i < tileValues.size(); i++) {
		for (int j = 0; j < tileValues[i].size(); j++) {
			x = j * tileSize.x;
			z = i * tileSize.y;
			tileType = tileValues[i][j];
			placeTile(tileType, tileSize, x, z, rp);
		}
	}
}

void LevelLoader::placeTile(int tileType, glm::vec2 tileSize, int x, int z, sre::RenderPass& rp) {
	std::vector<glm::vec3>vertexPositions = {
		glm::vec3(x,0,z), glm::vec3(x,0,z + tileSize.x), glm::vec3(x + tileSize.x,0,z),
		glm::vec3(x + tileSize.x,0,z + tileSize.x), glm::vec3(x + tileSize.x,0,z), glm::vec3(x,0,z + tileSize.x)

	};

	static auto tile = sre::Mesh::create().withPositions(vertexPositions).build();
	glm::vec3 position(x, 0, z);
	std::shared_ptr<sre::Material> material = sre::Shader::getUnlit()->createMaterial();
	sre::Color color;
	switch (tileType) {
		case 0:
			color = sre::Color(0, 1, 0, 1);
			break;
		case 1:
			color = sre::Color(1, 0, 0, 1);
			break;
		default:
			color = sre::Color(0, 0, 1, 1);
	}
	material->setColor(color);
	rp.draw(tile, glm::translate(glm::mat4(1.f), position), material);
}