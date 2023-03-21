//
//	save.cpp
//
#include <string>
#include <fstream>
#include <filesystem>

#include "globals.h"
#include "save.h"

#include <Windows.h>


// Our file paths
static const std::string g_rFolder		= "Saves\\";
static const std::string g_rFilename	= "image";

// Get the path to our program
// Made static because this shouldnt be changed
static std::filesystem::path g_rLoadPath;

//
// Returns true if the given path exists
//
inline bool DoesPathExist(const std::filesystem::path& _rPath)
{
	return std::filesystem::exists(_rPath);
}

//
// Returns how many files we have saved in our saves folder
//
static int GetNumberOfSaves(const std::filesystem::path& _rPath)
{
	int iCount = 0;

	// Loop thru all the files/folders in the given 
	for (const std::filesystem::directory_entry& rEntry : std::filesystem::directory_iterator(_rPath))
		iCount++;

	return iCount;
}

//
// Saves the current canvas to an image
//
void FileLoader::SaveCanvas(sf::RenderTexture* _pRenderTex)
{
	if (g_rLoadPath.empty())
		g_rLoadPath = std::filesystem::current_path().string() + "\\" + g_rFolder;

	// Check if our saves folder exists, if not then create it
	if (!DoesPathExist(g_rLoadPath))
		std::filesystem::create_directory(g_rLoadPath);

	// We're just gonna consider everything in this folder as a saved image
	// because thats all we need for now, can improve in the future
	int iSaves = GetNumberOfSaves(g_rLoadPath);

	_pRenderTex->display();

	std::string strPath = g_rLoadPath.string() + g_rFilename + std::to_string(iSaves + 1) + ".jpg";

	// Save the image to Saves/image1.png
	_pRenderTex->getTexture().copyToImage().saveToFile(strPath);
}

//
// Sets all the correct info for our OPENFILENAME to use
//
void InitialiseOpenFileName(OPENFILENAME& _rFile, wchar_t* _szFile, const int _kSize)
{
	if (g_rLoadPath.empty())
		g_rLoadPath = std::filesystem::current_path().string() + "\\" + g_rFolder;

	// Our path converted to a wstring
	std::wstring rWide(g_rLoadPath.c_str());

	// Clear the memory before using
	ZeroMemory(&_rFile, sizeof(_rFile));
	_rFile.lStructSize		= sizeof(_rFile);
	_rFile.hwndOwner		= GetActiveWindow();
	_rFile.lpstrFile		= _szFile;
	_rFile.lpstrFile[0]		= '\0';							// Set the first char a null
	_rFile.nMaxFile			= _kSize;
	_rFile.lpstrFilter		= L"*.png\0*.jpg\0*.jpeg\0";	// Only want to search for png
	_rFile.nFilterIndex		= 1;
	_rFile.lpstrFileTitle	= NULL;
	_rFile.lpstrInitialDir	= rWide.c_str();				// Set the start dir to our saved folder
	_rFile.Flags			= OFN_ENABLETEMPLATEHANDLE | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;	// Enforce our files only
	_rFile.lpTemplateName	= L"Test";
}

//
// Loads a new canvas from a file
//
bool FileLoader::LoadCanvas(sf::RenderTexture* _pRenderTex)
{
	OPENFILENAME rFile;

	static const int kSize = 256;
	wchar_t szFile[kSize];

	InitialiseOpenFileName(rFile, szFile, kSize);

	GetOpenFileName(&rFile);

	std::wstring strWide = std::wstring(rFile.lpstrFile);
	std::string strFilePath = std::string(strWide.begin(), strWide.end());

	// If the filename is empty then  dont load
	if (strFilePath.empty())
		return false;

	// Load the image
	sf::Image rImage;
	rImage.loadFromFile(strFilePath);

	// Load the image into a texture
	sf::Texture rTex;
	rTex.loadFromImage(rImage);

	// Shape to draw to
	sf::RectangleShape rRect;

	// Make the rect the size of the screen
	rRect.setPosition(0, 0);
	rRect.setSize(sf::Vector2f(Global::iWindowWidth, Global::iWindowHeight));

	rRect.setTexture(&rTex);

	// Clear the screen before we load the new 
	_pRenderTex->clear(sf::Color::White);

	// Draw the rect with our image on it
	_pRenderTex->draw(rRect);

	return true;
}