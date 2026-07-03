# Universal Totem + Corne Dongle Handoff

## Goal

Build one dongle image that remains paired to both the Totem and Corne XIAO Rev2, so changing keyboards does not require reflashing. Keep the existing Totem and Corne projects and rollback UF2 files unchanged.

The first practical version can assume only one keyboard is powered at a time. Supporting both keyboards concurrently would require explicit event-source filtering or an active-board selector.

## Recommended structure

Create a third sibling project, `zmk-config-universal-dongle`, using the same pinned ZMK revision and Docker-only workflow as `zmk-config-corne-xiao`.

Copy into it:

- The current 44-position Corne keymap and six-layer behavior snapshot. It already contains the 38 shared Totem positions plus the six Corne-only keys.
- The dongle shield, D0 PC-layer toggle, and D1 SYM-layer toggle.
- The corrected OLED shield and custom dashboard.
- The Hyper Z/X/C/V actions, navigation morphs, combos, Studio support, and PC-mode behavior.

Do not make the universal dongle a source dependency of either keyboard project yet. Duplication is preferable for this experiment because each working firmware set remains independently reproducible.

## Firmware changes

1. Set `CONFIG_ZMK_SPLIT_BLE_CENTRAL_PERIPHERALS=4` in the universal dongle shield.
2. Confirm `BT_MAX_CONN` and `BT_MAX_PAIRED` leave room for four split peripherals plus the USB/Bluetooth host profiles. The Totem dongle currently uses `7`; retain or increase it only if Kconfig/build output proves necessary.
3. Use the same split transport identity and compatible ZMK revision for all four peripheral images.
4. Verify both boards report the intended logical key positions. Shared Totem keys should land on the same bindings in the 44-position keymap; Corne-only positions remain unused by Totem.
5. Expand the OLED battery model from two sources to four and label them `TL`, `TR`, `CL`, and `CR`. Do not assume reconnect order equals board identity without testing the stored split source IDs.
6. Build OLED and non-OLED universal dongle UF2 files plus a settings-reset image with Docker. Keep the existing Totem and Corne dongle UF2 files as rollback images.

## Main proof-of-concept risk

Stock ZMK supports multiple central peripherals, but the important question is how the central assigns and restores each peripheral's source ID. Before polishing the OLED, prove that four bonds remain stable across power cycles and that Totem and Corne events map to the correct logical positions.

If both keyboards are connected simultaneously, the simple implementation will accept input from both. The low-complexity operating rule is therefore: turn off the inactive keyboard. A later version can add a dongle control that filters events to the selected pair.

## Pairing procedure

1. Flash settings reset to the dongle and all four halves.
2. Flash the universal dongle.
3. Flash compatible peripheral images to each half.
4. Pair in a fixed documented order: Totem left, Totem right, Corne left, Corne right.
5. Power-cycle everything and confirm each bond reconnects in the same source slot.
6. Repeat with only Totem powered, then only Corne powered.

Resetting all devices is expected to remove the existing bonds. A normal board switch after initial setup must require only powering the desired keyboard, not reflashing or resetting.

## Verification checklist

- All 38 Totem keys produce the existing Totem behavior.
- All 44 Corne keys and both encoders work.
- Home-row mod/layer taps retain the 200 ms balanced behavior.
- NUM, SYM, ADJ, NAV, PC mode, combos, and Hyper/AHK actions work from both boards.
- D0 and D1 operate identically regardless of the connected board.
- Four split bonds survive dongle and keyboard power cycles.
- OLED identifies the active board and shows the correct half batteries.
- Sleeping one board does not prevent the other board from connecting.
- Studio unlock and editing do not corrupt the four-peripheral mapping.
- Reflashing the saved Totem or Corne dongle image restores the current single-board setup.

## Suggested first milestone

Build a non-OLED four-peripheral dongle first. Prove pairing, reconnection, and key-position routing with both keyboards. Once that is stable, add the four-battery OLED dashboard; this keeps display work from obscuring split-routing failures.
