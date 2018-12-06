#pragma once

#include "Graphics.h"
#include "Sound.h"
#include <string>
#include <vector>
#include "MemeMessage.h"
#include <memory>
#include "Clue.h"
#include <unordered_map>

class MemeField
{
public:
	enum class State
	{
		Fucked,
		Winrar,
		Memeing
	};
	
private:
	class Tile
	{
	public:
		enum class State
		{
			Hidden,
			Flagged,
			Revealed
		};
		struct TileSprite
		{
			int index;
			std::wstring name;
		};
		std::vector <TileSprite> TileSprites =  //14 tiles
		{
			{0, L"tile0.bmp"			},
			{1, L"tile1.bmp"			},
			{2, L"tile2.bmp"			},
			{3, L"tile3.bmp"			},
			{4, L"tile4.bmp"			},
			{5, L"tile5.bmp"			},
			{6, L"tile6.bmp"			},
			{7, L"tile7.bmp"			},
			{8, L"tile8.bmp"			},
			{9,L"tilebutton.bmp"		},
			{10, L"tilecross.bmp"	},
			{11, L"tileflag.bmp"		},
			{12, L"tilebomb.bmp"		},
			{13, L"tilebombred.bmp"	}
		};
	public:
		//Tile() = default;
		Tile()
		{
			for (size_t i = 0; i < sizeof(TileSurfaces) / sizeof(TileSurfaces[0]); i++)
			{
				TileSurfaces[i] = (Codex<Surface>::Retrieve(L"Images/TileSprites/" + TileSprites[i].name));
			}		
		}

		void SpawnMeme();
		void SpawnLetterMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, MemeField::State fucked, Graphics& gfx) const;
		void DrawTileNumber(const Vei2& pos, int n, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		bool HasNoNeighborMemes() const;
		void SetNeighborMemeCount(int memeCount);
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		bool isLetter = false;
		int nNeighborMemes = -1;
		const Surface* TileSurfaces[14];
	};
public:
	MemeField ( ) = default;
	MemeField ( const Vei2& center, int nMemes, int fieldWidth, int fieldHeight, Graphics& gfx);
	void Draw ( Graphics& gfx ) const;
	RectI GetRect ( ) const;
	void OnRevealClick ( const Vei2& screenPos );
	void OnFlagClick ( const Vei2& screenPos );
	State GetState ( ) const;
	static int GetWidth ( );
	static int GetHeight ( );
	static int GetMemeBaseNum ( );
private:
	void RevealTile ( const Vei2& gridPos );
	Tile& TileAt ( const Vei2& gridPos );
	const Tile& TileAt ( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid ( const Vei2& screenPos );
	int CountNeighborMemes ( const Vei2& gridPos );
	bool GameIsWon ( ) const;
private:
	static constexpr int baseWidth = 20;
	static constexpr int baseHeight = 5;
	static constexpr int baseMemes = 6;
	static constexpr int letterHeight = 6;
	static constexpr int letterWidth = 6;

	int nMemes;
	int width;
	int height;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound ( L"spayed.wav" );
	Vei2 topLeft;
	State state = State::Memeing;
	std::unique_ptr<Tile[]> pTileField;
	std::unique_ptr<MemeMessage> message;
	Clue clue;
	Graphics& gfx;
	int memeXPos = 0;
	int memeYPos = 0;
	int rowTop = 0;
	int rowBottom = height / 2;
};