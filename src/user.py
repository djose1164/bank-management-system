"""This module -user- will contain the neccessary methods of an user
for interacting with the bank.
"""
from database import db

class Person:
    """Base class for User. Here are implemented some basic methods for
    inheretance.
    """
    def __init__(self, name: str, direction: str, phone_number: int):
        """Create a new person with some required attribute.

        Args:
            name (str): The person's name.
            direction (str): The person's direction.
            phone_number (int): The peron's phone number.
        """
        self._name: str = name
        self._direction: str = direction
        self._phone_number: int = phone_number

    def get_name(self):
        return self._name

    def get_direction(self):
        return self._direction

    def get_phone_number(self):
        return self._phone_number

    def get_full_info(self):
        return [
            self._name,
            self._direction,
            self._phone_number,
        ]


class User(Person):
    def __init__(
        self, name: str, direction: str, phone_number, 
        username: str, password: str
    ):
        super().__init__(name, direction, phone_number)
        self._username: str = username
        self.__password: str = password

    def get_username(self):
        return self._username

    def get_full_info(self):
        """Get the full info from Person class and add to it the username.

        Returns:
            list: A list containing the full info of this user.
        """
        data_show: list = super().get_full_info()
        data_show.append(self._username)
        return data_show
