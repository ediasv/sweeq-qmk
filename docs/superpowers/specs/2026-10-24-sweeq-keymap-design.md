# Sweeq keymap design (ediasv)

Date: 2026-10-24
Replaces the current skeleton in `keyboards/holykeebs/sweeq/keymaps/ediasv/`.

## Intent

A 34-key QWERTY keymap for the holykeebs Sweeq, for a programmer on macOS who
writes TypeScript, C/C++, assembly, Python and Go. The point is **minimal finger
movement**: quick access to symbols, numbers and arrows, an Esc that needs no
reach, and Cmd/Option always at hand, without having to slow down typing to
suit the tap-hold keys.

Background the design leans on:

- The owner has used tap-hold keys before. They didn't misfire, but the owner
  had to **slow down their typing** to match the layer activation. Avoiding that
  is a goal.
- Esc is the most awkward key today. It's used constantly, both for Vim and to
  close macOS dialogs.
- The mouse is in the **right** hand. Option is held (left hand) while clicking
  and while dragging windows to tile them.
- Most-used Cmd shortcuts: Cmd+Q, Cmd+Space, Ctrl+Cmd+Q (lock), and the
  screenshot shortcuts Cmd+Shift+3/4/5.
- Navigation beyond arrows is done with Vim motions on the base layer, so the
  navigation layer stays minimal.

## Goals

- Every key the owner listed is reachable without leaving home position:
  Esc, Tab, Backspace, Space, Enter, Cmd, Option, Ctrl, Shift, all symbols,
  digits, arrows.
- Tap-hold keys don't force slow typing (see "Tap-hold behaviour").
- Option + mouse click/drag works with the left hand alone, held as long as
  needed.
- Screenshots on dedicated keys.

## Non-goals

- No Caps Lock key.
- No rich navigation layer (no Home/End/PgUp/PgDn, word-jump keys, etc.).
- No layout change away from QWERTY.

## Anti-goals

- Letters misfiring as modifiers while typing fast, or modifiers that need a
  deliberate pause to activate. If the home-row mods do this, the design
  has failed even if every key is technically present.

## Layers

Five layers: `BASE`, `NAV`, `SYM`, `NUM`, `SYS`. Thumb order below is
left-outer, left-inner, right-inner, right-outer. The inner keys (nearer the
middle of the board) are where the thumbs rest. `·` = no key
(`KC_NO`/`_______`, builder's choice).

### BASE

```
Q        W        E        R        T          Y        U        I        O        P
A/Ctrl   S/Opt    D/Cmd    F/Shift  G          H        J/Shift  K/Cmd    L/Opt    ;/Ctrl
Z        X        C        V        B          N        M        ,        .        /
                  Tab/NUM  Bksp/NAV               Space/SYM  Enter/SYS
```

- **Home-row mods**, mirrored. Tap = letter, hold = modifier. Left: A=Ctrl,
  S=Option, D=Cmd, F=Shift. Right: J=Shift, K=Cmd, L=Option, ;=Ctrl.
  (Left-hand keycodes on the left half, right-hand ones on the right, e.g.
  `LCTL_T(KC_A)` … `RCTL_T(KC_SCLN)`.)
- **Thumbs**: tap = key, hold = layer.
  `LT(NUM, KC_TAB)`, `LT(NAV, KC_BSPC)`, `LT(SYM, KC_SPC)`, `LT(SYS, KC_ENT)`.
- **Esc = combo W + E** pressed together. W and E still type normally when
  pressed alone. (Rejected: a combo on Tab+Backspace with the thumb, too
  awkward; J+K, collides with the home-row mods.)
- Backspace auto-repeat: tap then press-and-hold repeats (quick-tap behaviour),
  since a plain hold opens NAV.

How the owner's shortcuts land (all cross-hand, see Chordal Hold below):

| Shortcut | Fingers |
|---|---|
| Option + click/drag | hold S (left ring), mouse in right hand |
| Cmd+Q | hold K (right), tap Q |
| Ctrl+Cmd+Q | hold K + ; (right), tap Q |
| Cmd+Space | hold D (left), tap Space (right thumb) |
| Cmd+Shift+3/4/5 | dedicated keys on SYS |

### NAV: hold Backspace (left thumb)

```
·        ·        ·        ·        ·          =        -        +        _        ·
Ctrl     Opt      Cmd      Shift    ·          ←        ↓        ↑        →        :
·        ·        ·        ·        ·          ?        !        &        |        ·
                  ·        (held)                  ·        ·
```

- Arrows on hjkl.
- Left home row: **plain held modifiers** (`KC_LCTL`, `KC_LALT`, `KC_LGUI`,
  `KC_LSFT`), so Shift+arrow selects and Option+arrow jumps by word.
- Right-hand operators so symbols can also be typed with the right hand.

### SYM: hold Space (right thumb)

```
!        @        #        $        %          ^        &        *        ·        ·
<        [        {        (        "          '        )        }        ]        >
`        ~        \        |        ·          ·        ·        ·        ·        ·
                  ·        ·                       (held)   ·
```

- Brackets mirrored on the home row: openers left, closers right, on
  matching fingers. `( )` index, `{ }` middle, `[ ]` ring, `< >` pinky.
  Quotes `"` and `'` on the inner columns (G and H).
- Top row follows Shift + number row order (`! @ # $ % ^ & *`).

### NUM: hold Tab (left thumb)

```
·        ·        ·        ·        ·          *        7        8        9        +
Ctrl     Opt      Cmd      Shift    ·          /        4        5        6        -
·        ·        ·        ·        ·          .        1        2        3        =
                  (held)   ·                       0        ,
```

- Numpad shape on the right hand, `0` on the right inner thumb (the Space
  position), `,` on the right outer thumb.
- Left home row: plain held modifiers (same as NAV), so Cmd+1…9 works.

### SYS: hold Enter (right thumb)

```
F7       F8       F9       F12      ·          Shot     Area     Panel    ·        Reset
F4       F5       F6       F11      ·          Prev     Vol-     Vol+     Next     ·
F1       F2       F3       F10      ·          Mute     Play     Bri-     Bri+     ·
                  ·        ·                       ·        (held)
```

- Shot = Cmd+Shift+3 (whole screen), Area = Cmd+Shift+4, Panel = Cmd+Shift+5
  (`LGUI(LSFT(KC_3))` etc.).
- Media: `KC_MPRV`, `KC_VOLD`, `KC_VOLU`, `KC_MNXT`, `KC_MUTE`, `KC_MPLY`.
  Brightness: `KC_BRID`, `KC_BRIU`.
- Reset = `QK_BOOT` in the outer top-right corner.

## Tap-hold behaviour

The owner's past complaint was having to slow down for tap-hold keys. The local
QMK clone is 0.34.7, which supports everything below. Intent first; the exact
values are the builder's to tune.

- **Flow Tap on the home-row mods (all eight, including the F/J Shifts)**:
  while typing fast, a home-row mod pressed right after a letter or Space
  settles as a tap. This is the main defence against slowing down.
  **Capitals mid-flow need a brief pause before the Shift key. The owner
  explicitly accepted this** rather than risk stray capitals. Don't exempt F/J.
- **Chordal Hold**: a home-row mod only becomes a modifier when the next key is
  on the other hand. All the owner's shortcuts above are cross-hand by design.
  Consequences, accepted: same-hand shortcuts like left Cmd + Q don't work, and
  a capital uses the opposite hand's Shift (left-hand letter → hold J,
  right-hand letter → hold F).
- **Modifier + mouse needs no wait**: holding S (Option) or D/K (Cmd) and then
  clicking must apply the modifier immediately. QMK's Speculative Hold does this,
  but by default it only covers Shift/Ctrl mod-taps, so extend it to the Option
  and Cmd home-row keys. The owner uses Option-click/drag and Cmd-click a lot and
  **will hold the key past the tapping term**. A release before the term
  types the letter (the keyboard can't see the click), and that's accepted. Lone
  Option/Cmd taps from speculation are harmless on macOS.
- **Thumb layer keys**:
  - Permissive Hold: the layer activates when another key is pressed *and
    released* while the thumb is held.
  - No Hold On Other Key Press, so rolling off Space ("e␣c") stays text.
  - Exclude all four thumb keys from Flow Tap, so holding Space for SYM right
    after a letter or Space (e.g. typing `if (`) doesn't need a pause.
  - Quick-tap (tap-then-hold repeats) only on Backspace; disable it on Space,
    Tab and Enter so tap-Space-then-hold-Space opens SYM instead of repeating
    spaces.

## Repo changes

- `keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c`: new layers, combo,
  and any tap-hold callbacks.
- `config.h` / `rules.mk`: enable combos and the tap-hold options above.
  Caps Word (`CAPS_WORD_ENABLE`, `CW_TOGG`) isn't in the new design, so drop it.
- `README.md`: the Layers table and the "Later updates" flashing instructions
  currently say *hold SYM, press QK_BOOT (left top-outer)*. That becomes
  *hold Enter (right outer thumb), press Reset (right top-outer)*. The note
  about `QK_CLEAR_EEPROM` on SYM's corners goes away with it.

## Left to the builder

- Exact timing values (tapping term, flow tap term, combo term, quick-tap term).
- Whether empty slots are `KC_NO` or transparent.
- Code organisation (defines/aliases for the mod-taps, combo definitions).

## Decided by Claude, not the owner (review these)

- Plain held modifiers (not one-shot) on the NAV and NUM left home rows.
- `QK_CLEAR_EEPROM` is dropped from the keymap (bootmagic still clears EEPROM).
- Caps Word removed (the owner chose the capital-letter pause and didn't ask
  for it).
- Thumb-key tap-hold policy above (Permissive Hold, no Flow Tap on thumbs,
  quick-tap only on Backspace).
- Speculative Hold extended to Cmd as well as Option.
- One-press operator keys (`->`, `=>`, `:=`, `::`, `!=`) were floated and not
  taken up, so they're not included. Empty slots stay free.
