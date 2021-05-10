"""Bank Management System by Lusecita.

brief: Implementation of the database using MySQL.

Copyright© 2021 Lusecita Malvadita. 
"""
import sqlite3


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

    def load_id(self):
        """Return the loaded data from database.

        Raises:
            e: Error that happened trying to fetch data.

        Returns:
            dict: The dict with the fetched data.
        """
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute("SELECT id FROM users")
                return cur.fetchall()[0]
            except Exception as e:
                raise e

    def create_table(self, sql: str):
        """Create a table with the given sql statement.

        Args:
            sql (str): Must be the sql code to create the new table.

        Raises:
            e: The error cought.
        """
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
    
if __name__ == '__main__':
    main()
    
