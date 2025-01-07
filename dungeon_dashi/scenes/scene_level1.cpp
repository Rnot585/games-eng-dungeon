#include "scene_level1.h"
#include "../components/cmp_player_physics.h"
#include "../components/cmp_sprite.h"
#include "../components/cmp_npc.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
#include <thread>
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_physics.h"
#include <maths.h>

using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
static shared_ptr<Entity> npc;

void Level1Scene::Load() {
	cout << "Scene 1 Load" << endl;

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
		s->addAnimation("walk", { 0, 7 });
		s->addAnimation("fireball", { 8, 11 });
		s->addAnimation("jump", { 12, 17 });
		s->getSprite().setScale(Vector2f(tileSize, tileSize)/16.f);
		s->getSprite().setOrigin(Vector2f(8.f, 8.f));
		player->addComponent<PlayerPhysicsComponent>(Vector2f(0.4f, 0.8f) * tileSize);

		player->addTag("player");
	}

	//***************************
	// Create Enemies
	{

		auto enemies = ls::findTiles(ls::ENEMY);
		//auto enemies = ls::findTiles(ls::WAYPOINT);
		for (auto n : enemies) {
			auto pos = ls::getTilePosition(n);
			pos += Vector2f(0, 24);
			auto e = makeEntity();
			e->setPosition(pos);
			// *********************************
			// Add HurtComponent

			e->addComponent<HurtComponent>();
			

			shared_ptr<sf::Texture> enmTex = make_shared<sf::Texture>();
			enmTex->loadFromFile("res/spritesheets/EnemyWalk.png");

			auto s = e->addComponent<SpriteComponent>();
			s->setTexure(enmTex);
			s->getSprite().setScale(Vector2f(tileSize, tileSize) / 16.f);
			s->getSprite().setOrigin(Vector2f(8.f, 8.f));
			s->addAnimation("walk", { 0, 7 });

			// Add EnemyAIComponent

			e->addComponent<EnemyAIComponent>();
			e->addTag("enemy");
		}

		// *********************************
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

	npc = makeEntity();
	auto s = npc->addComponent<NPCComponent>();
	npc->setPosition(ls::getTilePosition(ls::findTiles(ls::WAYPOINT)[0]) + Vector2f(0.f, 20.f));
	s->getSprite().setScale(Vector2f(tileSize, tileSize) / 16.f);
	s->getSprite().setOrigin(Vector2f(8.f, 8.f));

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
	Scene::Update(dt);
	const auto pp = player->getPosition();

	if (length(player->getPosition() - npc->getPosition()) < 30 && Keyboard::isKeyPressed(Keyboard::E)) {
		npc->get_components<NPCComponent>()[0]->playerInteract();

	}

	if (ls::getTileAt(pp) == ls::END) {
		Engine::ChangeScene((Scene*)&level2);
	}
	else if (!player->isAlive()) {
		Engine::ChangeScene((Scene*)&level1);
	}

	

	Scene::Update(dt);
}

void Level1Scene::Render() {
	ls::render(Engine::GetWindow());
	Scene::Render();
}
