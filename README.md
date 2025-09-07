# WebFramework
* A web server that follows the principles of ease of use and ease of development
* Supports easy scalability
* Allows parallel development(each team member can develop single isolated module)
* Server functionality development available on ```C++ in Windows and Linux```
* Server running API available on [C++ Windows and Linux](https://github.com/LazyPanda07/WebFramework/wiki/API#c), [C Windows and Linux](https://github.com/LazyPanda07/WebFramework/wiki/API#c-1), [Python](https://github.com/LazyPanda07/WebFramework/wiki/API#python), [C# .NET 8.0](https://github.com/LazyPanda07/WebFramework/wiki/API#c-net-80), [Flutter (Windows, Android)](https://github.com/LazyPanda07/WebFramework/wiki/API#flutter-windows-android)

## Getting Started Build
Start by cloning the repository
```console
git clone https://github.com/LazyPanda07/WebFramework.git --recursive
```

## Build Requirements
* CMake >= 3.27.0
* Compiler with C++20 support(MSVC, GCC, Clang)
### Windows
* Windows >= 10
### Linux
* uuid-dev

## Samples
[Samples available](https://github.com/LazyPanda07/WebFramework/tree/master/samples) for Windows and Linux in C/C++, C# .NET 8.0, Python

## Build
```console
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary .. && cmake --build . -j --config Release && cmake --install . --config Release
```

## Getting Started Application
[Simple example of web application with ```WebFramework```](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-getting-started)
