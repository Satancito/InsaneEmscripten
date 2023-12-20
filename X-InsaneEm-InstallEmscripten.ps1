$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$X_INSANE_EM_Z_PS_CORE_FXS_INTERNAL_SCRIPT")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

& $X_PS_EMSCRIPTEN_INTERNAL_SCRIPT -Install