#include "Header.h"


/*
std::ifstream sword("sword.txt");
std::ifstream kirk("kirk.txt");

std::vector<loot> _loot = {loot(sword, "sword", 0, 0), loot(kirk, "kirk", 0, 0)};*/

int rand_int(int down_bord, int up_bord) {
    if (up_bord > down_bord && up_bord > 0) {

        std::random_device os_seed;
        const uint_least32_t seed = os_seed();

        std::mt19937 generator(seed);
        std::uniform_int_distribution< uint_least32_t > distribute(0, up_bord + std::abs(down_bord));

        return down_bord + distribute(generator);
    }
    return (down_bord + up_bord)/2;
}

/*
void download_loot() {
    std::ifstream sword_file;
    sword_file.open("sword_grathics.txt");

    loot.push_back(picture(sword_file, 0, 0));
}*/

bool was_button_pressed();

/*
void Player::loot_to_the_musium(loot add_loot) {
    if (!std::any_of(musium_items.begin(), musium_items.end(), [&](loot mus_loot) {return add_loot.name == mus_loot.name; })) {
        musium_items.push_back(add_loot);
    }
}*/