# BromaIDA

# Deprecated, and will no longer be updated! Use master branch instead

IDA Broma support (now real).

Parses a Broma file and exports the bindings into a Broma file/imports the bindings into the current IDA project.

[![BromaIDA](assets/bida.gif)](https://github.com/SpaghettDev/BromaIDA/releases)

## Requirements

- IDA 7.0 or higher
- IDAPython Python v3.0.0 support
- If you want to be able to import types:
  - IDAClang
  - MSVC

## Features

TODO

## Installation

1. Download the release/zip file
2. Copy `BromaIDA.py` and the `broma_ida` folder into `IDA_DIRECTORY/plugins`

## Usage

1. `Ctrl-Shift-B` to start importing/exporting
2. Browse and select the broma file (not tested with anything but `GeometryDash.bro`)
3. Let the script handle the rest and enjoy free/exported bindings

## Thanks

Special thanks to [CallocGD](https://github.com/CallocGD)'s [PyBroma](https://github.com/CallocGD/PyBroma) which is used to import bindings from Broma.
