"""
Implementation of bank module.
"""
import database


class Bank:
    def __init__(self, user, available_cash: float = 500000) -> None:
        self._current_user = user
        self._current_database = database.db


    def make_deposit(self, cash: float):
        """Save a new deposit into the bank.

        The information needed will be taken from the current user.
        """

        self._current_database.save_new_deposit(
            self._current_user.get_id, 
            cash
        )


bank = None


def init_bank(user):
    global bank
    print("## Bank's module ", user)
    bank = Bank(user)
