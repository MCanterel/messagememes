#pragma once

#include "Vei2.h"
#include "SpriteEffect.h"
#include "Codex.h"

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
	Clue( const Vei2& pos )
		:
		pos( pos )
	{
	}
	void Draw( Graphics& gfx ) const
	{
		// calculate drawing base
		const auto draw_pos = pos;
		// switch on effectState to determine drawing method
		switch( effectState )
		{
		case EffectState::Hit:
			// flash white for hit
			gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*pClueSurface,
							SpriteEffect::Substitution{ Colors::White,Colors::White }
			);
			break;
		case EffectState::Dying:
			// draw dissolve effect during dying (tint red)
			gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*pClueSurface,
							SpriteEffect::DissolveHalfTint{ Colors::White,Colors::Red,
							1.0f - effectTime / dissolveDuration }
			);
			break;
		case EffectState::Normal:
			gfx.DrawSprite( int( draw_pos.x ),int( draw_pos.y ),*pClueSurface,
							SpriteEffect::Chroma{ Colors::White }
			);
			break;
		}
	}
	
	void Update( float dt )
	{
	
		// always update effect time (who cares brah?)
		effectTime += dt;
		// effect state machine logic
		//switch( effectState )
		//{
		//case EffectState::Hit:
		//	if( effectTime >= hitFlashDuration )
		//	{
		//		// if we are dead, transition to dying dissolve state
		//		if( IsDead() )
		//		{
		//			effectState = EffectState::Dying;
		//			effectTime = 0.0f;
		//		}
		//		else
		//		{
		//			effectState = EffectState::Normal;
		//		}
		//	}
		//	break;
		//}
	}

	const Vei2& GetPos() const
	{
		return pos;
	}

private:
	const Surface* pClueSurface = Codex<Surface>::Retrieve( L"Images/y_u_no_guy.jpg" );
	Vei2 pos;
	static constexpr float dissolveDuration = 0.6f;
	static constexpr float hitFlashDuration = 0.045f;
	float effectTime = 0.0f;
	EffectState effectState = EffectState::Normal;
};