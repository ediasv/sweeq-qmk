// Copyright 2023 Idan Kamara (@idank)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// USB always goes to the right half; handedness is inferred from that.
#define MASTER_RIGHT

#define USB_VBUS_PIN GP19

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U
