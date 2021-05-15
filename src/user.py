"""This module -user- will contain the neccessary methods of an user
for interacting with the bank.
"""
import ctypes
import database



class Person:
    """Base class for User. Here are implemented some basic methods for
    inheretance.
    """

    def __init__(self, name: str, address: str, phone_number: int):
        """Create a new person with some required attribute.

        Args:
            name (str): The person's name.
            address (str): The person's address.
            phone_number (int): The peron's phone number.
        """
        self._name: str = name
        self._address: str = address
        self._phone_number: int = phone_number

    def get_name(self):
        return self._name

    def get_address(self):
        return self._address

    def get_phone_number(self):
        return self._phone_number

    def get_full_info(self):
        return [
            self._name,
            self._address,
            self._phone_number,
        ]


class User:
    """For the manage of the bank."""

    def __init__(self, username: str, password: str):
        self._username: str = username
        self.__password: str = password
        self._id = database.db.load_id(username, password)
        print(f"## id: {self._id}")

    @property
    def get_username(self):
        return self._username
    
    @property
    def get_password(self):
        return self.__password
    
    @property
    def get_id(self):
        return self._id
    
user = None
def init_user(username, password):
    global user
    user = User(username, password)
