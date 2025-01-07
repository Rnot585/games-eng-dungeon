#include "cmp_hurt_player.h"
#include <engine.h>

using namespace std;
using namespace sf;

void HurtComponent::update(double dt) {
  if (auto pl = _player.lock()) {
    if (length(pl->getPosition() - _parent->getPosition()) < 25.0 && _plyr == false) {
        
      pl->setForDelete();
      _parent->setForDelete();
    }
  }

  if (auto enm = _enemy.lock()) {
      if (length(enm->getPosition() - _parent->getPosition()) < 25.0 && _plyr == true) {
          enm->setForDelete();
          _parent->setForDelete();
      }
  }
}

HurtComponent::HurtComponent(Entity* p)
    : Component(p), _player(_parent->scene->ents.find("player")[0]) {}

HurtComponent::HurtComponent(Entity* p, bool player)
    : Component(p), _player(_parent->scene->ents.find("player")[0]), _plyr(player) {
    if (_parent->scene->ents.find("enemy").empty() == false) {
        _enemy = _parent->scene->ents.find("enemy").at(0);
    }
}
