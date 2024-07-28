# WebFramework
* A web server that follows the principles of ease of use and ease of development
* Supports easy scalability
* Allows parallel development(each member can develop single isolated module)

## Getting Started Build
Start by cloning the repository
```console
git clone https://github.com/LazyPanda07/WebFramework.git WebFrameworkLibrary --recursive
```

## Requirements
* CMake >= 3.27.0
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

### Use as project in Visual Studio
Run ```WebFrameworkBuilder.exe``` and choose your .sln file and .vcxproj file. This program modify your files (added dependencies, link with WebFrameworkLibrary).

#### WebFrameworkBuilder
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
