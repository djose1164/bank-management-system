"""Bank Management System by Lusecita.

brief: Implementation of GUI.

Copyright© 2021 Lusecita Malvadita.   
"""

# Kivy modules.
import kivy

kivy.require("1.11.1")
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen, WipeTransition
from kivy.uix.popup import Popup
from kivy.uix.label import Label
from kivy.properties import ObjectProperty
from database import db

# CPython.
from ctypes import CDLL


# This' the first screen (1)
# This screen will display the login. Both, signup and login options.
class LoginScreen(Screen):
    username = ObjectProperty(None)
    password = ObjectProperty(None)

    def login(self):
        if db.validate_user(self.username.text, self.password.text):
            sm.current = "menu"
        else:
            popup = Popup(
                title="Error!",
                content=Label(
                    text="Ups! It seems like you haven't created any "
                    + "account yet\nTry to create a new one first!"
                ),
                size_hint=(0.5, 0.4),
            )
            popup.open()

    def sign_up(self):
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
        if db.create_new_user(self.username.text, self.password.text):
            popup = Popup(
                title="Done!",
                content=Label(text="User created succefully!", bold=True),
                size_hint=(0.5, 0.4),
                on_dismiss = self.go_to_menu
            )

            popup.open()
        else:
            Popup(
                title="Error!",
                content=Label(
                    text="Ups! We've caught a bug!\nPlease send an issue with"
                    + " an extend description of this annoying bug!",
                    italic=True,
                    font_size=20,
                    halign="justify",
                ),
                size_hint=(0.8, 0.5),
            ).open()
            
    def go_to_menu(self, instance):
        print(f"{instance}")
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


# Run the app.
if __name__ == "__main__":
    app = MyApp()
    app.run()
