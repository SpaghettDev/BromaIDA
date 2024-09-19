# BromaIDA

IDA Broma support (now real).

Parses a Broma file and exports the bindings into a Broma file/imports the bindings into the current IDA project.

[![BromaIDA](assets/bida.gif)](https://github.com/SpaghettDev/BromaIDA/releases)

## Requirements

- IDA 7.0 or higher
- IDAPython
- Python v3.10.0+ (tested with v3.11.9)
- PyBroma
- If you want to be able to import types:
  - IDAClang
  - STL Headers (see [Importing Types Requirements](#importing-types-requirements))

## Features

- Importing
  - Import function names
  - Import types (see [Importing Types Requirements](#importing-types-requirements)). If enabled, will do the following:
    - Import Broma classes members, virtual functions, etc...
    - Import function return types
    - Import function arguments types and names
    - On Android binaries, fixes IDA completely messing up the arguments of functions for no reason
- Exporting
  - Export function addresses for any platform
  - Export function return types (if enabled, see [BromaIDA Settings](#bromaida-settings))
  - Export function arguments' names (if enabled, see [BromaIDA Settings](#bromaida-settings))

## Installation

1. Get python, then `git clone https://github.com/CallocGD/PyBroma --recursive`, `cd PyBroma` then finally `python .\setup.py install` to install PyBroma (blame CallocGD for breaking `pip install https://github.com/CallocGD/PyBroma/archive/refs/heads/main.zip`)
2. Download the release/zip file
3. Copy `BromaIDA.py` and the `broma_ida` folder into `IDA_DIRECTORY/plugins`

## Usage

1. `Ctrl-Shift-B`, or from the Top Bar (Edit -> Plugins -> BromaIDA) to start importing/exporting
2. Browse and select the Broma file
3. Let the script handle the rest and enjoy free/exported bindings

## Importing Types Requirements

- To import types:
  - You must have IDAClang.
  - You must have the STL headers for the target platform (MSVC headers for Windows binary, GCC headers for the others). You can however use MSVC headers on binaries that aren't Windows (_but not the other way around_).
  - Extras.bro **_must_** be in the same directory as the imported Broma file

- Get your STL Headers path (on Windows)
  - For Windows binary (or if not using custom GNU STL)
     1. Get MSVC Headers by installing MSVC using the [Visual Studio Installer](https://visualstudio.microsoft.com/downloads)
     2. Your header path should be `C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\include`
  - For Android binaries (Android 64 and 32 bit)
     1. First get gcc-arm-none-linux-gnueabihf (i did so by using scoop; `scoop install extras/gcc-arm-none-linux-gnueabihf`)
     2. Your header path should be `C:\Users\YOUR_NAME\scoop\apps\gcc-arm-none-linux-gnueabihf\current\arm-none-linux-gnueabihf\include\c++\13.3.1;C:\Users\YOUR_NAME\scoop\apps\gcc-arm-none-linux-gnueabihf\current\arm-none-linux-gnueabihf\include\c++\13.3.1\arm-none-linux-gnueabihf;C:\Users\YOUR_NAME\scoop\apps\gcc-arm-none-linux-gnueabihf\current\arm-none-linux-gnueabihf\libc\usr\include;C:\Users\YOUR_NAME\scoop\apps\gcc-arm-none-linux-gnueabihf\current\lib\gcc\arm-none-linux-gnueabihf\13.3.1\include` (be sure to replace YOUR_NAME with your windows username)
  - For MacOS (Intel and M1 Mac) and iOS binaries
     1. First get gcc (i did so by using scoop; `scoop install gcc`)
     2. Your header path should be `C:\Users\YOUR_NAME\scoop\apps\gcc\current\include\c++\13.2.0;C:\Users\YOUR_NAME\scoop\apps\gcc\current\include\c++\13.2.0\x86_64-w64-mingw32;C:\Users\YOUR_NAME\scoop\apps\gcc\current\x86_64-w64-mingw32\include` (be sure to replace YOUR_NAME with your windows username)

1. Launch the BromaIDA plugin using Ctrl-Shift-B, or from the Top Bar (Edit -> Plugins -> BromaIDA)
2. Click on the Settings button
3. Follow the steps above to get and get your STL Headers path (be sure to check "Import Types")
   - Once you get your STL Header path, paste it in the correct folder input. ("MSVC STL Directory" for MSVC, ...)
   - If you **_aren't_** using custom GNU STL, be sure to **uncheck** "Use Custom [Android/Mac] GNU STL".
   - If you **_are_** using custom GNU STL, be sure to **check** "Use Custom [Android/Mac] GNU STL", otherwise you won't be able to select the custom GNU STL path.
4. Exit out of the settings window

Start the script and the plugin will handle everything :)

## BromaIDA Settings

Can be accessed through the main popup by clicking on the "Settings" button.
This replaces the old "ask everything everytime" method.

Here you will find some options you can set, such as importing types, settings default parser parameters, etc...
The options you chose will be saved into a [shelf](https://docs.python.org/3/library/shelve.html) stored in the `IDA_DIRECTORY/plugins/broma_ida` folder.

![BromaIDA Settings](assets/settings.png)

## Utilities

This section is meant for people who want to contribute.

In the `utils` folder, you can find some utilities that can help in contributing.

- Currently has:
  - `tasks.json`: To be put in `/BromaIDA/.vscode`, adds 2 tasks for VSCode. "Install Plugin" will copy `BromaIDA.py` and the `broma_ida` to the `IDA_DIRECTORY/plugins` directory; And "Launch IDA v8.3" which will launch IDA (shocker). Just be sure to replace IDA's directory with your IDA installation directory.

## Thanks

- Special thanks to:
  - The [IDAPython API](https://hex-rays.com/products/ida/support/idapython_docs): Used for everything.
  - [CallocGD](https://github.com/CallocGD)'s [PyBroma](https://github.com/CallocGD/PyBroma): Used to parse imported Broma files.
  - @sleepyut: Issuing 3 trillion bug reports. Also for suggesting a bunch of features. (they also made BromaBJ)
  - [AngelDev06](https://github.com/AngelDev06): Contributing features.
