# Sweeq ediasv Keymap Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

## Context

The owner approved a new keymap design for their holykeebs Sweeq, built over a
brainstorming session. It replaces the QWERTY skeleton in
`keyboards/holykeebs/sweeq/keymaps/ediasv/` with five layers, mirrored home-row
mods, an Esc combo (W+E) and tap-hold tuning that avoids slowing down
typing. This plan turns the approved spec into firmware plus README updates.

**Goal:** Implement the approved 5-layer keymap (BASE/NAV/SYM/NUM/SYS) with home-row mods, the W+E Esc combo and the tap-hold tuning from the spec.

**Architecture:** Everything lives in the existing userspace keymap folder: `keymap.c` (layers, combo, tap-hold callbacks), `config.h` (feature defines), `rules.mk` (feature flags). A stdlib `unittest` test runs `qmk c2json` on `keymap.c` and asserts every layer matches the spec key for key. Timing behaviour can't be unit-tested, so it's checked with `qmk compile` plus an on-device checklist the owner runs.

**Tech Stack:** QMK 0.34.7 (local clone `/Users/ediasv/Code/qmk_firmware`), C, Python 3 stdlib `unittest` (pytest isn't installed), `qmk` CLI.

**Spec:** `docs/superpowers/specs/2026-10-24-sweeq-keymap-design.md` (approved; currently untracked).

## Global Constraints

- Layout macro: `LAYOUT` (the board's `LAYOUT_split_3x5_2`), 34 keys, order = 3 rows × (5 left + 5 right), then thumbs left-outer, left-inner, right-inner, right-outer.
- Layer enum order: `enum layers { BASE, NAV, SYM, NUM, SYS };`
- Empty keys (`·` in spec) = `XXXXXXX`; the held thumb on its own layer = `_______`. (Spec left this to the builder; pinned here so the test is exact.)
- Keep `TAPPING_TERM 200` and `PERMISSIVE_HOLD` from the current `config.h`.
- Caps Word removed (`CAPS_WORD_ENABLE`, `CW_TOGG`). `QK_CLEAR_EEPROM` removed.
- Match existing code style: SPDX header, 4-space indent, short `//` comments explaining *why*, `#define` aliases for compound keycodes.
- Build: `qmk compile -kb holykeebs/sweeq -km ediasv` must succeed with no warnings.
- Put `chordal_hold_layout`, combo arrays and callbacks **after** the `keymaps[]` array. `qmk c2json` (used by the test) finds the array by scanning for the token sequence `const uint16_t PROGMEM keymaps`, and other `const uint16_t PROGMEM` arrays placed before it could confuse that scan.
- Work on branch `feat/keymap-v2` (we're on `main`; harness rule: branch before committing).

## Review Focus

1. **Typing "we" fast** (were, web, power) must not produce Esc. Vim would leave insert mode and run the next letters as commands. → Task 3 device check D12; guarded by `combo_should_trigger`.
2. **A thumb layer plus a same-hand key** (Backspace held + left home row on NAV/NUM, Space held + right-hand closers on SYM) must give the layer key, not a tap. Chordal Hold would otherwise treat them as same-hand taps. → Task 2 `chordal_hold_layout` thumbs `'*'`, device checks D3/D4.
3. **Tap Space, then hold Space for a symbol** (`if (`) must open SYM, not repeat spaces or settle as tap. → Task 2 quick-tap/flow-tap exclusions, device check D5.
4. **Holding j/k to repeat Vim motions** must still work via tap-then-hold. → Task 2 keeps default quick-tap on home-row mods, device check D10.
5. **Option-click/Cmd-click held past the tapping term** must not type a stray letter, and the modifier must apply immediately. → Task 2 Speculative Hold, device checks D6/D7.

---

## Files

- Modify: `keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c`: layers, aliases, combo, tap-hold callbacks.
- Modify: `keyboards/holykeebs/sweeq/keymaps/ediasv/config.h`: tap-hold and combo defines.
- Modify: `keyboards/holykeebs/sweeq/keymaps/ediasv/rules.mk`: drop Caps Word, enable combos.
- Create: `tests/test_layout.py`: layer-by-layer check of `keymap.c` against the spec.
- Modify: `README.md`: Layers table, flashing instructions, Esc combo.
- Add (commit): `docs/superpowers/specs/2026-10-24-sweeq-keymap-design.md`, and this plan copied to `docs/superpowers/plans/2026-10-24-sweeq-keymap.md`.

---

### Task 1: Layers (BASE/NAV/SYM/NUM/SYS) with a layout test

**Files:**
- Create: `tests/test_layout.py`
- Modify: `keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c`, `rules.mk`, `README.md`

**Interfaces:**
- Produces (used by Tasks 2–3), in `keymap.c`:
  - `enum layers { BASE, NAV, SYM, NUM, SYS };`
  - Home-row mods: `HM_A LCTL_T(KC_A)`, `HM_S LALT_T(KC_S)`, `HM_D LGUI_T(KC_D)`, `HM_F LSFT_T(KC_F)`, `HM_J RSFT_T(KC_J)`, `HM_K RGUI_T(KC_K)`, `HM_L RALT_T(KC_L)`, `HM_SCLN RCTL_T(KC_SCLN)`
  - Thumbs: `NUM_TAB LT(NUM, KC_TAB)`, `NAV_BSP LT(NAV, KC_BSPC)`, `SYM_SPC LT(SYM, KC_SPC)`, `SYS_ENT LT(SYS, KC_ENT)`
  - Screenshots: `SC_FULL LGUI(LSFT(KC_3))`, `SC_AREA LGUI(LSFT(KC_4))`, `SC_PANL LGUI(LSFT(KC_5))`
  - `tests/test_layout.py`, run with `python3 -m unittest discover -s tests -v`.

- [ ] **Step 1: Branch and commit the spec and plan**

```bash
git checkout -b feat/keymap-v2
mkdir -p docs/superpowers/plans
cp /Users/ediasv/.claude/plans/iterative-bubbling-fern.md docs/superpowers/plans/2026-10-24-sweeq-keymap.md
git add docs/superpowers
git commit -m "docs: keymap v2 design and implementation plan"
```

- [ ] **Step 2: Write the failing layout test** `tests/test_layout.py`

The test runs `qmk c2json -kb holykeebs/sweeq -km ediasv <repo>/keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c` (subprocess, `check=True`), parses the JSON and strips all whitespace from each keycode. `c2json` expands the `#define` aliases, prints `XXXXXXX` as `KC_NO` and `_______` as `KC_TRNS`. Tests:
- `test_layer_count`: `len(layers) == 5`.
- `test_<layer>` for base/nav/sym/num/sys: `assertEqual(actual[i], EXPECTED[i])` with a per-position message `f"{name}[{pos}]"` (use `subTest` per key, so one run lists every wrong key).

Expected values (exact, from the spec; `N = "KC_NO"`, `T = "KC_TRNS"`):

```python
N, T = "KC_NO", "KC_TRNS"
BASE = ["KC_Q","KC_W","KC_E","KC_R","KC_T",  "KC_Y","KC_U","KC_I","KC_O","KC_P",
        "LCTL_T(KC_A)","LALT_T(KC_S)","LGUI_T(KC_D)","LSFT_T(KC_F)","KC_G",
        "KC_H","RSFT_T(KC_J)","RGUI_T(KC_K)","RALT_T(KC_L)","RCTL_T(KC_SCLN)",
        "KC_Z","KC_X","KC_C","KC_V","KC_B",  "KC_N","KC_M","KC_COMM","KC_DOT","KC_SLSH",
        "LT(NUM,KC_TAB)","LT(NAV,KC_BSPC)","LT(SYM,KC_SPC)","LT(SYS,KC_ENT)"]
NAV  = [N,N,N,N,N,  "KC_EQL","KC_MINS","KC_PLUS","KC_UNDS",N,
        "KC_LCTL","KC_LALT","KC_LGUI","KC_LSFT",N,  "KC_LEFT","KC_DOWN","KC_UP","KC_RGHT","KC_COLN",
        N,N,N,N,N,  "KC_QUES","KC_EXLM","KC_AMPR","KC_PIPE",N,
        N,T,N,N]
SYM  = ["KC_EXLM","KC_AT","KC_HASH","KC_DLR","KC_PERC",  "KC_CIRC","KC_AMPR","KC_ASTR",N,N,
        "KC_LT","KC_LBRC","KC_LCBR","KC_LPRN","KC_DQUO",  "KC_QUOT","KC_RPRN","KC_RCBR","KC_RBRC","KC_GT",
        "KC_GRV","KC_TILD","KC_BSLS","KC_PIPE",N,  N,N,N,N,N,
        N,N,T,N]
NUM  = [N,N,N,N,N,  "KC_ASTR","KC_7","KC_8","KC_9","KC_PLUS",
        "KC_LCTL","KC_LALT","KC_LGUI","KC_LSFT",N,  "KC_SLSH","KC_4","KC_5","KC_6","KC_MINS",
        N,N,N,N,N,  "KC_DOT","KC_1","KC_2","KC_3","KC_EQL",
        T,N,"KC_0","KC_COMM"]
SYS  = ["KC_F7","KC_F8","KC_F9","KC_F12",N,  "LGUI(LSFT(KC_3))","LGUI(LSFT(KC_4))","LGUI(LSFT(KC_5))",N,"QK_BOOT",
        "KC_F4","KC_F5","KC_F6","KC_F11",N,  "KC_MPRV","KC_VOLD","KC_VOLU","KC_MNXT",N,
        "KC_F1","KC_F2","KC_F3","KC_F10",N,  "KC_MUTE","KC_MPLY","KC_BRID","KC_BRIU",N,
        N,N,N,T]
```

- [ ] **Step 3: Run it and confirm it fails**

Run: `python3 -m unittest discover -s tests -v`
Expected: FAIL. `test_layer_count` reports 4 ≠ 5, and the layer tests list mismatches (e.g. `base[10]`: `KC_A` ≠ `LCTL_T(KC_A)`).

- [ ] **Step 4: Rewrite the layers in `keymap.c`**

Use the aliases from Interfaces. Drop the old `OS_*` one-shot defines, the old `NAV_SPC/NUM_BSP/SYM_ENT`, `CW_TOGG` and `QK_CLEAR_EEPROM`. Keep a short comment on SYS saying why `QK_BOOT` is in the outer corner.

- [ ] **Step 5: Remove Caps Word from `rules.mk`**

Delete `CAPS_WORD_ENABLE = yes`. Task 3 adds `COMBO_ENABLE`, so the file may be empty in between.

- [ ] **Step 6: Run the test and compile**

Run: `python3 -m unittest discover -s tests -v` → Expected: all tests `ok`.
Run: `qmk compile -kb holykeebs/sweeq -km ediasv` → Expected: `[OK]` for every step, ends with `holykeebs_sweeq_ediasv.uf2` copied.

- [ ] **Step 7: Update README `## Layers` and "Later updates"**

- Layers table: BASE (QWERTY, home-row mods Ctrl/Opt/Cmd/Shift mirrored, thumbs `Tab`/NUM, `Bksp`/NAV, `Space`/SYM, `Enter`/SYS), NAV (hold left inner, arrows on hjkl, operators, plain mods), SYM (hold right inner, mirrored brackets), NUM (hold left outer, numpad, `0` on right thumb), SYS (hold right outer, F-keys, screenshots, media, `QK_BOOT`).
- "Later updates": *hold `Enter` (right outer thumb) and press `QK_BOOT` (right top-outer key, the `P` position)*. The rest of that paragraph (only the right half reboots; keymap-only changes need only the right half) stays.
- Delete the note about `QK_CLEAR_EEPROM` on SYM's corners. Keep the bootmagic and `Q` notes.

- [ ] **Step 8: Commit**

```bash
git add tests/test_layout.py keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c keyboards/holykeebs/sweeq/keymaps/ediasv/rules.mk README.md
git commit -m "feat: keymap v2 layers with home-row mods"
```

---

### Task 2: Tap-hold tuning (Chordal Hold, Flow Tap, quick-tap, Speculative Hold)

**Files:**
- Modify: `keyboards/holykeebs/sweeq/keymaps/ediasv/config.h`, `keymap.c`

**Interfaces:**
- Consumes: the `HM_*` and thumb aliases from Task 1.
- Produces: `process_record_user` doesn't exist yet. Task 3 adds it; nothing here depends on it.

- [ ] **Step 1: Add defines to `config.h`** (keep `TAPPING_TERM 200`, `PERMISSIVE_HOLD`)

```c
#define CHORDAL_HOLD
#define FLOW_TAP_TERM 150
#define SPECULATIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY
```

- [ ] **Step 2: `const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM` in `keymap.c`**

`LAYOUT(...)`: `'L'` ×5 / `'R'` ×5 per row, and `'*'` on all four thumbs. The thumbs must be `'*'`, or holding Backspace plus a left-hand key (NAV/NUM mods) or Space plus a right-hand key (SYM closers) settles as a tap. Add a comment saying so.

- [ ] **Step 3: `uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode)`**

Return `0` for `NUM_TAB`, `NAV_BSP`, `SYM_SPC`, `SYS_ENT`. Otherwise use the QMK default: `is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode) ? FLOW_TAP_TERM : 0`. Don't override `is_flow_tap_key`: its default treats a Space tap as a flow key, which is what makes "home-row mod right after a letter or Space settles as tap" work. All eight home-row mods, F/J included, get Flow Tap (the owner accepted the capital-letter pause).

- [ ] **Step 4: `uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record)`**

Return `0` for `NUM_TAB`, `SYM_SPC`, `SYS_ENT`; otherwise `QUICK_TAP_TERM`. Backspace keeps tap-then-hold repeat. Home-row mods keep it too (my call), so tap-then-hold `j`/`k` repeats Vim motions.

- [ ] **Step 5: `bool get_speculative_hold(uint16_t keycode, keyrecord_t *record)`**

Return `true` for the eight `HM_*` keys (`IS_QK_MOD_TAP(keycode)` is fine, since only home-row mods are mod-taps), `false` otherwise. This extends the default (Shift/Ctrl only) to Option and Cmd, for Option-click and Cmd-click.

- [ ] **Step 6: Run the test and compile**

Run: `python3 -m unittest discover -s tests -v` → all `ok` (layers unchanged).
Run: `qmk compile -kb holykeebs/sweeq -km ediasv` → `[OK]`, no warnings.

- [ ] **Step 7: Commit**

```bash
git add keyboards/holykeebs/sweeq/keymaps/ediasv/config.h keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c
git commit -m "feat: chordal hold, flow tap and speculative hold tuning"
```

---

### Task 3: Esc combo (W + E) with a fast-typing guard

**Files:**
- Modify: `keyboards/holykeebs/sweeq/keymaps/ediasv/rules.mk`, `config.h`, `keymap.c`, `README.md`

**Interfaces:**
- Consumes: nothing from earlier tasks beyond the file.
- Produces: `combo_t key_combos[]` with one combo; `process_record_user` (new).

- [ ] **Step 1: `rules.mk`:** add `COMBO_ENABLE = yes`.

- [ ] **Step 2: `config.h`:**

```c
#define COMBO_TERM 40
#define COMBO_SHOULD_TRIGGER
// Combos are blocked if a key was pressed less than this many ms ago, so
// rolling "we" while typing never fires Esc.
#define COMBO_IDLE_MS 100
```

(Values are my call; tune on device. The spec leaves timings to the builder.)

- [ ] **Step 3: `keymap.c`:** the combo

`const uint16_t PROGMEM esc_combo[] = {KC_W, KC_E, COMBO_END};` and `combo_t key_combos[] = { COMBO(esc_combo, KC_ESC) };`

- [ ] **Step 4: `keymap.c`:** the guard

- `static uint16_t last_press_time;`
- `bool process_record_user(uint16_t keycode, keyrecord_t *record)`: on `record->event.pressed`, set `last_press_time = timer_read();` and return `true`. It runs after combo processing, so W/E consumed by a combo never update it.
- `bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record)`: return `timer_elapsed(last_press_time) >= COMBO_IDLE_MS`.

- [ ] **Step 5: Run the test and compile**

Run: `python3 -m unittest discover -s tests -v` → all `ok`.
Run: `qmk compile -kb holykeebs/sweeq -km ediasv` → `[OK]`.

- [ ] **Step 6: README:** add one line under the Layers table: *Esc: press `W` + `E` together (doesn't fire while typing fast).*

- [ ] **Step 7: Commit**

```bash
git add keyboards/holykeebs/sweeq/keymaps/ediasv/ README.md
git commit -m "feat: W+E Esc combo with fast-typing guard"
```

---

## Verification

**Automated (each task):** `python3 -m unittest discover -s tests -v` and `qmk compile -kb holykeebs/sweeq -km ediasv`.

**On device (owner):** flash the right half only (keymap-only change) with the README's "Later updates" method using the *old* firmware's key (hold the old SYM thumb = right outer, press the left top-outer `QK_BOOT`), or double-tap reset. Copy `holykeebs_sweeq_ediasv.uf2` to `RPI-RP2`. Then, in a text editor:

| # | Do | Expect |
|---|---|---|
| D1 | Type a fast paragraph with "asdf jkl;" rolls | No stray modifiers |
| D2 | `fooBar` fast, then with a short pause before holding J for B | Fast gives `foojbar` (accepted); paused gives `fooBar` |
| D3 | Hold Backspace + NAV Shift (F position) + `l` | Selects one char right |
| D4 | Hold Space + `j` / `f` | `)` / `(` |
| D5 | Type `if (` at speed (tap Space, hold Space + F) | `if (` |
| D6 | Hold S, drag a window / Option-click | Option applies immediately, no `s` typed |
| D7 | Hold K (>200 ms), click a link | Opens in new tab, no `k` typed |
| D8 | K + Q in a throwaway app; D + Space | App quits; Spotlight opens |
| D9 | Tap then hold Backspace; tap then hold Space | Deletes repeatedly; Space opens SYM (no repeated spaces) |
| D10 | In Vim normal mode, tap then hold `j` | Cursor keeps moving down |
| D11 | Pause, press W+E together (Vim insert mode, a macOS dialog) | Esc: leaves insert mode / closes dialog |
| D12 | Type "were we went power" fast in insert mode | Text typed, no Esc |
| D13 | Hold Enter + `P` | Right half reboots to `RPI-RP2` |
| D14 | SYS screenshot keys, Vol±, Bri± | Cmd+Shift+3/4/5 behave; volume/brightness change |

## Execution

Recommended: **Native** (I implement the tasks in this session, then one fresh reviewer checks the whole branch). There are three small, sequential tasks in the same three files, and a mistake costs only a reflash, so a fresh subagent and reviewer per task isn't worth the cost. Say "subagent-driven" when approving if you'd prefer a separate implementer and reviewer for each task.

Calls I made in this plan (not in the spec): empty keys `XXXXXXX` and held thumbs `_______`; the combo's fast-typing guard (`COMBO_TERM 40`, `COMBO_IDLE_MS 100`); `FLOW_TAP_TERM 150`; home-row mods keep tap-then-hold repeat for Vim `j`/`k`.
