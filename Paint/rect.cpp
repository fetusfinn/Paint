//
//	rect.cpp
//
#include "globals.h"

#include "rect.h"

//
// Updates the line as we draw, main functionality
//
void CRectangle::Update(const sf::RenderWindow& _rWindow)
{
	// If drawing the line
	if (m_bDrawing)
	{
		// Store the mouse's pos for the end point so we draw a line between
		// points while the user is still deciding where to actually end the line
		m_rEndPos = sf::Vector2f(sf::Mouse::getPosition(_rWindow));
	}

	// The size of the rectangle is the difference between the start end end points
	sf::Vector2f rSize = m_rEndPos - m_rStartPos;

	m_rRect.setPosition(m_rStartPos);
	m_rRect.setSize(rSize);

	m_rRect.setFillColor(g_rBrushColour);
}

//
// Called when the user clicks
//
void CRectangle::OnClick(const sf::RenderWindow& _rWindow)
{
	// Already drawing
	if (m_bDrawing)
		return;

	// Then store the mouse's position to use as our starting
	// point for our line
	m_rStartPos = sf::Vector2f(sf::Mouse::getPosition(_rWindow));

	// Start to draw the line
	m_bDrawing = true;
}

//
// Called when the user releases the mouse
//
void CRectangle::OnRelease()
{
	// Then we should draw the line to our texture/canvas to finialise it
	m_bShouldPlace = true;

	// And stop drawing the temp line
	m_bDrawing = false;
}

//
// Draws the either the placeholder line or the real one
//
void CRectangle::Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex)
{
	if (m_bDrawing)
	{
		// If we're still drawing the line then
		// draw a placeholder from the origin to the mouse
		_rWindow.draw(m_rRect);
	}
	else if (m_bShouldPlace)
	{
		// Finished drawing so now draw the final line
		// to our texture to set it in stone
		_pRenderTex->draw(m_rRect);

		// Line has been placed
		m_bShouldPlace = false;
	}
}


