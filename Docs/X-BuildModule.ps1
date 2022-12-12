[CmdletBinding()]
param (
    
)

$ErrorActionPreference = "Stop"
Import-Module -Name "$(Get-Item "./Z-PsCoreFxs.ps1")" -Force -NoClobber
Write-InfoDarkGray "▶▶▶ Running: $PSCommandPath"

Write-Host
Write-InfoBlue "████ Compiling Insane.js"
Write-Host
& "./X-InsaneEmscripten-SetEmscriptenEnvVars.ps1"
Write-Host "Minifying js files..."

Test-LastExitCode

$closureCompiler = Get-Item "./Tools/closure-compiler-v*.jar"
java -jar "$closureCompiler" `
--js "./Js/Pre.js" `
--js_output_file "./Js/Pre_Compiled.js" `
--language_in ECMASCRIPT_NEXT `
--language_out STABLE `

Test-LastExitCode

java -jar "$closureCompiler" `
--js "./Js/Post.js" `
--js_output_file "./Js/Post_Compiled.js" `
--language_in ECMASCRIPT_NEXT `
--language_out STABLE `

Test-LastExitCode

java -jar "$closureCompiler" `
--js "./Js/ExternPre.js" `
--js_output_file "./Js/ExternPre_Compiled.js" `
--language_in ECMASCRIPT_NEXT `
--language_out STABLE `

Test-LastExitCode

java -jar "$closureCompiler" `
--js "./Js/ExternPost.js" `
--js_output_file "./Js/ExternPost_Compiled.js" `
--language_in ECMASCRIPT_NEXT `
--language_out STABLE `

Test-LastExitCode

Write-Host "Compiling..."
& "$env:EMSCRIPTEN_COMPILER" `
main.cpp `
Lib/libInsane.a `
-I Include `
-o Insane.js `
-std=c++17 `
-lembind `
-s WASM=1 `
-s DISABLE_EXCEPTION_CATCHING=0 `
-s USE_WEBGPU=1 `
-s SINGLE_FILE=1 `
-s ASYNCIFY=1 `
-s VERBOSE=0 `
-O2 `
-s EXPORT_NAME=`'CreateModuleInstance`' `
-s MODULARIZE=1 `
-s EXPORTED_FUNCTIONS=[`'_main`'] `
-s ALLOW_MEMORY_GROWTH=1 `
-s EXPORTED_RUNTIME_METHODS=[`'ccall`',`'cwrap`',`'lengthBytesUTF8`',`'stringToUTF8`'] `
--pre-js "Js/Pre_Compiled.js" `
--post-js "Js/Post_Compiled.js" `
--extern-post-js "Js/ExternPost_Compiled.js" `
--extern-pre-js "Js/ExternPre_Compiled.js" `
# --profiling `
# -s ASYNCIFY_IMPORTS=[] `
#-s USE_PTHREADS=1 `
#-s PTHREAD_POOL_SIZE=2 `

Test-LastExitCode

Write-InfoBlue "█ End compiling Insane.js - Finished"
Write-Host