#include "scene_level2.h"
#include "../components/cmp_enemy_ai.h"
#include "../components/cmp_enemy_turret.h"
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include <LevelSystem.h>
#include <iostream>
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;
void Level2Scene::Load() {
  cout << "Scene 2 Load" << endl;

  float tileSize = 40.f;

  ls::loadLevelFile("res/level_2.txt", tileSize);

  auto ho = Engine::getWindowSize().y - (ls::getHeight() * tileSize);

  //ls::setOffset(Vector2f(0, ho));

  // Create player
  {
    // *********************************
      player = makeEntity();
      player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
      
 
      shared_ptr<sf::Texture> playerTex = make_shared<sf::Texture>();
      playerTex->loadFromFile("res/spritesheets/WeeCharacter sprst.png");

      auto s = player->addComponent<SpriteComponent>();
      s->setTexure(playerTex);
      s->getSprite().setScale(Vector2f(tileSize, tileSize) / 16.f);
      s->getSprite().setOrigin(Vector2f(8.f, 8.f));
      player->addComponent<PlayerPhysicsComponent>(Vector2f(0.4f, 0.8f) * tileSize)->fireBallUnlocked = true;

      player->addTag("player");

      s->addAnimation("walk", { 0, 7 });
      s->addAnimation("fireball", { 8, 11 });
      s->addAnimation("jump", { 12, 17 });
    //player->addComponent<PlayerPhysicsComponent>(Vector2f(20.f, 30.f));
  }

  // Create Enemies
  {
    
      auto enemies = ls::findTiles(ls::ENEMY);
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

  // Create Turret
  {
    auto turret = makeEntity();
    turret->setPosition(ls::getTilePosition(ls::findTiles('t')[0]) +
                        Vector2f(20, 0));
    auto s = turret->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(16.f, 3);
    s->getShape().setFillColor(Color::Red);
    s->getShape().setOrigin(Vector2f(16.f, 16.f));
    turret->addComponent<EnemyTurretComponent>();
  }

  // Add physics colliders to level tiles.
  {
    // *********************************

      auto walls = ls::findTiles(ls::WALL);
      for (auto w : walls) {
          auto pos = ls::getTilePosition(w);
          pos += Vector2f(tileSize / 2, tileSize / 2); //offset to center
          auto e = makeEntity();
          e->setPosition(pos);
          e->addComponent<PhysicsComponent>(false, Vector2f(tileSize, tileSize));
      }


    // *********************************
  }

  cout << " Scene 2 Load Done" << endl;
  setLoaded(true);
}

void Level2Scene::UnLoad() {
  cout << "Scene 2 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}

void Level2Scene::Update(const double& dt) {
  Scene::Update(dt);
  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level3);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&level2);
  }
}

void Level2Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
