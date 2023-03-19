//
//	Paint
//
#include "gui.h"

#include <iostream>

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
// Main
//
int main()
{
	// Initialise 
	ResetScreenAndWindowSize();

	// Our main paint window
	sf::RenderWindow rWindow(sf::VideoMode(g_iWindowWidth, g_iWindowHeight), "Paint");

	// TODO : have a second window for the colour picker
	// sf::RenderWindow rColourWindow(sf::VideoMode(g_iWindowWidth / 2, g_iWindowHeight / 2), "Colour picker");




	// To draw texture 
	sf::RenderTexture* pRenderTex = new sf::RenderTexture();
	pRenderTex->create(rWindow.getSize().x, rWindow.getSize().y);
	pRenderTex->clear(sf::Color::White);
	pRenderTex->display();

	// Our canvas to draw the texture too
	sf::RectangleShape* pCanvas = new sf::RectangleShape(sf::Vector2f(rWindow.getSize().x, rWindow.getSize().y));
	pCanvas->setTexture(&pRenderTex->getTexture());



	// Used to free draw lines
	sf::VertexArray rCurrentLine;
	rCurrentLine.setPrimitiveType(sf::LineStrip);


	// The last position of our mouse
	sf::Vector2i rPrevMousePos(0, 0);

	// Our free draw vertex array
	sf::VertexArray rFreeDrawLine;

	// Whether or not our mouse was pressed last tick
	bool bMouseDownLast = false;

	// Whether or not the mouse was released after being down
	bool bMouseReleased = false;


	// The vertex array for our straight line
	sf::VertexArray rLine;
	rLine.setPrimitiveType(sf::LineStrip);

	// The position of our line
	sf::Vector2f rLineStartPos, rLineEndPos;

	// Whether or not we are drawing our line
	bool bDrawingLine = false;

	// Whether or not we should draw the line to our texture and
	// finalise the transformation(s)
	bool bShouldPlaceLine = false;



	// Our gui
	CGui rGui(&rWindow);

	// Our menu bar items
	std::vector<TMenuItemData> vMenuItems;

	// Add them all 
	vMenuItems.emplace_back(TMenuItemData(MENU_ITEM_BUTTON, "Line",		MENU_ITEM_LINE));
	vMenuItems.emplace_back(TMenuItemData(MENU_ITEM_BUTTON, "Rectangle",	MENU_ITEM_RECT));
	vMenuItems.emplace_back(TMenuItemData(MENU_ITEM_BUTTON, "Ellipse",		MENU_ITEM_ELLIPSE));
	vMenuItems.emplace_back(TMenuItemData(MENU_ITEM_COLOUR, "Colours",		MENU_ITEM_COLOUR_PICKER));
	vMenuItems.emplace_back(TMenuItemData(MENU_ITEM_BUTTON, "Width",		MENU_ITEM_SIZE));
	vMenuItems.emplace_back(TMenuItemData(MENU_ITEM_BUTTON, "Clear",		MENU_ITEM_CLEAR));

	// TODO : 
	// vMenuItems.push_back(TMenuItem(MENU_ITEM_BUTTON, "Polygon",		MENU_ITEM_POLY));
	// vMenuItems.push_back(TMenuItem(MENU_ITEM_BUTTON, "Free draw",	MENU_ITEM_FREE));

	// To keep track of our brush size
	int iBrushSize = 0;


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

		// Generate our menu and handle the user's selection
		int iMenuSelection = rGui.HandleMenuBar(vMenuItems);

		// Now handle the user's selection
		switch (iMenuSelection)
		{
		case MENU_ITEM_LINE:
			g_eBrushType = BRUSH_LINE;
			break;

		case MENU_ITEM_RECT:
			g_eBrushType = BRUSH_RECT;
			break;

		case MENU_ITEM_ELLIPSE:
			g_eBrushType == BRUSH_ELLIPSE;
			break;

		case MENU_ITEM_COLOUR_PICKER:
			// TODO 
			break;

		case MENU_ITEM_SIZE:

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

			break;

		case MENU_ITEM_CLEAR:
			// Clear the screen so just reset the texture
			pRenderTex->clear(sf::Color::White);
			break;
		}


		// Draw with selected brush type
		switch (g_eBrushType)
		{
		case BRUSH_LINE:
			{
				// Reset the line each tick
				rLine.clear();

				sf::Vector2i rMousePos = sf::Mouse::getPosition(rWindow);

				// If the mouse was just clicked and we're not drawing
				if (IsMouseClicked() && !bDrawingLine)
				{
					// Then store the mouse's position to use as our starting
					// point for our line
					rLineStartPos = sf::Vector2f(rMousePos);

					// Start to draw the line
					bDrawingLine = true;
				}

				// If drawing the line
				if (bDrawingLine)
				{
					// Store the mouse's pos for the end point so we draw a line between
					// points while the user is still deciding where to actually end the line
					rLineEndPos = sf::Vector2f(rMousePos);
				}

				// If the mouse was released
				if (bMouseReleased)
				{
					// Then we should draw the line to our texture/canvas to finialise it
					bShouldPlaceLine = true;

					// And stop drawing the temp line
					bDrawingLine = false;
				}

				// Add the two end of our line to the vertex array
				rLine.append(sf::Vertex(rLineStartPos, sf::Color::Red));
				rLine.append(sf::Vertex(rLineEndPos, sf::Color::Red));
			}
			break;

		case BRUSH_RECT:
			{
				// TODO
			}
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
		case BRUSH_NONE: // The starting brush
			break;
		}



		// Mouse click stuff
		{
			// If the mouse was released last tick
			if (bMouseReleased)
			{
				// Then clear our free draw line
				rCurrentLine.clear();

				// And reset this
				// We should handle all mouse released stuff before this
				bMouseReleased = false;
			}

			if (IsMouseClicked())
			{
				// If the mouse is clicked then is is down this tick
				bMouseDownLast = true;
			}
			else
			{
				// If the mouse was down last and we're no longer clicking
				if (bMouseDownLast)
				{
					bMouseDownLast = false;

					// Then the mouse was just released
					bMouseReleased = true;
				}
			}
		}


		
		// Clear the window
		rWindow.clear(sf::Color::White);

		// Draw the canvas first so that we can draw our placeholder shapes on top of it
		rWindow.draw(*pCanvas);

		// Draw everything
		{
			// Draw our free draw line
			pRenderTex->draw(rCurrentLine);

			// 
			if (g_eBrushType == BRUSH_LINE)
			{
				if (bDrawingLine)
				{
					// If we havent confirmed the line yet then
					// draw a placeholder from the origin to the mouse
					rWindow.draw(rLine);
				}
				else if (bShouldPlaceLine)
				{
					// Finished drawing so now draw the final line
					// to our texture to set it in stone
					pRenderTex->draw(rLine);

					// Line has been placed
					bShouldPlaceLine = false;
				}
			}

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
