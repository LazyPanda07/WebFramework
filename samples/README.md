# Samples

## Requirements
* CMake >= 3.27.0
* Compiler with C++20 support(MSVC, GCC, Clang)

## Build
* Get shared WebFramework distribution(pip package, nuget, deb package, download from Github releases, build from source)
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
