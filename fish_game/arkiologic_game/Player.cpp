#include "Header.h"


void Player::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

    for (auto& couple : inventory) {
        couple.second->draw(buffer, screen);
    }

    AnimatbleObj::draw(buffer, screen);

    move(screen.deltaTime);
}

void  Player::move(float deltaTime) {

    if (true){//++move_count >= 1/speed) {
        Position dir;


        if (GetAsyncKeyState(VK_LEFT)) dir.x -= 1;
        if (GetAsyncKeyState(VK_RIGHT)) dir.x += 1;
        if (GetAsyncKeyState(VK_UP)) dir.y += 1;
        if (GetAsyncKeyState(VK_DOWN)) dir.y -= 1;

        if (dir.x != 0 || dir.y != 0) {
            dir = dir.normalise();
        }

        dir = dir.mult(speed * deltaTime);
        sum(dir);


        if (!inventory_vec.empty()) {
            if (inventory_vec[0] == nullptr) { // ÊÎÑÒÛÛÛÛÛÛËÜ
                inventory_vec.erase(inventory_vec.begin());
            }
        }

        if (!inventory_vec.empty()) {
            inventory_vec[0]->move_to(this->get_pos(), (max(inventory_vec[0]->wighth, inventory_vec[0]->hight) + max(wighth, hight))/4);

            for (int i = 1; i < inventory_vec.size(); i++) {

                if (inventory_vec[i] == nullptr) { // ÊÎÑÒÛÛÛÛÛÛËÜ
                    inventory_vec.erase(inventory_vec.begin() + i);
                }
                else {
                    inventory_vec[i]->move_to(
                        inventory_vec[i - 1]->get_pos(),
                        (max(inventory_vec[i]->wighth, inventory_vec[i]->hight)
                            + max(inventory_vec[i - 1]->wighth, inventory_vec[i - 1]->hight)) / 4);
                }
            }
        }

        move_count = 0;
    }

}

void Player::add_item_to_inventory(int item_id) {
	inventory[item_id] = std::make_unique<AnimatbleObj>(inventory_data_base[item_id], 0, x, y);
	inventory_vec.push_back(inventory[item_id]);
}

void Player::remove_item_from_inventory(int item_id) {
	inventory.erase(item_id);


}

bool Player::does_has_item(int item_id) {
    
    return item_id == -1 || (inventory.find(item_id) != inventory.end());
}

void Player::add_mission(int mission_id) {
    mission_vec.push_back(mission_id);
}