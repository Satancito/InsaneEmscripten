[CmdletBinding()]
param (
    [switch]
    $NoMinifyJsFiles,
    
    [switch]
    $ReleaseMode
)
    
$Error.Clear()  
$ErrorActionPreference = "Stop"
#Import-Module -Name "$(Get-Item "$PSScriptRoot/Z-InsaneEm.ps1")" -Force -NoClobber
Import-Module -Name "$PSScriptRoot/Z-Init.ps1" -Force -NoClobber
    
function Test-Requirements {
    param (
        
    )
    Write-Host
    Write-InfoBlue "Test - Build - Dependency tools"
    Write-Host

    Write-InfoMagenta "== Emscripten"
    $command = Get-Command "$env:EMSCRIPTEN_COMPILER"
    Write-Host "$($command.Source)"
    & "$($command.Source)" --version
    Write-Host

    Assert-JavaExecutable
}

Install-EmscriptenSDK
try {
    $json = [System.IO.File]::ReadAllText("$PSScriptRoot/ProductInfo.json")
    $productInfo = ConvertFrom-Json $json

    $functionExportName = "Create$($productInfo.Name)Module"
    $moduleExportName = $productInfo.Name
    $moduleVersion = $productInfo.Version
    $moduleExportExtension = $productInfo.IsES6Module ? "mjs": "js"
    $exportConfiguration = "$($ReleaseMode.IsPresent ? "Release" : "Debug")"
    $moduleFileName = "$moduleExportName-$moduleVersion-$exportConfiguration.$moduleExportExtension"
    $projectVariablePrefix = "__INSANEEM_PROJECT_$($productInfo.Name.ToUpper())"
    $tempDir = Get-Variable -Name "$($projectVariablePrefix)_TEMP_DIR" -ValueOnly
    $insaneVersion = Get-Variable -Name "$($projectVariablePrefix)_INSANE_VERSION" -ValueOnly
    $insaneDir = "$(Get-CppLibsDir)/Insane-$insaneVersion-Emscripten-Wasm-$($ReleaseMode.IsPresent ? "Debug": "Release")"
    Write-Host
    Write-InfoBlue "████ Building Module: ""$moduleFileName"""
    Write-Host
    Test-Requirements

    $toolsDir = "$tempDir/Tools"
    $buildDir = "$PSScriptRoot/Bin/$($ReleaseMode.IsPresent ? "Release" : "Debug")"
    $buildRelativeDir = "Bin/$($ReleaseMode.IsPresent ? "Release" : "Debug")"
    $compiledJsDir = "$buildDir/Js"
    $compiledModuleDir = "$buildDir/Module"
    $compiledModuleRelativeDir = "$buildRelativeDir/Module"
    $generatedCompilationDbDir = "$buildDir/clangd"
    $indexHtml = "$PSScriptRoot/index.html"
    $indexMjs = "$PSScriptRoot/index.mjs"
    $indexTs = "$PSScriptRoot/index.ts"
    Remove-Item -Path "$PSScriptRoot/Bin"-Force -Recurse -ErrorAction Ignore
    New-Item "$compiledJsDir" -ItemType Container -Force | Out-Null
    New-Item "$compiledModuleDir" -ItemType Container -Force | Out-Null
    New-Item "$generatedCompilationDbDir" -ItemType Container -Force | Out-Null
    if (!$NoMinifyJsFiles.IsPresent) {
        Write-InfoBlue "Minifying Js files"
        New-Item "$toolsDir" -ItemType Container -Force | Out-Null
        $closureCompilerUrl = "$(Get-Variable -Name "$($projectVariablePrefix)_CLOSURE_COMPILER_URL" -ValueOnly)"
        $closureCompilerHash = "$(Get-Variable -Name "$($projectVariablePrefix)_CLOSURE_COMPILER_HASH" -ValueOnly)"
        $closureCompiler = Invoke-HttpDownload -Url "$closureCompilerUrl" -DestinationPath "$toolsDir" -HashAlgorithm SHA1 -Hash "$closureCompilerHash" -ReturnFilename
    }


    $es6BrowserCode = @"
<!-- _BEGIN_MODULE_SCRIPT_ -->
    <!-- The provided code is autogenerated. Do not modify. -->
    <script type="module">
        import { default as $functionExportName } from "./$compiledModuleRelativeDir/$moduleFileName"
        $functionExportName(moduleInstantiationParameters).then((instance) => {
            globalThis["$moduleExportName"] = instance;
        });
    </script>
    <!-- _END___MODULE_SCRIPT_ -->
"@

    $jsBrowserCode = @"
<!-- _BEGIN_MODULE_SCRIPT_ -->
    <!-- The provided code is autogenerated. Do not modify. -->
    <script src="$compiledModuleRelativeDir/$moduleFileName"></script>
    <script>
        globalThis["$functionExportName"](moduleInstantiationParameters)
            .then(instance => {
                globalThis["$moduleExportName"]= instance ;
            });
    </script>
    <!-- _END___MODULE_SCRIPT_ -->
"@

    $denoCode = @"
/*_BEGIN_MODULE_INSTANTIATION_*/
//The provided code is autogenerated. Do not modify.
import {default as factory} from "./$compiledModuleRelativeDir/$moduleFileName"
const instance = await factory(moduleInstantiationParameters);
/*_END___MODULE_INSTANTIATION_*/
"@

    $nodeCode = @"
/*_BEGIN_MODULE_INSTANTIATION_*/
//The provided code is autogenerated. Do not modify.
import {default as factory} from "./$compiledModuleRelativeDir/$moduleFileName"
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

    $Crlf = [System.Environment]::NewLine

    $filenames = @("$PSScriptRoot/Js/ExternPost.js", "$PSScriptRoot/Js/ExternPre.js", "$PSScriptRoot/Js/Post.js", "$PSScriptRoot/Js/Pre.js")
    $additionalContents = @($AdditionalExternPostJsContent, $AdditionalExternPreJsContent, $AdditionalPostJsContent, $AdditionalPreJsContent)
    

   

    for ($i = 0; $i -lt $filenames.Length; $i++) {
        $content = [System.IO.File]::ReadAllText("$($filenames[$i])")
        $content += "$Crlf$Crlf$($additionalContents[$i])"
        $compiledFilename = "$compiledJsDir/$($filenames[$i] | Split-Path -Leaf)"
        [System.IO.File]::WriteAllText($compiledFilename, $content, [System.Text.Encoding]::UTF8)
        if ($NoMinifyJsFiles.IsPresent) {
            [System.IO.File]::WriteAllText($compiledFilename, $content, [System.Text.Encoding]::UTF8)        
        }
        else {
            Write-PrettyKeyValue "Minifying js file" "$compiledFilename"
            java -jar "$closureCompiler" `
                --js "$compiledFilename" `
                --js_output_file "$compiledFilename" `
                --language_in ECMASCRIPT_NEXT `
                --language_out STABLE
        }
    }
    
    $DEBUG_OPTIMIZATION = 0
    $RELEASE_OPTIMIZATION = 3
    Write-Host
    Write-InfoBlue "Building module js file..."
    $clangd_name = "$generatedCompilationDbDir/$(Get-HexRandomName)_.compile_commands.json"
    & "$env:EMSCRIPTEN_COMPILER" `
        -MJ "$clangd_name" `
        "$PSScriptRoot/Source/main.cpp" `
        "$insaneDir/Lib/libInsane.a" `
        -I $PSScriptRoot/Include `
        -I  "$insaneDir/Include" `
        -o "$compiledModuleDir/$moduleFileName" `
        -O"$($ReleaseMode.IsPresent ? $RELEASE_OPTIMIZATION : $DEBUG_OPTIMIZATION)" `
        -std=c++20 `
        -lembind `
        -fexceptions `
        -s SINGLE_FILE=1 `
        -s WASM=1 `
        -s VERBOSE=0 `
        -s USE_WEBGPU=1 `
        -s USE_ICU=1 `
        -s EXPORT_ES6=$($productInfo.IsES6Module ? 1 : 0) `
        -s MODULARIZE=1 `
        -s EXPORT_NAME=`'$functionExportName`' `
        -s ALLOW_MEMORY_GROWTH=1 `
        -s EXPORTED_RUNTIME_METHODS=[ccall, cwrap, lengthBytesUTF8, stringToUTF8] `
        --pre-js "$compiledJsDir/Pre.js" `
        --post-js "$compiledJsDir/Post.js" `
        --extern-post-js "$compiledJsDir/ExternPost.js" `
        --extern-pre-js "$compiledJsDir/ExternPre.js" `
        -s ASYNCIFY=1 `
        -D LIB_COMPILE_TIME `
        -D LIB_PRODUCT_NAME="\`"$($moduleExportName)\`"" `
        -D LIB_PRODUCT_VERSION="\`"$($moduleVersion)\`"" 

    Write-Host "Creating compiler database `"compile_commands.json`""
    #Clangd compile_commands.json gen.
    Join-CompileCommandsJson -SourceDir "$generatedCompilationDbDir" -DestinationDir "$PSScriptRoot"
    #Browser - Code gen.
    Write-Host "Generating code for browser..."
    $content = [System.IO.File]::ReadAllText($(Get-Item "$indexHtml"))
    $pattern = '<!-- _BEGIN_MODULE_SCRIPT_ -->[\s\S]*?<!-- _END___MODULE_SCRIPT_ -->'
    $replacement = $productInfo.IsES6Module ? $es6BrowserCode : $jsBrowserCode
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)

    Write-Host "Generating App title on browser code..."
    $pattern = "<!-- _BEGIN_APP_TITLE_ -->[\s\S]*?<!-- _END___APP_TITLE_ -->"
    $replacement = "<title>&quot;$moduleExportName $moduleVersion&quot; - Emscripten</title>"
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
    
    Write-Host "Updating browser code..."
    $pattern = "\/\*ModuleName\*\/[a-zA-Z_][a-zA-Z0-9_]*\."
    $replacement = "/*ModuleName*/$moduleExportName."
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
    [System.IO.File]::WriteAllText($(Get-Item "$indexHtml"), $content, [System.Text.Encoding]::UTF8)

    # Node - Code gen.
    Write-Host "Generating code for Deno script..."
    $content = [System.IO.File]::ReadAllText($(Get-Item "$indexMjs"))
    $pattern = '\/\*_BEGIN_MODULE_INSTANTIATION_\*[\s\S]*?\*_END___MODULE_INSTANTIATION_\*\/'
    $replacement = $nodeCode
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
    [System.IO.File]::WriteAllText($(Get-Item "$indexMjs"), $content, [System.Text.Encoding]::UTF8)

    # Deno - - Code gen.
    Write-Host "Generating code for Node.js script..."
    $content = [System.IO.File]::ReadAllText($(Get-Item "$indexTs"))
    $pattern = '\/\*_BEGIN_MODULE_INSTANTIATION_\*[\s\S]*?\*_END___MODULE_INSTANTIATION_\*\/'
    $replacement = $denoCode
    $content = [System.Text.RegularExpressions.Regex]::Replace("$content", "$pattern", $replacement, [System.Text.RegularExpressions.RegexOptions]::Multiline)
    [System.IO.File]::WriteAllText($(Get-Item "$indexTs"), $content, [System.Text.Encoding]::UTF8)

}
finally {
    Write-InfoBlue "█ End building ""$moduleFileName"" - Finished"
    Write-Host
}
