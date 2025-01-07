#include "cmp_npc.h"
#include <memory>
#include <LevelSystem.h>
#include <system_renderer.h>
#include <system_resources.h>
#include <engine.h>
#include "cmp_player_physics.h"

void NPCComponent::update(double dt) {
	_sprite->setPosition(_parent->getPosition());
	_sprite->setRotation(_parent->getRotation());

	_nameText.setPosition(_parent->getPosition() + sf::Vector2f(-16.f*_sprite->getScale()) + sf::Vector2f(_name.length()*3, 5));
	_nameText.setRotation(_parent->getRotation());

	_dialogueShape->setPosition(_parent->getPosition() + sf::Vector2f(-16.f * _sprite->getScale()) + sf::Vector2f(-_dialogueShape->getSize().x / 4, -_dialogueShape->getSize().y));
	_dialogueShape->setRotation(_parent->getRotation());

	_dialogueText.setPosition(_dialogueShape->getPosition() + sf::Vector2f(5, 5));

	if (_interactCooldown > 0) {
		_interactCooldown -= dt;
	}

	if (_isInteracting > 0) {
		_isInteracting -= dt;
	}

}

void NPCComponent::render() { 
    Renderer::queue(&_nameText);
	Renderer::queue(_sprite.get());
	if (_isInteracting > 0) {
		Renderer::queue(_dialogueShape.get());
		Renderer::queue(&_dialogueText);
	}
}

NPCComponent::NPCComponent(Entity* const p) : Component(p), _sprite(std::make_shared<sf::Sprite>()) {
	
	_name = "Berty!";
	_font = Resources::get<sf::Font>("dogica.ttf");
	_nameText.setString(_name);
	_nameText.setFont(*_font);
	_nameText.setCharacterSize(10);
	

	_dialogueText.setFont(*_font);
	_dialogueText.setCharacterSize(10);
	_dialogueText.setLineSpacing(1.5f);
	_dialogueText.setString("");

	_dialogue.push_back("Hello there!\nThe princess\nis in another\ncastle.");
	_dialogue.push_back("It's dangerous \nto go alone \ntake this!");
	_dialogue.push_back("Use RShift to\nshoot fireballs");


	_dialogueShape = std::make_shared<sf::RectangleShape>();
	_dialogueShape->setSize(sf::Vector2f(150, 100));
	_dialogueShape->setFillColor(sf::Color(70, 70, 70, 255));
	_dialogueShape->setOutlineColor(sf::Color(50, 50, 50, 255));
	_dialogueShape->setOutlineThickness(4.f);

	_texture = Resources::get<sf::Texture>("WeeCharacter.png");
	_sprite->setColor(sf::Color::Blue);
	_sprite->setTexture(*_texture);

	
}

void NPCComponent::addDialogue(const std::string& str)
{
	_dialogue.push_back(str);
}

void NPCComponent::playerInteract()
{
	
	if (_dialogue.size() > _dialogueIndex && _interactCooldown <= 0) {
		_dialogueText.setString(_dialogue[_dialogueIndex]);
		_dialogueIndex++;
		_interactCooldown = 0.5;
		_isInteracting = 5;
		std::cout << "Interacted with NPC\n";

		if (_dialogueIndex <= _dialogue.size())
		{
			auto player = _parent->scene->ents.find("player")[0];
			player->get_components<PlayerPhysicsComponent>()[0]->fireBallUnlocked = true;
		}
	}
	else if (_dialogue.size() <= _dialogueIndex && _interactCooldown <= 0) {
		_dialogueText.setString("...");
		_isInteracting = 5;
		_interactCooldown = 0.5;
		std::cout << "Interacted with NPC\n";
	}
}

void NPCComponent::SetName(const std::string& str) {
  _name = str;
  _nameText.setString(_name);
}

void NPCComponent::SetTexture(std::shared_ptr<sf::Texture> tex)
{
	_texture = tex;
}

sf::Sprite& NPCComponent::getSprite() const
{
	return *_sprite;
}