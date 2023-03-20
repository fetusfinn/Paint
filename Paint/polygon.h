//
//	polygon.h
//
#pragma once

#include <SFML/Graphics.hpp>

class CPolygon
{
private:

	sf::ConvexShape m_rPoly;

	// The points of our polygon
	std::vector<sf::Vector2i> m_vPoints;

	// Whether or not we are drawing our shape
	bool m_bDrawing = false;

	// Whether or not we should draw the shape to our texture 
	// and finalise the transformation(s)
	bool m_bShouldPlace = false;

public:

	CPolygon() { }

	void Update(const sf::RenderWindow& _rWindow, bool _bCleared);

	void OnClick(const sf::RenderWindow& _rWindow);
	void OnRelease();

	void Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex);
};