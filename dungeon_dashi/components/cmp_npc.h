#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp> 
#include <memory>
#include <string>
#include "ecm.h"

class NPCComponent : public Component {
public:
	NPCComponent() = delete;
  explicit NPCComponent(Entity* p);
  void update(double dt) override;

  void render() override;

  ~NPCComponent() override = default;

  void SetName(const std::string& str);
  void SetTexture(std::shared_ptr<sf::Texture> tex);
  void addDialogue(const std::string& str);
  void playerInteract();

  sf::Sprite& getSprite() const;

protected:
  std::shared_ptr<sf::Font> _font;
  std::shared_ptr<sf::Sprite> _sprite;
  std::shared_ptr<sf::Texture> _texture;

  std::vector<std::string> _dialogue;
  sf::Text _dialogueText;

  std::shared_ptr<sf::RectangleShape> _dialogueShape;

  std::string _name;
  sf::Text _nameText;

  double _interactCooldown = 0.0f;
  int _dialogueIndex = 0;
  double _isInteracting = 0;
};
