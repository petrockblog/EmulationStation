find . -iname "*.cpp" | xargs xgettext --from-code=UTF-8 --default-domain=emulationstation --keyword=_ --add-comments --sort-output -o locale/emulationstation.pot
