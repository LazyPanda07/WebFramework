# WebFramework
* C++ HTTP/HTTPS server


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


| Language | [WebFramework API](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-API) | [Executors API](https://github.com/LazyPanda07/WebFramework/wiki/Executors-API) | Windows                     | Linux                       | Linux AArch64               | Android AArch64             |
| -------- | :-----------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------: | :-------------------------: | :-------------------------: | :-------------------------: | :-------------------------: |
| C++      | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> |
| C        | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/check.svg"> |
| C#       | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/x.svg">     | <img src="icons/x.svg">     |
| Python   | <img src="icons/check.svg">                                                           | <img src="icons/check.svg">                                                     | <img src="icons/check.svg"> | <img src="icons/check.svg"> | <img src="icons/x.svg">     | <img src="icons/x.svg">     |
| Flutter  | <img src="icons/check.svg">                                                           | <img src="icons/x.svg">                                                         | <img src="icons/check.svg"> | <img src="icons/x.svg">     | <img src="icons/x.svg">     | <img src="icons/check.svg"> |


## Build
```console
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary .. && cmake --build . -j --config Release && cmake --install . --config Release
```


## Samples
[Samples available](https://github.com/LazyPanda07/WebFramework/tree/master/samples) for Windows and Linux in C/C++, C# .NET 8.0, Python


## Getting Started Application
[Simple example of web application with ```WebFramework```](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-getting-started)
