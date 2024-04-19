[CmdletBinding(DefaultParameterSetName = "None")]
param (
    [Parameter(ParameterSetName = "Emrun")]
    [switch]
    $Emrun,

    [Parameter(ParameterSetName = "BrowserNode")]
    [switch]
    $BrowserNode,

    [Parameter(ParameterSetName = "BrowserDeno")]
    [switch]
    $BrowserDeno,

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
Import-Module -Name "$PSScriptRoot/Z-Init.ps1" -Force -NoClobber

function Test-RequirementsEmrun {
    Assert-Executable -ExeName "$env:EMSCRIPTEN_EMRUN" -Parameters @("--list_browsers")
}

function Test-RequirementsNode {
    Assert-NodeExecutable
}

function Test-RequirementsDeno {
    Assert-DenoExecutable
}

try {

    $json = [System.IO.File]::ReadAllText("$PSScriptRoot/ProductInfo.json")
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
    
    $nodeScript = "$PSScriptRoot/index.mjs"
    $denoScript = "$PSScriptRoot/index.ts"

    Write-Host
    Write-InfoDarkGreen "████ Running - Module: `"$ModuleExportName, Version: $ModuleVersion`""
    Write-Host

    $serverDir = "$PSScriptRoot/Server"
    $denoServer = "$serverDir/DenoHttpServer.ts"
    $nodeServer = "$serverDir/NodeHttpServer.mjs"
    $indexHtml = "$PSScriptRoot/index.html"

    $debugModule = "$PSScriptRoot/Bin/Debug/Module/$ModuleExportName-$ModuleVersion-Debug.$ModuleExportExtension"
    $releaseModule = "$PSScriptRoot/Bin/Release/Module/$ModuleExportName-$ModuleVersion-Release.$ModuleExportExtension"
    if((!(Test-Path -Path "$debugModule" -PathType Leaf)) -and (!(Test-Path -Path "$releaseModule" -PathType Leaf))){
        Write-Warning """$ModuleExportExtension"" file is required. Build this project and run again."
        exit
    }
    
    $launch = !($NoLaunchBrowser.IsPresent)
    
    if ($BrowserNode.IsPresent) {
        Test-RequirementsNode 
        try {
            Push-Location $serverDir
            npm update
            Write-InfoYellow "Press Ctrl+C to exit!"
            node "$nodeServer" $NodeHttpServerPort "$launch" "$PSScriptRoot"
        }
        finally {
            Pop-Location
        }
    }

    if ($BrowserDeno.IsPresent) {
        Test-RequirementsDeno
        try {
            Push-Location $serverDir
            Write-InfoYellow "Press Ctrl+C to exit!"
            deno run --allow-run --allow-net --allow-read "$denoServer" $DenoHttpServerPort "$launch" "$PSScriptRoot"
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
        & node $ConsoleNodeOptions $nodeScript
    }

    if ($ConsoleDeno.IsPresent) {
        Test-RequirementsDeno
        if (!$ModuleIsES6Module) {
            Write-Warning "ES6 Module(mjs) is required to run on Deno."
            return
        }
        & deno run $ConsoleDenoOptions $denoScript
    }
    
    if ($Emrun.IsPresent -or ($PSCmdlet.ParameterSetName -eq "None")) {
        Install-EmscriptenSDK
        Test-RequirementsEmrun
        if ($ModuleIsES6Module) {
            Write-Warning "Emscripten Emrun is not compatible with ES6 Module(.$ModuleExportExtension extension)."
            return
        }
        Write-PrettyKeyValue "LaunchBrowser" $launch
        Write-PrettyKeyValue "Mode" (Get-VariableName $Emrun)
        Write-InfoYellow "Press Ctrl+C to exit!"
        $browser = $launch ?  [string]::Empty : "--no_browser"
        & $env:EMSCRIPTEN_EMRUN "$indexHtml" $browser --port $EmrunHttpServerPort --private_browsing
    }
}
finally {
    Write-InfoDarkGreen "█ End running - Module `"$ModuleExportName, Version: $ModuleVersion`""
}
