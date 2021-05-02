[CmdletBinding()]
param (
    [Parameter()]
    [switch]
    $Clean
)

$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-CoreFxs*.ps1")" -Force -NoClobber

function Test-LastExitCode {
    if($LASTEXITCODE -ne 0){
        Write-Host "ERROR: Check and try again. ErrorCode = $($LASTEXITCODE)." -ForegroundColor Red
        exit
    }  
}


$ErrorActionPreference = "Stop"

if($Clean.IsPresent)
{
    & ./CompileLib.ps1 -Clean
}
else {
    & ./CompileLib.ps1
}

Test-LastExitCode

Write-Host "██ Compiling Insane.js" -ForegroundColor DarkGray
Write-Host
# Write-Host "Minifying js files..."
# java -jar "./Tools/closure-compiler-v20200406.jar" `
# --js "./Js/Pre.js" `
# --js_output_file "./Js/Pre_Compiled.js" `
# --language_in ECMASCRIPT_NEXT `
# --language_out STABLE `

# Test-LastExitCode

# java -jar "./Tools/closure-compiler-v20200406.jar" `
# --js "./Js/Post.js" `
# --js_output_file "./Js/Post_Compiled.js" `
# --language_in ECMASCRIPT_NEXT `
# --language_out STABLE `

# Test-LastExitCode

# java -jar "./Tools/closure-compiler-v20200406.jar" `
# --js "./Js/ExternPre.js" `
# --js_output_file "./Js/ExternPre_Compiled.js" `
# --language_in ECMASCRIPT_NEXT `
# --language_out STABLE `

# Test-LastExitCode

# java -jar "./Tools/closure-compiler-v20200406.jar" `
# --js "./Js/ExternPost.js" `
# --js_output_file "./Js/ExternPost_Compiled.js" `
# --language_in ECMASCRIPT_NEXT `
# --language_out STABLE `

# Test-LastExitCode

Write-Host "Compiling..."
em++.bat `
main.cpp `
Insane.bc `
-I ../CommonCppIncludes `
-I Include `
-I Include/Insane `
-I ../InsaneCpp/include `
-o Insane.js `
-std=c++17 `
--bind `
-s WASM=1 `
-s DISABLE_EXCEPTION_CATCHING=0 `
-s USE_WEBGPU=1 `
-s SINGLE_FILE=0 `
-s ASYNCIFY=1 `
-s VERBOSE=0 `
-O2 `
-s EXPORT_NAME=`'CreateModuleInstance`' `
-s MODULARIZE=1 `
-s EXPORTED_FUNCTIONS=[`'_main`'] `
-s ALLOW_MEMORY_GROWTH=1 `
-s EXTRA_EXPORTED_RUNTIME_METHODS=[`'ccall`',`'cwrap`',`'lengthBytesUTF8`',`'stringToUTF8`'] `
--pre-js "Js/Pre_Compiled.js" `
--post-js "Js/Post_Compiled.js" `
--extern-post-js "Js/ExternPost_Compiled.js" `
--extern-pre-js "Js/ExternPre_Compiled.js" `
# --profiling `
# -s ASYNCIFY_IMPORTS=[] `
#-s USE_PTHREADS=1 `
#-s PTHREAD_POOL_SIZE=2 `

Test-LastExitCode

Write-Host "█ Compiling Insane.js - Finished" -ForegroundColor DarkGray
Write-Host