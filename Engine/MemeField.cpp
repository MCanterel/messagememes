#include "MemeField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <algorithm>


void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}

void MemeField::Tile::Draw(const Vei2& screenPos, MemeField::State fieldState, Graphics& gfx) const
{
	if (fieldState != MemeField::State::Fucked)
	{
		switch (state)
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeighborMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			break;
		}
	}
	else // we are fucked
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(screenPos, gfx);
			}
			break;
		case State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileFlag(screenPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(screenPos, gfx);
				SpriteCodex::DrawTileCross(screenPos, gfx);
			}
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				SpriteCodex::DrawTileNumber(screenPos, nNeighborMemes, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBombRed(screenPos, gfx);
			}
			break;
		}
	}
}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden);
	state = State::Revealed;
}

bool MemeField::Tile::IsRevealed() const
{
	return state == State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert(!IsRevealed());
	if (state == State::Hidden)
	{
		state = State::Flagged;
	}
	else
	{
		state = State::Hidden;
	}
}

bool MemeField::Tile::IsFlagged() const
{
	return state == State::Flagged;
}

bool MemeField::Tile::HasNoNeighborMemes() const
{
	return nNeighborMemes == 0;
}

void MemeField::Tile::SetNeighborMemeCount(int memeCount)
{
	assert(nNeighborMemes == -1);
	nNeighborMemes = memeCount;
}

MemeField::MemeField(const Vei2& center, int nMemes, int fieldWidth, int fieldHeight)
	:
	topLeft(center - Vei2(width * SpriteCodex::tileSize, height * SpriteCodex::tileSize) / 2),
	width(fieldWidth),
	height(fieldHeight),
	pField(new Tile[width * height]),
	nMemes(nMemes),  //this is a little roundabout- game.cpp gets the baseMemes from memefield, then, depending on game size, calls memefield here with nMemes. Need to fix.	
	//c("C"),
	//a("A"),
	//t("T"),
	m(new MemeMessage)

{
	assert(nMemes > 0 && nMemes < width * height);

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	//add message memes here
	for (auto letter : m->PhraseGrid)
	{
		Vei2 letterSpawnPos = { memeXPos,std::min(height - 5, yDist(rng)) };
		for (auto vec : letter->LetterGrid) {
			Vei2 letterSegmentPos = letterSpawnPos + vec;
			if (!(TileAt(letterSegmentPos).HasMeme())) {
				TileAt(letterSegmentPos).SpawnMeme();
			}
		}
		if (memeXPos <= (width - 10))
		{
			memeXPos += memeXSpacing;
		}
	}

/*
	int s = m->PhraseGrid.size();

	Vei2 letterSpawnPos1 = { std::min(width - 4, xDist(rng)),std::min(height - 5, yDist(rng)) };
	for (auto vec : c.LetterGrid) {    
		Vei2 letterSegmentPos = letterSpawnPos1 + vec;
		if (!(TileAt(letterSegmentPos).HasMeme())) {
			TileAt(letterSegmentPos).SpawnMeme();
		}
	}

	Vei2 letterSpawnPos2 = { std::min(width - 4, xDist(rng)),std::min(height - 5, yDist(rng)) };
	for (auto vec : a.LetterGrid) {    
		Vei2 letterSegmentPos = letterSpawnPos2 + vec;
		if (!(TileAt(letterSegmentPos).HasMeme())) {
			TileAt(letterSegmentPos).SpawnMeme();
		}
	}
	Vei2 letterSpawnPos3 = { std::min(width - 4, xDist(rng)),std::min(height - 5, yDist(rng)) };
	for (auto vec : t.LetterGrid) {    
		Vei2 letterSegmentPos = letterSpawnPos3 + vec;
		if (!(TileAt(letterSegmentPos).HasMeme())) {
			TileAt(letterSegmentPos).SpawnMeme();
		}
	}*/

	/*for (int nSpawned = 0; nSpawned < nMemes; ++nSpawned)
	{
		Vei2 spawnPos;
		do
		{
			spawnPos = { xDist(rng),yDist(rng) };
		} while (TileAt(spawnPos).HasMeme());

		TileAt(spawnPos).SpawnMeme();
	}*/

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).SetNeighborMemeCount(CountNeighborMemes(gridPos));
		}
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect().GetExpanded(borderThickness), borderColor);
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);
	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw(topLeft + gridPos * SpriteCodex::tileSize, state, gfx);
		}
	}
}

RectI MemeField::GetRect() const
{
	return RectI(topLeft, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize);
}

void MemeField::OnRevealClick(const Vei2& screenPos)
{
	if (state == State::Memeing)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
		RevealTile(gridPos);
		if (GameIsWon())
		{
			state = State::Winrar;
		}
	}
}

void MemeField::OnFlagClick(const Vei2 & screenPos)
{
	if (state == State::Memeing)
	{
		const Vei2 gridPos = ScreenToGrid(screenPos);
		assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);
		Tile& tile = TileAt(gridPos);
		if (!tile.IsRevealed())
		{
			tile.ToggleFlag();
			if (tile.IsFlagged() && GameIsWon())
			{
				state = State::Winrar;
			}
		}
	}
}

MemeField::State MemeField::GetState() const
{
	return state;
}

int MemeField::GetWidth()
{
	return baseWidth;
}

int MemeField::GetHeight()
{
	return baseHeight;
}

int MemeField::GetMemeBaseNum()
{
	return baseMemes;
}

void MemeField::FreeResources()
{
	delete[] pField;
	pField = nullptr;
}

void MemeField::RevealTile(const Vei2& gridPos)
{
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed() && !tile.IsFlagged())
	{
		tile.Reveal();
		if (tile.HasMeme())
		{
			state = State::Fucked;
			sndLose.Play();
		}
		else if (tile.HasNoNeighborMemes())
		{
			const int xStart = std::max(0, gridPos.x - 1);
			const int yStart = std::max(0, gridPos.y - 1);
			const int xEnd = std::min(width - 1, gridPos.x + 1);
			const int yEnd = std::min(height - 1, gridPos.y + 1);

			for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++)
			{
				for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
				{
					RevealTile(gridPos);
				}
			}
		}
	}
}

MemeField::Tile& MemeField::TileAt(const Vei2& gridPos)
{
	return pField[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	return pField[gridPos.y * width + gridPos.x];
}

Vei2 MemeField::ScreenToGrid(const Vei2& screenPos)
{
	return (screenPos - topLeft) / SpriteCodex::tileSize;
}

int MemeField::CountNeighborMemes(const Vei2 & gridPos)
{
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width - 1, gridPos.x + 1);
	const int yEnd = std::min(height - 1, gridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (TileAt(gridPos).HasMeme())
			{
				count++;
			}
		}
	}
	return count;
}

bool MemeField::GameIsWon() const {
	for (int i = 0; i < (width * height); i++) {
		const Tile& t = pField[i];
		if ((t.HasMeme() && !t.IsFlagged()) ||
			(!t.HasMeme() && !t.IsRevealed()))
		{
			return false;
		}
	}
	return true;
}

MemeField::Letter::Letter(const std::string & letter)
{
	LetterGrid.resize(15, { 0,0 });

	if (letter == "C") {
		LetterGrid[0] = { 0,0 };
		LetterGrid[1] = { 1,0 };
		LetterGrid[2] = { 2,0 };
		LetterGrid[3] = { 0,1 };
		LetterGrid[4] = { 0,2 };
		LetterGrid[5] = { 0,3 };
		LetterGrid[6] = { 0,4 };
		LetterGrid[7] = { 1,4 };
		LetterGrid[8] = { 2,4 };
		LetterGrid[9] = { 3,4 };
	}
	if (letter == "A") {
		LetterGrid[0] = { 0,0 };
		LetterGrid[1] = { 1,0 };
		LetterGrid[2] = { 2,0 };
		LetterGrid[3] = { 0,1 };
		LetterGrid[4] = { 2,1 };
		LetterGrid[5] = { 0,2 };
		LetterGrid[6] = { 1,2 };
		LetterGrid[7] = { 2,2 };
		LetterGrid[8] = { 0,3 };
		LetterGrid[9] = { 2,3 };
		LetterGrid[10] = { 0,4 };
		LetterGrid[11] = { 2,4 };
	}
	if (letter == "T") {
		LetterGrid[0] = { 0,0 };
		LetterGrid[1] = { 1,0 };
		LetterGrid[2] = { 2,0 };
		LetterGrid[3] = { 1,1 };
		LetterGrid[4] = { 1,2 };
		LetterGrid[5] = { 1,3 };
		LetterGrid[6] = { 1,4 };
	}
}

