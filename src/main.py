    """
    Bank Management System by Lusecita.
    
    brief: Implementation of the functions from C; and the implementation of the
    GUI.
    
    Copyright© 2021 Lusecita Malvadita.   
    """

# Kivy modules.
import kivy
kivy.require("1.11.1")
from kivy.uix.screenmanager import ScreenManager, Screen
# CPython.
from ctypes import CDLL

# This' the first screen (1)
# This screen will display the login. Both, signup and login.
class LoginScreen(Screen):
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

# Used to run the program. This class must be one method (build) and return it.
class MyApp(object):
    def build(self):
        return
       
# Run the app.        
if __name__ == "__main__":
    app = MyApp()
    app.run()