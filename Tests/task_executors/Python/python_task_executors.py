from web_framework_api import *


class PythonTestTaskExecutor(TaskExecutor):
    def __call__(self, data):
        with open(data["fileName"], "w") as file:
            file.write(data["message"])


class CXXTaskSerializer(TaskSerializerCxx):
    def get_task_name(self):
        return "CXXTestTaskExecutor"

    def serialize_arguments(self):
        return {
            "fileName": "python_cxx.txt",
            "message": "Python message"
        }


class CCTaskSerializer(TaskSerializerCc):
    def get_task_name(self):
        return "c_test_task_executor"

    def serialize_arguments(self):
        return {
            "fileName": "python_cc.txt",
            "message": "Python message"
        }


class PythonTaskSerializer(TaskSerializer):
    def get_task_name(self):
        return "PythonTestTaskExecutor"

    def serialize_arguments(self):
        return {
            "fileName": "python_python.txt",
            "message": "Python message"
        }


class CSharpTaskSerializer(TaskSerializerCSharp):
    def get_task_name(self):
        return "CSharpTestTaskExecutor"

    def serialize_arguments(self):
        return {
            "fileName": "python_csharp.txt",
            "message": "Python message"
        }


class PythonExecutor(StatelessExecutor):
    def do_get(self, request, response):
        request.enqueue_task(Internal, CXXTaskSerializer)

        request.enqueue_task(Internal, CCTaskSerializer)

        request.enqueue_task(Internal, PythonTaskSerializer)

        request.enqueue_task(Internal, CSharpTaskSerializer)

        response.set_body("Accepted")
        response.set_response_code(ResponseCodes.ACCEPTED)
