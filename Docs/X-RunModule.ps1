[CmdletBinding(DefaultParameterSetName = "None")]
param (
    [Parameter(ParameterSetName = "Emrun", Mandatory = $false)]
    [switch]
    $Emrun,

    [Parameter(ParameterSetName = "Node", Mandatory = $false)]
    [switch]
    $NodeBrowser,

    [Parameter(ParameterSetName = "Deno", Mandatory = $false)]
    [switch]
    $DenoBrowser,

    [Parameter()]
    [switch]
    $NoLaunchBrowser
)

$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶️▶️▶️ Running: $PSCommandPath"

$json = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name
$ModuleVersion = $productInfo.Version
$DenoHttpServerPort = $productInfo.DenoHttpServerPort
$NodeHttpServerPort = $productInfo.NodeHttpServerPort
$EmrunHttpServerPort = $productInfo.EmrunHttpServerPort

Write-Host
Write-InfoDarkGreen "████ Run - Module: ""$ModuleExportName.js"", Version: $ModuleVersion"
Write-Host

try {
    $SERVER_DIR = "$PSScriptRoot/Server"
    
    $launch = !($NoLaunchBrowser.IsPresent)
    Write-PrettyKeyValue "LaunchBrowser" $launch
    if ($NodeBrowser.IsPresent) {
        Write-PrettyKeyValue "Mode" (Get-VariableName $NodeBrowser)    
        Write-PrettyKeyValue "ServerPort" $NodeHttpServerPort
        try {
            Push-Location $SERVER_DIR
            npm update 
            node "$SERVER_DIR/NodeHttpServer.mjs" "$NodeHttpServerPort" "$launch" "$PSScriptRoot"
        }
        finally {
            Pop-Location
        }
    }

    if ($DenoBrowser.IsPresent) {
        Write-PrettyKeyValue "Mode" (Get-VariableName $DenoBrowser)
        try {
            Push-Location $SERVER_DIR
            deno run --allow-run --allow-net --allow-read "$SERVER_DIR/DenoHttpServer.ts" "$DenoHttpServerPort" "$launch" "$PSScriptRoot"
            return
        }
        finally {
            Pop-Location
        }
    }

    if ($Emrun.IsPresent -or ($PSCmdlet.ParameterSetName -eq "None")) {
        Write-PrettyKeyValue "Mode" (Get-VariableName $Emrun)
        & "$PSScriptRoot/X-InsaneEm-SetEmscriptenEnvVars.ps1"
        Write-InfoYellow "Press Ctrl+C to exit!"
        $browser = $launch ?  [string]::Empty : "--no_browser"
        & $env:EMSCRIPTEN_EMRUN "$PSScriptRoot/index.html" $browser --port $EmrunHttpServerPort
    }
}
finally {
    Write-InfoDarkGreen "█ End run - Module - Test client"
}
