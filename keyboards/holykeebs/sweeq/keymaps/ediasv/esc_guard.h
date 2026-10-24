// SPDX-License-Identifier: GPL-2.0-or-later

// Fast-typing guard for the W+E Esc combo. Plain C (no QMK headers) so
// tests/test_esc_guard.py can compile it on the host.

#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifndef COMBO_IDLE_MS
#    define COMBO_IDLE_MS 100
#endif

static uint16_t esc_guard_last, esc_guard_prev;

// Call on every raw key press, before tap-hold keys settle, with the
// press's own timestamp.
static inline void esc_guard_press(uint16_t time) {
    esc_guard_prev = esc_guard_last;
    esc_guard_last = time;
}

// The chord's first key is allowed only if the press before it was at least
// COMBO_IDLE_MS earlier; later keys of an admitted chord always pass.
static inline bool esc_guard_allows(uint16_t time, bool first_key) {
    return !first_key || (uint16_t)(time - esc_guard_prev) >= COMBO_IDLE_MS;
}
