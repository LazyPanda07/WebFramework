import random
import time
import uuid

from typing import Dict

from web_framework_api import *


class TableData:
    @staticmethod
    def create(row: Dict[str, SqlValue]) -> "TableData":
        return TableData(
            row["id"].get_as_int(),
            row["name"].get_as_string(),
            row["amount"].get_as_int()
        )

    def __init__(self, id: int, name: str, amount: int):
        self.id = id
        self.name = name
        self.amount = amount


class CRUDExecutor(StatelessExecutor):
    def init(self, settings):
        settings.get_or_create_database("test_database").get_or_create_table(
            "test_table",
            "CREATE TABLE IF NOT EXISTS test_table ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name VARCHAR(255) NOT NULL, "
            "amount INTEGER NOT NULL)"
        )

    def do_get(self, request, response):
        response.set_body({
            "data": request.get_table("test_database", "test_table").execute("SELECT * FROM test_table WHERE name = ?",
                                                                             TableData, make_sql_values("glue"))
        })

    def do_put(self, request, response):
        database = request.get_database("test_database")
        table = database.get_table("test_table")

        random.seed(time.time())

        table.execute(
            "INSERT INTO test_table (name, amount) VALUES(?, ?)",
            make_sql_values("glue", -1)
        )

        for i in range(10):
            table.execute(
                "INSERT INTO test_table (name, amount) VALUES(?, ?)",
                make_sql_values("glue", random.randint(1, 200))
            )

    def do_patch(self, request, response):
        database = request.get_database("test_database")
        table = database.get_table("test_table")

        table.execute(
            "UPDATE test_table "
            "SET name = ? "
            "WHERE amount = ?",
            make_sql_values("empty", -1)
        )

        response.set_body({
            "data": table.execute("SELECT * FROM test_table WHERE name = ?", TableData, make_sql_values("empty"))
        })

    def do_delete(self, request, response):
        request.get_database("test_database").get_table("test_table").execute("DROP TABLE test_table")


class MultiUserExecutor(HeavyOperationStatefulExecutor):
    def __init__(self):
        super().__init__()
        self._user_id = str(uuid.uuid4())

    def init(self, settings):
        settings.get_or_create_database("test_database").get_or_create_table("multi_user",
                                                                             "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)")

    def do_get(self, request, response):
        table = request.get_table("test_database", "multi_user")
        data = []

        result = table.execute(
            "SELECT * FROM multi_user WHERE user_id = ?",
            make_sql_values(self._user_id)
        )

        for value in result:
            data.append({
                "id": value["id"].get(),
                "user_id": value["user_id"].get(),
                "data": value["data"].get(),
            })

        response.set_body({
            "data": data
        })

    def do_put(self, request, response):
        table = request.get_table("test_database", "multi_user")

        table.execute(
            "INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
            make_sql_values(self._user_id, request.get_json()["data"])
        )


class RedisExecutor(StatelessExecutor):
    def init(self, settings):
        without_redis_tests = os.getenv("WITHOUT_REDIS_TESTS", "OFF")

        if without_redis_tests == "OFF":
            settings.get_or_create_database("127.0.0.1:10010:password", RedisDatabase).get_or_create_table("", "")

    def do_get(self, request, response):
        connect = request.get_table("127.0.0.1:10010:password", "", RedisDatabase)
        result = {
            "string": next(iter(connect.execute("GET", make_sql_values("string"))[0].values()), None).get(),
            "int": next(iter(connect.execute("GET", make_sql_values("int"))[0].values()), None).get(),
            "double": next(iter(connect.execute("GET", make_sql_values("double"))[0].values()), None).get(),
            "bool": next(iter(connect.execute("GET", make_sql_values("bool"))[0].values()), None).get_as_bool(),
        }

        response.set_body(result)

    def do_put(self, request, response):
        connect = request.get_table("127.0.0.1:10010:password", "", RedisDatabase)

        connect.execute("SET", make_sql_values("string", "qwe"))
        connect.execute("SET", make_sql_values("int", 5))
        connect.execute("SET", make_sql_values("double", 2.3))
        connect.execute("SET", make_sql_values("bool", True))

        response.set_response_code(ResponseCodes.CREATED)
