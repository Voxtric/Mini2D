#include "TextManager.h"

#include <iostream>

TextManager::~TextManager()
{
  for (auto& iterator : m_fonts)
  {
  delete iterator.second;
  }
}

void TextManager::drawText(sf::RenderWindow* window)
{
  checkStrings();
  for (auto& iterator : m_textDrawables)
  {
    iterator.second.move(window->mapPixelToCoords(sf::Vector2i(0, 0)));
    window->draw(iterator.second);
    iterator.second.move(-window->mapPixelToCoords(sf::Vector2i(0, 0)));
  }
}

void TextManager::addTextToDisplay(const std::string* text, 
  sf::Vector2f textPosition, sf::Color textColor, unsigned int textSize, 
  const std::string& textFontPath, sf::Text::Style textStyle)
{
  sf::Text textDrawable(*text, *getFont(textFontPath));
  textDrawable.setPosition(textPosition);
  textDrawable.setColor(textColor);
  textDrawable.setCharacterSize(textSize);
  textDrawable.setStyle(textStyle);
  m_textDrawables.emplace((std::string*)text, textDrawable);
}

void TextManager::removeTextFromDisplay(const std::string* text)
{
  m_textDrawables.erase(m_textDrawables.find((std::string*)text));
}

sf::Font* TextManager::getFont(const std::string& fontPath)
{
  sf::Font* font;
  auto iterator = m_fonts.find(fontPath);
  if (iterator != m_fonts.end())
  {
  font = iterator->second;
  }
  else
  {
  font = new sf::Font();
  if (font->loadFromFile(fontPath))
  {
  m_fonts.emplace(fontPath, font);
  std::cout << "Loaded and cached " << fontPath << ".\n";
  }
  }
  return font;
}

void TextManager::checkStrings()
{
  for (auto& iterator : m_textDrawables)
  {
  if (*iterator.first != iterator.second.getString())
  {
  iterator.second.setString(*iterator.first);
  }
  }
}