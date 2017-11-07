/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#include <assert.h>

Game::Game ( MainWindow& wnd )
	:
	wnd ( wnd ),
	gfx ( wnd ),
	menu ( { gfx.GetRect ( ).GetCenter ( ).x,200 } )
	// ,	field ( gfx.GetRect ( ).GetCenter ( ), 4 )  //old static field
{
}


void Game::Go ( )
{
	gfx.BeginFrame ( );
	UpdateModel ( );
	ComposeFrame ( );
	gfx.EndFrame ( );
}
//```cpp
void Game::UpdateModel ( )
{
	while ( !wnd.mouse.IsEmpty ( ) )
	{
		const auto e = wnd.mouse.Read ( );

		if ( state == State::Memesweeper )
		{
			MemeField::State fState = pField->GetState ( );

			if ( fState == MemeField::State::Memeing )
			{
				if ( e.GetType ( ) == Mouse::Event::Type::LPress )
				{
					const Vei2 mousePos = e.GetPos ( );
					if ( pField->GetRect ( ).Contains ( mousePos ) )
					{
						pField->OnRevealClick ( mousePos );
					}
				}
				else if ( e.GetType ( ) == Mouse::Event::Type::RPress )
				{
					const Vei2 mousePos = e.GetPos ( );
					if ( pField->GetRect ( ).Contains ( mousePos ) )
					{
						pField->OnFlagClick ( mousePos );
					}
				}
			}
			else  {
				if ( e.GetType ( ) == Mouse::Event::Type::LPress ) {
					DestroyField ( );
					state = State::SelectionMenu;
				}
			}
		}
		else
		{
			const SelectionMenu::Size s = menu.ProcessMouse ( e );
			int baseMemes = MemeField::GetMemeBaseNum ( );
			int baseWidth = MemeField::GetWidth ( );
			int baseHeight = MemeField::GetHeight ( );
			
			switch ( s )
			{
			case SelectionMenu::Size::Small:
				CreateField ( (int)Size::Small * baseMemes, (int)Size::Small * baseWidth, (int)Size::Small * baseHeight );
				state = State::Memesweeper;
				break;

			case SelectionMenu::Size::Medium:
				CreateField ( (int)Size::Medium * baseMemes, (int)Size::Medium * baseWidth, (int)Size::Medium * baseHeight );
				state = State::Memesweeper;
				break;

			case SelectionMenu::Size::Large:
				CreateField ( (int)Size::Large * baseMemes, (int)Size::Large * baseWidth, (int)Size::Large * baseHeight );
				state = State::Memesweeper;
				break;	
			}
			//state = State::Memesweeper;  //Doing this here instead of inside the switch cases
			//creates a read access violation, saying pField is a nullptr! Why? Because on the first loop through, none of the cases
			//are met, so CreateField doesn't get called, and yet the state has been changed to Memesweeper. Oops!
		}
	}
}//```

void Game::CreateField ( int nMemes,int width, int height )
{
	assert ( pField == nullptr );
	pField = new MemeField ( gfx.GetRect ( ).GetCenter ( ), nMemes, width, height );
	//state = State::Memesweeper;  //Doing it here is fine. Why???
}

void Game::DestroyField ( )
{
	pField->FreeResources ( );
	delete pField;
	pField = nullptr;
}

void Game::ComposeFrame ( )
{
	if ( state == State::Memesweeper )
	{
		pField->Draw ( gfx );
		if ( pField->GetState ( ) == MemeField::State::Winrar )
		{
			SpriteCodex::DrawWin ( gfx.GetRect ( ).GetCenter ( ), gfx );
		}
	}
	else
	{
		menu.Draw ( gfx );
	}
}
