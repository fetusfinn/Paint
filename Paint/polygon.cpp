//
//	polygon.cpp
//
#include "globals.h"

#include "polygon.h"

//
// Updates the line as we draw, main functionality
//
void CPolygon::Update(const sf::RenderWindow& _rWindow, bool _bCleared)
{
	if (_bCleared)
	{
		m_bDrawing = false;
		m_bShouldPlace = false;

		m_vPoints.clear();

		return;
	}

	// Place the poly when the user holds down right click
	if (Global::iClickCounter > 1000)
		m_bShouldPlace = true;

	// If the poly has points then we are currently drawing it
	// and if we havent finishing drawing
	m_bDrawing = !m_vPoints.empty() && !m_bShouldPlace;

	if (m_bShouldPlace)
	{
		const int kPointCount = m_vPoints.size();

		// If no points then dont place
		if (!kPointCount)
		{
			m_bShouldPlace = false;
			return;
		}

		// Set how many points there are
		m_rPoly.setPointCount(kPointCount);

		// Build the polygon
		for (int i = 0; i < kPointCount; i++)
			m_rPoly.setPoint(i, sf::Vector2f(m_vPoints.at(i)));

		// Set the position of our polygon to the first point the user clicked
		//m_rPoly.setPosition(sf::Vector2f(m_vPoints.at(0)));
		m_rPoly.setFillColor(Global::rBrushColour);
	}
}

//
// Called when the user clicks
//
void CPolygon::OnClick(const sf::RenderWindow& _rWindow)
{
	// Dont let the user draw on the menu bar
	if (Global::InArea(0, 0, Global::rExclusionZone.x, Global::rExclusionZone.y, _rWindow))
		return;

	// Add a point for each click
	m_vPoints.push_back(sf::Mouse::getPosition(_rWindow));
}

//
// Called when the user releases the mouse
//
void CPolygon::OnRelease()
{
	// Unused
}

//
// Draws the either the placeholder line or the real one
//
void CPolygon::Draw(sf::RenderWindow& _rWindow, sf::RenderTarget* _pRenderTex)
{
	if (m_bDrawing)
	{
		// The placeholder shape to draw
		sf::VertexArray rPlaceholder;

		rPlaceholder.setPrimitiveType(sf::LineStrip);

		// Build the vertex array
		for (const sf::Vector2i& rPoint : m_vPoints)
			rPlaceholder.append(sf::Vertex(sf::Vector2f(rPoint), Global::rBrushColour));

		// So that we can append this onto the end without effecting the real points
		rPlaceholder.append(sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(_rWindow)), Global::rBrushColour));

		// If we're still drawing the line then
		// draw a placeholder from the origin to the mouse
		_rWindow.draw(rPlaceholder);
	}
	else if (m_bShouldPlace)
	{
		// Finished drawing so now draw the final shape
		// to our texture to set it in stone
		_pRenderTex->draw(m_rPoly);

		// Poly has been placed
		m_bShouldPlace = false;

		// Reset the points for the next polygon
		m_vPoints.clear();

		// Clear the polygon after drawing it
		m_rPoly.setPointCount(0);
	}
}

