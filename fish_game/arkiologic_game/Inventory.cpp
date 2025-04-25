#include "Header.h"

/*


void Inventory_sell::add_invent_items() {
	_pictures.clear();
	_circles.clear();
	_text.clear();
	_frames.clear();


	add_grid_els(player->loot_inventory);

	add_srats();
}


void Inventory_sell::sell() {
	player->money += player->loot_inventory[selected_el].cost;
	player->loot_to_the_musium(player->loot_inventory[selected_el]);
	player->loot_inventory.erase(player->loot_inventory.begin() + selected_el);
	
	something_changed = true;

	if (player->musium_items.size() >= 9) {
		congrats_you_won();
	}
}

void Inventory_sell::select_actions() {
	if (selected_el < player->loot_inventory.size()) {
		show_text_and_pic("this is " + player->loot_inventory[selected_el].name + ", it costs " + std::to_string(player->loot_inventory[selected_el].cost), _pictures[selected_el]);

		while (!GetAsyncKeyState(VK_SPACE)) {}


		if (yes_no_choice("do you whant to sell " + player->loot_inventory[selected_el].name + " to the musium?")) {
			sell();
		}

		add_invent_items();
		something_changed = true;

	}
}

void Inventory_sell::congrats_you_won() {
	text_seq_render({ text_squere("congrandilations, you've found and sold to the musium all the loot I've added to the game, so that means that you won!!!!!!!!!!!!!!!!!!!!", 55), text_squere("congrandilations!!!!!!!!!!!!!", 0), text_squere("thank y so so much for playing", 0), text_squere("if you want, you can continue playing", 0) });
}



void Inventory_return::add_invent_items() {
	_pictures.clear();
	_circles.clear();
	_text.clear();
	_frames.clear();


	add_grid_els(player->eqip_inventory);

	add_srats();
}


void Inventory_return::select_actions() {
	show_text_and_pic("this is " + player->eqip_inventory[selected_el].name, player->eqip_inventory[selected_el]);

	while (!GetAsyncKeyState(VK_SPACE)) {}


	if (yes_no_choice("do you whant to use " + player->eqip_inventory[selected_el].name + " for this mission?")) {

		to_return = player->eqip_inventory[selected_el].for_what_mission;
		if (player->eqip_inventory[selected_el].name != "your hand") {
			(player->eqip_inventory).erase((player->eqip_inventory).begin() + selected_el);
		}

		is_quit = true;
	}
	add_invent_items();
	something_changed = true;
}

int Inventory_return::get_to_return() {
	return to_return;
}


void Inventory_tools::add_invent_items() {
	_pictures.clear();
	_circles.clear();
	_text.clear();
	_frames.clear();


	add_grid_els(player->eqip_inventory);

	add_srats();
}

void Inventory_tools::select_actions() {
	if (selected_el < player->eqip_inventory.size()) {
		show_text_and_pic("this is " + player->eqip_inventory[selected_el].name, _pictures[selected_el]);

		add_invent_items();
		for (int i = 0; i < _pictures.size(); i++) {
			_frames[i].is_big = i == selected_el;
		}
		something_changed = true;
		render();
	}
}*/