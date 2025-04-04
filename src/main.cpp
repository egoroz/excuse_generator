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
        if (origin == "http://localhost:9001") {  

            std::string gender_req = req.url_params.get("gender");
            std::string excuse_type_req = req.url_params.get("excuse_type");

            Gender gender = RecognizeGender(gender_req);
            ExcuseType excuse_type = RecognizeExcuseType(excuse_type_req);


            std::string excuse = GenerateExcuse(storage, excuse_type, gender);
            nlohmann::json response_json;
            response_json["excuse"] = excuse;
            crow::response response(response_json.dump());

            response.add_header("Content-Type", "application/json; charset=utf-8");
            response.add_header("Access-Control-Allow-Origin", origin);

            return response;
        } else {
            return crow::response(403);
            }
    });

    CROW_ROUTE(app, "/compose")
    ([&storage](const crow::request& req){

        std::string origin = req.get_header_value("Origin");
        if(origin == "http://localhost:9001"){

            const char* gender_req = req.url_params.get("gender");
            const char* type_param = req.url_params.get("excuse_type");

            std::string gender_str = gender_req;
            std::string excuse_type_str = type_param;
            std::string opening_key = "opening";
            std::string consequence_key = "consequence";

            Gender gender = RecognizeGender(gender_str);
            ExcuseType excuse_type = RecognizeExcuseType(excuse_type_str);

            try{
                SetKey(excuse_type, opening_key, consequence_key);
            } catch(const std::runtime_error& e){
                nlohmann::json error_json;
                error_json["error"] = "Unknown excuse type provided";
                error_json["provided_type"] = excuse_type_str;
                error_json["available_types"] = {"late", "canthelp", "nomoney", "notcoming"};
                return crow::response(400, error_json.dump());
            } catch(...){
                nlohmann::json error_json;
                error_json["error"] = "Unknown error on server";
                return crow::response(500, error_json.dump());
            }


            const std::string cause_key = "cause";
            const std::string incident_key = "incident";
            const std::string action_key = "action";
            const std::string object_key = "object";
            const std::string place_key = "place";

            const char* opening_param = req.url_params.get("opening");
            const char* cause_param = req.url_params.get("cause");
            const char* incident_param = req.url_params.get("incident");
            const char* action_param = req.url_params.get("action");
            const char* object_param = req.url_params.get("object");
            const char* place_param = req.url_params.get("place");
            const char* consequence_param = req.url_params.get("consequence");

            std::string opening_str;
            std::string cause_str;
            std::string incident_str;
            std::string action_str;
            std::string object_str;
            std::string place_str;
            std::string consequence_str;

            try {
                if (opening_param) opening_str = opening_param;
                else opening_str = GetRandomComponent(storage, opening_key);

                if (cause_param) cause_str = cause_param;
                else cause_str = GetRandomComponent(storage, cause_key);

                if (incident_param) incident_str = incident_param;
                else incident_str = GetRandomComponent(storage, incident_key);

                if (action_param) action_str = action_param;
                else action_str = GetRandomComponent(storage, action_key);

                if (object_param) object_str = object_param;
                else object_str = GetRandomComponent(storage, object_key);

                if (place_param) place_str = place_param;
                else place_str = GetRandomComponent(storage, place_key);

                if (consequence_param) consequence_str = consequence_param;
                else consequence_str = GetRandomComponent(storage, consequence_key);


                std::string final_excuse = opening_str + " я " + action_str + " " + object_str + " " + place_str + ", и внезапно " + incident_str + ", " + cause_str + " " + consequence_str;
                final_excuse = ProcessGender(final_excuse, gender);

                nlohmann::json response_json;
                response_json["excuse"] = final_excuse;
                response_json["components"] = {
                    {"type", excuse_type_str},
                    {"opening", opening_str},
                    {"cause", cause_str},
                    {"incident", incident_str},
                    {"action", action_str},
                    {"object", object_str},
                    {"place", place_str},
                    {"consequence", consequence_str}
                };


                crow::response response(response_json.dump());

                response.add_header("Access-Control-Allow-Origin", origin);
                return response;

            } catch (const std::runtime_error& e) {
                nlohmann::json error_json;
                error_json["error"] = "Failed to generate component for the excuse";
                error_json["details"] = e.what();
                return crow::response(500, error_json.dump());
            } catch (const std::exception& e) {
                nlohmann::json error_json;
                error_json["error"] = "Internal server error during excuse composition";
                error_json["details"] = e.what();
                return crow::response(500, error_json.dump());
            }

        } else {
            return crow::response(403); 
        }
    });
    

    app.port(9000)
       .multithreaded()
       .run();
       
    return 0;
}

