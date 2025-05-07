#include "Header.h"

bool is_working = true;


int main() {
    /*
    nlohmann::json bg_fish_json;
	bg_fish_json["name"] = "bg_fish";


    nlohmann::json world_json;
    output["name"] = "Bob";
    output["age"] = 25;
    output["skills"] = { "Java", "JavaScript" };
    output["address"] = {
        {"city", "New York"},
        {"zip", 10001}
    };

    // Write to a file
    std::ofstream file("output.json");
    if (!file.is_open()) {
        std::cerr << "Failed to create file!" << std::endl;
        return 1;
    }

    file << output.dump(4); // 4 spaces for indentation
*/


    using RenderLayer = std::vector<std::shared_ptr<RendrbleObject>>;
    
    World scr;

    std::shared_ptr<RenderLayer> bg;

    bg = std::make_shared<RenderLayer>();

    bg->push_back(std::make_shared<Picture>("beach.txt"));

//    std::vector<std::unique_ptr<RendrbleObject>> layer;
//    layer.push_back(std::make_unique<SinMovingObj>(66, 66, 0, 0, file, 1, 1));

    // Move the entire layer into addLayer
    scr.add_layer(bg, false);

    scr.process();
}
