// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define TAPPING_TERM 200
#define PERMISSIVE_HOLD

// Home-row mods only hold when the next key is on the other hand.
#define CHORDAL_HOLD
// Home-row mods pressed mid-typing settle as taps right away.
#define FLOW_TAP_TERM 150
// Apply the mod on keydown so Option/Cmd + mouse click needs no wait.
#define SPECULATIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY

#define COMBO_TERM 40
#define COMBO_SHOULD_TRIGGER
// Combos are blocked if a key was pressed less than this many ms ago, so
// rolling "we" while typing never fires Esc.
#define COMBO_IDLE_MS 100
