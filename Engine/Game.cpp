
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"
#include <assert.h>

Game::Game ( MainWindow& wnd )
	:
	wnd ( wnd ),
	gfx ( wnd ),
	menu ( { gfx.GetRect ( ).GetCenter ( ).x,800 } )
{
}

void Game::Go ( )
{
	gfx.BeginFrame ( );
	UpdateModel ( );
	ComposeFrame ( );
	gfx.EndFrame ( );
}

void Game::UpdateModel()
{
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();

		if (state == State::Memesweeper)
		{
			MemeField::State fState = pField->GetState();

			if (fState == MemeField::State::Memeing)
			{
				if (e.GetType() == Mouse::Event::Type::LPress)
				{
					const Vei2 mousePos = e.GetPos();
					if (pField->GetRect().Contains(mousePos))
					{
						pField->OnRevealClick(mousePos);
					}
				}
				else if (e.GetType() == Mouse::Event::Type::RPress)
				{
					const Vei2 mousePos = e.GetPos();
					if (pField->GetRect().Contains(mousePos))
					{
						pField->OnFlagClick(mousePos);
					}
				}
			}
			else {
				if (e.GetType() == Mouse::Event::Type::LPress) {
					state = State::SelectionMenu;
				}
			}
		}
		else
		{
			const SelectionMenu::Size s = menu.ProcessMouse(e);
			int baseMemes = MemeField::GetMemeBaseNum();
			int baseWidth = MemeField::GetWidth();
			int baseHeight = MemeField::GetHeight();

			switch (s)
			{
			case SelectionMenu::Size::Small:
				CreateField((int)Size::Small * baseMemes, (int)Size::Small * baseWidth, (int)Size::Small * baseHeight);
				state = State::Memesweeper;
				break;
			case SelectionMenu::Size::Large:
				CreateField((int)Size::Large * baseMemes, (int)Size::Large * baseWidth, (int)Size::Large * baseHeight);
				state = State::Memesweeper;
				break;
			}
		}
	}
}


void Game::CreateField ( int nMemes,int width, int height )
{
	pField = std::make_unique<MemeField>( gfx.GetRect ( ).GetCenter ( ), nMemes, width, height );
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
