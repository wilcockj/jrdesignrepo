import PySimpleGUI as sg
import requests
from PIL import ImageColor
sg.theme('DarkBlack1')   # Add a touch of color
URL = "http://192.168.4.1/posttest"
# All the stuff inside your window.
layout = [  [sg.Text('Animations')],
            [sg.Button('Animation 1', button_color=('#ff5e00')), 
                sg.Button('Animaiton 2', button_color=('#ff5e00')), 
                sg.Button('Animation 3', button_color=('#ff5e00'))],
            [sg.Text('Color Picker')],
            [sg.In("", visible=False, enable_events=True, key='set_line_color'),
                sg.ColorChooserButton('           ', target='set_line_color', key='set_line_color_chooser')],
            [sg.Text('Enter Messsage'), sg.InputText()],
            [sg.Button('Send'), sg.Button("Toggle")],
            [sg.Text('Audio Control')],
            [sg.Button('Audio', button_color=('#ff5e00'), key='audio_color')]
            ]

# Create the Window
window = sg.Window('Lattice', layout)
# Event Loop to process "events" and get the "values" of the inputs
data = {'red': 0, 'green': 0, 'blue': 0, 'animation': 0, 'message':'none', 'choice': 0, 'audio': 0};

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED: # if user closes window or clicks cancel
        break
   # if event == 'Animation 1':

    if event == 'set_line_color':
        window['set_line_color_chooser'].Update(button_color=(values[event], values[event]))
        RGB = ImageColor.getcolor(values[event], 'RGB')
        data['red'] = RGB[0]
        data['green'] = RGB[1]
        data['blue'] = RGB[2]
    if event == 'Send':
        data['message'] = values[0]
        r = requests.post(URL, json = data)
        print(r)
    if event == 'Toggle':
       if data['choice'] == 0:
           data['choice'] = 1
       else:
           data['choice'] = 0
       r = requests.post(URL, json = data)
    if event == 'Audio':
       if data['audio'] == 0:
           data['audio'] = 1
           window['audio_color'].Update(button_color=('#ff9933', '#ff9933'))
       else:
           data['audio'] = 0
           window['audio_color'].Update(button_color=('#ff5e00', '#ff5e00'))


window.close()
