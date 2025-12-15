# Samples


## Requirements
* CMake >= 3.27.0
* Compiler with C++20 support(MSVC, GCC, Clang)
* Python 3.14(if -DPYTHON_SAMPLE=ON)
* .NET 8.0(if -DCSHARP_SAMPLE=ON)


## Build
* Get shared WebFramework distribution([pip package](https://pypi.org/project/web-framework-api/), [nuget](https://www.nuget.org/packages/WebFrameworkCSharpAPI/), [deb package](https://github.com/LazyPanda07/WebFramework/wiki/API#linux), [download from Github releases](https://github.com/LazyPanda07/WebFramework/releases/latest), [build from source](https://github.com/LazyPanda07/WebFramework?tab=readme-ov-file#getting-started-build))
* Run build commands from samples directory
```console
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release  ..
cmake --build . -j --config Release
cmake --install . --config Release
```


---
**NOTE**

Default options:
* -DWEB_FRAMEWORK_SDK=../WebFrameworkLibrary
* -DWEB_FRAMEWORK_DIRECTORY=../WebFrameworkLibrary
* -DCXX_SAMPLE=ON
* -DCC_SAMPLE=ON
* -DPYTHON_SAMPLE=OFF
* -DCSHARP_SAMPLE=OFF
---
