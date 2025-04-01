cmake_minimum_required(VERSION 3.15)
project(ExcuseGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Crow REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SqliteOrm REQUIRED)

include_directories("include")

# aux_source_directory("src/database.cpp" DB)
add_executable(${PROJECT_NAME} "src/main.cpp" "src/database.cpp")

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Crow::Crow
    nlohmann_json::nlohmann_json
    sqlite_orm::sqlite_orm
)

#запуск
# cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake