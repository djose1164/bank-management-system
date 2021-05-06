"""Bank Management System by Lusecita.

brief: Implementation of GUI.

Copyright© 2021 Lusecita Malvadita.   
"""

# Kivy modules.
import kivy

kivy.require("1.11.1")
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen, WipeTransition
from database import Database

# CPython.
from ctypes import CDLL


# This' the first screen (1)
# This screen will display the login. Both, signup and login options.
class LoginScreen(Screen):

    def login(self):
        dir_path = "../src/login.so"
        sm.current = "menu"
        print("Holaaa")

    def sign_up(self):
        sm.current = "sign_up"


class SignupScreen(Screen):
    """Here, the user will be able to create a new account. After that, he'll go
    to menu screen imedeatly.

    Args:
        Screen (Screen): A different screen for signing up.
    """

    pass


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
