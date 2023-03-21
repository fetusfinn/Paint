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
CGui::CGui(sf::RenderWindow* _pWindow) : m_pWindow(_pWindow), m_rOffset({ 0,0 }), m_bColourOpen(false)
{

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
    m_rOffset = { 0 , 0 };
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
                    Global::rBrushColour = rCol;
	        }
        	break;

        default:
        case 0: // MENU_ITEM_INVALID
            // MENU_ITEM_INVALID should only be triggered if we forgot to
            // give our TMenuItem a type, which shouldn't happen but who knows
            continue;
        }
    }

    // Set to the size of the menu
    Global::rExclusionZone = sf::Vector2f(m_rOffset.x, 30);

    // Nothing clicked
    return iReturn;
}

//
// Create a button for us to draw and add functionality
//
bool CGui::Button(const std::string& _strLabel)
{
    // Get this buttons position
    float x = m_rOffset.x;
    float y = 0;

    // Dimensions and position of our button
    const float w = 80, h = 30;

    // Create our label
    sf::Text* pLabel = Global::CreateLabel(x + 10, y + 5, _strLabel);

    // The labels bounds
    sf::FloatRect rTextBounds = pLabel->getLocalBounds();

    // Center the label in the box, have to scale down the bounds since we scaled the text down
    pLabel->setPosition(x + (w / 2) - ((rTextBounds.width * 0.2f) / 2), y + (h / 2) - ((rTextBounds.height * 0.2f) / 2) - 3);

    // Offset the next item along the X axis
    // -1 so that the outlines on each side overlap and stop them
    // from looking thicker than they should
    m_rOffset.x += w - 1;

    // The fill colour of our box
    sf::Color rBoxCol = sf::Color::White;

    // Our return value
    bool bReturn = false;

    // Check if hovering
    if (Global::InArea(x, y, w, h, *m_pWindow))
    {
        // Change colour if hovering
        rBoxCol = sf::Color(200, 200, 200);

        // Change colour if clicked
        if (Global::IsMouseClicked())
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
    return Global::InArea(x, y, w, h, *m_pWindow) && Global::WasMouseJustClicked();
}

//
// Create a button for us to draw and add functionality
//
sf::Color CGui::ColourPicker(const std::vector<sf::Color>& _vColours)
{
    // Get the position
    float x = m_rOffset.x;
    float y = 0;

    // Dimensions of each individual colour box
    const float w = 30, h = 30;

    // Set its RGB to one that isnt one of our colours so we can
    // tell if a colour was chosen 
    sf::Color rReturn = g_rColourInvalid;

    // The outline colour for each box
    sf::Color rOutline(40, 40, 40);

    // Check if hovering in the main box
    if (Global::InArea(x, y, w, h, *m_pWindow))
    {
        // Open/close the colour picker when mouse clicked
        if (Global::WasMouseJustClicked())
            m_bColourOpen = !m_bColourOpen;
    }

    // The shape for the main button
    sf::RectangleShape* pButtonRect = new sf::RectangleShape(sf::Vector2f(w, h));

    pButtonRect->setPosition(x, y);
    pButtonRect->setFillColor(Global::rBrushColour);
    pButtonRect->setOutlineColor(rOutline);
    pButtonRect->setOutlineThickness(-1);

    m_pDrawables.push_back(pButtonRect);

    m_rOffset.x += w - 1;


    if (m_bColourOpen)
    {
        // Loop thru all colours and draw their box
        for (int i = 0; i < _vColours.size(); i++)
        {
            // The shape for the individual colours
            sf::RectangleShape* pColourRect = new sf::RectangleShape(sf::Vector2f(w, h));

            // Get x position
            x = m_rOffset.x;

            // Check if hovering
            if (Global::InArea(x, y, w, h, *m_pWindow))
            {
                // Update the colour on release
                if (Global::WasMouseJustClicked())
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
            if (Global::rBrushColour == _vColours.at(i))
                rOutline = sf::Color(100, 100, 100);


            pColourRect->setPosition(x, y);
            pColourRect->setFillColor(_vColours.at(i));
            pColourRect->setOutlineColor(rOutline);
            pColourRect->setOutlineThickness(-1);

            m_pDrawables.push_back(pColourRect);

            m_rOffset.x += w - 1;
        }
    }

    return rReturn;
}
