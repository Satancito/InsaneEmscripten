# InsaneEmscripten
Insane native lib for WebAssembly using the Emscripten SDK.

In addition this project allows to create an Emscripten skeleton project. The project allows build, run and publish a WebAssembly module to use in your main web project.

The created Project contains:
- Insane library configured as dependency.
- Build script.
- Run script.
- Publish script.
- Another related files scripts, tools, etc.   

## Prerequisites
Install the following packages in your operating system.
- [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell?view=powershell-7.4)*.
- [Python](https://www.python.org/)*
- [Make](https://www.gnu.org/software/make)
- [Git](https://git-scm.com/)*
- [OpenJDK](https://jdk.java.net/) or [OracleJDK](https://www.oracle.com/java/technologies/downloads/)
- [Node.js](https://nodejs.org/en)
- [Deno](https://deno.com/)
- [Wsl - Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/install)*   
- [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm)*   
- Build Botan library.
- Build ICU library.

> \* If your OS is Windows, you need to install the packages PowerShell, Python, Make, Git in WSL. This is required for building Botan due to incompatibilities with the Botan script on Windows platforms.

### Concept List
- ***DistDir, distDir, DIST_DIR*** refers to the dist folder of the library. Format is `/my/destination/dir/Insane-<Version>-Emscripten-Wasm-<Configuration><DistDirSuffix>`
Configuration values are Debug or Release. DistDirSuffix is user defined value. All values are computed by the script. In total there are 2 DIST_DIRs, 1 for Debug configuration and 1 for Release configuration.    
e.g.  
`/my/destination/dir/Insane-1.0.0-Emscripten-Wasm-Debug`     
`/my/destination/dir/Insane-1.0.0-Emscripten-Wasm-Release` 
<br />       
`/my/destination/dir/Insane-1.0.0-Emscripten-Wasm-Debug-Dev`   
`/my/destination/dir/Insane-1.0.0-Emscripten-Wasm-Release-Dev`  
### how to use?

## First steps
- Clone repository `git clone https://github.com/Satancito/InsaneEmscripten.git`
- Update repository submodules. Inside repo run `./T-UpdateSubmodules.ps1`

## Build Botan library
Command `./X-InsaneEm.ps1 -BuildBotan [-DestinationDir <string>]`    
This command allows building Botan library for WebAssembly. If you have previously built ICU, ignore this step.
### Parameters

- ***DestinationDir*** Optional. Defaults to `<USER_DIRECTORY>/.CppLibs`. This is the destination where the dist folder will be created.   

## Build ICU library
Command `./X-InsaneEm.ps1 -BuildICU`    
This command allows activating/pre building ICU library on the Emscripten SDK. If you have previously built ICU, ignore this step.    

## Build Insane library
Command `./X-InsaneEm.ps1 [-Build] [-DestinationDir <string>] [-DistDirSuffix <string>] [-LibSuffix <string>] [-LibsDir <string>]`    
This command allows building the Insane library for WebAssembly.

### Parameters
- ***DestinationDir*** Optional. Defaults to `<USER_DIRECTORY>/.CppLibs`. This is the destination where the dist folder will be created.

- ***DistDirSuffix*** Optional. Defaults to empty. This is a suffix to add to the name of the dist folder.    
e.g. if `DistDirSuffix` is `Dev`, the dist folder is `<USER_DIRECTORY>/.CppLibs/<DIST_DIR>-Dev`

- ***LibSuffix*** Optional. Defaults to empty. This is a suffix to add to the name of the library.    
e.g. `LibSuffix` is `Internal`, the library name is `<USER_DIRECTORY>/.CppLibs/<DIST_DIR>/Lib/libInsaneInternal.a`.

- ***LibsDir*** Optional. Defaults to `<USER_DIRECTORY>/.CppLibs` This is the path where Botan library and others are located. 

## Build Combo - (Botan + ICU + Insane)
Command `./X-InsaneEm.ps1 -BuildAll [-DestinationDir <string>] [-DistDirSuffix <string>] [-LibSuffix <string>] [-LibsDir <string>] [-SkipBotan] [-SkipICU]`    
This command allows building Botan, ICU and Insane libs. Building Botan and ICU can be skipped.

### Parameters

Same parameters as Insane build command with the following aditions

- ***SkipBotan*** Optional. This flag allow skipping Botan build.

- ***SkipICU*** Optional. This flag allow skipping ICU build.

## Clean - Temp
Command `./X-InsaneEm.ps1 -Clean`   
This Command allows to remove InsaneEmscripten temp dir `<USER_DIRECTORY>/.InsaneEm`

## Install Emscripten
Command `./X-InsaneEm.ps1 -InstallEmscriptenSDK [-Force]`   
This Command allows to install Emscripten SDK.
### Parameters
- ***Force*** Optional. This flag allow reinstall Emscripten SDK.

## Create Emscripten project + Insane
Command `./X-InsaneEm.ps1 -CreateProject [-Name] <string>`   
This Command allows to create an Emscripten skeleton project with Insane as dependency, build script, run script, publish script and other related files(scripts, tools, configs, etc).

### Parameters
- ***Name*** The name of the new project.   
The created project is located by default on `Dist/<PROJECT_NAME>`   
e.g `Name` is "MyProject", the project is created on ```Dist/MyProject```

## Results tree 
 
Compiled Insane lib with default options.
```
<DIST_DIR>
├── Insane-1.0.0-Emscripten-Wasm-Debug
│   ├── Include
│   │   └── Insane
│   │       ├── Insane.h
│   │       ├── InsaneEmscripten.h
│   │       ├── InsaneCore.h
│   │       ├── InsaneCryptography.h
│   │       ├── InsaneException.h
│   │       ├── InsanePreprocessor.h
│   │       ├── InsaneString.h
│   │       └── InsaneTest.h
│   └── Lib
│       └── libInsane.a
└── Insane-1.0.0-Emscripten-Wasm-Release
    ├── Include
    │   └── Insane
    │       ├── Insane.h
    │       ├── InsaneEmscripten.h
    │       ├── InsaneCore.h
    │       ├── InsaneCryptography.h
    │       ├── InsaneException.h
    │       ├── InsanePreprocessor.h
    │       ├── InsaneString.h
    │       └── InsaneTest.h
    └── Lib
        └── libInsane.a

```
<br />
<br />
<br />

# Next Steps - With new project 🚀
Change location to new project. Run `cd Dist/MyProject`.

You can change module type(normal script or ES6 module) in `ProductInfo.json`, the property `IsES6Module` you can set to `true` or `false`.


## Build WebAssembly module
Command `./X-Build.ps1 [-NoMinifyJsFiles] [-ReleaseMode]`   
This command allows to build project in Debug or Release configuration.
- Default configuration is Debug.
- All Emscripten pre/post js files on `./Js` are minified by default and generated in `./Bin/Js`.
- Generated module is located in `./Bin/Module`.

### Parameters
- ***NoMinifyJsFiles*** Optional. Allows skipping minify js files located in `./Js`. 
- ***ReleaseMode*** Optional. Apply Release configuration instead of Debug configuration.

## Run WebAssembly module
Command `./X-Run.ps1 [-NoLaunchBrowser]` or `./X-Run.ps1 -Emrun [-NoLaunchBrowser] `   
Starts an Emscripten Emrun web server for serve project files and launchs `./index.html` file on browser. This server type is not compatible with ES6 modules(*.mjs).

Command `./X-Run.ps1 -BrowserNode [-NoLaunchBrowser]`     
Starts a web server in NodeJs runtime for serve project files and launchs `./index.html` file on browser. ES6(*.mjs) or normal js(*.js) modules are compatible.

Command `./X-Run.ps1 -BrowserDeno [-NoLaunchBrowser]`     
Starts a web server in Deno runtime for serve project files and launchs `./index.html` file on browser. ES6(*.mjs) or normal js(*.js) modules are compatible.

Command `./X-Run.ps1 -ConsoleNode`       
Runs the module `./index.mjs` using NodeJs runtime. Only ES6 modules are compatible.

Command `./X-Run.ps1 -ConsoleDeno`       
Runs the module `./index.ts` using Deno runtime. Only ES6 modules are compatible.

### Parameters
***NoLaunchBrowser*** Optional. In conjunction with -Emrun, -BrowserNode, -BrowserDeno. `./index.html` is not launched in browser.  


## Publish WebAssembly module
Command `X-Publish.ps1`   
It creates an optimized ES6 or Js module on `./Dist` folder.   
e.g `./Dist/MyProject-1.0.0-Release.mjs` or `./Dist/MyProject-1.0.0-Release.js`


## Configure project
There are several configurations located in `./ProductInfo.json` that you can modifify.
- `Version` String. The product version. e.g. `MyProject`
- `IsES6Module` Boolean. True for ES6 module(*.mjs) or False for normal Js(*.js)
- `NodeHttpServerPort` Int. Port for web server.
- `DenoHttpServerPort` Int. Port for web server.
- `EmrunHttpServerPort` Int. Port for web server.
- `ConsoleNodeOptions` Additional options for send to NodeJs runtime.
- `ConsoleDenoOptions` Additional options for send to Deno runtime.
- `InsaneVersion` String. Insane version to find Insane in `<USER_DIRECTORY>/.CppLibs`

There are several files to modify and test your module functionality.
- `index.html` for Browser.
- `index.mjs` for NodeJs runtime.
- `index.ts` for Deno runtime.

> Caution: Do not modify the sections labeled as autogenerated code in the aforementioned files, as they regenerate automatically with each build. One section contains markers that the build script detects to generate the code and function correctly when invoking the run script.