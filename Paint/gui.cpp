//
//  gui.cpp
//
#include "globals.h"
#include "gui.h"

// Colour with an RBG value that isn't the same as one of
// our colour picker colours, so we can figure out if
// the user has actually picked a colour or not
static const sf::Color g_rColourInvalid(123, 123, 123);

//
// Processes mouse clicks, stores to bools for use within the class
//
void CGui::HandleMouseClicks()
{
    if (IsMouseClicked())
    {
        m_bMouseDownLast = true;
    }
    else
    {
        if (m_bMouseDownLast)
        {
            m_bMouseDownLast = false;

            m_bMouseReleased = true;
        }
    }
}

//
// Draws the menu to the window
//
void CGui::Draw()
{
    // Loop thru all our shapes then draw
	for (auto* pShape : m_pDrawables)
	{
        // Draw to the window because we dont want the gui to show
        // up on the users image when they save it, plus this lets
        // us draw on top of everything else
        m_pWindow->draw(*pShape);
	}

    // Clean up all the pointers
    for (auto* pShape : m_pDrawables)
    {
        delete pShape;
        pShape = nullptr;
    }

    // Reset our draw list
    m_pDrawables.clear();

    // Reset the menu offset
	m_tOffset = { 0 , 0 };
}

//
// Generate the shapes for the menu bar and handle its items inputs
//
int CGui::HandleMenuBar(int _iLastSelection, const std::vector<TMenuItemData>& _vItems, const std::vector<sf::Color>& _vColours)
{
    // Handle our mouse clicks
    HandleMouseClicks();

    // What we want to return
    int iReturn = _iLastSelection;

    // The colour returned from our colour picker
    sf::Color rCol;

    // Loop thru our menu items and draw them
    for (TMenuItemData rItem : _vItems)
    {
        switch (rItem.m_iType)
        {
        case 1: // MENU_ITEM_BUTTON
	        {
                // If our item is a button then draw it with the given label
                // and set its return value if clicked
	            if (Button(rItem.m_strLabel))
                    iReturn = rItem.m_iValue;
	        }
            break;

        case 2: // MENU_ITEM_COLOUR
	        {
                // This should only be called once

                // Get the colour
	            rCol = ColourPicker(_vColours);

                // Update our draw colour if the user chose a new colour
                if (rCol != g_rColourInvalid)
                {
                    g_rBrushColour = rCol;

                    debug("Colour updated");
                }
	        }
        	break;

        default:
        case 0: // MENU_ITEM_INVALID
            // MENU_ITEM_INVALID should only be triggered if we forgot to
            // give our TMenuItem a type, which shouldn't happen but who knows
            continue;
        }
    }

    // Nothing clicked
    return iReturn;
}

//
// Returns true if the mouse is within the given area
//
bool CGui::InArea(int x, int y, int w, int h)
{
    bool bRet = false;

    sf::Vector2i tMousePos = sf::Mouse::getPosition(*m_pWindow);

    // mouse within x range
    if (tMousePos.x >= x && tMousePos.x <= (x + w))
    {
	    // mouse within y range
        if (tMousePos.y >= y && tMousePos.y <= (y + h))
            bRet = true;
    }

    return bRet;
}

//
// Create a label for us to draw
//
sf::Text* CGui::Label(int x, int y, const std::string& _strLabel)
{
    sf::Text* pText = new sf::Text();

    pText->setFont(m_rFont);
    pText->setFillColor(sf::Color::Black);
    pText->setString(_strLabel);

    // Use a huge font size then it scale down to get rid of the blurriness 
    pText->setCharacterSize(60);
    pText->setPosition(x, y);
    pText->setScale(sf::Vector2f(0.2f, 0.2f));

    return pText;
}

//
// Create a button for us to draw and add functionality
//
bool CGui::Button(const std::string& _strLabel)
{
    // Get this buttons position
    int x = m_tOffset.x;
    int y = 0;

    // Dimensions and position of our button
    const int w = 80, h = 30;

    // Create our label
    sf::Text* pLabel = Label(x + 10, y + 5, _strLabel);

    // The labels bounds
    sf::FloatRect rTextBounds = pLabel->getLocalBounds();

    // Center the label in the box, have to scale down the bounds since we scaled the text down
    pLabel->setPosition(x + (w / 2) - ((rTextBounds.width * 0.2f) / 2), y + (h / 2) - ((rTextBounds.height * 0.2f) / 2) - 3);

    // Offset the next item along the X axis
    m_tOffset.x += w;

    // The fill colour of our box
    sf::Color rBoxCol = sf::Color::White;

    // Check if hovering
    if (InArea(x, y, w, h))
    {
        // Change colour if hovering
        rBoxCol = sf::Color(200, 200, 200);

        // Change colour if clicked
        if (IsMouseClicked())
            rBoxCol = sf::Color(170, 170, 170);
    }

    // Our box shape
    sf::RectangleShape* pRect = new sf::RectangleShape(sf::Vector2f(w, h));

    pRect->setPosition(x, y);
    pRect->setFillColor(rBoxCol);
    pRect->setOutlineColor(sf::Color(40, 40, 40));
    pRect->setOutlineThickness(-1);

    m_pDrawables.push_back(pRect);

    // Add the label after so its drawn on top
    m_pDrawables.push_back(pLabel);

    // return value = was the button clicked?
    return InArea(x, y, w, h) && IsMouseClicked();
}

//
// Create a button for us to draw and add functionality
//
sf::Color CGui::ColourPicker(const std::vector<sf::Color>& _vColours)
{
    // Get the position
    int x = m_tOffset.x;
    int y = 0;

    // Dimensions of each individual colour bxo
    const int w = 15, h = 15;

    // Whether or not we're on the bottom row
    bool bBottom = false;

    // Set its RGB to one that isnt one of our colours so we can
    // tell if a colour was chosen 
    sf::Color rReturn = g_rColourInvalid;

    sf::Color rOutline(40, 40, 40);

    // Loop thru all colours and draw their box
    for (int i = 0; i < _vColours.size(); i++)
    {
        sf::RectangleShape* pRect = new sf::RectangleShape(sf::Vector2f(w, h));

        // Check if we're on the bottom row
        bBottom = (i % 2 == 1);

        // Get x position
        x = m_tOffset.x;

        // Every second colour box is drawn on the bottom row
        y = bBottom ? 15 : 0;

        // Check if hovering
        if (InArea(x, y, w, h))
        {
            // Change colour if hovering
            rOutline = sf::Color(200, 200, 200);

            // Change colour if clicked
            if (IsMouseClicked())
            {
                rOutline = sf::Color(170, 170, 170);
            }
        }
        else
        {
            rOutline = sf::Color(40, 40, 40);
        }


        pRect->setPosition(x, y);
        pRect->setFillColor(_vColours.at(i));
        pRect->setOutlineColor(rOutline);
        pRect->setOutlineThickness(-1);

        m_pDrawables.push_back(pRect);

        // If we're on the bottom row, offset so we can move onto
        // the next column of colours
        if (bBottom)
            m_tOffset.x += w;
    }

    return rReturn;
}
