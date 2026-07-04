param(
    [switch]$Refresh
)

$ErrorActionPreference = "Stop"

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$outputDir = Join-Path $repoRoot "local-firmware"
$volumeName = "zmk-corne-xiao-workspace"
$image = "zmkfirmware/zmk-build-arm:stable"

New-Item -ItemType Directory -Force -Path $outputDir | Out-Null

$refreshValue = if ($Refresh) { "1" } else { "0" }

$dockerArgs = @(
    "run", "--rm",
    "--mount", "type=volume,src=$volumeName,dst=/workspace",
    "--mount", "type=bind,src=$repoRoot,dst=/config-repo,readonly",
    "--mount", "type=bind,src=$outputDir,dst=/output",
    "--env", "REFRESH=$refreshValue",
    "--workdir", "/workspace",
    $image,
    "bash", "-c", "sed 's/\r$//' /config-repo/scripts/build-corne-container.sh | bash"
)

& docker @dockerArgs
if ($LASTEXITCODE -ne 0) {
    throw "Docker ZMK build failed with exit code $LASTEXITCODE."
}

$artifacts = @(
    "corne_xiao_v2_left-zmk.uf2",
    "corne_xiao_v2_right-zmk.uf2",
    "corne_xiao_dongle_oled-zmk.uf2",
    "settings_reset-xiao_ble-zmk.uf2"
)

foreach ($artifact in $artifacts) {
    $path = Join-Path $outputDir $artifact
    if (-not (Test-Path -LiteralPath $path)) {
        throw "Expected artifact was not produced: $path"
    }

    $hash = Get-FileHash -Algorithm SHA256 -LiteralPath $path
    Write-Host "Built: $path"
    Write-Host "SHA256: $($hash.Hash)"
}
