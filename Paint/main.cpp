//
//	Paint
//
#include <sstream>

#include "globals.h"

#include "gui.h"

#include "line.h"
#include "rect.h"
#include "ellipse.h"




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

bool HasScreenSizeChanged()
{
	static int iLastScreenWidth = 0, iLastScreenHeight = 0;

	iLastScreenWidth = g_iScreenWidth;
	iLastScreenHeight = g_iScreenHeight;


	return false;
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

		// Only want to clear once
		iMenuSelection = 0;
		break;
	}

	return iMenuSelection;
}


//
// Change the brush size
//
void ChangeBrushSize()
{
	// Move to the next brush size
	g_fBrushSize++;

	// -1 because we're counting from 0
	static const int kMaxBrushSize = 4;

	// Loop thru the brush sizes
	if (g_fBrushSize > kMaxBrushSize)
		g_fBrushSize = 0;
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

	// Whether or not our mouse was pressed last tick
	bool bMouseDownLast = false;



	// Used to free draw lines
	sf::VertexArray rCurrentLine;
	rCurrentLine.setPrimitiveType(sf::LineStrip);

	// The last position of our mouse
	sf::Vector2i rPrevMousePos(0, 0);


	// Our shapes
	CLine rLine;
	CRectangle rRect;
	CEllipse rEllipse;

	// Our gui object
	CGui rGui(&rWindow);

	// Our menu bar items
	std::vector<TMenuItemData> vMenuItems;

	// Add them all 
	vMenuItems.emplace_back(TMenuItemData(1,	"Line",				MENU_INDEX_LINE));
	vMenuItems.emplace_back(TMenuItemData(1,	"Rectangle",		MENU_INDEX_RECT));
	vMenuItems.emplace_back(TMenuItemData(1,	"Ellipse",			MENU_INDEX_ELLIPSE));
	vMenuItems.emplace_back(TMenuItemData(1,	"Brush size: 1",	MENU_INDEX_BRUSH_SIZE));
	vMenuItems.emplace_back(TMenuItemData(1,	"Clear",			MENU_INDEX_CLEAR));
	// TODO 
	// vMenuItems.push_back(TMenuItem(1, "Polygon",		MENU_INDEX_POLY));
	// vMenuItems.push_back(TMenuItem(1, "Free draw",	MENU_INDEX_FREE));

	// Colours are done last so theyre drawn at the end of our menu
	vMenuItems.emplace_back(TMenuItemData(2, "Colours", MENU_INDEX_COLOURS));


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
	};

	// Our currnt selection for the menu
	int iMenuSelection = 0;

	// Whether or not the app should run
	bool bRunning = true;

	// Main loop
	while (bRunning)
	{
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
		


		// Draw with selected brush type
		switch (g_eBrushType)
		{
		case BRUSH_NONE:
			break;

		case BRUSH_LINE:
			rLine.Update(rWindow);
			break;

		case BRUSH_RECT:
			rRect.Update(rWindow);
			break;

		case BRUSH_ELLIPSE:
			rEllipse.Update(rWindow);
			break;

		case BRUSH_POLY:
			{
				// TODO 
			}
			break;

		case BRUSH_FREE:
			{
				// TODO 
				if (IsMouseClicked())
				{
					// Free draw stuff
					auto mouse_pos = sf::Mouse::getPosition(rWindow);
					if (rPrevMousePos != mouse_pos)
					{
						rCurrentLine.append(sf::Vertex(sf::Vector2f(mouse_pos.x, mouse_pos.y), sf::Color::Red));

						for (int i = 0; i < g_fBrushSize; i++)
						{
							rCurrentLine.append(sf::Vertex(sf::Vector2f(mouse_pos.x + i, mouse_pos.y), sf::Color::Red));
							rCurrentLine.append(sf::Vertex(sf::Vector2f(mouse_pos.x - i, mouse_pos.y), sf::Color::Red));
							rCurrentLine.append(sf::Vertex(sf::Vector2f(mouse_pos.x, mouse_pos.y + i), sf::Color::Red));
							rCurrentLine.append(sf::Vertex(sf::Vector2f(mouse_pos.x, mouse_pos.y - i), sf::Color::Red));
						}

						rPrevMousePos = mouse_pos;
					}
				}
			}
			break;

		default:
			break;
		}



		// Mouse click stuff
		if (IsMouseClicked())
		{
			// If the mouse is clicked then is is down this tick
			bMouseDownLast = true;

			if (g_eBrushType == BRUSH_LINE)
				rLine.OnClick(rWindow);
			else if (g_eBrushType == BRUSH_RECT)
				rRect.OnClick(rWindow);
			else if (g_eBrushType == BRUSH_ELLIPSE)
				rEllipse.OnClick(rWindow);
		}
		else
		{
			// Mouse was released
			if (bMouseDownLast)
			{
				bMouseDownLast = false;

				if (g_eBrushType == BRUSH_LINE)
					rLine.OnRelease();
				else if (g_eBrushType == BRUSH_RECT)
					rRect.OnRelease();
				else if (g_eBrushType == BRUSH_ELLIPSE)
					rEllipse.OnRelease();



				// If change brush size was selected
				if (iMenuSelection == MENU_INDEX_BRUSH_SIZE)
				{
					ChangeBrushSize();

					// The label for our width button on our menu
					// so we can update it as we go, using a
					// stringstream so we can set precision
					std::stringstream ssWidthLabel;

					// Dont want any trailing 0s since g_fBrushSize is a float
					ssWidthLabel.precision(0);

					// Build the string
					ssWidthLabel << "Brush size: " << g_fBrushSize + 1;

					// Update the label
					vMenuItems.at(MENU_INDEX_BRUSH_SIZE - 1).m_strLabel = ssWidthLabel.str();

					// Brush size changed, change selection so we dont change the
					// brush size again when we click
					iMenuSelection = 0;
				}

				// Then clear our free draw line
				rCurrentLine.clear();

			}
		}




		
		// Clear the window
		rWindow.clear(sf::Color::White);

		// Draw the canvas first so that we can draw our placeholder shapes on top of it
		rWindow.draw(*pCanvas);

		// Draw everything
		{
			// Draw our shapes
			if (g_eBrushType == BRUSH_LINE)
				rLine.Draw(rWindow, pRenderTex);
			else if (g_eBrushType == BRUSH_RECT)
				rRect.Draw(rWindow, pRenderTex);
			else if (g_eBrushType == BRUSH_ELLIPSE)
				rEllipse.Draw(rWindow, pRenderTex);




			// Draw our GUI last so its on top of everything else
			rGui.Draw();
		}
		rWindow.display();

		// If the window was close this frame then stop running
		if (!rWindow.isOpen())
			bRunning = false;
	}

	return 0;
}
