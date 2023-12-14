# InsaneEmscripten

This project helps for:

1. Build Botan library for Emscripten. 
2. Build Insane library for Emscripten.
3. Create an empty Emscripten project with the compilation script and the run script. This project uses Insane as a dependency.

## Prerequisites 
Install the following packages in your operating system.

- [Python](https://www.python.org/)
- [Make](https://www.gnu.org/software/make)
- [Git](https://git-scm.com/)
- [OpenJDK](https://jdk.java.net/) or [OracleJDK](https://www.oracle.com/java/technologies/downloads/)
- [Node.js](https://nodejs.org/en)
- [Deno](https://deno.com/)
- [Wsl - Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/install)*   
- [Make for Windows](https://gnuwin32.sourceforge.net/packages/make.htm)*   

\* = Windows only.

If your OS is Windows, you need to install the same packages in WSL. This is required for building Botan due to incompatibilities with the Botan script on Windows platforms.

## Get everything up and running
Steps

### 1. Build Botan (libBotan.a)
Run ```./X-InsaneEm-BuildBotan.ps1```

Take a 30 mins coffee.

Compiled libs are copied in `USER_DIRECTORY/.CppLibs/`:  
For Windows `$env:USERPROFILE/.CppLibs/`   
For Linux `$env:HOME/.CppLibs/`

> Warning!!! Running `./X-InsaneEm-BuildBotan.ps1` removes the `<USER_DIRECTORY>/.CppLibs/Botan-$BotanVersion-Debug-Emscripten` and `<USER_DIRECTORY>/.CppLibs/Botan-$BotanVersion-Release-Emscripten` folder everytime that it is executed. Copy your unsaved changes to another location. Version is obtained from `Docs/ProductInfo.json`.

### 2. Build Insane (libInsane.a)
Run ```./X-InsaneEm-BuildInsane.ps1```

Compiled libs are copied in `USER_DIRECTORY/.CppLibs/`:  
For Windows `$env:USERPROFILE/.CppLibs/`   
For Linux `$env:HOME/.CppLibs/`

> Warning!!! Running `./X-InsaneEm-BuildInsane.ps1` removes the `<USER_DIRECTORY>/.CppLibs/Insane-Debug-Emscripten` and `<USER_DIRECTORY>/.CppLibs/Insane-Release-Emscripten` folder everytime that it is executed. Copy your unsaved changes to another location. Version is obtained from `Docs/ProductInfo.json`.

**Parameters**

*-Clean*: Deletes previously compiled files and start the compilation process from scratch.

Compiled libs are copied in:  
For Windows `$env:USERPROFILE/.CppLibs/`   
For Linux `$env:HOME/.CppLibs/`

### 3. Create project 
Run `./X-InsaneEm-CreateProject.ps1`

The created project is located by default on `Dist/$Name`,
`$Name` is the name of the project. This name can be configured in `Docs/ProductInfo.json` on the property `Name`. This name can be configured later on the created project.

For example if `Name` is "MyProject" in `Docs/ProductInfo.json`, the project is created on ```Dist/MyProject```


> Warning!!! Running `./X-InsaneEm-CreateProject.ps1` removes the `Dist/MyProject` folder everytime that it is executed. Copy your unsaved changes to another location.

### 4. Build created project 
 
1. Run `cd Dist/MyProject`
2. Run `./ThisBuild.ps1`   
**Parameters**  
*-NoMinifyJsFiles*: Don´t minify js files located in `Dist/MyProject/Js`. Those are used in build step.   
*-ReleaseMode*: Apply -O3 compiler optimization in the code. You can change this value in the script `Dist\MyProject\ThisBuild.ps1` on `$RELEASE_OPTIMIZATION` variable. Optionally, you can change the value for debug with the `$DEBUG_OPTIMIZATION` variable."

You can change module type(normal script or ES6 module) in `Docs/ProductInfo.json`, the property `IsES6Module` you can set to `true` or `false`.

### 5. Run created project.
1. Run `cd Dist/MyProject` 
2. Run `./ThisRun.ps1`   
**Parameters**  
*-Emrun*: Default option. Serve project using Emscripten Emrun. `Index.html`is launched in browser. Not compatible with ES6 modules.  
*-BrowserNodeServer*: Serve project using `Server/NodeHttpServer.mjs`. `Index.html`is launched in browser.     
*-BrowserDenoServer*: Serve project using `Server/DenoHttpServer.ts`. `Index.html`is launched in browser.      
*-ConsoleNode*: Run "index.mjs" using [Node.js](https://nodejs.org/en) runtime.     
*-ReleaseMode*: Run "index.ts" using [Deno](https://deno.com/) runtime.      
*-NoLaunchBrowser*: In conjunction with -Emrun, -BrowserNodeServer, BrowserDenoServer. `index.html` is not launched in browser.  

