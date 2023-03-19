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

	// Menu offset, used to automatically calculate the next
	// menu items position
	struct TOffset
	{
		int x, y;

	}	m_tOffset;

	// Our menu font 
	sf::Font m_rFont;

	// List of all the items to draw to the window
	std::vector<sf::Drawable*> m_pDrawables;

	bool InArea(int x, int y, int w, int h);

	bool Button(const std::string& _strLabel);
	sf::Text* Label(int x, int y, const std::string& _strLabel);

public:

	//
	// Constructor
	//
	CGui(sf::RenderWindow* _pWindow) : m_pWindow(_pWindow), m_tOffset({0,0})
	{
		// Load our font, maybe make this static or global
		m_rFont.loadFromFile("fonts/arial.ttf");
	}

	void Draw();

	int HandleMenuBar(int _iLastSelection, const std::vector<TMenuItemData>& _vItems);
};

//
// Returns true if the left mouse button is down, placed in the header
// so that the function gets inlined, and because it is used in both
// gui.cpp and main.cpp
//
inline bool IsMouseClicked()
{
	return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}