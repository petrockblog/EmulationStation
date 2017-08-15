find . -iname "*.cpp" | xargs xgettext --from-code=UTF-8 --default-domain=emulationstation --keyword=_ --add-comments -o locale/emulationstation.pot
