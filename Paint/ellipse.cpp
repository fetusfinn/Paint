//
//	ellipse.cpp
//
#include "globals.h"

#include "ellipse.h"

//
// Updates the shape as we draw, main functionality
//
void CEllipse::Update(const sf::RenderWindow& _rWindow, bool _bCleared)
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

	// The diameter of the ellipes on it's two axes
	sf::Vector2f rDiameter = m_rEndPos - m_rStartPos; 

	// The position that we want to draw the ellipse
	sf::Vector2f rDrawPos = m_rStartPos;

	// Put it in between our start and end points
	rDrawPos.x += rDiameter.x / 2;
	rDrawPos.y += rDiameter.y / 2;

	// TODO : Maybe make the number of points variable depending how big the ellipse is
	int iPointCount = m_rEllipse.getPointCount();

	// Loop thru each point
	for (unsigned short i = 0; i < iPointCount; i++)
	{
		// Calculate the radians
		float fRads = (360 / iPointCount * i) / (360 / M_PI / 2);

		// Calculate position of point using the rads of the point and its radius
		float fX = cos(fRads) * (rDiameter.x / 2);
		float fY = sin(fRads) * (rDiameter.y / 2);

		m_rEllipse.setPoint(i, sf::Vector2f(fX, fY));
	}
;
	// Set the position
	m_rEllipse.setPosition(rDrawPos);
	m_rEllipse.setFillColor(Global::rBrushColour);
}

//
// Called when the user clicks
//
void CEllipse::OnClick(const sf::RenderWindow& _rWindow)
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
void CEllipse::OnRelease()
{
	// Then we should draw the shape to our texture/canvas to finialise it
	m_bShouldPlace = true;

	// And stop drawing the temp shape
	m_bDrawing = false;
}

//
// Draws the either the placeholder shape or the real one
//
void CEllipse::Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex)
{
	if (m_bCancel)
		return;


	if (m_bDrawing)
	{
		// If we're still drawing the shape then
		// draw a placeholder from the origin to the mouse
		_rWindow.draw(m_rEllipse);
	}
	else if (m_bShouldPlace)
	{
		// Finished drawing so now draw the final shape
		// to our texture to set it in stone
		_pRenderTex->draw(m_rEllipse);

		// shape has been placed
		m_bShouldPlace = false;
	}
}


