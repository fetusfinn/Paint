//
//	notif.cpp
//
#include "notif.h"

#include "globals.h"

//
// Updates each notification
//
void CNotificationManager::Update()
{
	std::chrono::time_point<std::chrono::system_clock> rNow = std::chrono::system_clock::now();

	// How much to decrease the alpha by each millisecond
	static const float fDecrease = (200.f / 5000.f);

	// Loop thru all our notifications
	for (int i = 0; i < m_vNotifs.size(); i++)
	//(const TNotification& rNotif : m_vNotifs)
	{
		TNotification& rNotif = m_vNotifs.at(i);

		std::chrono::duration<float> rDiff = rNotif.m_rCreationTime - rNow;

		// Get how many milliseconds left
		const int iMillisec = rDiff.count() * 1000;

		// How many milliseconds have elapsed since the last update 
		int iDiff = rNotif.m_iLastCount - iMillisec;

		// Decrease the alpha but fDecrease for every millisecond that passes
		rNotif.m_fAlpha -= fDecrease * iDiff;

		// Clamp
		if (rNotif.m_fAlpha < 0)
			rNotif.m_fAlpha = 0;

		// If the timer has run out or the alpha is 0 then erase this index
		if (iMillisec <= 0 || rNotif.m_fAlpha <= 0)
			m_vNotifs.erase(m_vNotifs.begin() + i);

		// Set the last count
		rNotif.m_iLastCount = iMillisec;
	}
}

//
// Draws all the notifications
//
void CNotificationManager::Draw(sf::RenderWindow* _pWindow)
{
	int w = 90, h = 10;
	int x = Global::iWindowWidth - 20 - w;
	int y = Global::iWindowHeight - 20 - h;

	// Loop thru all and draw
	for (int i = 0; i < m_vNotifs.size(); i++)
	{
		const TNotification& rNotif = m_vNotifs.at(i);

		sf::Text* pLabel = Global::CreateLabel(x, y, rNotif.m_strMessage);

		// The labels bounds
		sf::FloatRect rTextBounds = pLabel->getLocalBounds();

		// Center the label, have to scale down the bounds since we scaled the text down
		pLabel->setPosition(x + (w / 2) - ((rTextBounds.width * 0.2f) / 2), y + (h / 2) - ((rTextBounds.height * 0.2f) / 2) - 3 - (i * (h + 10)));

		pLabel->setFillColor(sf::Color(0, 0, 0, rNotif.m_fAlpha));

		_pWindow->draw(*pLabel);

		// Clean up because labels are created dynamically
		delete pLabel;
		pLabel = nullptr;
	}
}
