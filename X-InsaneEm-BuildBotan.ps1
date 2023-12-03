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
    
    $PS_BOTAN_REPO_DIR = "$INSANE_EM_DEFAULT_TEMP_DIR/PsBotan"

    Install-GitRepository -Url "$PS_BOTAN_REPO_URL" -Path "$PS_BOTAN_REPO_DIR" -Force

    $PS_BOTAN_SCRIPT = "$PS_BOTAN_REPO_DIR/X-PsBotan.ps1"

    $json = [System.IO.File]::ReadAllText($(Get-Item "$DOCS_PRODUCT_INFO_JSON"))
    $productInfo = ConvertFrom-Json $json
    $BotanVersion = $productInfo.BotanVersion
    & "$PS_BOTAN_SCRIPT" -Build -EmscriptenCompiler -Version $BotanVersion -BotanModules @() -BotanOptions @()
    & "$PS_BOTAN_SCRIPT" -Build -EmscriptenCompiler -Version $BotanVersion -ReleaseMode -BotanModules @() -BotanOptions @()
}
finally {
        Write-InfoBlue "████ End - Building libBotan"
        Write-Host
}




