#pragma once

#include <unordered_map>
#include <string>
#include <SFML\Graphics.hpp>

#define IMAGE Game::getActiveGame().getTextureManager()

class TextureManager
{
public:
  ~TextureManager();
  sf::Image& getImage(const sf::Texture* texture);
  sf::Texture& getTexture(const std::string& filePath);

private:
  std::unordered_map<sf::Texture*, sf::Image*> m_images;
  std::unordered_map<std::string, sf::Texture*> m_textures;
};