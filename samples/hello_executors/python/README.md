# Python Executors


## Create Executor
1. Inherit from some executor subclass such as ```StatelessExecutor```
2. Service endpoint with overriding specific HTTP method: ```do_<get, post, ...>(request: HttpRequest, response: HttpResponse)```
