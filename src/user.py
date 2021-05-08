"""This module -user- will contain the neccessary methods of an user
for interacting with the bank.
"""


class Person:
    def __init__(self, name, direction, phone_number):
        self._name = name
        self._direction = direction
        self._phone_number = phone_number

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
    def __init__(self, name, direction, phone_number, username, password):
        super().__init__(name, direction, phone_number)
        self._username = username
        self.__password = password
        
    def get_username(self):
        return self._username
    
    def get_full_info(self):
        data_show = super().get_full_info()
        data_show.append(self._username)
        return data_show
