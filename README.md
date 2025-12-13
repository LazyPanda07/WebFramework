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


| Language | [WebFramework API](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-API) | [Executors API](https://github.com/LazyPanda07/WebFramework/wiki/Executors-API) | Windows | Linux | Linux AArch64 | Android AArch64 |
| -------- | :-----------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------: | :-----: | :---: | :-----------: | :-------------: |
| C++      | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> |
| C        | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> |
| C#       | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> |
| Python   | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> |
| Flutter  | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M6 6 L18 18 M18 6 L6 18" stroke="#ff4d4d" stroke-width="2.5" stroke-linecap="round"/></svg> | <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="16" height="16"><path d="M5 13 L9.5 17.5 L19 7" fill="none" stroke="#3fb950" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"/></svg> |


## Build
```console
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../WebFrameworkLibrary .. && cmake --build . -j --config Release && cmake --install . --config Release
```


## Samples
[Samples available](https://github.com/LazyPanda07/WebFramework/tree/master/samples) for Windows and Linux in C/C++, C# .NET 8.0, Python


## Getting Started Application
[Simple example of web application with ```WebFramework```](https://github.com/LazyPanda07/WebFramework/wiki/WebFramework-getting-started)
