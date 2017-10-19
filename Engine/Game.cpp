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
			else if ( fState == MemeField::State::Fucked || fState == MemeField::State::Winrar ) {
				if ( e.GetType ( ) == Mouse::Event::Type::LPress ) {  //needed this conditional to insert pause
					delete pField;
					state = State::SelectionMenu;
				}


			}
		}
		else
		{

			//const Mouse::Event e = wnd.mouse.Read ( );
			//while ( e.GetType ( ) != Mouse::Event::Type::LPress ) { }
			//trying to insert a delay here... SEE ABOVE

			const SelectionMenu::Size s = menu.ProcessMouse ( e );


			switch ( s )
			{
			case SelectionMenu::Size::Small:
				pField = new MemeField ( gfx.GetRect ( ).GetCenter ( ), 4, (int)Size::Small * MemeField::GetWidth ( ), (int)Size::Small * MemeField::GetHeight ( ) );
				state = State::Memesweeper;

				break;
			case SelectionMenu::Size::Medium:
				pField = new MemeField ( gfx.GetRect ( ).GetCenter ( ), 4, (int)Size::Medium * MemeField::GetWidth ( ), (int)Size::Medium * MemeField::GetHeight ( ) );
				state = State::Memesweeper;
				break;
			case SelectionMenu::Size::Large:
				pField = new MemeField ( gfx.GetRect ( ).GetCenter ( ), 4, (int)Size::Large * MemeField::GetWidth ( ), (int)Size::Large * MemeField::GetHeight ( ) );
				state = State::Memesweeper;
				break;
				//state = State::Memesweeper;
			}
		}
	}
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
