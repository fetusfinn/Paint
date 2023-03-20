//
//	line.cpp
//
#include "globals.h"

#include "line.h"

//
// Updates the line as we draw, main functionality
//
void CLine::Update(const sf::RenderWindow& _rWindow, bool _bCleared)
{
	if (_bCleared)
		m_bCancel = true;

	if (m_bCancel)
		return;

	if (!m_bDrawing)
		return;

	// Store the mouse's pos for the end point so we draw a line between
	// points while the user is still deciding where to actually end the line
	m_rEndPos = sf::Vector2f(sf::Mouse::getPosition(_rWindow));

	// The difference between the points
	sf::Vector2f fDifference = m_rEndPos - m_rStartPos;

	// Calculate the angle between the two points
	float fAngle = atan2f(fDifference.y, fDifference.x) * 180 / M_PI;

	// Calculate the distance between
	float fDist = sqrtf(std::pow(fDifference.x, 2) + std::pow(fDifference.y, 2));

	// Set the transformations
	m_rLine.setPosition(m_rStartPos);
	m_rLine.setSize(sf::Vector2f(fDist, (Global::fBrushSize * 2) + 1));
	m_rLine.setRotation(fAngle);
	m_rLine.setFillColor(Global::rBrushColour);
}

//
// Called when the user clicks
//
void CLine::OnClick(const sf::RenderWindow& _rWindow)
{
	// Dont let the user draw on the menu bar
	if (Global::InArea(0, 0, Global::rExclusionZone.x, Global::rExclusionZone.y, _rWindow))
		return;

	// Stop cancelling 
	if (m_bCancel)
		m_bCancel = false;

	// Then store the mouse's position to use as our starting
	// point for our line
	m_rStartPos = sf::Vector2f(sf::Mouse::getPosition(_rWindow));

	// Start to draw the line
	m_bDrawing = true;
}

//
// Called when the user releases the mouse
//
void CLine::OnRelease()
{
	// Then we should draw the line to our texture/canvas to finialise it
	m_bShouldPlace = true;

	// And stop drawing the temp line
	m_bDrawing = false;
}

//
// Draws the either the placeholder line or the real one
//
void CLine::Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex)
{
	if (m_bCancel)
		return;

	if (m_bDrawing)
	{
		// If we're still drawing the line then
		// draw a placeholder from the origin to the mouse
		_rWindow.draw(m_rLine);
	}
	else if (m_bShouldPlace)
	{
		// Finished drawing so now draw the final line
		// to our texture to set it in stone
		_pRenderTex->draw(m_rLine);

		// Line has been placed
		m_bShouldPlace = false;
	}
}


