#include <crow.h>
#include <nlohmann/json.hpp>
#include <sqlite_orm/sqlite_orm.h>

#include "../include/database.h"
#include "../include/logic.h"


int main(){
    crow::SimpleApp app;

    auto storage = InitStorage("../data/excuses.db");
    storage.sync_schema();

    CROW_ROUTE(app, "/generate")
    ([&storage]() {
        std::string excuse = GenerateExcuse(storage, ExcuseType::CANT_HELP, Gender::FEMALE);
        nlohmann::json response_json;
        response_json["excuse"] = excuse;
        return crow::response(response_json.dump());
    });

    app.port(1234)
       .multithreaded()
       .run();
       
    return 0;
}

