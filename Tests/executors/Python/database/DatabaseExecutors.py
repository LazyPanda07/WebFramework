import random
import time
import uuid

from web_framework_api import *


class CRUDExecutor(StatelessExecutor):
    def do_get(self, request, response):
        database = request.get_database("test_database")
        table = database.get_table("test_table")
        result = table.execute("SELECT * FROM test_table WHERE name = ?", make_sql_values("glue"))
        data = []

        for value in result:
            data.append({
                "id": value["id"].get(),
                "name": value["name"].get(),
                "amount": value["amount"].get()
            })

        response.set_body({
            "data": data
        })

    def do_post(self, request, response):
        request.get_or_create_database("test_database").get_or_create_table(
            "test_table",
            "CREATE TABLE IF NOT EXISTS test_table ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name VARCHAR(255) NOT NULL, "
            "amount INTEGER NOT NULL)"
        )

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
        data = []

        table.execute(
            "UPDATE test_table "
            "SET name = ? "
            "WHERE amount = ?",
            make_sql_values("empty", -1)
        )

        result = table.execute(
            "SELECT * FROM test_table WHERE name = ?",
            make_sql_values("empty")
        )

        for value in result:
            data.append({
                "id": value["id"].get(),
                "name": value["name"].get(),
                "amount": value["amount"].get()
            })

        response.set_body({
            "data": data
        })

    def do_delete(self, request, response):
        request.get_database("test_database").get_table("test_table").execute("DROP TABLE test_table")


class MultiUserExecutor(HeavyOperationStatefulExecutor):
    def __init__(self):
        super().__init__()
        self._user_id = str(uuid.uuid4())

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

    def do_post(self, request, response):
        request.get_or_create_database("test_database").get_or_create_table("multi_user",
                                                                            "CREATE TABLE IF NOT EXISTS multi_user (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id TEXT NOT NULL, data TEXT NOT NULL)")

    def do_put(self, request, response):
        table = request.get_table("test_database", "multi_user")

        table.execute(
            "INSERT INTO multi_user (user_id, data) VALUES(?, ?)",
            make_sql_values(self._user_id, request.get_json()["data"])
        )
