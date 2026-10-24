// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers { BASE, NAV, SYM, NUM, SYS };

// Home-row mods, mirrored: tap for the letter, hold for Ctrl/Opt/Cmd/Shift.
#define HM_A LCTL_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LGUI_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J RSFT_T(KC_J)
#define HM_K RGUI_T(KC_K)
#define HM_L RALT_T(KC_L)
#define HM_SCLN RCTL_T(KC_SCLN)

// Thumb keys: tap for the key, hold for the layer.
#define NUM_TAB LT(NUM, KC_TAB)
#define NAV_BSP LT(NAV, KC_BSPC)
#define SYM_SPC LT(SYM, KC_SPC)
#define SYS_ENT LT(SYS, KC_ENT)

// macOS screenshots: whole screen, area, and the screenshot panel.
#define SC_FULL LGUI(LSFT(KC_3))
#define SC_AREA LGUI(LSFT(KC_4))
#define SC_PANL LGUI(LSFT(KC_5))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        HM_A,    HM_S,    HM_D,    HM_F,    KC_G,         KC_H,    HM_J,    HM_K,    HM_L,    HM_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                                   NUM_TAB, NAV_BSP,      SYM_SPC, SYS_ENT
    ),

    // Plain held mods on the left so Shift/Opt + arrows select and jump words.
    [NAV] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_EQL,  KC_MINS, KC_PLUS, KC_UNDS, XXXXXXX,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_COLN,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_QUES, KC_EXLM, KC_AMPR, KC_PIPE, XXXXXXX,
                                   XXXXXXX, _______,      XXXXXXX, XXXXXXX
    ),

    // Brackets mirrored on the home row: openers left, closers right.
    [SYM] = LAYOUT(
        KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,      KC_CIRC, KC_AMPR, KC_ASTR, XXXXXXX, XXXXXXX,
        KC_LT,   KC_LBRC, KC_LCBR, KC_LPRN, KC_DQUO,      KC_QUOT, KC_RPRN, KC_RCBR, KC_RBRC, KC_GT,
        KC_GRV,  KC_TILD, KC_BSLS, KC_PIPE, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                   XXXXXXX, XXXXXXX,      _______, XXXXXXX
    ),

    [NUM] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_ASTR, KC_7,    KC_8,    KC_9,    KC_PLUS,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, XXXXXXX,      KC_SLSH, KC_4,    KC_5,    KC_6,    KC_MINS,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      KC_DOT,  KC_1,    KC_2,    KC_3,    KC_EQL,
                                   _______, XXXXXXX,      KC_0,    KC_COMM
    ),

    // QK_BOOT sits on the outer top corner so it is hard to hit by accident.
    [SYS] = LAYOUT(
        KC_F7,   KC_F8,   KC_F9,   KC_F12,  XXXXXXX,      SC_FULL, SC_AREA, SC_PANL, XXXXXXX, QK_BOOT,
        KC_F4,   KC_F5,   KC_F6,   KC_F11,  XXXXXXX,      KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, XXXXXXX,
        KC_F1,   KC_F2,   KC_F3,   KC_F10,  XXXXXXX,      KC_MUTE, KC_MPLY, KC_BRID, KC_BRIU, XXXXXXX,
                                   XXXXXXX, XXXXXXX,      XXXXXXX, _______
    )
};

// Thumbs are exempt ('*') from the opposite-hands rule: holding Backspace with
// a left-hand key (NAV/NUM mods) or Space with a right-hand key (SYM closers)
// must still activate the layer.
const char chordal_hold_layout[MATRIX_ROWS][MATRIX_COLS] PROGMEM = LAYOUT(
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
    'L', 'L', 'L', 'L', 'L',      'R', 'R', 'R', 'R', 'R',
                   '*', '*',      '*', '*'
);

static bool is_thumb_layer_key(uint16_t keycode) {
    switch (keycode) {
        case NUM_TAB:
        case NAV_BSP:
        case SYM_SPC:
        case SYS_ENT:
            return true;
    }
    return false;
}

// No Flow Tap on thumbs, so holding Space for SYM right after a letter or a
// Space (typing `if (`) needs no pause. Home-row mods keep QMK's default.
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    if (is_thumb_layer_key(keycode)) {
        return 0;
    }
    return is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode) ? FLOW_TAP_TERM : 0;
}

// Tap-then-hold repeats only Backspace (and home-row letters, for Vim j/k);
// on Tab, Space and Enter it opens the layer instead.
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case NUM_TAB:
        case SYM_SPC:
        case SYS_ENT:
            return 0;
    }
    return QUICK_TAP_TERM;
}

// Speculate on every home-row mod, not just Shift/Ctrl, for Option/Cmd + click.
bool get_speculative_hold(uint16_t keycode, keyrecord_t *record) {
    return IS_QK_MOD_TAP(keycode);
}

// Included here, after keymaps[], because qmk c2json (tests/test_layout.py)
// loses track of the keymaps array if function bodies come before it.
#include "esc_guard.h"

// Esc: press W and E together.
const uint16_t PROGMEM esc_combo[] = {KC_W, KC_E, COMBO_END};
combo_t key_combos[] = {
    COMBO(esc_combo, KC_ESC),
};

// Runs on the raw press, just before combo processing and before tap-hold
// keys settle, so a Space or home-row mod still held over W+E counts.
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        esc_guard_press(record->event.time);
    }
    return true;
}

// Called before the combo records this key, so an empty state means W or E
// is the chord's first key.
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    return esc_guard_allows(record->event.time, combo->state == 0);
}
