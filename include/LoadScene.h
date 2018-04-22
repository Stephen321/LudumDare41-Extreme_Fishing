#pragma once

#include "Scene.h"
#include "Spriter.h"
#include <thread>
#include <mutex>

class LoadScene : public Scene {
public:
	LoadScene(sf::RenderWindow* _window);
	~LoadScene();
	void start() override;
	void stop() override;
	void handleEvents(const sf::Event& ev) override;
	void update(float dt) override;
	void render(sf::RenderStates states) const override;
	void loadData(sf::RenderWindow& window);

private:
	void setFinished(bool finished);
	bool getFinished();
	se::SpriterModel* m_load;
	se::EntityInstance* m_background; 
	bool finished;
	std::mutex mutex;
};