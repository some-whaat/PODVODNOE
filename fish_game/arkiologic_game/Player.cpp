#include "Header.h"


void Player::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {
	
    for (AnimatbleObj item : inventory) {
        item.draw(buffer, screen);
    }

    AnimatbleObj::draw(buffer, screen);

    move();
}

void  Player::move() {

    if (++move_count >= 1/speed) {
        Position dir;

        if (GetAsyncKeyState(VK_LEFT))
        {
            dir.x += -1;

        }

        if (GetAsyncKeyState(VK_RIGHT))
        {
            dir.x += 1;

        }

        if (GetAsyncKeyState(VK_UP))
        {
            dir.y += 1;

        }

        if (GetAsyncKeyState(VK_DOWN))
        {
            dir.y += -1;

        }

        dir = dir.normalise();


        //sum(dir.mult(speed));
        sum(dir);

        if (!inventory.empty()) {
            inventory[0].move_to(this->get_pos(), (max(inventory[0].wighth, inventory[0].hight) + max(wighth, hight))/4);

            for (int i = 1; i < inventory.size(); i++) {
                inventory[i].move_to(inventory[i - 1].get_pos(), (max(inventory[i].wighth, inventory[i].hight) + max(inventory[i - 1].wighth, inventory[i - 1].hight))/4);
            }
        }

        move_count = 0;
    }

}

void Player::add_item_to_inventory(int item_id) {
	inventory.emplace_back(inventory_data_base[item_id], 0, x, y);
	inventory_ids.push_back(item_id);
}

bool Player::does_has_item(int item_id) {
    return item_id == -1 || std::find(inventory_ids.begin(), inventory_ids.end(), item_id) != inventory_ids.end();
}