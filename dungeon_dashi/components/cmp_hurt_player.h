#pragma once
#include "cmp_physics.h"
#include <ecm.h>

class HurtComponent : public Component {
protected:
  void fire() const;
  std::weak_ptr<Entity> _player;
  std::weak_ptr<Entity> _enemy;
  bool _plyr = false;
  

public:
  void update(double dt) override;
  void render() override {}
  explicit HurtComponent(Entity* p);
  explicit HurtComponent(Entity* p, bool player);
  HurtComponent() = delete;
};
