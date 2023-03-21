//
//	save.h
//
#pragma once

#include <SFML/Graphics.hpp>

namespace FileLoader
{
	extern void SaveCanvas(sf::RenderTexture* _pRenderTex);
	extern bool LoadCanvas(sf::RenderTexture* _pRenderTex);
}