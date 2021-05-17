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
from kivy.properties import StringProperty, ObjectProperty
from kivy.uix.recycleview import RecycleView
from kivy.lang import Builder
# Database modules.
import database
from database import main
# User package
import user
# Bank moudle
from bank import init_bank
import bank

kivy.require("1.11.1")

Builder.load_file("popup_layout.kv")
Builder.load_file("status.kv")


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
    pass


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
        """Get the data from the bank and then shows it to the current user.
        """
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
                label.text = str(data)
        except Exception as e:
            popup_msg(msg=str(e))


# The screen's manager; to change between different screens
class _ScreenManager(ScreenManager):
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
            {"text": "Tomar un prestamo"},
            {"text": "Transacciones"},
            {"text": "Consulta de estado", "on_press": MyLayout.show_status},
            {"text": "Pago de prestamo"},
            {"text": "Cambio de moneda extranjera"},
            {"text": "Guardar un objeto"},
        ]


class MyLayout(BoxLayout):
    """For being used with the popups.

    Args:
        BoxLayout (BoxLayout): The layout to be used.
    """
    amount = ObjectProperty(None)

    @staticmethod
    def show_deposit():
        popup_msg(content=MyLayout(), title="Make deposit")

    def do_deposit(self):
        try:
            bank.bank.make_deposit(float(self.amount.text))
            popup_msg(msg="Deposito realizado con exito!", status=True)
            print(float(self.amount.text))
            # else:
            #    popup_msg()
        except Exception as e:
            popup_msg(msg=str(e))

    @staticmethod
    def show_status():
        sm.get_screen("status").show_data()
        sm.current = "status"


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
    **kwargs
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
