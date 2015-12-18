#include "TextureManager.h"

TextureManager::~TextureManager()
{
  for (auto& it : m_images)
  {
  delete it.second;
  }
  for (auto& it : m_textures)
  {
  delete it.second;
  }
}

sf::Image& TextureManager::getImage(const sf::Texture* texture)
{
  sf::Image* image;
  auto iterator = m_images.find((sf::Texture*)texture);
  if (iterator != m_images.end())
  {
  image = iterator->second;
  }
  else
  {
  std::string name;
  for (auto& it : m_textures)
  {
  if (it.second == texture)
  {
  name = it.first;
  }
  }
  image = new sf::Image();
  m_images.emplace((sf::Texture*)texture, image);
  image->loadFromFile(name);
  }
  return *image;
}

sf::Texture& TextureManager::getTexture(const std::string& filePath)
{
  sf::Texture* texture;
  auto iterator = m_textures.find(filePath);
  if (iterator != m_textures.end())
  {
  texture = iterator->second;
  }
  else
  {
  texture = new sf::Texture();
  m_textures.emplace(filePath, texture);
  texture->loadFromImage(getImage(texture));
  }
  return *texture;
}