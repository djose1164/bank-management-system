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
import database
from database import main


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
        if database.db.validate_user(self.username.text, self.password.text):
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
        if database.db.create_new_user(self.username.text, self.password.text):
            popup_msg(func=self.go_to_menu, msg="User created successfully!")
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
    func=None, msg: str = "Ups! A bug caught!", status: bool = True, *args, **kwargs
):
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
        content=lbl,
        title_size=title_size,
        size_hint=(0.8, 0.35),
        title_align=title_align,
        title_color="green",
    )
    if func:
        popup.bind(on_dismiss=func)

    popup.open()


# Run the app.
if __name__ == "__main__":
    main()
    print(f"## In main {database.db}")
    app = MyApp()
    app.run()
