#include "cmp_text.h"
#include <system_renderer.h>
#include <system_resources.h>

void TextComponent::update(double dt) {}

void TextComponent::render() { Renderer::queue(&_text); }

TextComponent::TextComponent(Entity* const p, const std::string& str)
    : Component(p), _string(str) {
  _text.setString(_string);
  _font = Resources::get<sf::Font>("dogica.ttf");
  _text.setFont(*_font);
  _text.setLineSpacing(1.5);
}

void TextComponent::SetText(const std::string& str) {
  _string = str;
  _text.setString(_string);
}
