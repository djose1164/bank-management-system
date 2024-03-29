"""Bank Management System by Lusecita.

brief: Implementation of the database using MySQL.

Copyright© 2021 Lusecita Malvadita. 
"""
import sqlite3
from sqlite3 import Error
import os


class Database:
    """For manage the database. There're listed the useful method for the app
    bellow.
    """

    def __init__(self, database_name="test.db", table=None):
        self._database_name = database_name
        if table is None:
            table = """CREATE TABLE IF NOT EXISTS users(
                    id INTEGER PRIMARY KEY NOT NULL, 
                    username TEXT,
                    password TEXT,
                    deposit_count INT DEFAULT 0,
                    loan_count INT DEFAULT 0,
                    deposit_total DOUBLE DEFAULT 0.0,
                    loan_total DOUBLE DEFAULT 0.0,
                    euros DOUBLE DEFAULT 0.0,
                    dollars DOUBLE DEFAULT 0.0,
                    object TEXT DEFUALT NULL);
                    """
        self.create_table(table)

    def load_id(self, username: str, password: str):
        """Return the loaded data from database.

        Raises:
            e: Error that happened trying to fetch data.

        Returns:
            dict: The dict with the fetched data.
        """
        sql = """SELECT id FROM users 
              WHERE username = ? AND password = ?"""
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
                fetched_data = cur.fetchall()
                if fetched_data:
                    return fetched_data[0][0]
            except Error as e:
                print(e)

        return False

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
            except Error as e:
                raise e

    def create_new_user(self, username: str, password: str):
        """To create a new user inside the database.

        Args:
            username (str): The new user's name.
            password (str): The new user's password.
        """
        sql = "INSERT INTO users(id, username, password) VALUES(NULL, ?, ?)"
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
            except Error as e:
                os.system("clear")

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
            except Error as e:
                pass

        return False

    def save_new_deposit(self, id: int, cash: float):
        """Save a new deposit into the database. The requested information
        must be passed from the current user.

        Args:
            id (int): The user's id making the deposit.
            cash (float): The amount that the user want to deposit.
        """
        sql = """UPDATE users
              SET deposit_count = deposit_count + 1,
              deposit_total = deposit_total + ?
              WHERE id = ?"""
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute(
                    sql,
                    (
                        cash,
                        id,
                    ),
                )
                return True
            except Error as e:
                print(e)
                return e

    def save_new_loan(self, id: int, cash: float):
        sql = """UPDATE users
              SET loan_count = loan_count + 1,
              loan_total = loan_total + ?
              WHERE id = ?"""
        with sqlite3.connect(self._database_name) as conn:
            cur = conn.cursor()
            cur.execute(
                sql,
                (
                    cash,
                    id,
                ),
            )

    def save_cash_transaction(self, from_id: int, to_id: int, cash: float):
        if self.verify_id(to_id):
            # SQL query: If the id matches, and if the current user has enough
            # money
            sql = """UPDATE users
            SET deposit_total = deposit_total - ?
            WHERE id = ?  
            AND (SELECT deposit_total FROM users WHERE id = ?) > ?;
            """
            with sqlite3.connect(self._database_name) as conn:
                cur = conn.cursor()
                cur.execute(sql, (cash, from_id, from_id, cash,))
                cur.execute(
                    "UPDATE users SET deposit_total = deposit_total + ? WHERE id = ?",
                    (
                        cash,
                        to_id,
                    ),
                )
                
    def save_payment_loan(self, id: int, cash: float):
        sql = """UPDATE users 
        SET loan_total = loan_total - ?, deposit_total = deposit_total - ? 
        WHERE id = ? AND (SELECT deposit_total FROM users WHERE id = ?) > ?
        """
        with sqlite3.connect(self._database_name) as conn:
            cur = conn.cursor()
            cur.execute(sql, (cash, cash, id, id, cash,))
            if cur.rowcount:
                return True
            
        return False

    def see(self):
        with sqlite3.connect(self._database_name) as conn:
            try:
                cur = conn.cursor()
                cur.execute("SELECT * FROM users")
                print(cur.fetchall())
            except Error as e:
                raise e

    def get_data_by_id(self, id: int):
        sql = """SELECT deposit_count, loan_count, deposit_total, loan_total, 
        euros, dollars, object
        FROM users
        WHERE id = ?"""
        with sqlite3.connect(self._database_name) as conn:
            cur = conn.cursor()
            cur.execute(sql, (id,))
            data = cur.fetchone()
            return data

    def verify_id(self, id: int):
        with sqlite3.connect(self._database_name) as conn:
            cur = conn.cursor()
            cur.execute("SELECT id FROM users WHERE id = ?", (id,))
            if cur.fetchone():
                return True

        return False

    def get_total_loan(self, id: int):
        with sqlite3.connect(self._database_name) as conn:
            cur = conn.cursor()
            cur.execute("SELECT loan_total FROM users WHERE id = ?", (id,))
            data = cur.fetchone()
            return data[0]

db = None


def main():
    """Instantiace db (Database class) object"""
    global db
    db = Database()
    db.see()
    #db.save_cash_transaction(2, 1, 107)
    #db.see()


if __name__ == "__main__":
    main()
