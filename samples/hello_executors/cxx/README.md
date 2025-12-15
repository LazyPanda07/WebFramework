# C++ Executors


## Create Executor
1. Inherit from some executor subclass such as ```StatelessExecutor```
2. Service endpoint with overriding specific HTTP method: ```do<Get, Post, ...>(HttpRequest request, HttpResponse response)```
3. Export executor from shared library with ```DEFINE_EXECUTOR(className)``` macro
