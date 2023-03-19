//
//	gui.h
//
#pragma once

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

//
// Enum of all our items that are in the menu
//
enum EMenuItem
{
	MENU_ITEM_NONE = 0,

	MENU_ITEM_LINE,
	MENU_ITEM_RECT,
	MENU_ITEM_ELLIPSE,
	MENU_ITEM_COLOUR_PICKER,
	MENU_ITEM_SIZE,		// Brush size
	MENU_ITEM_CLEAR,	// Clear the screen

	// TODO :
	MENU_ITEM_POLY,
	MENU_ITEM_FREE,

	MENU_ITEM_COUNT = 7 - 2, // 7 is total, -2 are the extras
};

//
// Enum of the types of menu items
//
enum EMenuItemType
{
	MENU_ITEM_INVALID = 0,

	MENU_ITEM_BUTTON,
	MENU_ITEM_COLOUR,

	MENU_ITEM_TYPE_COUNT = 2,
};

//
// Data for menu items such as buttons and colour pickers
//
struct TMenuItemData
{
	// The type of item this is
	EMenuItemType m_eType;

	// The label for this item
	std::string m_strLabel;

	// The return value for when it's selected
	EMenuItem m_eValue;

	// Constructor
	TMenuItemData(EMenuItemType _eType, const std::string& _strLabel, EMenuItem _eValue) : m_eType(_eType), m_strLabel(_strLabel), m_eValue(_eValue) { }
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

	EMenuItem HandleMenuBar(const std::vector<TMenuItemData>& _vItems);
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