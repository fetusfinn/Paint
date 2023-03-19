//
//	Paint
//
#include <iostream>

#include "gui.h"

#include "line.h"
#include "rect.h"



#define debug(msg) std::cout << "[debug] " << msg << std::endl

enum EBrushType
{
	BRUSH_NONE = 0,
	BRUSH_LINE,
	BRUSH_RECT,
	BRUSH_ELLIPSE,
	BRUSH_FREE,
	BRUSH_POLY,
};

enum EBrushThickness
{
	BRUSH_THIN = 0,
	BRUSH_MEDIUM,
	BRUSH_THICK,

	BRUSH_SIZE_COUNT = 3,
};

// The brush we want to use
EBrushType g_eBrushType = BRUSH_LINE;

// The size of our brush
EBrushThickness g_eBrushSize = BRUSH_THICK;

// The colour we want to draw with
sf::Color g_rBrushColour = sf::Color::Black;

// The size of the user's screen
int g_iScreenWidth, g_iScreenHeight;

// The size of our window
int g_iWindowWidth, g_iWindowHeight;

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
int HandleMenuSelection(int _iSelection, CGui& _rGui, const std::vector<TMenuItemData>& _vMenuItems, sf::RenderTexture* _pRenderTex)
{
	// Generate our menu and handle the user's selection
	int iMenuSelection = _rGui.HandleMenuBar(_iSelection, _vMenuItems);

	// Now handle the user's selection
	switch (iMenuSelection)
	{
	case 0:
		g_eBrushType = BRUSH_NONE;
		break;

	case 1:
		g_eBrushType = BRUSH_LINE;
		break;

	case 2:
		g_eBrushType = BRUSH_RECT;
		break;

	case 3:
		g_eBrushType == BRUSH_ELLIPSE;
		break;

	case 4:
		// TODO 
		break;

	case 5:
		// Functionality is done elsewhere
		break;

	case 6:
		// Clear the screen so just reset the texture
		_pRenderTex->clear(sf::Color::White);

		// Only want to clear once
		iMenuSelection = 0;
		break;
	}

	return iMenuSelection;
}

//
// Handles all mouse click events
//
void HandleMouseClicks()
{
	
}

//
// Change the brush size
//
void ChangeBrushSize()
{
	static int iBrushSize = 0;

	// Move to the next brush size
	iBrushSize++;

	// Loop thru the brush sizes
	if (iBrushSize == BRUSH_SIZE_COUNT)
		iBrushSize = BRUSH_THIN;

	// Set the brush size
	// Dont really need to use an enum for this to be honest
	// but it's good for readability
	g_eBrushSize = static_cast<EBrushThickness>(iBrushSize);

	debug(iBrushSize);
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


	// Our line object
	CLine rLine;
	CRectangle rRect;

	// Our gui object
	CGui rGui(&rWindow);

	// Our menu bar items
	std::vector<TMenuItemData> vMenuItems;

	// Add them all 
	vMenuItems.emplace_back(TMenuItemData(1, "Line",		1));
	vMenuItems.emplace_back(TMenuItemData(1, "Rectangle",	2));
	vMenuItems.emplace_back(TMenuItemData(1, "Ellipse",		3));
	vMenuItems.emplace_back(TMenuItemData(2, "Colours",		4));
	vMenuItems.emplace_back(TMenuItemData(1, "Width",		5));
	vMenuItems.emplace_back(TMenuItemData(1, "Clear",		6));

	// TODO : 
	// vMenuItems.push_back(TMenuItem(MENU_ITEM_BUTTON, "Polygon",		MENU_ITEM_POLY));
	// vMenuItems.push_back(TMenuItem(MENU_ITEM_BUTTON, "Free draw",	MENU_ITEM_FREE));



	// Our currnt selection for the menu
	int iMenuSelection = 0;
	int iLastMenuSelection = 0;

	// Whether or not the app should run
	bool bRunning = true;

	// Main loop
	while (bRunning)
	{
		sf::Event rEvent;
		while (rWindow.pollEvent(rEvent))
		{
			if (rEvent.type == sf::Event::Closed)
				rWindow.close();
		}

		if (iLastMenuSelection != iMenuSelection)
			debug(iMenuSelection);

		iLastMenuSelection = iMenuSelection;
		iMenuSelection = HandleMenuSelection(iMenuSelection, rGui, vMenuItems, pRenderTex);
		


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
			{
				// TODO 
			}
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

						for (int i = 0; i < g_eBrushSize; i++)
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





				// If change brush size was selected
				if (iMenuSelection == 5)
				{
					ChangeBrushSize();

					// Brush size changed, change selection so we dont change the
					// brush size again when we click
					iMenuSelection = 0;
				}

				// Then clear our free draw line
				rCurrentLine.clear();

			}
		}

		// TODO 
		HandleMouseClicks();



		
		// Clear the window
		rWindow.clear(sf::Color::White);

		// Draw the canvas first so that we can draw our placeholder shapes on top of it
		rWindow.draw(*pCanvas);

		// Draw everything
		{
			// Draw
			if (g_eBrushType == BRUSH_LINE)
				rLine.Draw(rWindow, pRenderTex);

			else if (g_eBrushType == BRUSH_RECT)
				rRect.Draw(rWindow, pRenderTex);






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
