#pragma once
#include "database.h"
#include <random>

enum class Gender { MALE, FEMALE };


enum class ExcuseType { LATE, CANT_HELP, NO_MONEY, NOT_COMING };


std::string ProcessGender(std::string, Gender);

std::string GetRandomComponent(Storage&, const std::string&);

std::string GenerateExcuse(Storage&, ExcuseType, Gender);