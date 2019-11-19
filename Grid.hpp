#include <string>
#include <vector>
#include "glm/glm.hpp"


class Grid {
public:
	void loadMap(std::string filename);
	int getTile(int x, int y);
	int getWidth();
	int getHeight();

	glm::vec2 getStartingPosition();
	glm::vec2 getBasePosition();

	void setStartingPosition(glm::vec2 newStartingPosition);
	void setBasePosition(glm::vec2 newBasePosition);
	void changeTileType(int x, int y, int newTileType);

	bool allowsTowers(int x, int y);
	
private:
	std::vector<std::vector<int>> tileValues;
	glm::vec2 startingPosition;
	glm::vec2 basePosition;
};
