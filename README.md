# UntitledRuntimeLibraryLoader
A cross platform runtime shared object loader(windows and unix).
## Installation an usage
The library is the `urll.cpp` and `urll.h` files, compile them into your project you're good to go. The library provides a Unix-like version of the `dlopen`, `dlsym`, `dlerror` and `dlclose` functions and they work in pretty much the same way but are abstracted to also work on windows. The functions can be found under the `URLL` namespace

The library has a macro called `URLL_USE_FUNCTIONAL` which can enable/disable a version of `dlsym` that supports `std::function`. All you need to do is define it with like this:
```cpp
#define URLL_USE_FUNCTIONAL
#include "urll.h"
```
### Requirements
As always on the library end, you should remove any name mangling from the C++ compiler by using `extern "C"` and compile in a position independent way, on windows add `__declspec(dllexport)` between your function type and name when declaring a function. Here is an example library header file:
```cpp
#pragma once

extern "C"
{
	void __declspec(dllexport) begin();
	void __declspec(dllexport) end();
}
```
