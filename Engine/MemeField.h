#pragma once

#include "Graphics.h"
#include "Sound.h"

class MemeField
{
public:
	enum class State
	{
		Fucked,
		Winrar,
		Memeing
	};
	enum class FieldSize {
		Small,
		Medium,
		Large
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
		void SpawnMeme();
		bool HasMeme() const;
		void Draw( const Vei2& screenPos,MemeField::State fucked,Graphics& gfx ) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		bool HasNoNeighborMemes() const;
		void SetNeighborMemeCount( int memeCount );
		
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
	};
public:
	MemeField ( ) = default;
	MemeField( const Vei2& center,int nMemes );
	void Draw( Graphics& gfx ) const;
	RectI GetRect() const;
	void OnRevealClick( const Vei2& screenPos );
	void OnFlagClick( const Vei2& screenPos );
	State GetState() const;
	void SetSize ( MemeField::FieldSize size );
	void SetState ( MemeField::State state );
private:
	void RevealTile( const Vei2& gridPos );
	Tile& TileAt( const Vei2& gridPos );
	const Tile& TileAt( const Vei2& gridPos ) const;
	Vei2 ScreenToGrid( const Vei2& screenPos );
	int CountNeighborMemes( const Vei2& gridPos );
	bool GameIsWon() const;
	
private:
	std::pair <int, int> smallField = { 8,6 };
	std::pair <int, int> mediumField = { 11,8 };
	std::pair <int, int> largeField = { 14,10 };

	int width = 8;
	int height = 6;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound( L"spayed.wav" );
	Vei2 topLeft;
	State state = State::Memeing;
	Tile field[48];
};