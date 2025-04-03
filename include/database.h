#pragma once
#include <sqlite_orm/sqlite_orm.h>
#include <string>

struct ExcuseComponent {
    int id;
    std::string type;
    std::string text;
};

namespace sq = sqlite_orm;
using Storage = decltype(   sq::make_storage("", 
                                sq::make_table("",
                                    sq::make_column("", &ExcuseComponent::id, sq::primary_key().autoincrement()),
                                    sq::make_column("", &ExcuseComponent::type),
                                    sq::make_column("", &ExcuseComponent::text)
                                )
                            )
                        );

Storage InitStorage(const std::string& path_to_db = "excuses.db");