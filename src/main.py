"""Bank Management System by Lusecita.

brief: Implementation of GUI.

Copyright© 2021 Lusecita Malvadita.   
"""

# Kivy modules.
import kivy
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen, WipeTransition
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.properties import StringProperty, ObjectProperty, NumericProperty
from kivy.uix.recycleview import RecycleView
from kivy.lang import Builder

# Database modules.
import database
from database import main

import user

from bank import init_bank
import bank

import calc

kivy.require("1.11.1")

from os import listdir

kv_path = "./kv/"
for kv in listdir(kv_path):
    Builder.load_file(kv_path + kv)


class LoginScreen(Screen):
    """This' the first screen (1)
    This screen will display the login. Both, signup and login options.

    Args:
        Screen (Screen): A new window.
    """

    username = StringProperty()
    password = StringProperty()

    def login(self):
        # If True, go to menu screen, otherwise show a popup.
        if database.db.validate_user(
            self.username,
            self.password,
        ):
            # Create an instance for the current user; with his info start
            # running the bank.
            user.init_user(self.username, self.password)
            init_bank(user.user)
            sm.transition.direction = "up"
            sm.current = "menu"
        else:
            popup_msg(
                msg="Ups! It seems like you haven't created any "
                + "account yet\nTry to create a new one first!",
                status=False,
            )

    def sign_up(self):
        """Go to sign up screen."""
        sm.transition.direction = "right"
        sm.current = "sign_up"


class SignupScreen(Screen):
    """Here, the user will be able to create a new account. After that, he'll go
    to menu screen imedeatly.

    Args:
        Screen (Screen): A different screen for signing up.
    """

    username = StringProperty()
    password = StringProperty()

    def add_new_user(self):
        if database.db.create_new_user(
            self.username,
            self.password,
        ):
            popup_msg(
                func=self.go_to_menu, msg="User created successfully!", status=True
            )
            # After sign up as log in, creates a new user and run the bank.
            user.init_user(self.username, self.password)
            init_bank(user.user)
        else:
            popup_msg(
                msg="Ups! We've caught a bug!\nPlease send an issue with"
                + " an extend description of this annoying bug!",
                status=False,
            )

    def go_to_menu(self, *args):
        sm.transition.direction = "up"
        sm.current = "menu"


class MenuScreen(Screen):
    """This' the second screen (2)
    # Will display the different available options to the user.

    Args:
        Screen (Screen): The screen.
    """

    pass


class TransactionScreen(Screen):
    """This' the third screen (3)

    Args:
        Screen (Screen): The screen.
    """

    user_id = ObjectProperty(None)
    cash = ObjectProperty(None)
    object = StringProperty()

    def make_transaction(self):
        try:
            bank.bank.cash_transaction(int(self.user_id.text), float(self.cash.text))
            popup_msg(msg="Transaccion completada!", status=True)
        except Exception as e:
            print(e)
            popup_msg(msg=str(e))


class StatusScreen(Screen):
    """Screen for displying the info of the actual user only.

    Args:
        Screen (Screen): The screen.
    """

    deposit_count = ObjectProperty(rebind=True)
    loan_count = ObjectProperty(rebind=True)
    deposit_total = ObjectProperty(None)
    loan_total = ObjectProperty(None)
    euros = ObjectProperty(None)
    dollars = ObjectProperty(None)
    object = ObjectProperty(None)

    def show_data(self):
        """Get the data from the bank and then shows it to the current user."""
        labels = (
            self.deposit_count,
            self.loan_count,
            self.deposit_total,
            self.loan_total,
            self.euros,
            self.dollars,
            self.object,
        )
        datas = bank.bank.load_data_user()
        try:
            for label, data in zip(labels, datas):
                label.text = str(data) if not isinstance(data, float) else f"{data:.6}"
        except Exception as e:
            popup_msg(msg=str(e))


class ConverterScreen(Screen):
    input_amount = NumericProperty()
    lbl_convert = ObjectProperty(None)

    def __init__(self, **kw):
        super().__init__(**kw)
        self.spinner_value_from = None
        self.spinner_value_to = None

    def set_spinner_value_from(self, spinner):
        self.spinner_value_from = spinner.text

    def set_spinner_value_to(self, spinner):
        self.spinner_value_to = spinner.text

    def get_match_currency(self):
        if self.spinner_value_from == "Dollars":
            if self.spinner_value_to == "Euros":
                return calc.dollars_to_euros
            elif self.spinner_value_to == "Dominican pesos":
                return calc.dollars_to_dop

        elif self.spinner_value_from == "Euros":
            if self.spinner_value_to == "Dollars":
                return calc.euros_to_dollars
            elif self.spinner_value_to == "Dominican pesos":
                return calc.euros_to_dop

        elif self.spinner_value_from == "Dominican pesos":
            if self.spinner_value_to == "Dollars":
                return calc.dop_to_dollars
            elif self.spinner_value_to == "Euros":
                return calc.dop_to_euros
        else:
            popup_msg()

        if self.spinner_value_from == self.spinner_value_to:
            self.lbl_convert.text = str(round(self.input_amount, 2))

    def do_convertion(self):
        conditions = (
            self.spinner_value_from is not None,
            self.spinner_value_to is not None,
        )

        if all(conditions):
            action = self.get_match_currency()
            if action:
                value = action(self.input_amount)
                self.lbl_convert.text = str(value)
        else:
            self.lbl_convert.text = "0.0"
        print(f"## From: {self.spinner_value_from} To: {self.spinner_value_to}")


# The screen's manager; to change between different screens
class Manager(ScreenManager):
    pass


class RV(RecycleView):
    """For containing the menu's buttons.

    Args:
        RecycleView (RecycleView): The RecycleView to be used.
    """

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.data = [
            {"text": "Realizar un deposito", "on_press": MyLayout.show_deposit},
            {"text": "Tomar un prestamo", "on_press": MyLayout.show_loan},
            {"text": "Transacciones", "on_press": MyLayout.show_transaction},
            {"text": "Consulta de estado", "on_press": MyLayout.show_status},
            {"text": "Pago de prestamo", "on_press": MyLayout.show_payment},
            {"text": "Cambio de divisas", "on_press": MyLayout.show_converter},
            {"text": "Guardar un objeto"},
        ]


class MyLayout(BoxLayout):
    """For being used with the popups.

    Args:
        BoxLayout (BoxLayout): The layout to be used.
    """

    message = ObjectProperty(None)
    amount = StringProperty()
    button = ObjectProperty(None)

    def __init__(self, msg: str, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.message.text = msg

    @staticmethod
    def show_deposit():
        layout = MyLayout("Enter the amount to be saved.")
        popup_msg(content=layout, title="Make deposit")
        layout.button.text = "Save deposit!"
        layout.button.bind(on_press=layout.do_deposit)

    def do_deposit(self, *args):
        try:
            bank.bank.make_deposit(float(self.amount))
            popup_msg(msg="Deposito realizado con exito!", status=True)
            # else:
            #    popup_msg()
        except Exception as e:
            popup_msg(msg=str(e))

    @staticmethod
    def show_loan():
        layout = MyLayout("Enter the needed cash.")
        popup_msg(content=layout, title="Make a loan")
        layout.button.text = "Receive the loan!"
        layout.button.bind(on_press=layout.make_loan)

    def make_loan(self, *args):
        try:
            bank.bank.make_loan(float(self.amount))
            popup_msg(msg="Prestamo recibido!", status=True)
        except Exception as e:
            popup_msg(msg=str(e))

    @staticmethod
    def show_transaction():
        sm.current = "transaction"

    @staticmethod
    def show_status():
        sm.get_screen("status").show_data()
        sm.current = "status"

    @staticmethod
    def show_payment():
        layout = MyLayout(f"Debes {bank.bank.get_total_loan:.6}")
        popup_msg(content=layout, title="Payment")
        layout.button.text = "Pay loan!"
        layout.button.bind(on_press=layout.make_payment)

    def make_payment(self, *args):
        try:
            bank.bank.pay_loan(float(self.amount))
            popup_msg(msg="Payment done!", status=True)
        except Exception as e:
            popup_msg(msg=str(e))

    @staticmethod
    def show_converter():
        sm.current = "converter"


# Create the screen manager.
sm = ScreenManager(transition=WipeTransition())

# Used to run the program. This class must be one method (build) and return it.
class BankManagementApp(App):
    def build(self):
        # A tuple with the different screens
        screens = (
            LoginScreen(name="login"),
            SignupScreen(name="sign_up"),
            MenuScreen(name="menu"),
            TransactionScreen(name="transaction"),
            StatusScreen(name="status"),
            ConverterScreen(name="converter"),
        )

        for i in screens:
            sm.add_widget(i)
        return sm


def popup_msg(
    func=lambda *args: None,
    msg: str = "Ups! A bug caught!",
    status: bool = False,
    content=None,
    title: str = None,
    *args,
    **kwargs,
):
    """Display a popup depending in the given optional arguments.

    Args:
        func (def, optional): The function to be bind (on_dismiss). Defaults to None.
        msg (str, optional): The menssage to show. Defaults to "Ups! A bug caught!".
        status (bool, optional): True for done; False to error. Defaults to True.
        content (Layout): The layout to be used by the popup. If no passed a
        label will be used.
        title (str): For the title of the popup. If no passed a title will be
        chose depending the status (Error or Done).
    """
    # Set the title.
    if title is not None:
        popup_title = title
    else:
        if status:
            popup_title = "Done!"
        else:
            popup_title = "Error!"
    # Create the predefined label, to be used if any content didn't be passed.
    lbl = Label(
        text=msg,
        italic=True,
        font_size=20,
        halign="justify",
    )
    title_size = 20
    title_align = "center"
    title_color = 1, 0, 0, 0.8
    # Create a new popup.
    popup = Popup(
        title=popup_title,
        content=content if content is not None else lbl,
        title_size=title_size,
        size_hint=(0.8, 0.65),
        title_align=title_align,
        title_color=title_color,
        on_dismiss=func,
    )

    popup.open()


# Run the app.
if __name__ == "__main__":
    main()
    app = BankManagementApp()
    app.run()
