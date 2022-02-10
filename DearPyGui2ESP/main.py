import PySimpleGUI as sg
import requests
from PIL import ImageColor
sg.theme('DarkBlack1')   # Add a touch of color
URL = "http://192.168.4.1/posttest"
font = ('Arial', 15)
# All the stuff inside your window.
layout = [ [sg.Button('Animation 1', button_color=('#ff5e00'), size = (10,1.5), font = ('Arial', 15), key = 'animation1'), 
            sg.Button('Animaiton 2', button_color=('#ff5e00'), size = (10,1.5), font = ('Arial', 15), key = 'animation2'), 
            sg.Button('Animation 3', button_color=('#ff5e00'), size = (10,1.5), font = ('Arial', 15), key = 'animation3')],
            [sg.Text('')],
            [sg.Text('Enter Messsage', font = font), sg.InputText(font = font, size = (37,5))],
            [sg.Text('')],
            [sg.In("", visible=False, enable_events=True, key='set_line_color'),
                sg.ColorChooserButton('Color Picker', button_color=('#000000'), target='set_line_color', 
                    key='set_line_color_chooser', font = font, size = (10,1.5)), 
                sg.Button('Audio Mode', button_color=('#0000ff'), key='audio_color', font = font, size = (10, 1.5)),
                sg.Button('Send', font = font, size = (10,1.5), button_color=('#ffcd01'))],
            [sg.Text('')]
            #[sg.Button("Toggle")] 
         ] 

# Create the Window
window = sg.Window('Lattice', layout)
# Event Loop to process "events" and get the "values" of the inputs
data = {'red': 255, 'green': 255, 'blue': 255, 'animation': 0, 'message':'none', 'choice': 0, 'audio': 0};
while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED: # if user closes window or clicks cancel
        break
    if event == 'animation1':
       if data['animation'] != 1:
           data['animation'] = 1
           window.find_element('animation1').Update(button_color=('#ff5e00','#ffffff'))
           window.find_element('animation2').Update(button_color=('#ffffff','#ff5e00'))
           window.find_element('animation3').Update(button_color=('#ffffff','#ff5e00'))

       else:
           data['animation'] = 0
           window.find_element('animation1').Update(button_color=('#ffffff', '#ff5e00'))

    if event == 'animation2':
       if data['animation'] != 2:
           data['animation'] = 2
           window.find_element('animation1').Update(button_color=('#ffffff','#ff5e00'))
           window.find_element('animation2').Update(button_color=('#ff5e00','#ffffff'))
           window.find_element('animation3').Update(button_color=('#ffffff','#ff5e00'))

       else:
           data['animation'] = 0
           window.find_element('animation2').Update(button_color=('#ffffff', '#ff5e00'))

    if event == 'animation3':
       if data['animation'] != 3:
           data['animation'] = 3
           window.find_element('animation1').Update(button_color=('#ffffff','#ff5e00'))
           window.find_element('animation2').Update(button_color=('#ffffff','#ff5e00'))
           window.find_element('animation3').Update(button_color=('#ff5e00','#ffffff'))

       else:
           data['animation'] = 0
           window.find_element('animation3').Update(button_color=('#ffffff', '#ff5e00'))

    if event == 'set_line_color':
        if values[event] == 'None':
            values[event] = '#000000' 
        window['set_line_color_chooser'].Update(button_color=('#ffffff', values[event]))
        RGB = ImageColor.getcolor(values[event], 'RGB')
        data['red'] = RGB[0]
        data['green'] = RGB[1]
        data['blue'] = RGB[2]
    #if event == 'Toggle':
       #if data['choice'] == 0:
          # data['choice'] = 1
       #else:
           #data['choice'] = 0
      # r = requests.post(URL, json = data)
    if event == 'audio_color':
       if data['audio'] == 0:
           data['audio'] = 1
           window.find_element('audio_color').Update(button_color=('#0000ff','#ffffff'))
       else:
           data['audio'] = 0
           window.find_element('audio_color').Update(button_color=('#ffffff', '#0000ff'))
       try:
           requests.post(URL, json = data, timeout = 2)
       except:
           sg.Popup("Not connected to Lattice", title = "Error")
       #print(data)    

    if event == 'Send':
        data['message'] = values[0]
        if data['audio'] == 1:
            sg.Popup("Turn off Audio Mode", title = "Error")
        elif data['animation'] != 0 and data['message'] != '':
            sg.Popup("Cannot send animation and message", title = "Error")
        elif data['animation'] == 0 and data['message'] == '':
            sg.Popup("Choose animation or message", title = "Error")
        else:    
            try:
                requests.post(URL, json = data, timeout = 2)
            except:
                sg.Popup("Not connected to Lattice", title = "Error")
        #print(data)

window.close()
