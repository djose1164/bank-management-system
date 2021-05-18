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

        self._current_database.save_new_deposit(self._current_user.get_id, cash)

    def make_loan(self, cash: float):
        self._current_database.save_new_loan(
            self._current_user.get_id,
            cash,
        )

    def load_data_user(self):
        return database.db.get_data_by_id(self._current_user.get_id)

    def cash_transaction(self, to_id: int, cash: float):
        print(f"## id: {to_id}" f"## cash: {cash}")
        self._current_database.save_cash_transaction(
            self._current_user.get_id, to_id, cash
        )

    def pay_loan(self, cash: float):
        return self._current_database.save_payment_loan(
            self._current_user.get_id, cash
        )
    
    @property
    def get_total_loan(self):
        return self._current_database.get_total_loan(self._current_user.get_id)

bank = None


def init_bank(user):
    global bank
    print("## Bank's module ", user)
    bank = Bank(user)
