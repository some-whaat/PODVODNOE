#include "Header.h"

/*

void Menu::action() {
	
	add_srats();
	render();
	something_changed = true;
	
	Sleep(333);

	while (!GetAsyncKeyState(VK_SPACE))
	{
		if (GetAsyncKeyState(VK_UP)) {

			Sleep(66);

			selected_el -= 1;

			if (selected_el < 0) {
				selected_el = select_options.size() - 1;
			}

			for (int i = 0; i < select_options.size(); i++) {
				_text[i].is_big = i == selected_el;
			}
			something_changed = true;
		}

		if (GetAsyncKeyState(VK_DOWN)) {

			Sleep(66);

			selected_el += 1;

			if (selected_el >= select_options.size()) {
				selected_el = 0;
			}

			for (int i = 0; i < select_options.size(); i++) {
				_text[i].is_big = i == selected_el;
			}
			something_changed = true;
		}

		render();
	}

	while (GetAsyncKeyState(VK_SPACE)) {}

	switch (selected_el)
	{
	case 0:
		fishing_game(player).action();
		//go_to_rungom_mission(player);
		break;

	case 1:
		Store(player).action();
		_text.erase(_text.end() - 1, _text.end());
		action();
		break;

	case 2:
		buy_food(3);
		_text.erase(_text.end() - 1, _text.end());
		action();
		break;

	case 3:
		Inventory_sell(player).action();
		_text.erase(_text.end() - 1, _text.end());
		action();
		break;

	case 4:
		Inventory_tools(player).action();
		_text.erase(_text.end() - 1, _text.end());
		action();
		break;


	case 5:
		Musium(player).action();
		_text.erase(_text.end() - 1, _text.end());
		action();
		break;

	case 6:
		is_working = false;
		break;

	default:
		break;
	}
}

void Menu::buy_food(int how_much) {
	if (player->money >= how_much) {

		player->food++;
		player->money -= how_much;
	}
	else {
		text_seq_render({ TextSquere("sorry, you don't have enoth money", 0) });
	}

	something_changed = true;
}

void Menu::select_actions() {}*/