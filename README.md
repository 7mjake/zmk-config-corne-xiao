# Corne XIAO Rev2 - Totem-parity test firmware

This local branch starts from `friction07/zmk-config-corne-xiao` commit
`7da635b97cfa0cb9c5fb765bef535bdc28d7b2ae` and adds a temporary dedicated
dongle build for the Rev2 Corne XIAO.

The Corne keymap is an independent 44-position snapshot of the current Totem
behavior. It does not share source files with the Totem checkout.

## Local Docker build

From PowerShell in this repository:

```powershell
.\scripts\build-corne.ps1
```

Artifacts and SHA-256 hashes are written under `local-firmware`. Use
`-Refresh` only when the ZMK manifest needs to be refreshed.

## Flash order

1. Save the current Totem dongle UF2 as the rollback image.
2. Flash `settings_reset-xiao_ble-zmk.uf2` to the Corne left half, Corne right
   half, and shared dongle.
3. Flash `corne_xiao_v2_left-zmk.uf2` and
   `corne_xiao_v2_right-zmk.uf2` to their matching halves.
4. Flash `corne_xiao_dongle_oled-zmk.uf2` to the shared dongle.
5. Pair the left half first, then power and pair the right half so the OLED
   battery rows remain left-to-right.

The display-free `corne_xiao_dongle-zmk.uf2` is a fallback. Returning to the
Totem requires restoring its dongle firmware and re-pairing its halves because
the settings reset clears the dongle's existing split bonds.

## Charging status and PC encoder

The left and right half images report the XIAO's true active-low charger state
from P0.17. The OLED dongle draws a lightning bolt beside each half while its
BQ25101 charger is active. Updating this feature requires flashing both half
images and the OLED dongle image; it does not require settings reset or
re-pairing.

On the PC base layer, clockwise right-encoder rotation sends F24 and
counterclockwise rotation sends F23. The Mac and other layer bindings retain
their existing brightness behavior.
