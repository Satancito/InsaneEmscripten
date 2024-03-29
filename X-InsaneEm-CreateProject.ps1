[CmdletBinding()]
param (
    
)
$Error.Clear()
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$(Get-Item "$X_INSANE_EM_Z_PS_CORE_FXS_INTERNAL_SCRIPT")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

try {
    Write-Host
    Write-InfoBlue "████ Creating Insane Emscripten - New Project"
    Write-Host

    Update-GitSubmodules -Path $PSScriptRoot
    $json = [System.IO.File]::ReadAllText($(Get-Item "$PRODUCT_INFO_JSON_FILE"))
    $productInfo = ConvertFrom-Json $json
    $ModuleExportName = $productInfo.Name

    $DIST_DIR = "$PSScriptRoot/Dist"

    Write-Host "Copying files..."

    $DEST_DIR = "$DIST_DIR/$ModuleExportName"
    $DEST_INCLUDE_DIR = "$DEST_DIR/Include"
    $DEST_LIB_DIR = "$DEST_DIR/Lib"
    $DEST_JS_DIR = "$DEST_DIR/Js"
    $DEST_ASSETS_DIR = "$DEST_DIR/Assets"
    $DEST_SERVER_DIR = "$DEST_DIR/Server"
    $DEST_SRC_DIR = "$DEST_DIR/Source"

    $SOURCE_ASSETS_DIR = "$PSScriptRoot/Assets"
    $SOURCE_DOCS_DIR = "$PSScriptRoot/Docs"
    $SOURCE_JS_DIR = "$PSScriptRoot/Js"
    $SOURCE_SERVER_DIR = "$PSScriptRoot/Server"
    $SOURCE_SRC_DIR = "$PSScriptRoot/Src"

    Remove-Item "$DIST_DIR" -Force -Recurse -ErrorAction Ignore
    New-Item "$DEST_DIR" -ItemType Directory -Force | Out-Null

    New-Item "$DEST_INCLUDE_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$DEST_SRC_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$DEST_LIB_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$DEST_JS_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$DEST_ASSETS_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$DEST_SERVER_DIR" -ItemType Directory -Force | Out-Null

    New-Item "$SOURCE_ASSETS_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$SOURCE_DOCS_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$SOURCE_JS_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$SOURCE_SERVER_DIR" -ItemType Directory -Force | Out-Null
    New-Item "$SOURCE_SRC_DIR" -ItemType Directory -Force | Out-Null

    try {
        Push-Location "$DEST_DIR"
        $null = Test-ExternalCommand "git init" -ThrowOnFailure
        Update-GitSubmodules -Path $(Get-Location) -Force
        $null = Test-ExternalCommand "git submodule add ""$PS_CORE_FXS_REPO_URL"" ""submodules/PsCoreFxs""" -ThrowOnFailure
    }
    finally {
        Pop-Location
    }
    Write-Host "Copying files..."
    Copy-Item -Path "$SOURCE_ASSETS_DIR/*" -Destination "$DEST_ASSETS_DIR" -Force -Recurse
    Copy-Item -Path "$SOURCE_DOCS_DIR/*" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$SOURCE_JS_DIR/*" -Destination "$DEST_JS_DIR" -Force
    Copy-Item -Path "$SOURCE_SERVER_DIR/*" -Destination "$DEST_SERVER_DIR" -Force -Recurse
    Copy-Item -Path "$SOURCE_SRC_DIR/*" -Destination "$DEST_SRC_DIR" -Force -Recurse

    Copy-Item -Path "$Z_INSANE_EM_SCRIPT" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$PRODUCT_INFO_JSON_FILE" -Destination "$DEST_DIR" -Force -Recurse
    Copy-Item -Path "$INSANE_EM_CLANG_FORMAT_FILE" -Destination "$DEST_DIR" -Force -Recurse

    Write-Host "Generating code..."
    $INDEX_HTML_FILE = "$DEST_DIR/index.html"
    $content = [System.IO.File]::ReadAllText($(Get-Item "$INDEX_HTML_FILE"))

    $pattern = "<!-- _BEGIN_APP_TITLE_ -->[\s\S]*?<!-- _END___APP_TITLE_ -->"
    $replacement = "<title>$ModuleExportName - Emscripten - Tests</title>"
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

    $pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
    $replacement = "/*ModuleName*/$ModuleExportName."
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

    [System.IO.File]::WriteAllText($(Get-Item "$INDEX_HTML_FILE"), $content, [System.Text.Encoding]::UTF8)
}
finally {
    Write-InfoBlue "█ End creating Insane Emscripten - New Project"
    Write-Host
}