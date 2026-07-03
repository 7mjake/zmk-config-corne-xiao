#!/usr/bin/env bash
set -euo pipefail

mkdir -p /workspace/config /output
cp -f /config-repo/config/west.yml /workspace/config/west.yml

if [[ ! -d /workspace/.west ]]; then
    echo "Initializing cached ZMK workspace..."
    west init -l /workspace/config
    west update --fetch-opt=--filter=tree:0
elif [[ "${REFRESH:-0}" == "1" ]]; then
    echo "Refreshing ZMK modules..."
    west update --fetch-opt=--filter=tree:0
fi

# Docker containers are ephemeral, so CMake's per-user Zephyr package
# registry must be restored even when the West workspace volume is cached.
west zephyr-export

build_target() {
    local build_dir="$1"
    local shield="$2"
    local artifact="$3"
    local snippet="$4"
    shift 4

    local west_args=(
        -p auto
        -s zmk/app
        -d "build/${build_dir}"
        -b "xiao_ble//zmk"
    )

    if [[ -n "${snippet}" ]]; then
        west_args+=(-S "${snippet}")
    fi

    west build "${west_args[@]}" \
        -- \
        -DZMK_CONFIG=/config-repo/config \
        -DZMK_EXTRA_MODULES=/config-repo \
        -DSHIELD="${shield}" \
        "$@"

    cp -f "build/${build_dir}/zephyr/zmk.uf2" "/output/${artifact}"
}

build_target \
    corne_xiao_v2_left \
    corne_xiao_v2_left \
    corne_xiao_v2_left-zmk.uf2 \
    "" \
    -DCONFIG_ZMK_SPLIT=y \
    -DCONFIG_ZMK_SPLIT_ROLE_CENTRAL=n

build_target \
    corne_xiao_v2_right \
    corne_xiao_v2_right \
    corne_xiao_v2_right-zmk.uf2 \
    "" \
    -DCONFIG_ZMK_SPLIT=y \
    -DCONFIG_ZMK_SPLIT_ROLE_CENTRAL=n

build_target \
    corne_xiao_dongle \
    corne_xiao_dongle \
    corne_xiao_dongle-zmk.uf2 \
    studio-rpc-usb-uart \
    -DCONFIG_ZMK_STUDIO=y

build_target \
    corne_xiao_dongle_oled \
    "corne_xiao_dongle_oled corne_xiao_oled_yads" \
    corne_xiao_dongle_oled-zmk.uf2 \
    studio-rpc-usb-uart \
    -DCONFIG_ZMK_STUDIO=y

build_target \
    settings_reset_xiao_ble \
    settings_reset \
    settings_reset-xiao_ble-zmk.uf2 \
    ""
