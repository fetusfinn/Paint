//
//	line.h
//
#pragma once

#include <SFML/Graphics.hpp>

class CLine
{
private:

	sf::RectangleShape m_rLine;

	// The position of our line
	sf::Vector2f m_rStartPos, m_rEndPos;

	// Whether or not we are drawing our line
	bool m_bDrawing;

	// Whether or not we should draw the line to our texture and
	// finalise the transformation(s)
	bool m_bShouldPlace;

public:

	CLine() : m_bDrawing(false), m_bShouldPlace(false) { }

	void Update(const sf::RenderWindow& _rWindow);

	void OnClick(const sf::RenderWindow& _rWindow);
	void OnRelease();

	void Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex);

};