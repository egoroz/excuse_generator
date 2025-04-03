#include "../include/logic.h"

std::string GetRandomComponent(Storage& storage, const std::string& type){
    auto components = storage.get_all<ExcuseComponent>(sq::where(sq::c(&ExcuseComponent::type)==type));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, components.size() - 1);
    int index = distrib(gen);

    return components[index].text;
}