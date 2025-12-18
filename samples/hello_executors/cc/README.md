# C Executors


## Create Executor
1. Define executor with: ```DEFINE_DEFAULT_EXECUTOR(executor_name, executor_type)```
2. Service endpoint with specific HTTP method: ```DEFINE_EXECUTOR_METHOD(executor_name, <DO, POST, ...>_METHOD, request_variable_name, response_variable_name)```
3. ```DEFINE_EXECUTOR_METHOD``` also has hidden parameter ```executor``` that can be casted to executor struct if defined with ```DEFINE_EXECUTOR(executor_name, executor_type)```
4. Needs to be defined once per shared library: ```DEFINE_INITIALIZE_WEB_FRAMEWORK()``` 
