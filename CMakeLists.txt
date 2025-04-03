cmake_minimum_required(VERSION 3.15)
project(ExcuseGenerator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Crow REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SqliteOrm REQUIRED)

add_executable(${PROJECT_NAME} "src/main.cpp" "src/database.cpp" "src/logic.cpp")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE
    Crow::Crow
    nlohmann_json::nlohmann_json
    sqlite_orm::sqlite_orm
)

#запуск
# cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake