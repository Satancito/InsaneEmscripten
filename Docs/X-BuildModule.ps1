[CmdletBinding()]
param (
    [switch]
    $NoMinifyJsFiles,

    [switch]
    $TestMode
)

$Error.Clear()  
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

$json = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name
$ModuleVersion = $productInfo.Version
$ModuleIsES6Module = $productInfo.IsES6Module
$exportName = "Create$($ModuleExportName)Module"
$exportExtension = $ModuleIsES6Module ? "js": "js"
$exportEs6 = $ModuleIsES6Module ? 1 : 0

Write-Host
Write-InfoBlue "████ Building Module: ""$ModuleExportName.$exportExtension"", Version: $ModuleVersion"
Write-Host

$TOOLS_DIR = "$(Get-UserHome)/.InsaneEmscripten/Tools"
New-Item "$TOOLS_DIR" -ItemType Container -Force | Out-Null
$closureCompilerUrl = "https://repo1.maven.org/maven2/com/google/javascript/closure-compiler/v20230802/closure-compiler-v20230802.jar"
$closureCompiler = "$TOOLS_DIR/$(Split-Path "$closureCompilerUrl" -Leaf)"
$closureCompilerHash = "F6E52E1DCDDB9160489AC1B8CF32C129"
if (!(Test-Path -Path "$closureCompiler" -PathType Leaf)) {
    Invoke-WebRequest -Uri "$closureCompilerUrl" -OutFile "$closureCompiler"
}
$computedClosureCompilerHash = (Get-FileHash -Path "$closureCompiler" -Algorithm MD5).Hash
if ($computedClosureCompilerHash -ne $closureCompilerHash) {
    Write-Host "Invalid computed Closure Compiler hash. Computed($computedClosureCompilerHash) ≠ Expected($closureCompilerHash). File ""$closureCompiler""."
    Write-Host "Removing ""$closureCompiler"""
    Remove-Item -Path $closureCompiler -Force -Recurse -ErrorAction Ignore
    Write-Host "Downloading ""$closureCompilerUrl"""
    Invoke-WebRequest -Uri "$closureCompilerUrl" -OutFile "$closureCompiler"
    $computedClosureCompilerHash = (Get-FileHash -Path "$closureCompiler" -Algorithm MD5).Hash
    if ($computedClosureCompilerHash -ne $closureCompilerHash) {    
        Write-Host "Invalid computed Closure Compiler hash. Computed($computedClosureCompilerHash) ≠ Expected($closureCompilerHash). File ""$closureCompiler""."
    }
}

$es6ModuleCode = @"
#1#({ noInitialRun: false })
    .then(instance => {
        window["#2#"]= instance ;
    });
"@

$jsModuleCode = @"
window["#1#"]({ noInitialRun: false })
    .then(instance => {
        window["#2#"]= instance ;
    });
"@

$AdditionalExternPostJsContent = ($ModuleIsES6Module ? $es6ModuleCode : $jsModuleCode).Replace("#1#", "$exportName").Replace("#2#", "$ModuleExportName")

$AdditionalExternPreJsContent = ""

$AdditionalPostJsContent = ""

$AdditionalPreJsContent = ""

$ExternPostJsFileName = Get-Item "$PSScriptRoot/Js/ExternPost.js"
$ExternPreJsFileName = Get-Item "$PSScriptRoot/Js/ExternPre.js"
$PostJsFileName = Get-Item "$PSScriptRoot/Js/Post.js"
$PreJsFileName = Get-Item "$PSScriptRoot/Js/Pre.js"

$generatedPrefix = "Generated"
$compiledPrefix = "Minified"
$Crlf = [System.Environment]::NewLine

$filenames = @($ExternPostJsFileName, $ExternPreJsFileName, $PostJsFileName, $PreJsFileName)
$additionalContents = @($AdditionalExternPostJsContent, $AdditionalExternPreJsContent, $AdditionalPostJsContent, $AdditionalPreJsContent)

$COMPILED_JS_DIR = "$PSScriptRoot/Build/Js/$compiledPrefix"
$GENERATED_JS_DIR = "$PSScriptRoot/Build/Js/$generatedPrefix"
New-Item "$COMPILED_JS_DIR" -ItemType Container -Force | Out-Null
New-Item "$GENERATED_JS_DIR" -ItemType Container -Force | Out-Null


for ($i = 0; $i -lt $filenames.Length; $i++) {
    $content = [System.IO.File]::ReadAllText("$($filenames[$i])")
    $content += "$Crlf$Crlf$($additionalContents[$i])"
    $generatedFilename = "$GENERATED_JS_DIR/$generatedPrefix$($filenames[$i].Name)"
    $compiledFilename = "$COMPILED_JS_DIR/$compiledPrefix$($filenames[$i].Name)"
    [System.IO.File]::WriteAllText($generatedFilename, $content, [System.Text.Encoding]::UTF8)
    if ($NoMinifyJsFiles.IsPresent) {
        [System.IO.File]::WriteAllText($compiledFilename, $content, [System.Text.Encoding]::UTF8)        
    }
    else {
        Write-PrettyKeyValue "Minifying js file" "$generatedFilename"
        java -jar "$closureCompiler" `
            --js "$generatedFilename" `
            --js_output_file "$compiledFilename" `
            --language_in ECMASCRIPT_NEXT `
            --language_out STABLE
    }
}

Remove-Item -Path "$PSScriptRoot/$ModuleExportName.*js"-Force -Recurse -ErrorAction Ignore

Write-Host "Building ""$ModuleExportName.$exportExtension""... "
$clangdJson = "$PSScriptRoot/compile_commands.json"
& "$env:EMSCRIPTEN_COMPILER" `
    -MJ $clangdJson `
    $PSScriptRoot/main.cpp `
    $PSScriptRoot/Lib/libInsane.a `
    -I $PSScriptRoot/Include `
    -o "$PSScriptRoot/$ModuleExportName.$exportExtension" `
    -std=c++20 `
    -lembind `
    -fexceptions `
    -O0 `
    -s USE_WEBGPU=1 `
    -s SINGLE_FILE=1 `
    -s WASM=1 `
    -s VERBOSE=0 `
    -s USE_ICU=1 `
    -s EXPORT_NAME=`'$exportName`' `
    -s EXPORT_ES6=$exportEs6 `
    -s MODULARIZE=1 `
    -s ALLOW_MEMORY_GROWTH=1 `
    -s EXPORTED_RUNTIME_METHODS=[ccall, cwrap, lengthBytesUTF8, stringToUTF8] `
    --pre-js "$COMPILED_JS_DIR/$($compiledPrefix)Pre.js" `
    --post-js "$COMPILED_JS_DIR/$($compiledPrefix)Post.js" `
    --extern-post-js "$COMPILED_JS_DIR/$($compiledPrefix)ExternPost.js" `
    --extern-pre-js "$COMPILED_JS_DIR/$($compiledPrefix)ExternPre.js" `
    -s ASYNCIFY=1 `
    --emrun `
    -D LIB_COMPILE_TIME `
    -D LIB_PRODUCT_NAME="\""$($ModuleExportName)\""" `
    -D LIB_PRODUCT_VERSION="\""$($ModuleVersion)\""" 


$compileCommandsJson = [System.IO.File]::ReadAllText($(Get-Item "$clangdJson"))
[System.IO.File]::WriteAllText($(Get-Item "$clangdJson"), "[ $compileCommandsJson ]", [System.Text.Encoding]::UTF8)

$content = [System.IO.File]::ReadAllText($(Get-Item "$PSScriptRoot/index.html"))
$pattern = '<script\s+src\s*=\s*"[a-zA-Z0-9_]+\.(js|mjs)"\s*(type="module")?\s*>\s*\/\/#\*Module\*#\s*<\s*\/script\s*>'
$module = $ModuleIsES6Module ? " type=`"module`"": [string]::Empty
$replacement = "<script src=""$($ModuleExportName).$exportExtension""$module>//#*Module*#</script>"
$content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

if ($TestMode.IsPresent) {
    $pattern = "<title>.*?<\/title>"
    $replacement = "<title>$ModuleExportName - Emscripten - Tests</title>"
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

    $pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
    $replacement = "/*ModuleName*/$ModuleExportName."
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
}

[System.IO.File]::WriteAllText($(Get-Item "$PSScriptRoot/index.html"), $content, [System.Text.Encoding]::UTF8)

Write-InfoBlue "█ End building $ModuleExportName.$exportExtension - Finished"
Write-Host
