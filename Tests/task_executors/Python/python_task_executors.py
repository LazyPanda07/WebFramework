from web_framework_api import *


class PythonTestTaskExecutor(TaskExecutor):
    def __call__(self, data):
        with open("python_task_executor.txt", "w") as file:
            file.write(data["message"])
