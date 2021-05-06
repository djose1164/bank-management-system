from kivy.app import App
from kivy.uix.image import Image
from kivy.uix.behaviors import ButtonBehavior


class MyButton(ButtonBehavior, Image):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        #self.source = 'atlas://data/images/defaulttheme/checkbox_off'

    def on_press(self):
        self.source = 'atlas://data/images/defaulttheme/checkbox_on'

    def on_release(self):
        self.source = 'atlas://data/images/defaulttheme/checkbox_off'


class SampleApp(App):
    def build(self):
        return MyButton()


SampleApp().run()