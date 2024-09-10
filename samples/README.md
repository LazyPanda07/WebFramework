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
	cmake -DWEB_FRAMEWORK_DIRECTORY=path to directory with WebFramework.cmake file -G "NMake Makefiles" ..
	nmake install
	```
	* Linux
	```bash
	mkdir build
	cd build
	cmake -DWEB_FRAMEWORK_DIRECTORY=path to directory with WebFramework.cmake ..
	make install -j $(nproc)
	```
