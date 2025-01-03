#include "string.h"
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;

void SpriteComponent::playAnimation(std::string name) {

	if (_animations.find(name) != _animations.end()) {
		_currentAnim = _animations.at(name);
	}
}

void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
  _spriteSheetWidth = _texture->getSize().x / 16;
}


SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {

	_currentFrame = 0;
	_animations = { {"walk", {0,7}} };
	_isAnimPlaying = false;
	_frameTime = 0;
	_spriteSheetWidth = 1;
}

void SpriteComponent::setCurrentFrameNumber(int frameNum) {_currentFrame = frameNum;}

void SpriteComponent::update(double dt) {
  _sprite->setPosition(_parent->getPosition());
  _sprite->setRotation(_parent->getRotation());

  if (_currentAnim.second && _frameTime <= 0) {
	  if (_currentAnim.first >= _currentAnim.second) {
		  _currentAnim.second = 0;
	  }
	  _sprite->setTextureRect(sf::IntRect((_currentAnim.first%_spriteSheetWidth) * 16, int(_currentAnim.first/16)*16, 16, 16));
	  _frameTime = 0.1;
	  _currentAnim.first++;
	  std::cout << _currentAnim.first << std::endl;
  }
  else if (_frameTime <= 0) {
	  _sprite->setTextureRect(sf::IntRect(0, 0, 16, 16));
  }
  else {
	  _frameTime -= dt;
  }
}

void SpriteComponent::render() { Renderer::queue(_sprite.get()); }

void ShapeComponent::update(double dt) {
  _shape->setPosition(_parent->getPosition());
  _shape->setRotation(_parent->getRotation());
}

void ShapeComponent::render() { Renderer::queue(_shape.get()); }

sf::Shape& ShapeComponent::getShape() const { return *_shape; }

ShapeComponent::ShapeComponent(Entity* p)
    : Component(p), _shape(make_shared<sf::CircleShape>()) {}

sf::Sprite& SpriteComponent::getSprite() const { return *_sprite; }
