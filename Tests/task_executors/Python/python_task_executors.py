from web_framework_api import *


class PythonTestTaskExecutor(TaskExecutor):
    def execute(self, data, context):
        with open(data["fileName"], "w") as file:
            file.writelines(
                [
                    "From PythotTestTaskExecutor",
                    data["message"]
                ]
            )


class CXXTaskSerializer(TaskSerializerCxx):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self._suffix = kwargs["suffix"]

    def get_task_executor_name(self):
        return "CXXTestTaskExecutor"

    def serialize_arguments(self):
        return {
            "fileName": f"python_cxx_{self._suffix}.txt",
            "message": "Python message"
        }


class CCTaskSerializer(TaskSerializerCc):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self._suffix = kwargs["suffix"]

    def get_task_executor_name(self):
        return "c_test_task_executor"

    def serialize_arguments(self):
        return {
            "fileName": f"python_cc_{self._suffix}.txt",
            "message": "Python message"
        }


class PythonTaskSerializer(TaskSerializer):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self._suffix = kwargs["suffix"]

    def get_task_executor_name(self):
        return "PythonTestTaskExecutor"

    def serialize_arguments(self):
        return {
            "fileName": f"python_python_{self._suffix}.txt",
            "message": "Python message"
        }


class CSharpTaskSerializer(TaskSerializerCSharp):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        self._suffix = kwargs["suffix"]

    def get_task_executor_name(self):
        return "CSharpTestTaskExecutor"

    def serialize_arguments(self):
        return {
            "fileName": f"python_csharp_{self._suffix}.txt",
            "message": "Python message"
        }


class PythonExecutor(StatelessExecutor):
    def do_get(self, request, response):
        task_broker = request.get_json()["taskBroker"]

        if task_broker == Internal.task_broker_name:
            request.enqueue_task(Internal, CXXTaskSerializer, suffix="internal")
            request.enqueue_task(Internal, CCTaskSerializer, suffix="internal")
            request.enqueue_task(Internal, PythonTaskSerializer, suffix="internal")
            request.enqueue_task(Internal, CSharpTaskSerializer, suffix="internal")
        elif task_broker == RabbitMQ.task_broker_name:
            request.enqueue_task(RabbitMQ, CXXTaskSerializer, suffix="rabbitmq")
            request.enqueue_task(RabbitMQ, CCTaskSerializer, suffix="rabbitmq")
            request.enqueue_task(RabbitMQ, PythonTaskSerializer, suffix="rabbitmq")
            request.enqueue_task(RabbitMQ, CSharpTaskSerializer, suffix="rabbitmq")
        else:
            request.throw_exception(f"Wrong taskBroker name: {task_broker}", ResponseCodes.BAD_REQUEST)

        response.set_body("Accepted")
        response.set_response_code(ResponseCodes.ACCEPTED)
