[CmdletBinding()]
param (
    [switch]
    $NoMinifyJsFiles,

    [switch]
    $TestMode
)

$Error.Clear()  
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

$json = [System.IO.File]::ReadAllText($(Get-Item "ProductInfo.json"))
$productInfo = ConvertFrom-Json $json
$ModuleExportName = $productInfo.Name
$ModuleVersion = $productInfo.Version

Write-Host
Write-InfoBlue "████ Building Module: ""$ModuleExportName.js"", Version: $ModuleVersion"
Write-Host
    
$exportName = "Create$($ModuleExportName)Module"
$AdditionalExternPostJsContent = @"
window["#1#"]({ noInitialRun: false })
    .then(instance => {
        window["#2#"]= instance ;
    });
"@.Replace("#1#", "$exportName").Replace("#2#", "$ModuleExportName")

$AdditionalExternPreJsContent = ""

$AdditionalPostJsContent = ""

$AdditionalPreJsContent = ""

$ExternPostJsFileName = Get-Item "./Js/ExternPost.js"
$ExternPreJsFileName = Get-Item "./Js/ExternPre.js"
$PostJsFileName = Get-Item "./Js/Post.js"
$PreJsFileName = Get-Item "./Js/Pre.js"

$generatedPrefix = "Generated"
$compiledPrefix = "Minified"
$Crlf = [System.Environment]::NewLine

$filenames = @($ExternPostJsFileName, $ExternPreJsFileName, $PostJsFileName, $PreJsFileName)
$additionalContents = @($AdditionalExternPostJsContent, $AdditionalExternPreJsContent, $AdditionalPostJsContent, $AdditionalPreJsContent)


for ($i = 0; $i -lt $filenames.Length; $i++) {
    $content = [System.IO.File]::ReadAllText("$($filenames[$i])")
    $content += "$Crlf$Crlf$($additionalContents[$i])"
    $generatedFilename = "$($filenames[$i].DirectoryName)/$generatedPrefix$($filenames[$i].Name)"
    $compiledFilename = "$($filenames[$i].DirectoryName)/$compiledPrefix$($filenames[$i].Name)"
    [System.IO.File]::WriteAllText($generatedFilename, $content, [System.Text.Encoding]::UTF8)
    if ($NoMinifyJsFiles.IsPresent) {
        [System.IO.File]::WriteAllText($compiledFilename, $content, [System.Text.Encoding]::UTF8)        
    }
    else {
        Write-PrettyKeyValue "Minifying js file" "$generatedFilename"
        $closureCompiler = Get-Item "./Tools/closure-compiler-v*.jar"
        java -jar "$closureCompiler" `
            --js "$generatedFilename" `
            --js_output_file "$compiledFilename" `
            --language_in ECMASCRIPT_NEXT `
            --language_out STABLE
    }
}
    
Test-LastExitCode
Write-Host "Building..."
$clangdJson = "compile_commands.json"
& "$env:EMSCRIPTEN_COMPILER" `
    -MJ $clangdJson `
    main.cpp `
    Lib/libInsane.a `
    -I Include `
    -o "$ModuleExportName.js" `
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
    -s MODULARIZE=1 `
    -s ALLOW_MEMORY_GROWTH=1 `
    -s EXPORTED_RUNTIME_METHODS=[ccall, cwrap, lengthBytesUTF8, stringToUTF8] `
    --pre-js "Js/$($compiledPrefix)Pre.js" `
    --post-js "Js/$($compiledPrefix)Post.js" `
    --extern-post-js "Js/$($compiledPrefix)ExternPost.js" `
    -s ASYNCIFY=1 `
    --extern-pre-js "Js/$($compiledPrefix)ExternPre.js" `
    -D LIB_COMPILE_TIME `
    -D LIB_PRODUCT_NAME="\""$($ModuleExportName)\""" `
    -D LIB_PRODUCT_VERSION="\""$($ModuleVersion)\""" 
    
$filenames.ForEach({
        $generatedFilename = "$($_.DirectoryName)/$generatedPrefix$($_.Name)"
        $compiledFilename = "$($_.DirectoryName)/$compiledPrefix$($_.Name)"
        Remove-Item -Path $generatedFilename -Force -ErrorAction Ignore
        Remove-Item -Path $compiledFilename -Force -ErrorAction Ignore
    }) 

$compileCommandsJson = [System.IO.File]::ReadAllText($(Get-Item $clangdJson))
[System.IO.File]::WriteAllText($(Get-Item $clangdJson), "[ $compileCommandsJson ]", [System.Text.Encoding]::UTF8)

$content = [System.IO.File]::ReadAllText($(Get-Item "index.html"))
$pattern = '<script\s+src\s*=\s*"[a-zA-Z0-9_]+\.js"\s*>\s*\/\/#\*Module\*#\s*<\s*\/script\s*>\s*'
$replacement = "<script src=""$($ModuleExportName).js"">//#*Module*#</script>"
$content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

if ($TestMode.IsPresent) {
    $pattern = "<title>.*?<\/title>"
    $replacement = "<title>$ModuleExportName - Emscripten - Tests</title>"
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
}

$pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
$replacement = "/*ModuleName*/$ModuleExportName."
$content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

[System.IO.File]::WriteAllText($(Get-Item "index.html"), $content, [System.Text.Encoding]::UTF8)

Write-InfoBlue "█ End building $ModuleExportName.js - Finished"
Write-Host
