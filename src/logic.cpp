#include "../include/logic.h"
#include <regex>  


    #include <iostream>

std::string ProcessGender(std::string text, Gender gender) {
    // std::cerr << "BEFORE:" << text << '\n';
    if (gender == Gender::MALE) {
        // std::cerr << "IN_MALE" << '\n';
        text = std::regex_replace(text, std::regex("\\(а\\)"), "");
        text = std::regex_replace(text, std::regex("\\(ся/ась\\)"), "ся");
        text = std::regex_replace(text, std::regex("\\(ел/ла\\)"), "ел");
        text = std::regex_replace(text, std::regex("\\(ал/ала\\)"), "ал");
        text = std::regex_replace(text, std::regex("\\(ен/на\\)"), "ен");
        text = std::regex_replace(text, std::regex("\\(ой/ая\\)"), "ой");
        text = std::regex_replace(text, std::regex("\\(ая\\)"), "ый");
        text = std::regex_replace(text, std::regex("\\(ый/ая\\)"), "ый");
         // Добавить другие правила по необходимости
    } else { // FEMALE
        // std::cerr << "IN_FEMALE" << '\n';
        text = std::regex_replace(text, std::regex("\\(а\\)"), "а");
        text = std::regex_replace(text, std::regex("\\(ся/ась\\)"), "ась");
        text = std::regex_replace(text, std::regex("\\(ел/ла\\)"), "ла");
        text = std::regex_replace(text, std::regex("\\(ал/ала\\)"), "ала");
        text = std::regex_replace(text, std::regex("\\(ен/на\\)"), "на");
        text = std::regex_replace(text, std::regex("\\(ой/ая\\)"), "ая");
        text = std::regex_replace(text, std::regex("\\(ая\\)"), "ая");
        text = std::regex_replace(text, std::regex("\\(ый/ая\\)"), "ая");
         // Добавить другие правила по необходимости
    }
    text = std::regex_replace(text, std::regex("\\([^\\)]*\\)"), ""); // Если пол не определился
    // std::cerr << "AFTER:" << text << '\n';
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


std::string GenerateExcuse(
    Storage& storage,
    ExcuseType excuseType,
    Gender gender)
{
    std::string opening_key, consequence_key;

    switch (excuseType) {
        case ExcuseType::LATE:
            opening_key = "opening_late";
            consequence_key = "consequence_late";
            break;
        case ExcuseType::CANT_HELP:
            opening_key = "opening_canthelp";
            consequence_key = "consequence_canthelp";
            break;
        case ExcuseType::NO_MONEY:
            opening_key = "opening_nomoney";
            consequence_key = "consequence_nomoney";
            break;
        case ExcuseType::NOT_COMING:
            opening_key = "opening_notcoming";
            consequence_key = "consequence_notcoming";
            break;
        default:
            throw std::runtime_error("Unknown excuse type");
    }

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

    result = std::regex_replace(result, std::regex(" +"), " "); // Убрать двойные пробелы
    result = std::regex_replace(result, std::regex(" ,"), ","); // Убрать пробел перед запятой
    result = std::regex_replace(result, std::regex(" \\."), "."); // Убрать пробел перед точкой

    return result;
}