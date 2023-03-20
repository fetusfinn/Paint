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

	// To stop us from drawing when we dont want to
	bool m_bCancel = false;

	// Whether or not we just placed a shape
	bool m_bJustPlaced = false;

public:

	CRectangle() { }

	void Update(const sf::RenderWindow& _rWindow, bool _bCleared);

	void OnClick(const sf::RenderWindow& _rWindow);
	void OnRelease();

	void Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex);

};