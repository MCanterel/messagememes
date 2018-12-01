#pragma once

#include "Graphics.h"
#include "Sound.h"
#include <string>
#include <vector>
#include "MemeMessage.h"
#include <memory>

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
		Tile() = default;
		void SpawnMeme();
		void SpawnLetterMeme();
		bool HasMeme() const;
		void Draw(const Vei2& screenPos, MemeField::State fucked, Graphics& gfx) const;
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
private:
	void RevealTile ( const Vei2& gridPos );
	Tile& TileAt ( const Vei2& gridPos );
	const Tile& TileAt ( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid ( const Vei2& screenPos );
	int CountNeighborMemes ( const Vei2& gridPos );
	bool GameIsWon ( ) const;
private:
	static constexpr int baseWidth = 20;
	static constexpr int baseHeight = 4;
	static constexpr int baseMemes = 0;
	int nMemes;
	int width = baseWidth;
	int height = baseHeight;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound ( L"spayed.wav" );
	Vei2 topLeft;
	State state = State::Memeing;
	std::unique_ptr<Tile[]> pTileField;
	std::unique_ptr<MemeMessage> message;
	int memeXPos = 0;
	int memeYPos = 0;
	int memeXSpacing = 6;
	int memeYSpacing = 3;
	int rowTop = 0;
	int rowBottom = height / 2;
};