# fightboard

![fightboard](https://cartex10.github.io/assets/fightboard/v1.jpg)

The first version of the fightstick-keyboard, including 12 mx-compatible switches and 4 pushbuttons.

* Keyboard Maintainer: [Carlos Ortiz](https://github.com/Cartex10)
* Hardware Supported: *Custom PCB featuring a XIAO RP2040 MCU and MCP23018 I/O Expander*

Make example for this keyboard (after setting up your build environment):

    make fightboard:default

Flashing example for this keyboard:

    make fightboard:default:flash
    qmk compile -kb fightboard -km default
See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down the 'W' key and plug in the keyboard
* **Physical reset button**: Press the button on the bottom right of the MCU(USB-C port up) and plug in the keyboard
