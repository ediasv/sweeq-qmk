# Sweeq

*Sweeq is a Ferris Sweep clone that uses diodes, instead of the latter's direct pin matrix.*

Board definition ported from [holykeebs/qmk_firmware](https://github.com/holykeebs/qmk_firmware/tree/hk-master/keyboards/holykeebs/sweeq) to build against upstream QMK, for a Sweeq without pointing devices or OLED.

* Keyboard Maintainer: [@idank](https://github.com/idank)
* Hardware Supported: [sweeq](https://github.com/idank/keyboards/tree/main/sweeq)
* Hardware Availability: *[holykeebs](https://holykeebs.com)*

Make example for this keyboard (after setting up your build environment):

    qmk compile -kb holykeebs/sweeq -km default

## Bootloader

* **Double-tap reset**: Double-tap the reset button on the controller
* **Bootmagic reset**: Hold the top-left key of the right half (`Y`) while plugging USB into the right half
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
