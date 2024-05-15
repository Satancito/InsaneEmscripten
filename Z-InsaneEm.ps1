$ErrorActionPreference = "Stop"
Import-Module -Name "$PSScriptRoot/submodules/PsCoreFxs/Z-PsCoreFxs.ps1" -Force -NoClobber
Import-Module -Name "$PSScriptRoot/submodules/PsBotan/Z-PsBotan.ps1" -Force -NoClobber

$__INSANEEM_GITHUB_URL = "https://github.com/Satancito/InsaneEmscripten.git"; $null = $__INSANEEM_GITHUB_URL
$__INSANEEM_X_PSBOTAN_EMSCRIPTEN_INTERNAL_SCRIPT = "$PSScriptRoot/submodules/PsBotan/X-PsBotan-Emscripten.ps1"; $null = $__INSANEEM_X_PSBOTAN_EMSCRIPTEN_INTERNAL_SCRIPT
$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX = "Insane"; $null = $__INSANEEM_INSANE_BOTAN_DIR_SUFFIX
$__INSANEEM_Z_INSANEEM_SCRIPT = "$PSCommandPath"; $null = $__INSANEEM_Z_INSANEEM_SCRIPT
$__INSANEEM_CLANG_FORMAT_FILE = "$PSScriptRoot/.clang-format"; $null = $__INSANEEM_CLANG_FORMAT_FILE
$__INSANEEM_TEMP_DIR = "$(Get-UserHome)/.InsaneEm"; $null = $__INSANEEM_TEMP_DIR

$__INSANEEM_INSANE_VERSION = "1.0.0" # Update on next version
$__INSANEEM_INSANE_DEBUG_DIST_DIR_NAME = "Insane-$__INSANEEM_INSANE_VERSION-Emscripten-Wasm-Debug"; $null = $__INSANEEM_INSANE_DEBUG_DIST_DIR_NAME
$__INSANEEM_INSANE_RELEASE_DIST_DIR_NAME = "Insane-$__INSANEEM_INSANE_VERSION-Emscripten-Wasm-Release"; $null = $__INSANEEM_INSANE_RELEASE_DIST_DIR_NAME

$__INSANEEM_INSANE_BUILD_CONFIGURATIONS = [ordered]@{
    $__PSCOREFXS_EMSCRIPTEN_CONFIGURATIONS.Wasm.NameDebug   = [ordered]@{
        Name              = $__PSCOREFXS_DEBUG_CONFIGURATION
        DistDirName       = "$__INSANEEM_INSANE_DEBUG_DIST_DIR_NAME"
        BotanLibDir       = "$(Get-CppLibsDir)/$($__PSBOTAN_EMSCRIPTEN_CONFIGURATIONS.Debug.DistDirName)-$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX"
        CurrentWorkingDir = "$__INSANEEM_TEMP_DIR/Bin/Debug"
    }
    $__PSCOREFXS_EMSCRIPTEN_CONFIGURATIONS.Wasm.NameRelease = [ordered]@{
        Name              = $__PSCOREFXS_RELEASE_CONFIGURATION
        DistDirName       = "$__INSANEEM_INSANE_RELEASE_DIST_DIR_NAME"
        BotanLibDir       = "$(Get-CppLibsDir)/$($__PSBOTAN_EMSCRIPTEN_CONFIGURATIONS.Release.DistDirName)-$__INSANEEM_INSANE_BOTAN_DIR_SUFFIX"
        CurrentWorkingDir = "$__INSANEEM_TEMP_DIR/Bin/Release"
    }
}; $null = $__INSANEEM_INSANE_BUILD_CONFIGURATIONS

function Test-InsaneDependencies {
    Write-Host
    Write-InfoBlue "Test - Insane - Dependency tools"
    Write-Host
    Assert-GitExecutable
    Assert-MakeExecutable
}

function Remove-InsaneEm {
    Write-InfoBlue "Removing InsaneEm"
    Write-Host "$__INSANEEM_TEMP_DIR"
    Remove-Item -Path "$__INSANEEM_TEMP_DIR" -Force -Recurse -ErrorAction Ignore
}

function Build-BotanLibraryForInsane {
    [CmdletBinding()]
    param (
        [string]
        $DestinationDir = [string]::Empty
    )
    $DestinationDir = [string]::IsNullOrWhiteSpace($DestinationDir) ? "$(Get-CppLibsDir)" : $DestinationDir
    & "$PSScriptRoot/submodules/PsBotan/X-PsBotan-Emscripten.ps1" -Clean
    & "$PSScriptRoot/submodules/PsBotan/X-PsBotan-Emscripten.ps1" -DistDirSuffix "Insane" -DestinationDir $DestinationDir
}

function Build-IcuLibrary {
    Install-EmscriptenSDK
    Write-InfoBlue "████ Enabling ICU "
    $objDir = "$__INSANEEM_TEMP_DIR/Bin/Obj"
    try {
        New-Item -Path "$objDir" -ItemType Directory -Force | Out-Null
        & "$($env:EMSCRIPTEN_COMPILER)" -c "$PSScriptRoot/Source/IcuInput.cpp" -o "$objDir/IcuInput.d.o" -s USE_ICU=1 -O0 -g
        & "$($env:EMSCRIPTEN_COMPILER)" -c "$PSScriptRoot/Source/IcuInput.cpp" -o "$objDir/IcuInput.o" -s USE_ICU=1 -O3
    }
    finally {
        Remove-Item -Path "$objDir" -Force -Recurse -ErrorAction Ignore
    }
    Write-Host "Enabled."
}

function Build-InsaneLibrary {
    [CmdletBinding()]
    param (
        [Parameter(ParameterSetName = "Build_Lib")]
        [string]
        $DestinationDir = [string]::Empty,
    
        [Parameter(ParameterSetName = "Build_Lib")]
        [string]
        $DistDirSuffix = [string]::Empty,
    
        [Parameter(ParameterSetName = "Build_Lib")]
        [string]
        $LibSuffix = [string]::Empty,
    
        [Parameter(ParameterSetName = "Build_Lib")]
        [string]
        $LibsDir = [string]::Empty
    )
    $DestinationDir = [string]::IsNullOrWhiteSpace($DestinationDir) ? "$(Get-CppLibsDir)" : $DestinationDir
    $LibsDir = [string]::IsNullOrWhiteSpace($LibsDir) ? "$(Get-CppLibsDir)" : $LibsDir
    $DistDirSuffix = [string]::IsNullOrWhiteSpace($DistDirSuffix) ? [string]::Empty : "-$($DistDirSuffix)"
    
    Test-InsaneDependencies
    Install-EmscriptenSDK

    $__INSANEEM_INSANE_BUILD_CONFIGURATIONS.Keys | ForEach-Object {
        $configuration = $__INSANEEM_INSANE_BUILD_CONFIGURATIONS[$_]
        $botanConfiguration = $__PSBOTAN_EMSCRIPTEN_BUILD_CONFIGURATIONS[$_]

        $botanConfiguration.DistDirName = "$($botanConfiguration.DistDirName -f $AndroidAPI)-Insane"
        $botanConfiguration.DistDir = "$LibsDir/$($botanConfiguration.DistDirName)"

        if (!(Test-Path "$($botanConfiguration.DistDir)" -PathType Container)) {
            throw "Botan library path not found `"$($botanConfiguration.DistDir)`"."
        }

        $prefix = "$DestinationDir/$($configuration.DistDirName)$DistDirSuffix"
        try {
            Write-OutputIntroOutroMessage "████ InsaneEm - Building `"$prefix`"" -ForegroundColor Blue
            Write-Host
            New-Item -Path "$($configuration.CurrentWorkingDir)" -ItemType Directory -Force | Out-Null
            Push-Location "$($configuration.CurrentWorkingDir)"

            $env:CXX = $($env:EMSCRIPTEN_COMPILER) 
            $env:AR = $($env:EMSCRIPTEN_EMAR) 
            $env:SOURCES_DIR = $PSScriptRoot 
            $env:BUILD_DIR = $($configuration.CurrentWorkingDir) 
            $env:LIB_SUFFIX = $LibSuffix 
            $env:DIST_DIR = $prefix 
            $env:BUILD_CONFIGURATION = $($configuration.Name) 
            $env:BOTAN_MAJOR_VERSION = $__PSBOTAN_BOTAN_MAJOR_VERSION 
            $env:BOTAN_DIST_DIR = $($botanConfiguration.DistDir) 

            $null = Test-ExternalCommand "`"$env:EMSCRIPTEN_EMMAKE`" make -f $PSScriptRoot/Makefile All -j8" -ThrowOnFailure -NoAssertion
            $null = Test-ExternalCommand "`"$env:EMSCRIPTEN_EMMAKE`" make -f $PSScriptRoot/Makefile Install -j8" -ThrowOnFailure -NoAssertion
            Write-Host "Creating compiler database(`"$__PSCOREFXS_CLANGD_COMPILATION_DATABASE_JSON`")..."
            Join-CompileCommandsJson -SourceDir "$($configuration.CurrentWorkingDir)" -DestinationDir "$PSScriptRoot"
        }
        finally {
            Pop-Location
        }    
    }
}

function New-Project {
    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)]
        [string]
        $ProjectName
    )
        Write-Host
        Write-InfoBlue "████ Creating InsaneEm - New Project: " -NoNewLine
        Write-Host $ProjectName
        Write-Host

        Write-Host "Copying files..."

        $destinationDir = "$PSScriptRoot/Dist/$projectName"
        Remove-Item -Path "$destinationDir" -Force -Recurse -ErrorAction Ignore

        $filesToCopy = @(
            @{  Source = "$PSScriptRoot/Docs/*"; Destination = "$destinationDir"; DestinationItemType = "Directory" },
            @{  Source = "$PSScriptRoot/Server/*"; Destination = "$destinationDir/Server"; DestinationItemType = "Directory" },
            @{  Source = "$PSScriptRoot/Assets/*"; Destination = "$destinationDir/Assets"; DestinationItemType = "Directory" },
            @{  Source = "$PSScriptRoot/Src/*"; Destination = "$destinationDir/Source"; DestinationItemType = "Directory" },
            @{  Source = "$PSScriptRoot/Js/*"; Destination = "$destinationDir/Js"; DestinationItemType = "Directory" },
            @{  Source = "$PSScriptRoot/.clang-format"; Destination = "$destinationDir"; DestinationItemType = "Directory" }
            @{  Source = "$PSScriptRoot/T-UpdateSubmodules.ps1"; Destination = "$destinationDir"; DestinationItemType = "Directory" }
        )

        $filesToCopy | ForEach-Object {
            New-Item -Path $_.Destination -ItemType $_.DestinationItemType -Force | Out-Null
            Copy-Item -Path $_.Source -Destination $_.Destination -Force -Recurse    
        }
 
        Write-Host "Configuring product info..."
        $productInfoFilename = "$destinationDir/ProductInfo.json"
        $json = [System.IO.File]::ReadAllText($productInfoFilename)
        $productInfo = ConvertFrom-Json -InputObject $json
        $productInfo.Name = $ProjectName
        $productInfo.Version = "0.0.0"
        $productInfo.InsaneVersion = $__INSANEEM_INSANE_VERSION
        [System.IO.File]::WriteAllText("$productInfoFilename", "$(ConvertTo-Json -InputObject $productInfo)", [System.Text.Encoding]::UTF8)
        $productInfo

        Write-Host "Generating code..." 
        $indexHtmlFilename = "$destinationDir/index.html"
        $content = [System.IO.File]::ReadAllText("$indexHtmlFilename")

        $pattern = "<!-- _BEGIN_APP_TITLE_ -->[\s\S]*?<!-- _END___APP_TITLE_ -->"
        $replacement = "<title>$projectName - Emscripten - Tests</title>"
        $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

        $pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
        $replacement = "/*ModuleName*/$projectName."
        $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
        [System.IO.File]::WriteAllText("$indexHtmlFilename", $content, [System.Text.Encoding]::UTF8)

        $zInitFilename = "$destinationDir/Z-Init.ps1"
        $content = [System.IO.File]::ReadAllText("$zInitFilename")
        $content = $content.Replace("__INSANEEM_PROJECT_PROJECTNAME", "__INSANEEM_PROJECT_$($projectName.ToUpper())")
        [System.IO.File]::WriteAllText("$zInitFilename", $content, [System.Text.Encoding]::UTF8)

        $xBuildFilename = "$destinationDir/X-Build.ps1"
        $content = [System.IO.File]::ReadAllText("$xBuildFilename")
        $content = $content.Replace("__INSANEEM_PROJECT_PROJECTNAME", "__INSANEEM_PROJECT_$($projectName.ToUpper())")
        [System.IO.File]::WriteAllText("$xBuildFilename", $content, [System.Text.Encoding]::UTF8)

        Write-Host "Configuring git repo..."
        try {
            Push-Location -Path "$destinationDir"
            $null = Test-ExternalCommand "git init" -ThrowOnFailure -NoAssertion
            $null = Test-ExternalCommand "git submodule add --force --name `"$__PSCOREFXS_REPO_NAME`"  `"$__PSCOREFXS_REPO_URL`" `"submodules/PsCoreFxs`"" -ThrowOnFailure -NoAssertion
            $null = Test-ExternalCommand "git submodule update --init --recursive --force" -ThrowOnFailure -NoAssertion
            $null = Test-ExternalCommand "git submodule update --remote --recursive --force" -ThrowOnFailure -NoAssertion   
        }
        catch {
            throw
        }
        finally {
            Pop-Location
        }
}

