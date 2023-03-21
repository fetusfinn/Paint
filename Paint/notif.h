//
//	notif.h
//
#pragma once

#include <string>
#include <vector>
#include <chrono>

#include <SFML/Graphics.hpp>

// Better for readability, and dont want to make a variable
#define NOTIF_DURATION 5

//
// Our notification type
//
struct TNotification
{
	// The message for our notif to display
	std::string m_strMessage;

	// The alpha of our notif
	float m_fAlpha;

	// The value our timer was on last tick
	int m_iLastCount;

	// The exact time the 
	std::chrono::time_point<std::chrono::system_clock> m_rCreationTime;

	TNotification(const std::string& _strMsg) : m_strMessage(_strMsg), m_fAlpha(200.f), m_iLastCount(5000)
	{
		// Notifcation will disappear after 5 seconds
		m_rCreationTime = std::chrono::system_clock::now() + std::chrono::seconds(NOTIF_DURATION);
	}
};

//
// Notification manager classes, handles all notifications
//
class CNotificationManager
{
private:

	// List of all our notifications
	std::vector<TNotification> m_vNotifs;

public:

	void Update();

	void Draw(sf::RenderWindow* _pWindow);

	//
	// Adds a new notification for us to draw
	//
	void Add(const std::string& _strMessage)
	{
		m_vNotifs.push_back(TNotification(_strMessage));
	}
};