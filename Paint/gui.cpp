//
//  gui.cpp
//
#include "gui.h"

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
EMenuItem CGui::HandleMenuBar(const std::vector<TMenuItemData>& _vItems)
{
    // Loop thru our menu items and draw them
    for (TMenuItemData rItem : _vItems)
    {
        switch (rItem.m_eType)
        {
        case MENU_ITEM_BUTTON:
	        {
                // If our item is a button then draw it with the given label
                // and return its value if clicked
	            if (Button(rItem.m_strLabel))
	                return rItem.m_eValue;
	        }
            break;

        case MENU_ITEM_COLOUR:
	        {
		        // TODO : 
	        }
            break;

        default:
        case MENU_ITEM_INVALID:
            // MENU_ITEM_INVALID should only be triggered if we forgot to
            // give our TMenuItem a type, which shouldn't happen but who knows
            continue;
        }
    }

    // Nothing clicked
    return MENU_ITEM_NONE;
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
    const int w = 70, h = 30;

    // Create our label
    sf::Text* pLabel = Label(x + 10, y + 5, _strLabel);

    // The labels bounds
    sf::FloatRect rTextBounds = pLabel->getLocalBounds();

    // Center the label in the box, have to scale down the bounds since we scaled the text down
    pLabel->setPosition(x + (w / 2) - ((rTextBounds.width * 0.2f) / 2), y + (h / 2) - ((rTextBounds.height * 0.2f) / 2) - 3);

    // Offset the next item along the X axis
    m_tOffset.x += w;

    // The fill colour of our box
    sf::Color tBoxCol = sf::Color::White;

    // Check if hovering
    if (InArea(x, y, w, h))
    {
        // Change colour if hovering
        tBoxCol = sf::Color(200, 200, 200);

        // Change colour if clicked
        if (IsMouseClicked())
            tBoxCol = sf::Color(170, 170, 170);
    }

    // Our box shape
    sf::RectangleShape* pRect = new sf::RectangleShape(sf::Vector2f(w, h));

    pRect->setPosition(x, y);
    pRect->setFillColor(tBoxCol);
    pRect->setOutlineColor(sf::Color(40, 40, 40));
    pRect->setOutlineThickness(-1);

    m_pDrawables.push_back(pRect);

    // Add the label after so its drawn on top
    m_pDrawables.push_back(pLabel);

    // return value = was the button clicked?
    return InArea(x, y, w, h) && IsMouseClicked();
}
