
#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "MemeField.h"
#include "SelectionMenu.h"
#include <memory>

class Game
{
private:
	enum class State
	{
		SelectionMenu,
		Memesweeper
	};
	enum class Size
	{
		Small = 3,
		Large = 4
	};
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
	void CreateField ( int nMemes, int width, int height );
	private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	std::unique_ptr<MemeField> pField;
	SelectionMenu menu;
	State state = State::SelectionMenu;
	/********************************/
};