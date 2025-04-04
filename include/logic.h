#pragma once
#include "database.h"
#include <random>

enum class Gender { MALE, FEMALE };


enum class ExcuseType { LATE, CANT_HELP, NO_MONEY, NOT_COMING };

template<typename T>
void SetKey(const ExcuseType&, T&);

template<typename T, typename... Args>
void SetKey(const ExcuseType&, T& first, Args&...);

ExcuseType RecognizeExcuseType(const std::string&);
Gender RecognizeGender(const std::string&);

std::string ProcessGender(std::string, Gender);

std::string GetRandomComponent(Storage&, const std::string&);

std::string GenerateExcuse(Storage&, ExcuseType, Gender);