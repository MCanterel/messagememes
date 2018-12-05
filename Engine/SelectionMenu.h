#pragma once

#include "RectI.h"
#include "SpriteCodex.h"
#include "Mouse.h"
#include "Sound.h"
#include "SpriteEffect.h"
#include "Codex.h"

class SelectionMenu
{
public:
	enum class Size
	{
		Small,
		Large,
		Count,
		Invalid
	};
private:
	class Entry
	{
	public:
		Entry() = default;
		Entry( Size s,const Vei2& pos )
			:
			s( s ),
			rect( RectI::FromCenter( pos,SpriteCodex::sizeselWidth / 2,SpriteCodex::sizeselHeight / 2 ) )
		{}
		void Draw( Graphics& gfx ) const
		{
			if( highlighted )
			{
				gfx.DrawRect( rect.GetExpanded( highlightThickness ),highlightColor );
			}			 
			gfx.DrawRect( rect,Colors::Black );
			gfx.DrawSprite(gfx.ScreenWidth / 2 - pMemeTitle->GetWidth() / 2,100, *pMemeTitle, SpriteEffect::Copy{});
			DrawSizeText( s,rect.GetCenter(),gfx );
		}
		bool IsHit( const Vei2& pt ) const
		{
			return rect.Contains( pt );
		}
		void ResetHighlight()
		{
			highlighted = false;
		}
		void SetHighlight()
		{
			highlighted = true;
		}
		bool IsHighlighted() const
		{
			return highlighted;
		}
		Size GetSize() const
		{
			return s;
		}
	private:
		static void DrawSizeText( Size s,const Vei2& pos,Graphics& gfx )
		{
			switch( s )
			{
			case Size::Small:
				SpriteCodex::DrawSmallMenuItem( pos,gfx );
				break;
			case Size::Large:
				SpriteCodex::DrawLargeMenuItem( pos,gfx );
				break;
			}
		}
	private:
		static constexpr int highlightThickness = 6;
		static constexpr Color highlightColor = Colors::Yellow;
		bool highlighted = false;
		Size s;
		RectI rect;
		const Surface* pMemeTitle = Codex<Surface>::Retrieve(L"Images/all_your_memes.bmp");
	};
public:
	SelectionMenu( const Vei2& pos )
	{
		auto center = pos;
		for( int i = 0; i < int( Size::Count ); i++ )
		{
			entries[i] = { Size( i ),center };
			center.y += verticalSpacing;
		}
	}
	// returns Size::Something when an entry is clicked, otherwise returns Size::Invalid
	Size ProcessMouse( const Mouse::Event& e )
	{
		switch( e.GetType() )
		{
		case Mouse::Event::Type::Move:
			for( auto& n : entries )
			{
				if( n.IsHit( e.GetPos() ) )
				{
					// need to test here to prevent sfx from firing
					// on every mouse move event
					if( !n.IsHighlighted() )
					{
						ResetHighlights();
						n.SetHighlight();
						hover.Play(1.0, 0.1f);
					}
					// immediately exit if found a hit
					// (if we don't, highlight will be reset below)
					return Size::Invalid;
				}
			}
			// if no entry was hit, reset all highlights
			ResetHighlights();
			break;
		case Mouse::Event::Type::LPress:
			for( auto& n : entries )
			{
				if( n.IsHit( e.GetPos() ) )
				{
					return n.GetSize();
				}
			}
			break;
		}
		return Size::Invalid;
	}
	void Draw( Graphics& gfx ) const
	{
		for( const auto& n : entries )
		{
			n.Draw( gfx );
		}
	}
private:
	void ResetHighlights()
	{
		for( auto& n : entries )
		{
			n.ResetHighlight();
		}
	}
private:
	static constexpr int verticalSpacing = SpriteCodex::sizeselHeight * 2;
	Sound hover = { L"menu_boop.wav" };
	Entry entries[int( Size::Count )];
};