//
//	rect.h
//
#pragma once

#include <SFML/Graphics.hpp>

class CRectangle
{
private:

	sf::RectangleShape m_rRect;

	// The position of our line
	sf::Vector2f m_rStartPos, m_rEndPos;

	// Whether or not we are drawing our shape
	bool m_bDrawing = false;

	// Whether or not we should draw the shape to our texture 
	// and finalise the transformation(s)
	bool m_bShouldPlace = false;

public:

	CRectangle() { }

	void Update(const sf::RenderWindow& _rWindow);

	void OnClick(const sf::RenderWindow& _rWindow);
	void OnRelease();

	void Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex);

};