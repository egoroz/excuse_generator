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
    ([&storage](const crow::request& req) {

        std::string origin = req.get_header_value("Origin");
        if (origin == "http://localhost:8000") {  // Access only through localhost:8000
            std::string excuse = GenerateExcuse(storage, ExcuseType::CANT_HELP, Gender::FEMALE);
            nlohmann::json response_json;
            response_json["excuse"] = excuse;
            crow::response response(response_json.dump());

            response.add_header("Content-Type", "application/json; charset=utf-8");
            response.add_header("Access-Control-Allow-Origin", origin);

            return response;
        } else {
            return crow::response(403); // Forbidden
        }
    });

    app.port(1234)
       .multithreaded()
       .run();
       
    return 0;
}

