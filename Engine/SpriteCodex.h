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

	// I'm trying to implement surfaces here to draw the tiles using static functions:
	//static Surface* pTileBombSurface = new Surface(L"Images/tile_bomb.bmp");
	// or:
	//static Surface* pTileBombSurface = (Codex<Surface>::Retrieve(L"Images/tile_bomb.bmp"));
	// then in cpp file DrawTileBomb() would be something like:
	//---->  gfx.DrawSprite(pos.x, pos.y, pTileBombSurface, SpriteEffect::Copy{});
	// but it doesn't work as a static data member. why?
	
	static void DrawTile0( const Vei2& pos,Graphics& gfx );
	static void DrawTile1( const Vei2& pos,Graphics& gfx );
	static void DrawTile2( const Vei2& pos,Graphics& gfx );
	static void DrawTile3( const Vei2& pos,Graphics& gfx );
	static void DrawTile4( const Vei2& pos,Graphics& gfx );
	static void DrawTile5( const Vei2& pos,Graphics& gfx );
	static void DrawTile6( const Vei2& pos,Graphics& gfx );
	static void DrawTile7( const Vei2& pos,Graphics& gfx );
	static void DrawTile8( const Vei2& pos,Graphics& gfx );
	static void DrawTileButton( const Vei2& pos,Graphics& gfx );
	static void DrawTileCross( const Vei2& pos,Graphics& gfx );
	static void DrawTileFlag( const Vei2& pos,Graphics& gfx );
	static void DrawTileBomb( const Vei2& pos,Graphics& gfx );
	static void DrawTileBombRed( const Vei2& pos,Graphics& gfx );
	// Tile selector function valid input 0-8
	static void DrawTileNumber( const Vei2& pos,int n,Graphics& gfx );
	// Win Screen 254x192 center origin
	static void DrawWin( const Vei2& pos,Graphics& gfx );
	// Text for size selection (center origin)
	static constexpr int sizeselWidth = 240;
	static constexpr int sizeselHeight = 56;
	static void DrawSmallMenuItem( const Vei2& pos,Graphics& gfx );
	static void DrawLargeMenuItem( const Vei2& pos,Graphics& gfx );
};