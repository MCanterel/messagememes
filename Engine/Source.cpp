```cpp void Game::UpdateModel ( )
{
	//....
	//do stuff associated with state == State::Memesweeper here
	//then...

	else  //state = State::SelectionMenu;
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
		//creates a read access violation, saying pField is a nullptr! Why?
	}
}
}```