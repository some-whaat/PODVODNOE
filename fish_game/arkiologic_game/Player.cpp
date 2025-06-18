#include "Header.h"


void Player::draw(std::vector<CHAR_INFO>& buffer, Screen& screen) {

    for (auto& couple : inventory) {
        couple.second->draw(buffer, screen);
    }

    AnimatbleObj::draw(buffer, screen);

    move(screen.deltaTime, screen);
}

void Player::move(float deltaTime, Screen& screen) {

    //move_count += deltaTime * speed;

    if (true) {//(move_count >= 1) {
        Position dir;


        if (GetAsyncKeyState(VK_LEFT)) dir.x -= 1;
        if (GetAsyncKeyState(VK_RIGHT)) dir.x += 1;
        if (GetAsyncKeyState(VK_UP)) dir.y += 1;
        if (GetAsyncKeyState(VK_DOWN)) dir.y -= 1;

        if (dir.x != 0 || dir.y != 0) {
            dir = dir.normalise();
        }

        dir = dir.mult(speed);//* deltaTime);
        Position new_pos = this->sum(dir);

        if (screen.is_inside_borders(new_pos)) {
            set_pos(new_pos);
        }


        if (!inventory_vec.empty()) {
            // Update first item's position
            inventory_vec[0]->move_to(
                this->get_pos(),
                ((inventory_vec[0]->wighth/2, inventory_vec[0]->hight) + (wighth/2, hight)) / 2
            );

            // Update positions of subsequent items
            for (size_t i = 1; i < inventory_vec.size(); i++) {
                inventory_vec[i]->move_to(
                    inventory_vec[i - 1]->get_pos(),
                    (max(inventory_vec[i]->wighth/2, inventory_vec[i]->hight) +
                        (inventory_vec[i - 1]->wighth/2, inventory_vec[i - 1]->hight)) / 2
                );
            }
        }
        move_count = 0;
    }

}

void Player::add_item_to_inventory(int item_id) {
	inventory[item_id] = std::make_unique<AnimatbleObj>(inventory_data_base[item_id]);
	inventory_vec.push_back(inventory[item_id]);
}

void Player::remove_item_from_inventory(int item_id) {
    auto map_it = inventory.find(item_id);
    if (map_it != inventory.end()) {
        // Get the shared_ptr from the map
        auto& item_ptr = map_it->second;

        // Remove from vector
        auto vec_it = std::find(inventory_vec.begin(), inventory_vec.end(), item_ptr);
        if (vec_it != inventory_vec.end()) {
            inventory_vec.erase(vec_it);
        }

        // Remove from map
        inventory.erase(map_it);
    }
}

bool Player::does_has_item(int item_id) {
    
    return (inventory.find(item_id) != inventory.end());
}