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
2. Enter the bootloader. **Double-tap the reset button** works on either half.
   Bootmagic also works. Hold the key while plugging in USB, and EEPROM is cleared too:
   - **right half:** hold `Y`;
   - **left half:** hold `T`. A half plugged in alone always thinks it is the
     right half, so its keys use the right half's positions.
3. Copy `holykeebs_sweeq_ediasv.uf2` onto the `RPI-RP2` drive. It ejects when done.
4. Repeat for the other half.

When you're done, connect TRRS first, then plug USB into the **right** half.

Later updates: with TRRS connected and USB on the right, hold `Enter` (right
outer thumb, the SYS layer) and press `QK_BOOT` (right top-outer key, the `P`
position). This reboots **only the right half** into the bootloader. Keymap-only changes just need the right half, because only
the half on USB runs the keymap. After changing the board definition or updating
QMK, flash **both** halves with the steps above so they stay compatible.

Notes:
- Don't hold `Q` while plugging in with TRRS connected. That triggers bootmagic
  on the left half: it drops into the bootloader and looks dead until you replug.

## Layers

| Layer | Hold | Contents |
|---|---|---|
| BASE | — | QWERTY; home-row mods Ctrl/Opt/Cmd/Shift on `A S D F`, mirrored on `J K L ;`; thumbs: `Tab`/NUM, `Bksp`/NAV, `Space`/SYM, `Enter`/SYS |
| NAV | left inner thumb (`Bksp`) | arrows on `hjkl`, operators `= - + _ : ? ! & \|`, plain Ctrl/Opt/Cmd/Shift on left home row |
| SYM | right inner thumb (`Space`) | brackets mirrored on the home row (openers left, closers right), `! @ # $ % ^ & *`, quotes, `` ` ~ \ \| `` |
| NUM | left outer thumb (`Tab`) | numpad on the right hand, `0` and `,` on the right thumbs, plain mods on left home row |
| SYS | right outer thumb (`Enter`) | F1–F12, screenshots (Cmd+Shift+3/4/5), media, brightness, `QK_BOOT` |

Esc: press `W` + `E` together (doesn't fire while typing fast).

## Updating QMK

```sh
git -C ~/Code/qmk_firmware pull
qmk git-submodule
```

If upstream ever adds its own `keyboards/holykeebs/sweeq`, `git pull` stops
because the symlink is in the way. Remove it with
`rm ~/Code/qmk_firmware/keyboards/holykeebs/sweeq` (no trailing slash, so only the
link is removed), pull again, and then either use upstream's board or move
upstream's folder aside and re-run `./setup.sh` to keep this one.
