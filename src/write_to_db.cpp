#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <iostream>

#include "../include/database.h"
#include "../include/logic.h"

#include <fstream>
    void WriteToDB(Storage& st, std::string&& fn){
        
        std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
        std::ifstream inputFile(fn); 
        inputFile.imbue(utf8_locale);

        std::string currentLine;
        while (std::getline(inputFile, currentLine)) {
            if (!currentLine.empty()) {
            ExcuseComponent component;
            component.type = fn.substr(10);
            component.text = currentLine;
            component.id = st.insert(component);
        }}
        inputFile.close(); 
    };


int main(){
    auto storage = InitStorage("../data/excuses.db");
    storage.sync_schema();


    WriteToDB(storage,"../drafts/action");
    WriteToDB(storage,"../drafts/cause");
    WriteToDB(storage,"../drafts/consequence_canthelp");
    WriteToDB(storage,"../drafts/consequence_late");
    WriteToDB(storage,"../drafts/consequence_nomoney");
    WriteToDB(storage,"../drafts/consequence_notcoming");
    WriteToDB(storage,"../drafts/incident");
    WriteToDB(storage,"../drafts/object");
    WriteToDB(storage,"../drafts/opening_canthelp");
    WriteToDB(storage,"../drafts/opening_late");
    WriteToDB(storage,"../drafts/opening_nomoney");
    WriteToDB(storage,"../drafts/opening_notcoming");
    WriteToDB(storage,"../drafts/place");


    std::string t = GenerateExcuse(storage, ExcuseType::LATE, Gender::MALE);
    std::cout << t << std::endl;

    return 0;
}

