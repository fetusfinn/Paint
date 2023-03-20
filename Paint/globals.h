//
//	globals.h
//
#pragma once

#define _USE_MATH_DEFINES	// So we can access M_PI

#include <SFML/Graphics.hpp>

// Print macro
#include <iostream>
#define debug(msg) std::cout << "[debug] " << msg << std::endl

// Our global variables
namespace Global
{
	// The size of our brush
	extern float fBrushSize;

	// The colour we want to draw with
	extern sf::Color rBrushColour;

	// Whether or not the mouse is currently clicked
	extern bool bMouseDown;

	// Whether or not the mouse was released this tick
	extern bool bMouseReleased;

	// To keep track of how long the user holds click
	extern int iClickCounter;

	// The area where the user shouldnt be able to draw
	// which is just the menu bar
	extern sf::Vector2f rExclusionZone;

	// Inlined functions because they are tiny

	//
	// Returns true if the mouse was clicked this tick
	//
	inline bool WasMouseJustClicked()
	{
		return iClickCounter == 1;
	}

	//
	// Returns true if the left mouse button is down, placed in the header
	// so that the function gets inlined, and because it is used in both
	// gui.cpp and main.cpp
	//
	inline bool IsMouseClicked()
	{
		return sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}

	extern bool InArea(float x, float y, float w, float h, const sf::RenderWindow& _rWindow);
}
