"""Bank Management System by Lusecita.

brief: Implementation of the database using MySQL.

Copyright© 2021 Lusecita Malvadita. 
"""
import sqlite3
import ctypes


class Database:
    """For manage the database. There're listed the useful method for the app
    bellow.
    """

    def __init__(self, database_name="test.db", table=None):
        self._database_name = database_name
        if not table:
            table = """CREATE TABLE IF NOT EXISTS users(
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    username CHAR(20), 
                    password CHAR(20))
                    """
        self.create_table(table)
        c_lib = ctypes.CDLL(
            "/home/djose1164/Desktop/Bank-Management-System-by-Lusecita/lib/database.so"
        )
        self.create_new_user = c_lib.add_user
        self.validate_user = c_lib.validate


    def see(self):
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute("SELECT * FROM users")
                print(cur.fetchall())
            except Exception as e:
                raise e


db = None


def main():
    """Instantiace db (Database class) object"""
    global db
    db = Database()
    db.see()


if __name__ == "__main__":
    main()
