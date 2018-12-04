#pragma once

#include "Vei2.h"
#include "SpriteEffect.h"
#include "Surface.h"
#include "Codex.h"
#include <string>
//#include "mc_utils.h"


class Clue
{
private:
	enum class EffectState
	{
		Normal,
		Hit,
		Dying
	};
public:
	Clue(std::string clueName )
		
	{
		std::wstring sName(clueName.begin(), clueName.end());
		std::wstring lName = L"Images/" + sName;
		//const std::wstring fullName = delExtraSpaces(lName);
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
	float effectTime = 0.0f;
	EffectState effectState = EffectState::Normal;
	//const std::wstring cName;
};