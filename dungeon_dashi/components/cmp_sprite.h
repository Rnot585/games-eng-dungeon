#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>

class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;

  std::pair<int, int> _currentAnim;
  double _frameTime;
  int _spriteSheetWidth;
  int _stillFrame;
  std::map<std::string, std::pair<int, int>> _animations;

  void animate(double dt);

public:
  SpriteComponent() = delete;

  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
  void render() override;

  sf::Sprite& getSprite() const;

  void setStillFrame(int frameNum);
  void addAnimation(std::string name, std::pair<int, int> frames);
  void playAnimation(std::string name, bool interupt);

  void setTexure(std::shared_ptr<sf::Texture> tex);
};

class ShapeComponent : public Component {
protected:
  std::shared_ptr<sf::Shape> _shape;
  // sf::Shape _shape;

public:
  ShapeComponent() = delete;

  explicit ShapeComponent(Entity* p);

  void update(double dt) override;
  void render() override;
  sf::Shape& getShape() const;
  template <typename T, typename... Targs> void setShape(Targs... params) {
    _shape.reset(new T(params...));
  }
};