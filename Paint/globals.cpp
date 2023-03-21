//
//	globals.h
//
#include "globals.h"

namespace Global
{
	// The size of our brush
	float fBrushSize = 0;

	// The colour we want to draw with
	sf::Color rBrushColour = sf::Color::Black;

	// The size of our window
	int iWindowWidth, iWindowHeight;

	// Whether or not the mouse is currently clicked
	bool bMouseDown = false;

	// Whether or not the mouse was released this tick
	bool bMouseReleased = false;

	// To keep track of how long the user holds click
	int iClickCounter = 0;

	// Our menu font 
	sf::Font rFont;

	// The area where the user shouldnt be able to draw
	// which is just the menu bar
	sf::Vector2f rExclusionZone;

	// Our notification manager obj
	// Is global because it is used thruout the project
	CNotificationManager rNotifs;
}

//
// Returns true if the mouse is within the given area
//
bool Global::InArea(float x, float y, float w, float h, const sf::RenderWindow& _rWindow)
{
	bool bRet = false;

	sf::Vector2i tMousePos = sf::Mouse::getPosition(_rWindow);

	// mouse within x range
	if (tMousePos.x >= x && tMousePos.x <= (x + w))
	{
		// mouse within y range
		if (tMousePos.y >= y && tMousePos.y <= (y + h))
			bRet = true;
	}

	return bRet;
}

//
// Create a label for us to draw
//
sf::Text* Global::CreateLabel(float x, float y, const std::string& _strLabel)
{
	sf::Text* pText = new sf::Text();

	pText->setFont(rFont);
	pText->setFillColor(sf::Color::Black);
	pText->setString(_strLabel);

	// Use a huge font size then it scale down to get rid of the blurriness 
	pText->setCharacterSize(60);
	pText->setPosition(x, y);
	pText->setScale(sf::Vector2f(0.2f, 0.2f));

	return pText;
}