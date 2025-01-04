#include "string.h"
#include "cmp_sprite.h"
#include "system_renderer.h"

using namespace std;

//Play the animation with the given name if it exists, if interupt is true the animation will play even if another is already playing
void SpriteComponent::playAnimation(std::string name, bool interupt) {

	if (_animations.find(name) == _animations.end()) {
		std::cout << "Animation " << name << " not found" << std::endl;
		return;
	}

	if (_currentAnim.first < _currentAnim.second && !interupt) {
		return;
	}

	_currentAnim = _animations.at(name);

	
}

//Add an animation with the given name and frame range, frame range must be in the format {start, end}
void SpriteComponent::addAnimation(std::string name, std::pair<int, int> frames)
{
	_animations.insert({ name, frames });
}

//Set the frame that the sprite will display when not playing an animation
void SpriteComponent::setStillFrame(int frameNum)
{
	std::cout << _currentAnim.first << std::endl;
	_stillFrame = frameNum;
	_currentAnim = { frameNum, 0 };
	std::cout << _currentAnim.first << std::endl;
}

void SpriteComponent::setTexure(std::shared_ptr<sf::Texture> tex)
{
  _texture = tex;
  _sprite->setTexture(*_texture);
  _spriteSheetWidth = _texture->getSize().x / 16;
}


SpriteComponent::SpriteComponent(Entity* p)
    : Component(p), _sprite(make_shared<sf::Sprite>()) {

	_currentAnim = { 0, 0 };
	_animations = { {"walk", {0,7}} };
	_frameTime = 0;
	_stillFrame = 0;
	_spriteSheetWidth = 1;
}


void SpriteComponent::update(double dt) {
  _sprite->setPosition(_parent->getPosition());
  _sprite->setRotation(_parent->getRotation());

  animate(dt);
  
}

//Animate the sprite, kept seperate from update to look cleaner
void SpriteComponent::animate(double dt)
{
	if (_currentAnim.second && _frameTime <= 0) {

		_sprite->setTextureRect(sf::IntRect((_currentAnim.first % _spriteSheetWidth) * 16, int(_currentAnim.first / 16) * 16, 16, 16));
		_frameTime = 0.1;

		if (_currentAnim.first >= _currentAnim.second) {
			_currentAnim = { 0, 0 };
		}
		else
		{
			_currentAnim.first++;
		}

		std::cout << _currentAnim.first << std::endl;
	}
	else if (_frameTime <= 0) {
		_sprite->setTextureRect(sf::IntRect((_stillFrame % _spriteSheetWidth) * 16, int(_stillFrame / 16) * 16, 16, 16));
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
