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

            std::string gender_req = req.url_params.get("gender");
            std::string excuse_type_req = req.url_params.get("excuse_type");


            Gender gender = Gender::MALE;
            if (gender_req == "FEMALE") {
                gender = Gender::FEMALE;
            } // else MALE


            ExcuseType excuse_type = ExcuseType::LATE;
            if (excuse_type_req == "CANT_HELP") {
                excuse_type = ExcuseType::CANT_HELP;
            } else if (excuse_type_req == "NO_MONEY") {
                excuse_type = ExcuseType::NO_MONEY;
            } else if (excuse_type_req == "NOT_COMING") {
                excuse_type = ExcuseType::NOT_COMING;
            } // else LATE


            std::string excuse = GenerateExcuse(storage, excuse_type, gender);
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

