try {
    Push-Location "$PSScriptRoot"
    git submodule init
    git submodule update --remote --recursive
}
finally {
    Pop-Location
}