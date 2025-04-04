#include "../include/logic.h"
#include <regex>  
    

std::string ProcessGender(std::string text, Gender gender) {
    if (gender == Gender::MALE) {
        text = std::regex_replace(text, std::regex("\\(а\\)"), "");
        text = std::regex_replace(text, std::regex("\\(ся/ась\\)"), "ся");
        text = std::regex_replace(text, std::regex("\\(ел/ла\\)"), "ел");
        text = std::regex_replace(text, std::regex("\\(ал/ала\\)"), "ал");
        text = std::regex_replace(text, std::regex("\\(ен/на\\)"), "ен");
        text = std::regex_replace(text, std::regex("\\(ой/ая\\)"), "ой");
        text = std::regex_replace(text, std::regex("\\(ая\\)"), "ый");
        text = std::regex_replace(text, std::regex("\\(ый/ая\\)"), "ый");
    } else { // FEMALE
        text = std::regex_replace(text, std::regex("\\(а\\)"), "а");
        text = std::regex_replace(text, std::regex("\\(ся/ась\\)"), "ась");
        text = std::regex_replace(text, std::regex("\\(ел/ла\\)"), "ла");
        text = std::regex_replace(text, std::regex("\\(ал/ала\\)"), "ала");
        text = std::regex_replace(text, std::regex("\\(ен/на\\)"), "на");
        text = std::regex_replace(text, std::regex("\\(ой/ая\\)"), "ая");
        text = std::regex_replace(text, std::regex("\\(ая\\)"), "ая");
        text = std::regex_replace(text, std::regex("\\(ый/ая\\)"), "ая");
    }
    text = std::regex_replace(text, std::regex("\\([^\\)]*\\)"), ""); // Если пол не определился
    return text;
}


std::string GetRandomComponent(Storage& storage, const std::string& type){
    auto components = storage.get_all<ExcuseComponent>(sq::where(sq::c(&ExcuseComponent::type)==type));

    if(components.empty()){return "???";}

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, components.size() - 1);
    int index = distrib(gen);

    return components[index].text;
}



template<typename T>
void SetKey(const ExcuseType& excuse_type, T& object){
    switch (excuse_type) {
        case ExcuseType::LATE:
            object+="_late";
            break;
        case ExcuseType::CANT_HELP:
            object+="_canthelp";
            break;
        case ExcuseType::NO_MONEY:
            object+="_nomoney";
            break;
        case ExcuseType::NOT_COMING:
            object+="_notcoming";
            break;
        default:
            throw std::runtime_error("Unknown excuse type");
    }
}


template<typename T, typename... Args>
void SetKey(const ExcuseType& excuse_type, T& first, Args&... remaining){
    SetKey(excuse_type, first);
    SetKey(excuse_type, remaining...);
}

ExcuseType RecognizeExcuseType(const std::string& excuse_type_text){
    if (excuse_type_text == "LATE"){
        return ExcuseType::LATE;
    } else if (excuse_type_text == "CANT_HELP") {
        return ExcuseType::CANT_HELP;
    } else if (excuse_type_text == "NO_MONEY") {
        return ExcuseType::NO_MONEY;
    } else if (excuse_type_text == "NOT_COMING") {
        return ExcuseType::NOT_COMING;
    } else{
        throw std::runtime_error("Wrong Excuse type");
    }
}

std::string GenerateExcuse(
    Storage& storage,
    ExcuseType excuse_type,
    Gender gender)
{
    std::string opening_key="opening", consequence_key="consequence";

    SetKey(excuse_type, opening_key, consequence_key);

    std::string opening = GetRandomComponent(storage, opening_key);
    std::string consequence = GetRandomComponent(storage, consequence_key);
    std::string incident = GetRandomComponent(storage, "incident");
    std::string cause = GetRandomComponent(storage, "cause");
    std::string action = GetRandomComponent(storage, "action");
    std::string object = GetRandomComponent(storage, "object");
    std::string place = GetRandomComponent(storage, "place");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 2); // 2 шаблона
    int template_choice = distrib(gen);

    std::string result = "";

    if (template_choice == 1) {
        // Шаблон: Вступление + Инцидент + потому что + Действие + Объект + Место + Последствие
        result = opening + " " + incident + ", " + cause + " я " + action + " " + object + " " + place + ", " + consequence;
    } else { // template_choice == 2
        // Шаблон: Вступление + Действие + Объект + Место + и внезапно + Инцидент + из-за чего + Последствие
        result = opening + " я " + action + " " + object + " " + place + ", и внезапно " + incident + ", " + cause + " " + consequence;
    }
    result = ProcessGender(result, gender);

    result = std::regex_replace(result, std::regex(" +"), " ");
    result = std::regex_replace(result, std::regex(" ,"), ",");
    result = std::regex_replace(result, std::regex(" \\."), ".");

    return result;
}