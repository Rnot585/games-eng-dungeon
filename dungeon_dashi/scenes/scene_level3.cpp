#include "scene_level3.h"
#include "../components/cmp_physics.h"
#include "../components/cmp_player_physics.h"
#include "../game.h"
#include "../components/cmp_bullet.h"
#include <LevelSystem.h>
#include <iostream>
#include "../components/cmp_hurt_player.h"
#include "../components/cmp_enemy_ai.h"
using namespace std;
using namespace sf;

static shared_ptr<Entity> player;

void Level3Scene::Load() {
  cout << "Scene 3 Load" << endl;

  float tileSize = 40.f;

  ls::loadLevelFile("res/level_3.txt", tileSize);
  auto ho = Engine::getWindowSize().y - (ls::getHeight() * tileSize);
  ls::setOffset(Vector2f(0, ho));

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

      s->addAnimation("walk", { 0, 7 });
      s->addAnimation("fireball", { 8, 11 });
      s->addAnimation("jump", { 12, 17 });

      player->addTag("player");


    // *********************************
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

  cout << " Scene 3 Load Done" << endl;
  setLoaded(true);
}

void Level3Scene::UnLoad() {
  cout << "Scene 3 UnLoad" << endl;
  player.reset();
  ls::unload();
  Scene::UnLoad();
}



void Level3Scene::Update(const double& dt) {
  Scene::Update(dt);
  const auto pp = player->getPosition();
  if (ls::getTileAt(pp) == ls::END) {
    Engine::ChangeScene((Scene*)&level1);
  } else if (!player->isAlive()) {
    Engine::ChangeScene((Scene*)&level3);
  }

  /*
  static float rocktime = 0.0f;
  rocktime -= dt;

  if (rocktime <= 0.f){
    rocktime  = 5.f;
    auto rock = makeEntity();
    rock->setPosition(ls::getTilePosition(ls::findTiles('r')[0]) +
                      Vector2f(0, 40) );
    rock->addComponent<BulletComponent>(30.f);
    auto s = rock->addComponent<ShapeComponent>();
    s->setShape<sf::CircleShape>(40.f);
    s->getShape().setFillColor(Color::Cyan);
    s->getShape().setOrigin(Vector2f(40.f, 40.f));
    auto p = rock->addComponent<PhysicsComponent>(true, Vector2f(75.f, 75.f));
    p->setRestitution(.4f);
    p->setFriction(.0001f);
    p->impulse(Vector2f(-3.f, 0));
    p->setMass(1000000000.f);
  }
  */
  
}

void Level3Scene::Render() {
  ls::render(Engine::GetWindow());
  Scene::Render();
}
