#pragma once

#include <unordered_map>
#include <SFML\Graphics.hpp>

#define TEXT Game::getActiveGame()->getTextManager()

class TextManager
{
public:
  ~TextManager();

  void drawText(sf::RenderWindow* window);
  void addTextToDisplay(const std::string* text, sf::Vector2f textPosition, 
  sf::Color textColor = sf::Color::Green, unsigned int textSize = 16, 
  const std::string& textFontPath = "arial.ttf", 
  sf::Text::Style textStyle = sf::Text::Regular);
  void removeTextFromDisplay(const std::string* text);
  sf::Font* getFont(const std::string& fontPath);

private:
  std::unordered_map<std::string*, sf::Text> m_textDrawables;
  //Pointer to the font because SFML fonts suck.
  std::unordered_map<std::string, sf::Font*> m_fonts;  

  void checkStrings();
};