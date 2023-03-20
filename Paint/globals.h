//
//	globals.h
//
#pragma once

#define _USE_MATH_DEFINES	// So we can access M_PI

#include <SFML/Graphics.hpp>

// Print macro
#include <iostream>
#define debug(msg) std::cout << "[debug] " << msg << std::endl

// The size of our brush
extern float g_fBrushSize;

// The colour we want to draw with
extern sf::Color g_rBrushColour;

// Whether or not the mouse is currently clicked
extern bool g_bMouseDown;

// Whether or not the mouse was released this tick
extern bool g_bMouseReleased;

// To keep track of how long the user holds click
extern int g_iClickCounter;


extern bool WasMouseJustClicked();


//
// Returns true if the left mouse button is down, placed in the header
// so that the function gets inlined, and because it is used in both
// gui.cpp and main.cpp
//
inline bool IsMouseClicked()
{
	return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}