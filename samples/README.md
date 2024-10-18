# Samples

## Requirements
* CMake >= 3.27.0
* Compiler with C++20 support(MSVC, GCC, Clang)
* Python(-DPYTHON_SAMPLE=ON)
* .NET 8.0(-DCSHARP_SAMPLE=ON)

## Build
* Get shared WebFramework distribution([pip package](https://pypi.org/project/web-framework-api/), [nuget](https://www.nuget.org/packages/WebFrameworkCSharpAPI/), [deb package](https://github.com/LazyPanda07/WebFramework/wiki/API#linux), [download from Github releases](https://github.com/LazyPanda07/WebFramework/releases/latest), [build from source](https://github.com/LazyPanda07/WebFramework?tab=readme-ov-file#getting-started-build))
* Run build commands from samples directory
	* Windows
	```cmd
	mkdir build
	cd build
	cmake -DWEB_FRAMEWORK_SDK=<path to WebFramework directory> -DWEB_FRAMEWORK_DIRECTORY=<path to directory with WebFramework.cmake file> -G "NMake Makefiles" ..
	nmake install
	```
	* Linux
	```bash
	mkdir build
	cd build
	cmake -DWEB_FRAMEWORK_SDK=<path to WebFramework directory> -DWEB_FRAMEWORK_DIRECTORY=<path to directory with WebFramework.cmake file> ..
	make install -j $(nproc)
	```
---
**NOTE**

By default all samples will be builded. If you want to build specific sample you need to disable all other samples.
* -DPYTHON_SAMPLE=ON (by default)
* -DCXX_SAMPLE=ON (by default)
* -DCC_SAMPLE=ON (by default)
* -DCSHARP_SAMPLE=ON (by default)
---
