#pragma once

#include "Vei2.h"
#include "SpriteEffect.h"
#include "Surface.h"
#include "Codex.h"
#include <string>

class Clue
{

public:
	Clue(std::string clueName )
		
	{
		std::wstring sName(clueName.begin(), clueName.end());
		std::wstring lName = L"Images/" + sName;
		lName.erase(std::remove_if(lName.begin(), lName.end(), ::isspace), lName.end());
		pClueSurface = (Codex<Surface>::Retrieve(lName));
	}

	void Draw(Graphics& gfx) const
	{
		gfx.DrawSprite(gfx.ScreenWidth /2 - pClueSurface->GetWidth() / 2 , posY, *pClueSurface, SpriteEffect::Copy{});
	}
	const int GetPosY() const
	{
		return posY;
	}

private:
	const Surface* pClueSurface;


	int posY = 100;
	static constexpr float dissolveDuration = 0.6f;
	static constexpr float hitFlashDuration = 0.045f;
};