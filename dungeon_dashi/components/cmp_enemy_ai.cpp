#include "cmp_enemy_ai.h"
#include "cmp_sprite.h"

using namespace std;
using namespace sf;

bool EnmDir;  //Direction enemy is facing  -  True = Left  /  False = Right

void EnemyAIComponent::update(double dt) {
  auto mov = _direction * (float)(dt * _speed);
  mov.x += _direction.x * 16.f;
  if (!validMove(_parent->getPosition() + mov)) {
    _direction *= -1.f;
  }
  _parent->get_components<SpriteComponent>()[0]->playAnimation("walk", false);
  move(_direction * (float)(dt * _speed));
  ActorMovementComponent::update(dt);

  if (_direction.x < 0) {
      EnmDir = true;
  }
  else {
      EnmDir = false;
  }

  //Handle direction enemy facing
  float spriteX = _parent->get_components<SpriteComponent>()[0]->getSprite().getScale().x;      //Get x value of sprite scale
  if (EnmDir == true && spriteX < 0) {     //Check if enemy is facing left while sprite is facing right
      _parent->get_components<SpriteComponent>()[0]->getSprite().scale(-1.f, 1.f);   //Flip sprite
      EnmDir = false;
  }
  else if (EnmDir == false && spriteX > 0) {    //Check if enemy is facing right while sprite is facing left
      _parent->get_components<SpriteComponent>()[0]->getSprite().scale(-1.f, 1.f);    //Flip sprite
      EnmDir = true;
  }

}

EnemyAIComponent::EnemyAIComponent(Entity* p) : ActorMovementComponent(p) {
  _direction = Vector2f(1.0f, 0);
  _speed = 100.0f;
}
