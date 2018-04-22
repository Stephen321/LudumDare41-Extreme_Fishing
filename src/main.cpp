#include "SceneManager.h"

int main() {
	srand((unsigned int)time(NULL));
	SceneManager& sceneManager = SceneManager::getInstance();
	while (getRunning()) {
		sceneManager.handleEvents();
		sceneManager.update();
		sceneManager.render();
	}
	return EXIT_SUCCESS;
}