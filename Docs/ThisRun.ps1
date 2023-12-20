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
    
$Error.Clear()  
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

function Test-RequirementsEmrun {
    Write-Host
    Write-InfoBlue "Test - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Emrun"
    $command = Get-Command "$env:EMSCRIPTEN_EMRUN"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --list_browsers
    Write-Host
}

function Test-RequirementsNode {
    Write-Host
    Write-InfoBlue "Test - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Node"
    $command = Get-Command "node"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host
}

function Test-RequirementsDeno {
    Write-Host
    Write-InfoBlue "Test - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Deno"
    $command = Get-Command "deno"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host
}

try {

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
    $ModuleFileName = "$ModuleExportName-*-$ModuleVersion-*.$ModuleExportExtension"

    $COMPILED_MODULE_DIR = "$PSScriptRoot/Build/Module"
    $COMPILED_MODULE_FILE = Get-Item "$COMPILED_MODULE_DIR/$ModuleFileName" -ErrorAction SilentlyContinue

    Write-Host
    Write-InfoDarkGreen "████ Running - Module: ""$COMPILED_MODULE_FILE"""
    Write-Host

    if ($null -eq $COMPILED_MODULE_FILE) {
        Write-Warning """$ModuleExportExtension"" file is required. Build this project and run again."
        return
    }

    $SERVER_DIR = "$PSScriptRoot/Server"
    
    $launch = !($NoLaunchBrowser.IsPresent)
    
    if ($BrowserNodeServer.IsPresent) {
        Test-RequirementsNode 
        try {
            Push-Location $SERVER_DIR
            npm update
            Write-InfoYellow "Press Ctrl+C to exit!"
            node "$SERVER_DIR/NodeHttpServer.mjs" "$NodeHttpServerPort" "$launch" "$PSScriptRoot"
        }
        finally {
            Pop-Location
        }
    }

    if ($BrowserDenoServer.IsPresent) {
        Test-RequirementsDeno
        try {
            Push-Location $SERVER_DIR
            Write-InfoYellow "Press Ctrl+C to exit!"
            deno run --allow-run --allow-net --allow-read "$SERVER_DIR/DenoHttpServer.ts" "$DenoHttpServerPort" "$launch" "$PSScriptRoot"
            return
        }
        finally {
            Pop-Location
        }
    }

    if ($ConsoleNode.IsPresent) {
        Test-RequirementsNode
        if (!$ModuleIsES6Module) {
            Write-Warning "ES6 Module(mjs) is required to run on Node."
            return
        }
        & node $ConsoleNodeOptions $NODE_SCRIPT
    }

    if ($ConsoleDeno.IsPresent) {
        Test-RequirementsDeno
        if (!$ModuleIsES6Module) {
            Write-Warning "ES6 Module(mjs) is required to run on Deno."
            return
        }
        & deno run $ConsoleDenoOptions $DENO_SCRIPT
    }
    
    if ($Emrun.IsPresent -or ($PSCmdlet.ParameterSetName -eq "None")) {
        Test-RequirementsEmrun
        if ($ModuleIsES6Module) {
            Write-Warning "Emscripten Emrun is not compatible with ES6 Module($ModuleExportName.$ModuleExportExtension)."
            return
        }
        Write-PrettyKeyValue "LaunchBrowser" $launch
        Write-PrettyKeyValue "Mode" (Get-VariableName $Emrun)
        & "$PSScriptRoot/X-InsaneEm-SetEmscriptenEnvVars.ps1"
        Write-InfoYellow "Press Ctrl+C to exit!"
        $browser = $launch ?  [string]::Empty : "--no_browser"
        & $env:EMSCRIPTEN_EMRUN "$PSScriptRoot/index.html" $browser --port $EmrunHttpServerPort --private_browsing
    }
}
finally {
    Write-InfoDarkGreen "█ End running - Module ""$COMPILED_MODULE_FILE"""
}
