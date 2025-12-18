# C# Executors


## Create Executor
1. Inherit from some executor subclass such as ```StatelessExecutor```
2. Service endpoint with overriding specific HTTP method: ```Do<Get, Post, ...>(HttpRequest request, HttpResponse response)```
3. Executor must be without namespace
