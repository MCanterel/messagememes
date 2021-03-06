
#pragma once
#include "MemeField.h"
#include <assert.h>
#include <random>
#include "Vei2.h"
#include "SpriteCodex.h"
#include <algorithm>
#include "Graphics.h"

MemeField::MemeField(const Vei2& center, int nMemes, int fieldWidth, int fieldHeight, Graphics& gfx)
	:
	topLeft(center - Vei2(width * SpriteCodex::tileSize, height * SpriteCodex::tileSize- 500) / 2),  //fix the magic number 400
	width(fieldWidth),
	height(fieldHeight),
	pTileField(std::make_unique<Tile[]>(width * height)),
	nMemes(nMemes),  //awkward- game.cpp gets the baseMemes from memefield. Need to fix.	
	message(std::make_unique<MemeMessage>()),	
	clue(message->GetPhraseText()),
	gfx(gfx)
{

	
	assert(nMemes >= 0 && nMemes < width * height);
	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	std::uniform_int_distribution<int> startXDist(0, 2);
	std::uniform_int_distribution<int> startYDist(0, 1);

	std::uniform_int_distribution<int> shiftXDist(0, 1);
	std::uniform_int_distribution<int> shiftYDist(0, 1);

	//TODO fix all this crap.
	//grab the length of the PhraseGrid, then center the lil fucker

	//size_t len = message->PhraseGrid.size();
	//if (len <= message->maxLettersPerLine)
	//{
	//	memeXPos = (width - len) / 2 + shiftXDist(rng);
	//}
	//else
	//{
		memeXPos = startXDist(rng) + shiftXDist(rng);
	//}
	
 //memeYPos = std::min(rowBottom - 2, yDist(rng) / 2 + 3);
	memeYPos = startYDist(rng) + shiftYDist(rng);
	//message->PhraseGrid.resize(message->PhraseGrid.size() - 1);

	for (auto letter : message->PhraseGrid)
	{
		bool isTab = letter->IsTab();
		if (!isTab)
		{
			Vei2 letterSpawnPos = { memeXPos, memeYPos };
			for (auto vec : letter->LetterGrid) {
				if (vec.y >= 0)  //if it's a valid tile (unused tiles are set to {-1,-1})
				{
					Vei2 letterSegmentPos = letterSpawnPos + vec;
					MemeField::Tile& curTile = TileAt(letterSegmentPos);
					if (!(curTile.HasMeme())) {
						curTile.SpawnLetterMeme();
					}
				}
			}
		}

		//next X,Y position sets up here
		//______________ YPos...
		if (memeYPos < rowTop + 1)  //too high
		{
			memeYPos = std::max(rowTop, rowTop + shiftYDist(rng));
		}
		else if (memeYPos > rowBottom - letterHeight - 1)  //too low
		{
			memeYPos = std::max(rowBottom - letterHeight, rowBottom - letterHeight - shiftYDist(rng));
		}
		else  //thass right
		{
			int moveY;
			startYDist(rng) == 1 ? moveY = shiftYDist(rng) : moveY = -shiftYDist(rng);
			memeYPos += moveY;
		}

		//______________ XPos...

		if (memeXPos <= (width - letterWidth * 2) && !isTab)  //tab used to demarcate new line
		{
			memeXPos += letterWidth + shiftXDist(rng);
		}
		else
		{
			// "carriage return" to new line
			rowTop = rowBottom;
			rowBottom = height;
			memeXPos = startXDist(rng) + shiftXDist(rng);
			memeYPos = rowTop + startYDist(rng);
		}
	}

	//add various extra memes per nMemes amount
	for (int nSpawned = 0; nSpawned < nMemes; ++nSpawned)
	{
		Vei2 spawnPos;
		do
		{
			spawnPos = { xDist(rng),yDist(rng) };
		} while (TileAt(spawnPos).HasMeme());

		TileAt(spawnPos).SpawnMeme();
	}

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).SetNeighborMemeCount(CountNeighborMemes(gridPos));
		}
	}
}

void MemeField::Tile::SpawnMeme()
{
	assert(!hasMeme);
	hasMeme = true;
}
void MemeField::Tile::SpawnLetterMeme()
{
	assert(!hasMeme);
	hasMeme = true;
	isLetter = true;
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
			
			gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[9], SpriteEffect::Copy{});
			break;
		case State::Flagged:
			gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[9], SpriteEffect::Copy{});
			SpriteCodex::DrawTileFlag(screenPos, gfx);  //todo: change to surface drawing
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				DrawTileNumber(screenPos, nNeighborMemes, gfx);
			}
			else
			{
				gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[12], SpriteEffect::Copy{});
			}
			break;
		}
	}
	else // we are done fucked
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				if (!isLetter)
				{
					gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[12], SpriteEffect::Copy{});
				}
				else
				{
					gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[13], SpriteEffect::Copy{});
				}

			}
			else
			{
				gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[9], SpriteEffect::Copy{});
			}
			break;
		case State::Flagged:
			if (HasMeme())
			{
				gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[12], SpriteEffect::Copy{});
				SpriteCodex::DrawTileFlag(screenPos, gfx);  //todo: change to surface drawing
			}
			else
			{
				gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[12], SpriteEffect::Copy{});
				SpriteCodex::DrawTileCross(screenPos, gfx);  //todo: change to surface drawing
			}
			break;
		case State::Revealed:
			if (!HasMeme())
			{
				DrawTileNumber(screenPos, nNeighborMemes, gfx);
			}
			else
			{
				gfx.DrawSprite(screenPos.x, screenPos.y, *TileSurfaces[13], SpriteEffect::Copy{});
			}
			break;
		}
	}
}

void MemeField::Tile::DrawTileNumber(const Vei2 & pos, int n, Graphics& gfx) const
{
	assert(n >= 0 && n <= 8);
	switch (n)
	{
	case 0:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[0], SpriteEffect::Copy{});
		break;
	case 1:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[1], SpriteEffect::Copy{});
		break;
	case 2:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[2], SpriteEffect::Copy{});
		break;
	case 3:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[3], SpriteEffect::Copy{});
		break;
	case 4:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[4], SpriteEffect::Copy{});
		break;
	case 5:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[5], SpriteEffect::Copy{});
		break;
	case 6:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[6], SpriteEffect::Copy{});
		break;
	case 7:
		gfx.DrawSprite(pos.x, pos.y, *TileSurfaces[7], SpriteEffect::Copy{});
		break;
	case 8:
		SpriteCodex::DrawTile8(pos,gfx);  //todo: change to surface drawing
		break;
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
	clue.Draw(gfx);

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
void MemeField::RevealTile(const Vei2& gridPos)
{
	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed() && !tile.IsFlagged())
	{
		tile.Reveal();
		if (tile.HasMeme())
		{
			state = State::Fucked;
			sndLose.Play(1, 0.3);
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
	//need assert here to check gridPos isn't past memefield bounds
	return pTileField[gridPos.y * width + gridPos.x];
}

const MemeField::Tile& MemeField::TileAt(const Vei2 & gridPos) const
{
	//need assert here to check gridPos isn't past memefield bounds
	return pTileField[gridPos.y * width + gridPos.x];
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
		const Tile& t = pTileField[i];
		if ((t.HasMeme() && !t.IsFlagged()) ||
			(!t.HasMeme() && !t.IsRevealed()))
		{
			return false;
		}
	}
	return true;
}
