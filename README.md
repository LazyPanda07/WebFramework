# WebFramework
* A web server that follows the principles of ease of use and ease of development
* Supports easy scalability
* Allows parallel development(each member can develop single isolated module)
* Server functionality development available on ```C++ in Windows and Linux```
* Server running API available on [C++ Windows and Linux](https://github.com/LazyPanda07/WebFramework/wiki/API#c), [C Windows and Linux](https://github.com/LazyPanda07/WebFramework/wiki/API#c-1), [Python](https://github.com/LazyPanda07/WebFramework/wiki/API#python), [C# .NET 8.0](https://github.com/LazyPanda07/WebFramework/wiki/API#c-net-80), [Flutter (Windows, Android)](https://github.com/LazyPanda07/WebFramework/wiki/API#flutter-windows-android)

## Getting Started Build
Start by cloning the repository
```console
git clone https://github.com/LazyPanda07/WebFramework.git WebFrameworkLibrary --recursive
```

## Build requirements
* CMake >= 3.27.0
* Compiler with C++20 support(MSVC, GCC, Clang)
### Windows
* Windows >= 10
### Linux
* uuid-dev

## Windows
### Build
1. Visual Studio
* Create ```build``` directory
* Run command from ```build``` directory
```cmd
cmake -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary ..
```
* Open ```build\WebFramework.sln```
* Select ```Release``` configuration
* In ```Solution Explorer``` build INSTALL project(expand ```CMakePredefinedTargets```)
* ```WebFramework``` will be installed in ```WebFrameworkLibrary``` directory in project directory

2. NMake
* Create ```build``` directory
* Run command from ```build``` directory
```cmd
cmake -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary -G "NMake Makefiles" .. && nmake install
```
***NMake doesn't support multithreaded builds***
* ```WebFramework``` will be installed in ```WebFrameworkLibrary``` directory in project directory

### Samples
[Samples available](https://github.com/LazyPanda07/WebFramework/tree/master/samples) for Windows and Linux in C/C++, C# .NET 8.0, Python

### Use as project in Visual Studio(deprecated)
Run ```WebFrameworkBuilder.exe``` and choose your .sln file and .vcxproj file. This program modify your files (added dependencies, link with WebFrameworkLibrary).

#### WebFrameworkBuilder(deprecated)
Link to [WebFrameworkBuilder](https://github.com/LazyPanda07/WebFrameworkBuilder)

## Linux
### Build
* Create ```build``` directory
* Run command from ```build``` directory
```bash
cmake -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary .. && make install -j $(nproc)
```
* ```WebFramework``` will be installed in ```WebFrameworkLibrary``` directory in project directory

## Getting Started Application
[Simple example of web application with ```WebFramework```](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-getting-started)

## WebFramework reference
[Reference link](https://lazypanda07.github.io/WebFramework/)
