#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <iostream>
#include "../include/database.h"

int main(){
    Storage excudes_storage = InitStorage();
    excudes_storage.sync_schema();

    ExcuseComponent component;
    component.component_type = "reason";
    component.text = "The dog ate my homework.";
    excudes_storage.insert(component);

    std::cout << "Database 'excuses.db' created/updated and data inserted." << std::endl;
    return 0;
}

