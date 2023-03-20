//
//	gui.h
//
#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

//
// Data for menu items such as buttons and colour pickers
//
struct TMenuItemData
{
	// The type of item this is
	int m_iType;

	// The label for this item
	std::string m_strLabel;

	// The return value for when it's selected
	int m_iValue;

	// Constructor
	TMenuItemData(int _iType, const std::string& _strLabel, int _iValue) : m_iType(_iType), m_strLabel(_strLabel), m_iValue(_iValue) { }
};

//
// Our GUI class
//
class CGui
{
private:

	// Our main window
	sf::RenderWindow* m_pWindow;

	// List of all the items to draw to the window
	std::vector<sf::Drawable*> m_pDrawables;

	// Menu offset, used to automatically calculate the next
	// menu items position
	sf::Vector2f m_rOffset;

	// Our menu font 
	sf::Font m_rFont;

	// Whether the colour picker is open
	bool m_bColourOpen;


	//bool InArea(float x, float y, float w, float h);
	bool Button(const std::string& _strLabel);
	sf::Text* Label(float x, float y, const std::string& _strLabel);
	sf::Color ColourPicker(const std::vector<sf::Color>& _vColours);

public:

	CGui(sf::RenderWindow* _pWindow);
	~CGui();

	void Draw();

	int HandleMenuBar(int _iLastSelection, const std::vector<TMenuItemData>& _vItems, const std::vector<sf::Color>& _vColours);
};