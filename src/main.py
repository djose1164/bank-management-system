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
from kivy.properties import ObjectProperty
from kivy.uix.recycleview import RecycleView
from kivy.lang import Builder
import database
from database import main

import user

from bank import init_bank
import bank

Builder.load_file("popup_layout.kv")

class LoginScreen(Screen):
    """This' the first screen (1)
    This screen will display the login. Both, signup and login options.

    Args:
        Screen (Screen): A new window.
    """

    username = ObjectProperty(None)
    password = ObjectProperty(None)

    def login(self):
        # If True, go to menu screen, otherwise show a popup.
        if database.db.validate_user(
            self.username.text,
            self.password.text,
        ):
            user.init_user(self.username.text, self.password.text)
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

    username = ObjectProperty(None)
    password = ObjectProperty(None)

    def add_new_user(self):
        if database.db.create_new_user(
            self.username.text,
            self.password.text,
        ):
            popup_msg(func=self.go_to_menu, msg="User created successfully!")
            user.init_user(self.username.text, self.password.text)
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


# This' the second screen (2)
# Will display the different available options to the user.
class MenuScreen(Screen):
    pass


# This' the third screen (3)
# After the user have chosen an option, this screen will be display.
# All transaction will be done here.
class TransactionScreen(Screen):
    pass


# The screen's manager; to change between different screens
class _ScreenManager(ScreenManager):
    pass


class RV(RecycleView):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.data = [
            {"text": "Realizar un deposito", "on_press": Deposit.show_deposit},
            {"text": "Tomar un prestamo"},
            {"text": "Transacciones"},
            {"text": "Consulta de estado"},
            {"text": "Pago de prestamo"},
            {"text": "Cambio de moneda extranjera"},
            {"text": "Guardar un objeto"},
        ]

    


# Create the screen manager.
sm = ScreenManager(transition=WipeTransition())

# Used to run the program. This class must be one method (build) and return it.
class MyApp(App):
    def build(self):
        # A tuple with the different screens
        screens = (
            LoginScreen(name="login"),
            SignupScreen(name="sign_up"),
            MenuScreen(name="menu"),
            TransactionScreen(name="transaction"),
        )

        for i in screens:
            sm.add_widget(i)
        return sm


def popup_msg(
    func=lambda *args: None,
    msg: str = "Ups! A bug caught!",
    status: bool = False,
    content = None,
    title = None,
    *args,
    **kwargs
):
    """Display a popup depending in the given optional arguments.

    Args:
        func (def, optional): The function to be bind (on_dismiss). Defaults to None.
        msg (str, optional): The menssage to show. Defaults to "Ups! A bug caught!".
        status (bool, optional): True for done; False to error. Defaults to True.
    """
    if title is not None:
        popup_title = title
    else:
        if status:
            popup_title = "Done!"
        else:
            popup_title = "Error!"

    lbl = Label(
        text=msg,
        italic=True,
        font_size=20,
        halign="justify",
    )
    title_size = 20
    title_align = "center"
    title_color = "red"

    popup = Popup(
        title=popup_title,
        content=content if content is not None else lbl,
        title_size=title_size,
        size_hint=(0.8, 0.65),
        title_align=title_align,
        title_color="green",
        on_dismiss=func,
    )

    popup.open()

class Deposit(BoxLayout):
    amount = ObjectProperty(None)

    @staticmethod
    def show_deposit():
        popup_msg(content=Deposit(), title="Make deposit")
    
    def do_deposit(self):
        bank.bank.make_deposit(float(self.amount.text))
        popup_msg(msg="Deposito realizado con exito!", status=True)
        print(float(self.amount.text))
        #else:
        #    popup_msg()

# Run the app.
if __name__ == "__main__":
    kivy.require("1.11.1")
    main()
    print("## bank: ", bank.bank, "\n## user: ", user.user)
    app = MyApp()
    app.run()
