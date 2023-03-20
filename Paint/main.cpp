//
//	Paint
//
#include <sstream>

#include "gui.h"

#include "line.h"
#include "rect.h"
#include "ellipse.h"

#include "globals.h"

//
// The different brush types
//
enum EBrushType
{
	BRUSH_NONE = 0,
	BRUSH_LINE,
	BRUSH_RECT,
	BRUSH_ELLIPSE,
	BRUSH_FREE,
	BRUSH_POLY,
};

//
// The different indices for our menu items
//
enum EMenuIndex
{
	MENU_INDEX_INVALID = 0,
	MENU_INDEX_LINE,
	MENU_INDEX_RECT,
	MENU_INDEX_ELLIPSE,
	MENU_INDEX_BRUSH_SIZE,
	MENU_INDEX_CLEAR,
	MENU_INDEX_COLOURS,

	// TODO
	MENU_INDEX_POLY,
	MENU_INDEX_FREE,
};

// The brush we want to use
EBrushType g_eBrushType = BRUSH_LINE;

// The size of our brush
float g_fBrushSize = 0;

// The colour we want to draw with
sf::Color g_rBrushColour = sf::Color::Black;

// The size of the user's screen
int g_iScreenWidth, g_iScreenHeight;

// The size of our window
int g_iWindowWidth, g_iWindowHeight;

// TODO : move to globals.h
// TODO : use a namespace

// Whether or not the mouse is currently clicked
bool g_bMouseDown = false;

// Whether or not the mouse was released this tick
bool g_bMouseReleased = false;

// To keep track of how long the user holds click
int g_iClickCounter = 0;

//
// Handles all left click events
//
void HandleMouseClicks()
{
	g_bMouseReleased = false;

	if (IsMouseClicked())
	{
		g_bMouseDown = true;

		// Increase the click counter
		g_iClickCounter++;
	}
	else
	{
		if (g_bMouseDown)
		{
			g_bMouseDown = false;

			g_iClickCounter = 0;

			g_bMouseReleased = true;
		}
	}
}

//
// Returns true if the mouse was clicked this tick
//
bool WasMouseJustClicked()
{
	return g_iClickCounter == 1;
}

//
// Returns true if the screen size has changed
// Currently unused, TODO
//
bool HasScreenSizeChanged()
{
	sf::VideoMode rScreen = sf::VideoMode::getDesktopMode();

	return g_iScreenWidth != rScreen.width || g_iScreenHeight != rScreen.height;
}

//
// Finds the users screen size and sets the window size
//
void ResetScreenAndWindowSize()
{
	g_iScreenWidth  = sf::VideoMode::getDesktopMode().width;
	g_iScreenHeight = sf::VideoMode::getDesktopMode().height;

	// Window should be half both dimensions of the screen
	g_iWindowWidth  = g_iScreenWidth / 2;
	g_iWindowHeight = g_iScreenHeight / 2;
}

//
// Returns the last selection we had
// Only used when iMenuSelection is set to 0
//
int GetLastMenuSelection()
{

	switch (g_eBrushType)
	{
	case BRUSH_LINE:
		return MENU_INDEX_LINE;

	case BRUSH_RECT:
		return MENU_INDEX_RECT;

	case BRUSH_ELLIPSE:
		return MENU_INDEX_ELLIPSE;
	}

	return 0;
}

//
// Handles the menu selection
//
int HandleMenuSelection(int _iSelection, CGui& _rGui, sf::RenderTexture* _pRenderTex, const std::vector<TMenuItemData>& _vMenuItems, const std::vector<sf::Color> _vColours)
{
	// Generate our menu and handle the user's selection
	int iMenuSelection = _rGui.HandleMenuBar(_iSelection, _vMenuItems, _vColours);

	// Now handle the user's selection
	switch (iMenuSelection)
	{
	case MENU_INDEX_INVALID:
		g_eBrushType = BRUSH_NONE;
		break;

	case MENU_INDEX_LINE:
		g_eBrushType = BRUSH_LINE;
		break;

	case MENU_INDEX_RECT:
		g_eBrushType = BRUSH_RECT;
		break;

	case MENU_INDEX_ELLIPSE:
		g_eBrushType = BRUSH_ELLIPSE;
		break;

	case MENU_INDEX_COLOURS:
		// Functionality is done in CGui::HandleMenuBar()
		break;

	case MENU_INDEX_BRUSH_SIZE:
		// Change brush size
		// Functionality is done elsewhere
		break;

	case MENU_INDEX_CLEAR:
		// Clear the screen so just reset the texture
		_pRenderTex->clear(sf::Color::White);

		// After clearing change back to our last brush
		iMenuSelection = GetLastMenuSelection();
		break;
	}

	return iMenuSelection;
}

//
// Change the brush size and update the button's label
//
void ChangeBrushSize(TMenuItemData& _rMenuItem)
{
	// Move to the next brush size
	g_fBrushSize++;

	// -1 because we're counting from 0
	static constexpr int kMaxBrushSize = 4;

	// Loop thru the brush sizes
	if (g_fBrushSize > kMaxBrushSize)
		g_fBrushSize = 0;

	// The label for our width button on our menu
	// so we can update it as we go, using a
	// stringstream so we can set precision
	std::stringstream ssWidthLabel;

	// Dont want any trailing 0s since g_fBrushSize is a float
	ssWidthLabel.precision(0);

	// Build the string
	ssWidthLabel << "Brush size: " << g_fBrushSize + 1;

	// Update the label
	_rMenuItem.m_strLabel = ssWidthLabel.str();
}

//
// Updates the brush stroke for the our selected brush
//
void UpdateBrushStrokes(const sf::RenderWindow& _rWindow, CLine& _rLine, CRectangle& _rRect, CEllipse& _rEllipse)
{
	switch (g_eBrushType)
	{
	case BRUSH_NONE:
		break;

	case BRUSH_LINE:
		_rLine.Update(_rWindow);
		break;

	case BRUSH_RECT:
		_rRect.Update(_rWindow);
		break;

	case BRUSH_ELLIPSE:
		_rEllipse.Update(_rWindow);
		break;

	// TODO

	case BRUSH_POLY:
		break;

	case BRUSH_FREE:
		break;
	}
}

//
// Called when the user clicks the mouse
//
void OnClick(const sf::RenderWindow& _rWindow, CLine& _rLine, CRectangle& _rRect, CEllipse& _rEllipse)
{
	if (g_eBrushType == BRUSH_LINE)
		_rLine.OnClick(_rWindow);
	else if (g_eBrushType == BRUSH_RECT)
		_rRect.OnClick(_rWindow);
	else if (g_eBrushType == BRUSH_ELLIPSE)
		_rEllipse.OnClick(_rWindow);
}

//
// Called when the user releases the mouse
//
void OnRelease(CLine& _rLine, CRectangle& _rRect, CEllipse& _rEllipse)
{
	if (g_eBrushType == BRUSH_LINE)
		_rLine.OnRelease();
	else if (g_eBrushType == BRUSH_RECT)
		_rRect.OnRelease();
	else if (g_eBrushType == BRUSH_ELLIPSE)
		_rEllipse.OnRelease();
}

//
// Draws our selected shape
//
void DrawShapes(sf::RenderWindow& _rWindow, sf::RenderTexture* _pRenderTex, CLine& _rLine, CRectangle& _rRect, CEllipse& _rEllipse)
{
	if (g_eBrushType == BRUSH_LINE)
		_rLine.Draw(_rWindow, _pRenderTex);
	else if (g_eBrushType == BRUSH_RECT)
		_rRect.Draw(_rWindow, _pRenderTex);
	else if (g_eBrushType == BRUSH_ELLIPSE)
		_rEllipse.Draw(_rWindow, _pRenderTex);
}

//
// Main
//
int main()
{
	// Initialise 
	ResetScreenAndWindowSize();

	// Our main paint window
	sf::RenderWindow rWindow(sf::VideoMode(g_iWindowWidth, g_iWindowHeight), "Paint");

	// To draw texture 
	sf::RenderTexture* pRenderTex = new sf::RenderTexture();

	pRenderTex->create(rWindow.getSize().x, rWindow.getSize().y);
	pRenderTex->clear(sf::Color::White);
	pRenderTex->display();

	// Our canvas to draw the texture too
	sf::RectangleShape* pCanvas = new sf::RectangleShape(sf::Vector2f(rWindow.getSize().x, rWindow.getSize().y));

	pCanvas->setTexture(&pRenderTex->getTexture());



	// Our shapes
	CLine rLine;
	CRectangle rRect;
	CEllipse rEllipse;

	// Our gui object
	CGui rGui(&rWindow);

	// Our menu bar items
	std::vector<TMenuItemData> vMenuItems =
	{
		TMenuItemData(1, "Line",			MENU_INDEX_LINE),
		TMenuItemData(1, "Rectangle",		MENU_INDEX_RECT),
		TMenuItemData(1, "Ellipse",			MENU_INDEX_ELLIPSE),
		TMenuItemData(1, "Brush size: 1",	MENU_INDEX_BRUSH_SIZE),
		TMenuItemData(1, "Clear",			MENU_INDEX_CLEAR),

		// TODO
		// TMenuItemData(1, "Polygon",		MENU_INDEX_POLY),
		// TMenuItemData(1, "Free draw",		MENU_INDEX_FREE),

		// Colours are done last so theyre drawn at the end of our menu
		TMenuItemData(2, "Colours",			MENU_INDEX_COLOURS),
	};


	// Colours for our colour picker
	std::vector<sf::Color> vColours = 
	{
		sf::Color::White,
		sf::Color::Black,
		sf::Color::Red,
		sf::Color::Green,
		sf::Color::Blue,
		sf::Color::Yellow,
		sf::Color::Magenta,
		sf::Color::Cyan,
		// Can add more if we want
	};

	// Our currnt selection for the menu
	int iMenuSelection = 0;

	// Whether or not the app should run
	bool bRunning = true;

	// Main loop
	while (bRunning)
	{
		if (HasScreenSizeChanged())
		{
			// TODO : resize
		}

		// Handle SFML events
		sf::Event rEvent;
		while (rWindow.pollEvent(rEvent))
		{
			// Properly close the window
			if (rEvent.type == sf::Event::Closed)
				rWindow.close();
		}

		// Get the menu selection
		iMenuSelection = HandleMenuSelection(iMenuSelection, rGui, pRenderTex, vMenuItems, vColours);

		// Handle all the left clicks
		HandleMouseClicks();

		// Draw with selected brush type
		UpdateBrushStrokes(rWindow, rLine, rRect, rEllipse);

		// Do stuff if we clicked or released
		if (WasMouseJustClicked())
		{
			OnClick(rWindow, rLine, rRect, rEllipse);

			// If change brush size was selected
			if (iMenuSelection == MENU_INDEX_BRUSH_SIZE)
			{
				ChangeBrushSize(vMenuItems.at(MENU_INDEX_BRUSH_SIZE - 1));

				// Change the menu selection back to the brush we were using
				// so that we can keep drawing
				iMenuSelection = GetLastMenuSelection();
			}
		}
		else if (g_bMouseReleased)
		{
			OnRelease(rLine, rRect, rEllipse);
		}

		// Clear the window
		rWindow.clear(sf::Color::White);

		// Draw the canvas first so that we can draw our placeholder shapes on top of it
		rWindow.draw(*pCanvas);

		// Draw our shapes
		DrawShapes(rWindow, pRenderTex, rLine, rRect, rEllipse);

		// Draw our GUI last so its on top of everything else
		rGui.Draw();

		// Display it all
		rWindow.display();

		// If the window was close this frame then stop running
		if (!rWindow.isOpen())
			bRunning = false;
	}

	return 0;
}
