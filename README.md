# Localized EmulationStation

This is a localized fork of [Retropie's EmulationStation fork](https://github.com/RetroPie/EmulationStation).

Inspired by similar work on the Recalbox fork.

## Languages currently supported

* Catalan
* French
* Italian
* Japanese
* Portuguese (Brazilian)
* Spanish

## How to use the localized EmulationStation

### Resolve dependencies of libraries

```bash
sudo apt-get install -y libboost-system-dev libboost-filesystem-dev libboost-date-time-dev libboost-locale-dev libfreeimage-dev libfreetype6-dev libeigen3-dev libcurl4-openssl-dev libasound2-dev cmake libsdl2-dev
```

### Create a clone from GitHub

```
cd /home/pi/
git clone --recursive https://github.com/flyinghead/EmulationStation.git
```

### Compile EmulationStation

```
cd /home/pi/EmulationStation
mkdir build
cd build
cmake ..
make
```

### Run EmulationStation

> Be aware that your system must be set to the same locale that you want EmulationStation to run on.

```
cd /home/pi/EmulationStation
LANG=[your_locale].UTF8 ./emulationstation
```

### Install EmulationStation

```
cd /home/pi/EmulationStation
sudo cp ./emulationstation /opt/retropie/supplementary/emulationstation/
sudo cp -r locale /opt/retropie/supplementary/emulationstation/
```

## How to add a translation

> You should have, at least, [created a clone from GitHub](#create-a-clone-from-github) before continuing with the following steps.

### Create files for the new locale

```
cd /home/pi/EmulationStation/locale
mkdir lang/[your_locale]
cp emulationstation2.pot lang/[your_locale]/emulationstation2.po
```

### Translate the strings

Open the newly created `emulationstation2.po` inside `[your_locale]` folder and start translating all the `msgstr` strings.

Once you are done, you can [compile EmulationStation](#compile-emulationstation) and then [run](#run-emulationstation) it to test your translations.

## Author

[flyinghead](https://github.com/flyinghead).

## Credits

Thanks to:

* [flyinghead](https://github.com/flyinghead) - For the French translations.
* [heloisa](https://github.com/heloisatech) - For the Portuguese (Brazilian) and Spanish (Spain) translations.
* [wakaya](https://retropie.org.uk/forum/user/wakaya) - For the Japanese translations and for the instructions on how to use the localized EmulationStation.
* [hiulit](https://github.com/hiulit) - For the Catalan and Spanish (Spain) translations.
* Unknown - For the Italian translations.
