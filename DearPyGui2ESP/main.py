#-------------------------------------------------#
# Blake Wiker
# 02/11/2021
# LED Cube GUI using PySimpleGUI
#-------------------------------------------------#

import PySimpleGUI as sg
import requests
from PIL import ImageColor

sg.theme('DarkBlack1') # Add Dark Theme

URL = "http://192.168.4.1/posttest" # URL for ESP32 Server

font = ('Arial', 15) # font type and size

# All the stuff inside your window.
layout = [[sg.Button('Animation 1', button_color=('#ff5e00'), 
            size = (10,1.5), font = ('Arial', 15), key = 'animation1'), 
           sg.Button('Animaiton 2', button_color=('#ff5e00'), 
            size = (10,1.5), font = ('Arial', 15), key = 'animation2'), 
           sg.Button('Animation 3', button_color=('#ff5e00'), 
            size = (10,1.5), font = ('Arial', 15), key = 'animation3')],
          [sg.Text('')],
          [sg.Text('Enter Messsage', font = font), 
           sg.InputText(font = font, size = (37,5))],
          [sg.Text('')],
          [sg.In("", visible=False, enable_events=True, 
            key='set_line_color'),
           sg.ColorChooserButton('Color Picker', 
            button_color=('#000000'), target='set_line_color', 
            key='set_line_color_chooser', 
            font = font, size = (10,1.5)), 
           sg.Button('Audio Mode', button_color=('#0000ff'), 
            key='audio_color', font = font, size = (10, 1.5)),
           sg.Button('Send', font = font, size = (10,1.5), 
            button_color=('#ffcd01'))],
          [sg.Text('')]
         #[sg.Button("Toggle")] 
         ] 

# Create the Window
window = sg.Window('Lattice', layout)

# data json: stores color, animation, message, and audio mode
data = {'red': 255, 'green': 255, 'blue': 255, 'animation': 0, 
        'message':'none', 'choice': 0, 'audio': 0};

# Event Loop to process "events" and get the "values" of the inputs
while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED: # if user closes window
        break

    if event == 'animation1': # if animation1 button is clicked
       if data['animation'] != 1: 
           data['animation'] = 1 # update json
           # update button colors to show selection
           window.find_element('animation1').Update(
                   button_color=('#ff5e00','#ffffff'))
           window.find_element('animation2').Update(
                   button_color=('#ffffff','#ff5e00'))
           window.find_element('animation3').Update(
                   button_color=('#ffffff','#ff5e00'))

       else: # if already selected, deselect button
           data['animation'] = 0
           window.find_element('animation1').Update(
                   button_color=('#ffffff', '#ff5e00'))

    if event == 'animation2': # if animation2 button is clicked
       if data['animation'] != 2:
           data['animation'] = 2 # update json
           # update button colors to indicate selection
           window.find_element('animation1').Update(
                   button_color=('#ffffff','#ff5e00'))
           window.find_element('animation2').Update(
                   button_color=('#ff5e00','#ffffff'))
           window.find_element('animation3').Update(
                   button_color=('#ffffff','#ff5e00'))

       else: # if already selected, deselect button
           data['animation'] = 0
           window.find_element('animation2').Update(
                   button_color=('#ffffff', '#ff5e00'))

    if event == 'animation3': # if animation3 button is clicked
       if data['animation'] != 3: 
           data['animation'] = 3 # update json
           # update button colors to indicate selection
           window.find_element('animation1').Update(
                   button_color=('#ffffff','#ff5e00'))
           window.find_element('animation2').Update(
                   button_color=('#ffffff','#ff5e00'))
           window.find_element('animation3').Update(
                   button_color=('#ff5e00','#ffffff'))

       else: # if already selected, deselect button
           data['animation'] = 0
           window.find_element('animation3').Update(
                   button_color=('#ffffff', '#ff5e00'))

    if event == 'set_line_color': # if color picker is clicked
        if values[event] == 'None': # if no color selected
           values[event] = '#000000' # default color is black
 
        #update button color to indicate selection and update json
        window['set_line_color_chooser'].Update(
                button_color=('#ffffff', values[event]))
        RGB = ImageColor.getcolor(values[event], 'RGB')
        data['red'] = RGB[0]
        data['green'] = RGB[1]
        data['blue'] = RGB[2]

    # the if statement below is for testing use
    #if event == 'Toggle':
       #if data['choice'] == 0:
          # data['choice'] = 1
       #else:
           #data['choice'] = 0
      # r = requests.post(URL, json = data)

    if event == 'audio_color': # if audio button is clicked
       if data['audio'] == 0: # if not selected            
           data['audio'] = 1  #update json and change color
           window.find_element('audio_color').Update(
                   button_color=('#0000ff','#ffffff'))
       else: # if selected, update json and change color
           data['audio'] = 0
           window.find_element('audio_color').Update(
                   button_color=('#ffffff', '#0000ff'))
       try: # try psoting json to ESP32 server
           requests.post(URL, json = data, timeout = 2)
       except: # if not connect, error popup occurs
           sg.Popup("Not connected to Lattice", title = "Error")  

    if event == 'Send': # if send button is clicked
        data['message'] = values[0] # update message json
        # if audio on, popup error occurs
        if data['audio'] == 1: 
            sg.Popup("Turn off Audio Mode", title = "Error")

        # if animation and message selected, popup error occurs
        elif data['animation'] != 0 and data['message'] != '':
            sg.Popup("Cannot send animation and message", 
                    title = "Error")

        # if no animation or message selected, popup error occrs
        elif data['animation'] == 0 and data['message'] == '':
            sg.Popup("Choose animation or message", title = "Error")
       
       # try to post json if no errors
        else:    
            try:
                requests.post(URL, json = data, timeout = 2)
            except: # error if not connected
                sg.Popup("Not connected to Lattice", title = "Error")

window.close() # close window on exit
