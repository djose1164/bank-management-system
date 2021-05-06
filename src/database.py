"""Bank Management System by Lusecita.

brief: Implementation of the database using MySQL.

Copyright© 2021 Lusecita Malvadita. 
"""
import sqlite3


class Database:
    def __init__(self, database_name="test.db", table=None):
        self._database_name = database_name
        if not table:
            table = """CREATE TABLE IF NOT EXISTS users(
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username CHAR(20), 
                    password CHAR(20))
                    """
        self.create_table(table)

    def see(self):
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute("SELECT * FROM users")
                data = cur.fetchall()
                print(data)
            except Exception as e:
                raise e

    def create_table(self, sql: str):
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute(sql)
            except Exception as e:
                raise e

    def create_new_user(self, username: str, password: str):
        """To create a new user inside the database.

        Args:
            username (str): The new user's name.
            password (str): The new user's password.
        """
        sql = "INSERT INTO users VALUES(NULL, ?, ?)"
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute(
                    sql,
                    (
                        username,
                        password,
                    ),
                )
                conn.commit()
                return True
            except Exception as e:
                print(f"## create_new_user(): {e}")
                return False


    def validate_user(self, username: str, password: str):
        sql = """
              SELECT username, password
              FROM users
              """
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute(sql)
                data = cur.fetchall()
                # Check is the username and password are in the given data.
                for usrname, psswd in data:
                    if username == usrname and password == psswd:
                        return True
                    else:
                        return False
            except Exception as e:
                pass


db = Database()

#initialize_database()    