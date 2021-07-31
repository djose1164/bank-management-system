from kivy.app import App
from kivy.uix.label import Label

class ClassApp(App):
    def build(self):
        return Label(text="Holalala")
    
if __name__ == '__main__':
    ClassApp().run()
    