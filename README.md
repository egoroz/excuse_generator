# Генератор нелепых извинений

Это приложение, способное придумать уникальную отмазку, которая сможет вам помочь (нет)

запуск (локально):
> В корне проекта
>```
>mkdir build && cd build
>cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
>```
> В папке frontend открываем порт 
>`python3 -m http.server 9001`
> Переходим на сайт http://localhost:9001/
