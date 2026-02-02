# WebFramework
* C++ HTTP/HTTPS server
* Multi language API
* Server can be launched from ```C++```, ```C```, ```Python```, ```C#```, ```Flutter```
* Endpoints can be served from ```C++```, ```C```, ```Python```, ```C#```


## Getting Started Build
Start by cloning the repository
```console
git clone https://github.com/LazyPanda07/WebFramework.git --recursive
```


| Language | [WebFramework API](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-API) | [Executors API](https://github.com/LazyPanda07/WebFramework/wiki/Executors-API) | Windows                     | Linux                       | Linux AArch64               | Android AArch64             |
| -------- | :-----------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------: | :-------------------------: | :-------------------------: | :-------------------------: | :-------------------------: |
| C++      | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> |
| C        | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> |
| C#       | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/x.svg">     | <img src="icons/x.svg">     |
| Python   | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/x.svg">     | <img src="icons/x.svg">     |
| Flutter  | <img src="icons/check.svg">                                                           | <img src="icons/x.svg">                                                         | <img src="icons/check.svg"> | <img src="icons/x.svg">     | <img src="icons/x.svg">     | <img src="icons/check.svg"> |


## Build Requirements
* CMake >= 3.27.0
* Compiler with C++20 support(MSVC, GCC, Clang)
* With Python embed requires ```Python >= 3.12```
* With .NET embed requires ```.NET 10.0```
### Windows
* Windows >= 10
### Linux
* uuid-dev


## Build
* Create ```build``` inside cloned directory
* Run all commands from ```build``` directory
* Default build
```console
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary .. 
cmake --build . -j --config Release
cmake --install . --config Release
```
* For generating configure command and getting download link use this [site](https://lazypanda07.github.io/WebFramework/)

## Samples
[Samples available](https://github.com/LazyPanda07/WebFramework/tree/master/samples) for Windows and Linux in C/C++, C# .NET 10.0, Python


## Getting Started Application
[Simple example of web application with ```WebFramework```](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-getting-started)


## Project Templates
You can use this repositories as templates with [cookiecutter](https://github.com/cookiecutter/cookiecutter)
* [Visual Studio](https://github.com/WebFrameworkStore/WebFrameworkVisualStudioProjectTemplate)
* [CMake](https://github.com/WebFrameworkStore/WebFrameworkCMakeProjectTemplate)


---
**NOTE**

.NET 10.0 requires ```hostfxr.dll``` in ```PATH``` for Windows and ```libhostfxr.so``` in ```LD_LIBRARY_PATH``` for Linux
---
