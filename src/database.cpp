#include "../include/database.h"

Storage InitStorage(const std::string& path_to_db = "excudes.db"){
    return sq::make_storage(path_to_db, 
                sq::make_table("excuse_components",
                    sq::make_column("id", &ExcuseComponent::id),
                    sq::make_column("component_type", &ExcuseComponent::component_type);
                    sq::make_column("text", &ExcuseComponent::text);
        )
    );
}

Storage StoreStorage(const std::string& path_to_db = "excudes.db"){

}