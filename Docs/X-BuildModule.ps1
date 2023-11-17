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
$fxExportName = "Create$($ModuleExportName)Module"
$exportExtension = $ModuleIsES6Module ? "mjs": "js"
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

$es6BrowserCode = @"
<!-- _BEGIN_MODULE_SCRIPT_ -->
    <!-- The provided code is autogenerated. Do not modify. -->
    <script type="module">
        import { default as $fxExportName } from "./$ModuleExportName.mjs"
        $fxExportName(moduleInstantiationParameters).then((instance) => {
            globalThis["$ModuleExportName"] = instance;
        });
    </script>
    <!-- _END___MODULE_SCRIPT_ -->
"@

$jsBrowserCode = @"
<!-- _BEGIN_MODULE_SCRIPT_ -->
    <!-- The provided code is autogenerated. Do not modify. -->
    <script>
        globalThis["$fxExportName"](moduleInstantiationParameters)
            .then(instance => {
                globalThis["$ModuleExportName"]= instance ;
            });
    </script>
    <script src="$ModuleExportName.js"></script>
    <!-- _END___MODULE_SCRIPT_ -->
"@

$denoCode = @"
/*_BEGIN_MODULE_INSTANTIATION_*/
//The provided code is autogenerated. Do not modify.
import {default as factory} from "./$ModuleExportName.mjs"
const instance = await factory(moduleInstantiationParameters);
/*_END___MODULE_INSTANTIATION_*/
"@

$nodeCode = @"
/*_BEGIN_MODULE_INSTANTIATION_*/
//The provided code is autogenerated. Do not modify.
import {default as factory} from "./$ModuleExportName.mjs"
const instance = await factory(moduleInstantiationParameters);
/*_END___MODULE_INSTANTIATION_*/
"@

$AdditionalExternPostJsContent = @"
"@

$AdditionalExternPreJsContent = @"
"@

$AdditionalPostJsContent = @"
"@

$AdditionalPreJsContent = @"
"@

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
    -s EXPORT_NAME=`'$fxExportName`' `
    -s EXPORT_ES6=$exportEs6 `
    -s MODULARIZE=1 `
    -s ALLOW_MEMORY_GROWTH=1 `
    -s EXPORTED_RUNTIME_METHODS=[ccall, cwrap, lengthBytesUTF8, stringToUTF8] `
    --pre-js "$COMPILED_JS_DIR/$($compiledPrefix)Pre.js" `
    --post-js "$COMPILED_JS_DIR/$($compiledPrefix)Post.js" `
    --extern-post-js "$COMPILED_JS_DIR/$($compiledPrefix)ExternPost.js" `
    --extern-pre-js "$COMPILED_JS_DIR/$($compiledPrefix)ExternPre.js" `
    -s ASYNCIFY=1 `
    -D LIB_COMPILE_TIME `
    -D LIB_PRODUCT_NAME="\""$($ModuleExportName)\""" `
    -D LIB_PRODUCT_VERSION="\""$($ModuleVersion)\""" 
    #--emrun `

$INDEX_HTML_FILE = "$PSScriptRoot/index.html"
$INDEX_MJS_FILE = "$PSScriptRoot/index.mjs"
$INDEX_TS_FILE = "$PSScriptRoot/index.ts"

#Browser
$compileCommandsJson = [System.IO.File]::ReadAllText($(Get-Item "$clangdJson"))
[System.IO.File]::WriteAllText($(Get-Item "$clangdJson"), "[ $compileCommandsJson ]", [System.Text.Encoding]::UTF8)

$content = [System.IO.File]::ReadAllText($(Get-Item "$INDEX_HTML_FILE"))
$pattern = '<!-- _BEGIN_MODULE_SCRIPT_ -->[\s\S]*?<!-- _END___MODULE_SCRIPT_ -->'
$replacement = $ModuleIsES6Module ? $es6BrowserCode : $jsBrowserCode
$content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

if ($TestMode.IsPresent) {
    $pattern = "<!-- _BEGIN_APP_TITLE_ -->[\s\S]*?<!-- _END___APP_TITLE_ -->"
    $replacement = "<title>$ModuleExportName - Emscripten - Tests</title>"
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

    $pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
    $replacement = "/*ModuleName*/$ModuleExportName."
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
}
[System.IO.File]::WriteAllText($(Get-Item "$INDEX_HTML_FILE"), $content, [System.Text.Encoding]::UTF8)

# Node
$content = [System.IO.File]::ReadAllText($(Get-Item "$INDEX_MJS_FILE"))
$pattern = '\/\*_BEGIN_MODULE_INSTANTIATION_\*[\s\S]*?\*_END___MODULE_INSTANTIATION_\*\/'
$replacement = $nodeCode
$content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
[System.IO.File]::WriteAllText($(Get-Item "$INDEX_MJS_FILE"), $content, [System.Text.Encoding]::UTF8)

# Deno
$content = [System.IO.File]::ReadAllText($(Get-Item "$INDEX_TS_FILE"))
$pattern = '\/\*_BEGIN_MODULE_INSTANTIATION_\*[\s\S]*?\*_END___MODULE_INSTANTIATION_\*\/'
$replacement = $denoCode
$content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
[System.IO.File]::WriteAllText($(Get-Item "$INDEX_TS_FILE"), $content, [System.Text.Encoding]::UTF8)

Write-InfoBlue "█ End building $ModuleExportName.$exportExtension - Finished"
Write-Host
