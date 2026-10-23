# sweeq-qmk

QMK keymap for a holykeebs Sweeq (no pointing device, no OLED), built against
**upstream** [qmk/qmk_firmware](https://github.com/qmk/qmk_firmware).

Upstream has no `holykeebs/sweeq` board, so this repo carries the board
definition in `board/holykeebs/sweeq/` (ported from
[holykeebs/qmk_firmware](https://github.com/holykeebs/qmk_firmware/tree/hk-master/keyboards/holykeebs/sweeq))
and links it into the upstream clone. The repo is also a QMK external
userspace holding the keymap.

```
board/holykeebs/sweeq/                    board definition (pins, matrix, split)
keyboards/holykeebs/sweeq/keymaps/ediasv/ the keymap
qmk.json                                  userspace build targets
setup.sh                                  links the board into qmk_firmware
```

## Setup

```sh
brew install qmk/qmk/qmk
qmk setup -H ~/Code/qmk_firmware
./setup.sh            # set QMK_HOME=... if the clone lives elsewhere
```

## Build

```sh
qmk userspace-compile                       # -> ./holykeebs_sweeq_ediasv.uf2
# or
qmk compile -kb holykeebs/sweeq -km ediasv
```

## Flash

> **Never plug or unplug the TRRS cable while the keyboard is powered.**
> It can short the controllers' GPIO pins.

Flash both halves with the same `.uf2`:

1. Unplug everything. Connect one half over USB, with TRRS disconnected.
2. Enter the bootloader. Use any of these:
   - double-tap the reset button;
   - hold `SYM` (right outer thumb) and press `QK_BOOT` (left top-outer key);
   - hold `Y` while plugging USB into the right half (bootmagic; this also clears EEPROM).
3. Copy `holykeebs_sweeq_ediasv.uf2` onto the `RPI-RP2` drive. It ejects when done.
4. Repeat for the other half.

When you're done, connect TRRS first, then plug USB into the **right** half.

## Layers

| Layer | Hold | Contents |
|---|---|---|
| BASE | — | QWERTY; thumbs: `Tab`, `Space`/NAV, `Bksp`/NUM, `Enter`/SYM |
| NAV | left inner thumb | arrows on `hjkl`, Home/PgDn/PgUp/End, Esc, Del, one-shot Gui/Alt/Ctrl/Shift on left home row, Caps Word |
| NUM | right inner thumb | 1–0, F1–F12 |
| SYM | right outer thumb | symbols, `QK_BOOT`, `QK_CLEAR_EEPROM` |

## Updating QMK

```sh
git -C ~/Code/qmk_firmware pull
qmk git-submodule
```
