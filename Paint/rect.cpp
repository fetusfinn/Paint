//
//	rect.cpp
//
#include "globals.h"

#include "rect.h"

//
// Updates the shape as we draw, main functionality
//
void CRectangle::Update(const sf::RenderWindow& _rWindow, bool _bCleared)
{
	if (_bCleared)
		m_bCancel = true;

	if (m_bCancel)
		return;

	if (!m_bDrawing)
		return;

	// Store the mouse's pos for the end point so we draw a shape between
	// points while the user is still deciding where to actually end the shape
	m_rEndPos = sf::Vector2f(sf::Mouse::getPosition(_rWindow));

	// The size of the rectangle is the difference between the start end end points
	sf::Vector2f rSize = m_rEndPos - m_rStartPos;

	m_rRect.setPosition(m_rStartPos);
	m_rRect.setSize(rSize);

	m_rRect.setFillColor(Global::rBrushColour);
}

//
// Called when the user clicks
//
void CRectangle::OnClick(const sf::RenderWindow& _rWindow)
{
	// Already drawing
	if (m_bDrawing)
		return;

	// Dont let the user draw on the menu bar
	if (Global::InArea(0, 0, Global::rExclusionZone.x, Global::rExclusionZone.y, _rWindow))
		return;

	// Stop cancelling 
	if (m_bCancel)
		m_bCancel = false;

	// Then store the mouse's position to use as our starting
	// point for our shape
	m_rStartPos = sf::Vector2f(sf::Mouse::getPosition(_rWindow));

	// Start to draw the shape
	m_bDrawing = true;
}

//
// Called when the user releases the mouse
//
void CRectangle::OnRelease()
{
	// Then we should draw the shape to our texture/canvas to finialise it
	m_bShouldPlace = true;

	// And stop drawing the temp shape
	m_bDrawing = false;
}

//
// Draws the either the placeholder shape or the real one
//
void CRectangle::Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex)
{
	if (m_bCancel)
		return;


	if (m_bDrawing)
	{
		// If we're still drawing the shape then
		// draw a placeholder from the origin to the mouse
		_rWindow.draw(m_rRect);
	}
	else if (m_bShouldPlace)
	{
		// Finished drawing so now draw the final shape
		// to our texture to set it in stone
		_pRenderTex->draw(m_rRect);

		// Shape has been placed
		m_bShouldPlace = false;
	}
}


