[CmdletBinding()]
param (
    [string]
    $ModuleExportName = "Insane",

    [switch]
    $NoMinifyJsFiles
)

$Error.Clear()  
$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"
    
Write-Host
Write-InfoBlue "████ Building Insane.js"
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
    else
    {
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
& "$env:EMSCRIPTEN_COMPILER" `
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
    --extern-pre-js "Js/$($compiledPrefix)ExternPre.js" 
    #-D_DEBUG `
    #-fno-use-cxa-atexit -emit-llvm `
    #-s EXPORTED_FUNCTIONS=[`'_main`'] `
    #-s DISABLE_EXCEPTION_CATCHING=0 `
    # --profiling `
    # -s TOTAL_MEMORY=256MB `
    # -s ASYNCIFY_IMPORTS=[] `
#-s USE_PTHREADS=1 `
#-s PTHREAD_POOL_SIZE=2 `
    
$filenames.ForEach({
        $generatedFilename = "$($_.DirectoryName)/$generatedPrefix$($_.Name)"
        $compiledFilename = "$($_.DirectoryName)/$compiledPrefix$($_.Name)"
        Remove-Item -Path $generatedFilename -Force -ErrorAction Ignore
        Remove-Item -Path $compiledFilename -Force -ErrorAction Ignore
    })  
    
Write-InfoBlue "█ End building Insane.js - Finished"
Write-Host
