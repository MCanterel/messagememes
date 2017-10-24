#pragma once

#include "Graphics.h"
#include "Sound.h"
#include <string>

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
	public:
		void SpawnMeme ( );
		bool HasMeme ( ) const;
		void Draw ( const Vei2& screenPos, MemeField::State fucked, Graphics& gfx ) const;
		void Reveal ( );
		bool IsRevealed ( ) const;
		void ToggleFlag ( );
		bool IsFlagged ( ) const;
		bool HasNoNeighborMemes ( ) const;
		void SetNeighborMemeCount ( int memeCount );
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
	class Letter {
	public:
		Letter(const std::string& letter);
		//Vei2* GetLetter(const std::string& letter);
		static const int GetLetterGridCount();
		Vei2 LetterGrid[12];
	private:
		static constexpr int nLetterTiles = 12;
		//Vei2 LetterGrid[nLetterTiles];
		//Vei2* pLetterGrid = LetterGrid;
	};

public:
	MemeField ( ) = default;
	MemeField ( const Vei2& center, int nMemes, int fieldWidth, int fieldHeight );
	void Draw ( Graphics& gfx ) const;
	RectI GetRect ( ) const;
	void OnRevealClick ( const Vei2& screenPos );
	void OnFlagClick ( const Vei2& screenPos );
	State GetState ( ) const;
	static int GetWidth ( );
	static int GetHeight ( );
	static int GetMemeBaseNum ( );
	void FreeResources ( );

	//void SetSize ( );
private:
	void RevealTile ( const Vei2& gridPos );
	Tile& TileAt ( const Vei2& gridPos );
	const Tile& TileAt ( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid ( const Vei2& screenPos );
	int CountNeighborMemes ( const Vei2& gridPos );
	bool GameIsWon ( ) const;
private:
	static constexpr int baseWidth = 4;
	static constexpr int baseHeight = 3;
	static constexpr int baseMemes = 2;
	int width = baseWidth;
	int height = baseHeight;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound ( L"spayed.wav" );
	Vei2 topLeft;
	State state = State::Memeing;
	Tile* pField = nullptr;
	Letter c;
	//Tile field [ width * height ];  old static field
};