#pragma once

#include "ecm.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <map>

class SpriteComponent : public Component {
protected:
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;

  //Will be set to 1 as default, if a spritesheet the number input the number of frames here. Each frame is assumed to be a 16/16 square
  int _currentFrame;
  std::pair<int, int> _currentAnim;
  bool _isAnimPlaying;

  double _frameTime;

  int _spriteSheetWidth;

  std::map<std::string, std::pair<int, int>> _animations;

public:
  SpriteComponent() = delete;

  explicit SpriteComponent(Entity* p);
  void update(double dt) override;
  void render() override;

  sf::Sprite& getSprite() const;

  void setCurrentFrameNumber(int frameNum);
  void playAnimation(std::string name);

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