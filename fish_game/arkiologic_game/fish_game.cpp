#include "Header.h"

bool is_working = true;


int main() {

    /*
    std::ifstream file("NPC_data.json");
    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

    try {
        nlohmann::json data = nlohmann::json::parse(file);


        int quest_id = data["id"];
        std::cout << "Quest ID: " << quest_id << "\n\n";


        nlohmann::json& states = data["states"];

        for (auto& state : states.items()) {
            const std::string& state_key = state.key();
            const nlohmann::json& state_data = state.value();

            std::cout << "State: " << state_key << "\n";
            std::cout << "Dialogue: " << state_data["dialogue"] << "\n";
            std::cout << "Needed Item ID: " << state_data["needed_item_id"] << "\n";
            std::cout << "Next State: " << state_data["next_state"] << "\n";
            std::cout << "Reward Item ID: " << state_data["reward_item_id"] << "\n\n";
        }

    }
    catch (nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        return 1;
    }*/




    using RenderLayer = std::vector<std::shared_ptr<RendrbleObject>>;
    
    World scr;

    std::shared_ptr<RenderLayer> bg;
    std::shared_ptr<RenderLayer> npc;

    bg = std::make_shared<RenderLayer>();
    npc = std::make_shared<RenderLayer>();

 
    bg->push_back(std::make_shared<Picture>("beach.txt", 0, 0, 0.5));
    bg->push_back(std::make_shared<Picture>("seaweed.txt", 0, -10, -0.5));
    npc->push_back(std::make_shared<NPC>("NPC_draft.json"));

//    std::vector<std::unique_ptr<RendrbleObject>> layer;
//    layer.push_back(std::make_unique<SinMovingObj>(66, 66, 0, 0, file, 1, 1));

    // Move the entire layer into addLayer
    scr.add_layer(bg, false);
    scr.add_layer(npc, true, "npcs");

    scr.process();
}
