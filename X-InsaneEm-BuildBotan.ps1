$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
        Write-Host
        Write-InfoBlue "████ Building libBotan"
        Write-Host
        & "$X_INSANE_EM_TEST_REQUIRED_TOOLS_SCRIPT"  
        & "$X_INSANE_EM_UPDATE_SUBMODULES_SCRIPT"

        $json = [System.IO.File]::ReadAllText($(Get-Item "$DOCS_PRODUCT_INFO_JSON"))
        $productInfo = ConvertFrom-Json $json
        $BotanVersion = $productInfo.BotanVersion
        & "$X_INSANE_EM_PS_BOTAN_SCRIPT" -Build -EmscriptenCompiler -DestinationSuffix "$INSANE_EM_BOTAN_SUFFIX" -Version $BotanVersion -BotanModules @() -BotanOptions @()
        & "$X_INSANE_EM_PS_BOTAN_INTERNAL_SCRIPT" -Build -EmscriptenCompiler -DestinationSuffix "$INSANE_EM_BOTAN_SUFFIX" -Version $BotanVersion -BotanModules @() -BotanOptions @() -ReleaseMode
}
finally {
        Write-InfoBlue "████ End - Building libBotan"
        Write-Host
}




