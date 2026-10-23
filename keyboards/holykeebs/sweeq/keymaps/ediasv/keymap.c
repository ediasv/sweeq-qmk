// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers { BASE, NAV, NUM, SYM };

// Thumb keys: tap for the key, hold for the layer.
#define NAV_SPC LT(NAV, KC_SPC)
#define NUM_BSP LT(NUM, KC_BSPC)
#define SYM_ENT LT(SYM, KC_ENT)

// One-shot modifiers: tap, then press the key to modify.
#define OS_GUI OSM(MOD_LGUI)
#define OS_ALT OSM(MOD_LALT)
#define OS_CTL OSM(MOD_LCTL)
#define OS_SFT OSM(MOD_LSFT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT(
        KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
        KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
                                   KC_TAB,  NAV_SPC,      NUM_BSP, SYM_ENT
    ),

    [NAV] = LAYOUT(
        KC_ESC,  _______, _______, _______, _______,      KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DEL,
        OS_GUI,  OS_ALT,  OS_CTL,  OS_SFT,  _______,      KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
        _______, _______, _______, _______, _______,      CW_TOGG, _______, _______, _______, _______,
                                   _______, _______,      _______, _______
    ),

    [NUM] = LAYOUT(
        KC_1,    KC_2,    KC_3,    KC_4,    KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,
        KC_F11,  KC_F12,  _______, _______, _______,      _______, _______, _______, _______, _______,
                                   _______, _______,      _______, _______
    ),

    // QK_BOOT and QK_CLEAR_EEPROM sit on the outer top corners so they are
    // hard to hit by accident.
    [SYM] = LAYOUT(
        QK_BOOT, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,       KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, QK_CLEAR_EEPROM,
        KC_GRV,  KC_QUOT, KC_DQUO, KC_MINS, KC_EQL,       KC_LPRN, KC_RPRN, KC_LBRC, KC_RBRC, KC_BSLS,
        KC_TILD, KC_PIPE, KC_UNDS, KC_PLUS, _______,      KC_LCBR, KC_RCBR, KC_LT,   KC_GT,   KC_QUES,
                                   _______, _______,      _______, _______
    )
};
