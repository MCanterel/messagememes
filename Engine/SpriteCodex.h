#pragma once

#include "Graphics.h"
#include "Vei2.h"
#include "SpriteEffect.h"
#include "Codex.h"


class SpriteCodex
{
public:
	static constexpr int tileSize = 16;
	static constexpr Color baseColor = { 192,192,192 };
	//still need to switch these mofos over to the surface drawing functions in Tile
	static void DrawTile8( const Vei2& pos,Graphics& gfx );
	static void DrawTileCross( const Vei2& pos,Graphics& gfx );
	static void DrawTileFlag( const Vei2& pos,Graphics& gfx );
	
	static void DrawWin( const Vei2& pos,Graphics& gfx );
	// Text for size selection (center origin)
	static constexpr int sizeselWidth = 240;
	static constexpr int sizeselHeight = 56;
	static void DrawSmallMenuItem( const Vei2& pos,Graphics& gfx );
	static void DrawLargeMenuItem( const Vei2& pos,Graphics& gfx );
};