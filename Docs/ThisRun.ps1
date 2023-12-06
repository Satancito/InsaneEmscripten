[CmdletBinding(DefaultParameterSetName = "None")]
param (
    [Parameter(ParameterSetName = "Emrun")]
    [switch]
    $Emrun,

    [Parameter(ParameterSetName = "BrowserNode")]
    [switch]
    $BrowserNodeServer,

    [Parameter(ParameterSetName = "BrowserDeno")]
    [switch]
    $BrowserDenoServer,

    [Parameter(ParameterSetName = "ConsoleNode")]
    [switch]
    $ConsoleNode,

    [Parameter(ParameterSetName = "ConsoleDeno")]
    [switch]
    $ConsoleDeno,

    [Parameter(ParameterSetName = "None")]
    [Parameter(ParameterSetName = "Emrun")]
    [Parameter(ParameterSetName = "BrowserNode")]
    [Parameter(ParameterSetName = "BrowserDeno")]
    [switch]
    $NoLaunchBrowser
)

$ErrorActionPreference = "Stop"


try {
    Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs*.ps1")" -Force -NoClobber
    Write-InfoDarkGray "▶️▶️▶️ Running: $PSCommandPath"

    $json = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
    $productInfo = ConvertFrom-Json $json
    $ModuleExportName = $productInfo.Name
    $ModuleVersion = $productInfo.Version
    $ModuleIsES6Module = $productInfo.IsES6Module
    $ModuleExportExtension = $ModuleIsES6Module ? "mjs": "js"
    $DenoHttpServerPort = $productInfo.DenoHttpServerPort
    $NodeHttpServerPort = $productInfo.NodeHttpServerPort
    $EmrunHttpServerPort = $productInfo.EmrunHttpServerPort
    $ConsoleNodeOptions = $productInfo.ConsoleNodeOptions
    $ConsoleDenoOptions = $productInfo.ConsoleDenoOptions
    $NODE_SCRIPT = "$PSScriptRoot/index.mjs"
    $DENO_SCRIPT = "$PSScriptRoot/index.ts"

    Write-Host
    Write-InfoDarkGreen "████ Run - Module: ""$ModuleExportName.$ModuleExportExtension"", Version: $ModuleVersion"
    Write-Host
    $SERVER_DIR = "$PSScriptRoot/Server"
    
    $launch = !($NoLaunchBrowser.IsPresent)
    
    if ($BrowserNodeServer.IsPresent) {
        Write-PrettyKeyValue "LaunchBrowser" $launch
        Write-PrettyKeyValue "Mode" (Get-VariableName $BrowserNodeServer)    
        try {
            Push-Location $SERVER_DIR
            npm update 
            node "$SERVER_DIR/NodeHttpServer.mjs" "$NodeHttpServerPort" "$launch" "$PSScriptRoot"
        }
        finally {
            Pop-Location
        }
    }

    if ($BrowserDenoServer.IsPresent) {
        Write-PrettyKeyValue "LaunchBrowser" $launch
        Write-PrettyKeyValue "Mode" (Get-VariableName $BrowserDenoServer)
        try {
            Push-Location $SERVER_DIR
            deno run --allow-run --allow-net --allow-read "$SERVER_DIR/DenoHttpServer.ts" "$DenoHttpServerPort" "$launch" "$PSScriptRoot"
            return
        }
        finally {
            Pop-Location
        }
    }

    if ($ConsoleNode.IsPresent) {
        if (!$ModuleIsES6Module) {
            Write-Warning "ES6 Module(mjs) is required to run on Node."
            return
        }
        & node $ConsoleNodeOptions $NODE_SCRIPT
    }

    if ($ConsoleDeno.IsPresent) {
        if (!$ModuleIsES6Module) {
            Write-Warning "ES6 Module(mjs) is required to run on Deno."
            return
        }
        & deno run $ConsoleDenoOptions $DENO_SCRIPT
    }
    
    if ($Emrun.IsPresent -or ($PSCmdlet.ParameterSetName -eq "None")) {
        if ($ModuleIsES6Module) {
            Write-Warning "Emscripten Emrun is not compatible with ES6 Module($ModuleExportName.$ModuleExportExtension)."
            return
        }
        Write-PrettyKeyValue "LaunchBrowser" $launch
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