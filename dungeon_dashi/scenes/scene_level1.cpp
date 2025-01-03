#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level1Scene::Load() {
	cout << " Scene 1 Load" << endl;

	float tileSize = 40.f;

	ls::loadLevelFile("res/level_1.txt", tileSize);

	auto ho = Engine::getWindowSize().y - (ls::getHeight() * tileSize);
	//ls::setOffset(Vector2f(0, ho));

	// Create player
	{
		player = makeEntity();
		player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
		//auto s = player->addComponent<ShapeComponent>();
		//s->setShape<sf::RectangleShape>(Vector2f(0.4f, 0.8f)*tileSize);
		//s->getShape().setFillColor(Color::Magenta);
		//s->getShape().setOrigin(Vector2f(0.4f, 0.8f)*tileSize/2.f);

		shared_ptr<sf::Texture> playerTex = make_shared<sf::Texture>();
		playerTex->loadFromFile("res/spritesheets/WeeCharacter sprst.png");

		auto s = player->addComponent<SpriteComponent>();
		s->setTexure(playerTex);
		s->getSprite().setScale(Vector2f(tileSize, tileSize)/16.f);
		s->getSprite().setOrigin(Vector2f(8.f, 8.f));
		player->addComponent<PlayerPhysicsComponent>(Vector2f(0.4f, 0.8f) * tileSize);
	}

	// Add physics colliders to level tiles.
	{
		auto walls = ls::findTiles(ls::WALL);
		for (auto w : walls) {
			auto pos = ls::getTilePosition(w);
			pos += Vector2f(tileSize/2, tileSize/2); //offset to center
			auto e = makeEntity();
			e->setPosition(pos);
			e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
		}
	}

	//Simulate long loading times
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	cout << " Scene 1 Load Done" << endl;

	setLoaded(true);
}

void Level1Scene::UnLoad() {
	cout << "Scene 1 Unload" << endl;
	player.reset();
	ls::unload();
	Scene::UnLoad();
}

void Level1Scene::Update(const double& dt) {

	if (ls::getTileAt(player->getPosition()) == ls::END) {
		Engine::ChangeScene((Scene*)&level2);
	}

	Scene::Update(dt);
}

void Level1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
