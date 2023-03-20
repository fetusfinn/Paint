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
// Constructor
//
CGui::CGui(sf::RenderWindow* _pWindow) : m_pWindow(_pWindow), m_tOffset({ 0,0 })
{
    // Load our font, maybe make this static or global
    m_rFont.loadFromFile("fonts/arial.ttf");
}

//
// Destructor
// 
CGui::~CGui()
{
    // Delete all the remaining pointers
	for (sf::Drawable* pDrawable : m_pDrawables)
	{
		if (pDrawable)
		{
            delete pDrawable;
            pDrawable = nullptr;
		}
	}

    // Clear the vector
    m_pDrawables.clear();
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
                    g_rBrushColour = rCol;
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
    // -1 so that the outlines on each side overlap and stop them
    // from looking thicker than they should
    m_tOffset.x += w - 1;

    // The fill colour of our box
    sf::Color rBoxCol = sf::Color::White;

    // Our return value
    bool bReturn = false;

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
    return InArea(x, y, w, h) && WasMouseJustClicked();
}

//
// Create a button for us to draw and add functionality
//
sf::Color CGui::ColourPicker(const std::vector<sf::Color>& _vColours)
{
    // Get the position
    int x = m_tOffset.x;
    int y = 0;

    // Dimensions of each individual colour box
    const int w = 30, h = 30;

    // Set its RGB to one that isnt one of our colours so we can
    // tell if a colour was chosen 
    sf::Color rReturn = g_rColourInvalid;

    // The outline colour for each box
    sf::Color rOutline(40, 40, 40);

    // Check if hovering in the main box
    if (InArea(x, y, w, h))
    {
        // Open/close the colour picker when mouse clicked
        if (WasMouseJustClicked())
            m_bColourOpen = !m_bColourOpen;
    }

    // The shape for the main button
    sf::RectangleShape* pButtonRect = new sf::RectangleShape(sf::Vector2f(w, h));

    pButtonRect->setPosition(x, y);
    pButtonRect->setFillColor(g_rBrushColour);
    pButtonRect->setOutlineColor(rOutline);
    pButtonRect->setOutlineThickness(-1);

    m_pDrawables.push_back(pButtonRect);

    m_tOffset.x += w - 1;


    if (m_bColourOpen)
    {
        // Loop thru all colours and draw their box
        for (int i = 0; i < _vColours.size(); i++)
        {
            // The shape for the individual colours
            sf::RectangleShape* pColourRect = new sf::RectangleShape(sf::Vector2f(w, h));

            // Get x position
            x = m_tOffset.x;

            // Check if hovering
            if (InArea(x, y, w, h))
            {
                // Update the colour on release
                if (WasMouseJustClicked())
                {
                    // If a colour hasnt already been chosen
                    // then set the return colour
                    if (rReturn == g_rColourInvalid)
                        rReturn = _vColours.at(i);

                    m_bColourOpen = false;
                }
            }
            else
            {
                rOutline = sf::Color(40, 40, 40);
            }

            // If the current colour is our selected colour
            // then give it a different outline
            if (g_rBrushColour == _vColours.at(i))
                rOutline = sf::Color(100, 100, 100);


            pColourRect->setPosition(x, y);
            pColourRect->setFillColor(_vColours.at(i));
            pColourRect->setOutlineColor(rOutline);
            pColourRect->setOutlineThickness(-1);

            m_pDrawables.push_back(pColourRect);

            m_tOffset.x += w - 1;
        }
    }

    return rReturn;
}
