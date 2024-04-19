Import-Module -Name "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1" -Force -NoClobber

$productInfoFilename = "$PSScriptRoot/ProductInfo.json"
$json = [System.IO.File]::ReadAllText($productInfoFilename)
$productInfo = ConvertFrom-Json -InputObject $json
$projectVariablePrefix = "__INSANEEM_PROJECT_$($productInfo.Name.ToUpper())"
Set-GlobalVariable -Name "$($projectVariablePrefix)_TEMP_DIR" -Value "$(Get-UserHome)/.Temp_$($productInfo.Name)"
Set-GlobalVariable -Name "$($projectVariablePrefix)_CLOSURE_COMPILER_URL" -Value "https://repo1.maven.org/maven2/com/google/javascript/closure-compiler/v20230802/closure-compiler-v20230802.jar"
Set-GlobalVariable -Name "$($projectVariablePrefix)_CLOSURE_COMPILER_HASH" -Value "D1A09A24CE1056943E01134A3D5A407D58B5E704"
Set-GlobalVariable -Name "$($projectVariablePrefix)_INSANE_VERSION" -Value "$($productInfo.InsaneVersion)"
