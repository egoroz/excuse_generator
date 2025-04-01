#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>

struct ExcuseComponent {
    int id;
    std::string component_type;
    std::string text;
};

namespace sq = sqlite_orm;
using Storage = decltype(sq::make_storage(""));

Storage InitStorage(const std::string& path_to_db = "excudes.db");
Storage StoreStorage(const std::string& path_to_db = "excudes.db");