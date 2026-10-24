"""Host-side checks of the W+E Esc guard (esc_guard.h) with the system C compiler.

Run: python3 -m unittest discover -s tests -v
"""
import subprocess
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
KEYMAP_DIR = REPO / "keyboards/holykeebs/sweeq/keymaps/ediasv"


def run(body):
    """Compile and run `body` inside main(); return what it prints."""
    src = f"""
#include <stdio.h>
#include "esc_guard.h"
int main(void) {{ {body} return 0; }}
"""
    with tempfile.TemporaryDirectory() as tmp:
        c, exe = Path(tmp, "t.c"), Path(tmp, "t")
        c.write_text(src)
        subprocess.run(["cc", "-std=c11", "-Wall", "-Werror", "-I", str(KEYMAP_DIR),
                        "-o", str(exe), str(c)], check=True, capture_output=True, text=True)
        return subprocess.run([str(exe)], check=True, capture_output=True, text=True).stdout


class EscGuardTest(unittest.TestCase):
    def test_space_still_held_when_we_rolls_blocks_esc(self):
        # " we": Space pressed 30 ms before W, not yet settled as tap or hold.
        out = run('esc_guard_press(1000); esc_guard_press(1150); esc_guard_press(1180);'
                  'printf("%d", esc_guard_allows(1180, true));')
        self.assertEqual(out, "0")

    def test_fast_roll_into_we_blocks_esc(self):
        out = run('esc_guard_press(1000); esc_guard_press(1060);'
                  'printf("%d", esc_guard_allows(1060, true));')
        self.assertEqual(out, "0")

    def test_pause_before_we_allows_esc(self):
        out = run('esc_guard_press(1000); esc_guard_press(1100);'
                  'printf("%d", esc_guard_allows(1100, true));')
        self.assertEqual(out, "1")

    def test_second_key_of_admitted_chord_allows_esc(self):
        out = run('esc_guard_press(1000); esc_guard_press(1300); esc_guard_press(1320);'
                  'printf("%d", esc_guard_allows(1320, false));')
        self.assertEqual(out, "1")

    def test_timer_wraparound_allows_esc(self):
        # 16-bit event time wraps; 136 ms really passed.
        out = run('esc_guard_press(65500); esc_guard_press(100);'
                  'printf("%d", esc_guard_allows(100, true));')
        self.assertEqual(out, "1")


if __name__ == "__main__":
    unittest.main()
