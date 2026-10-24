"""Check every layer in keymap.c against the approved design.

Run: python3 -m unittest discover -s tests -v
Spec: docs/superpowers/specs/2026-10-24-sweeq-keymap-design.md
"""
import json
import subprocess
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parent.parent
KEYMAP = REPO / "keyboards/holykeebs/sweeq/keymaps/ediasv/keymap.c"

# c2json prints XXXXXXX as KC_NO and _______ as KC_TRNS.
N, T = "KC_NO", "KC_TRNS"

# Per layer: 3 rows of 5 left + 5 right, then thumbs
# left-outer, left-inner, right-inner, right-outer.
BASE = ["KC_Q", "KC_W", "KC_E", "KC_R", "KC_T", "KC_Y", "KC_U", "KC_I", "KC_O", "KC_P",
        "LCTL_T(KC_A)", "LALT_T(KC_S)", "LGUI_T(KC_D)", "LSFT_T(KC_F)", "KC_G",
        "KC_H", "RSFT_T(KC_J)", "RGUI_T(KC_K)", "RALT_T(KC_L)", "RCTL_T(KC_SCLN)",
        "KC_Z", "KC_X", "KC_C", "KC_V", "KC_B", "KC_N", "KC_M", "KC_COMM", "KC_DOT", "KC_SLSH",
        "LT(NUM,KC_TAB)", "LT(NAV,KC_BSPC)", "LT(SYM,KC_SPC)", "LT(SYS,KC_ENT)"]
NAV = [N, N, N, N, N, "KC_EQL", "KC_MINS", "KC_PLUS", "KC_UNDS", N,
       "KC_LCTL", "KC_LALT", "KC_LGUI", "KC_LSFT", N,
       "KC_LEFT", "KC_DOWN", "KC_UP", "KC_RGHT", "KC_COLN",
       N, N, N, N, N, "KC_QUES", "KC_EXLM", "KC_AMPR", "KC_PIPE", N,
       N, T, N, N]
SYM = ["KC_EXLM", "KC_AT", "KC_HASH", "KC_DLR", "KC_PERC", "KC_CIRC", "KC_AMPR", "KC_ASTR", N, N,
       "KC_LT", "KC_LBRC", "KC_LCBR", "KC_LPRN", "KC_DQUO",
       "KC_QUOT", "KC_RPRN", "KC_RCBR", "KC_RBRC", "KC_GT",
       "KC_GRV", "KC_TILD", "KC_BSLS", "KC_PIPE", N, N, N, N, N, N,
       N, N, T, N]
NUM = [N, N, N, N, N, "KC_ASTR", "KC_7", "KC_8", "KC_9", "KC_PLUS",
       "KC_LCTL", "KC_LALT", "KC_LGUI", "KC_LSFT", N, "KC_SLSH", "KC_4", "KC_5", "KC_6", "KC_MINS",
       N, N, N, N, N, "KC_DOT", "KC_1", "KC_2", "KC_3", "KC_EQL",
       T, N, "KC_0", "KC_COMM"]
SYS = ["KC_F7", "KC_F8", "KC_F9", "KC_F12", N,
       "LGUI(LSFT(KC_3))", "LGUI(LSFT(KC_4))", "LGUI(LSFT(KC_5))", N, "QK_BOOT",
       "KC_F4", "KC_F5", "KC_F6", "KC_F11", N, "KC_MPRV", "KC_VOLD", "KC_VOLU", "KC_MNXT", N,
       "KC_F1", "KC_F2", "KC_F3", "KC_F10", N, "KC_MUTE", "KC_MPLY", "KC_BRID", "KC_BRIU", N,
       N, N, N, T]

EXPECTED = {"base": BASE, "nav": NAV, "sym": SYM, "num": NUM, "sys": SYS}


def load_layers():
    out = subprocess.run(
        ["qmk", "c2json", "-kb", "holykeebs/sweeq", "-km", "ediasv", str(KEYMAP)],
        check=True, capture_output=True, text=True,
    ).stdout
    return [["".join(kc.split()) for kc in layer] for layer in json.loads(out)["layers"]]


class LayoutTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.layers = load_layers()

    def test_layer_count(self):
        self.assertEqual(len(self.layers), len(EXPECTED))

    def check(self, index, name):
        self.assertGreater(len(self.layers), index, f"layer {name} missing")
        actual = self.layers[index]
        self.assertEqual(len(actual), 34, f"{name}: key count")
        for pos, want in enumerate(EXPECTED[name]):
            with self.subTest(key=f"{name}[{pos}]"):
                self.assertEqual(actual[pos], want)

    def test_base(self):
        self.check(0, "base")

    def test_nav(self):
        self.check(1, "nav")

    def test_sym(self):
        self.check(2, "sym")

    def test_num(self):
        self.check(3, "num")

    def test_sys(self):
        self.check(4, "sys")


if __name__ == "__main__":
    unittest.main()
