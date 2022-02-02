import time
import math
import board
import busio
from digitalio import DigitalInOut
import neopixel
from adafruit_esp32spi import adafruit_esp32spi
import displayio
import adafruit_touchscreen
import adafruit_imageload
import adafruit_requests as requests
import adafruit_esp32spi.adafruit_esp32spi_socket as socket
from adafruit_esp32spi import adafruit_esp32spi
import microcontroller
import usb_hid
import neopixel
import adafruit_adt7410
from adafruit_bitmap_font import bitmap_font
from adafruit_display_text.label import Label
from adafruit_button import Button
import traceback

# COLORS
RED = 0xFF0000
ORANGE = 0xFF8000
YELLOW = 0xFFFF00
GREEN = 0x00FF00
DARK_GREEN = 0x009900
CYAN = 0x00FFFF
BLUE = 0x0000FF
VIOLET = 0x7F00FF
PINK = 0xFF007F
WHITE = 0xFFFFFF
GRAY = 0x787878
DARK_GRAY = 0x1C1C1C
PURPLE = 0x800080
LIGHT_PURPLE = 0xB266FF
LIGHT_ORANGE = 0xFF9933
DARK_ORANGE = 0xFF5E00
BLACK = 0x000000

# response = None
# URL for HTTP connection with ESP32
URL = "http://192.168.4.1/posttest"
# response = None

# internet connection
try:
    from secrets import secrets
except ImportError:
    print("WiFi secrets are kept in secrets.py, please add them there!")
    raise

# If you are using a board with pre-defined ESP32 Pins:
esp32_cs = DigitalInOut(board.ESP_CS)
esp32_ready = DigitalInOut(board.ESP_BUSY)
esp32_reset = DigitalInOut(board.ESP_RESET)

spi = busio.SPI(board.SCK, board.MOSI, board.MISO)
esp = adafruit_esp32spi.ESP_SPIcontrol(spi, esp32_cs, esp32_ready, esp32_reset)

# print("Connecting to AP...")
# while not esp.is_connected:
# try:
# esp.connect_AP(secrets["ssid"], secrets["password"])
# except RuntimeError as e:
# print("could not connect to AP, retrying: ", e)
# continue
# print("Connected to", str(esp.ssid, "utf-8"), "\tRSSI:", esp.rssi)

# Initialize a requests object with a socket and esp32spi interface
socket.set_interface(esp)
requests.set_socket(socket, esp)

# set up display preferences
display = board.DISPLAY
display.rotation = 90

# Backlight function
# Value between 0 and 1
def set_backlight(val):
    val = max(0, min(1.0, val))
    board.DISPLAY.auto_brightness = False
    board.DISPLAY.brightness = val


# set backlight to 70%
set_backlight(1)

# Touchscreen setup
screen_width = 320
screen_height = 480
# -------Rotate 90:
ts = adafruit_touchscreen.Touchscreen(
    board.TOUCH_YU,
    board.TOUCH_YD,
    board.TOUCH_XL,
    board.TOUCH_XR,
    calibration=((5200, 59000), (5800, 57000)),
    size=(screen_width, screen_height),
)


# ------------- Display groups ------------- #
page1 = displayio.Group()
page2 = displayio.Group()

# Set the font and preload letters
font = bitmap_font.load_font("/fonts/Helvetica-Bold-16.bdf")
font.load_glyphs(b"abcdefghjiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890:*,")

buttons = []  # buttoon array for accessing button fucntion on touch
OUTLINE_COLOR = 0xFFFFFF  # white color for outline

# Button definitions for the buttons on page 1 and 2
# Three preset animation buttons (page 1)
button_a1 = Button(
    x=5,
    y=80,
    width=310,
    height=100,
    label="Animation 1",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_ORANGE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_a1)  # adding this button to the buttons group
page1.append(buttons[0])

button_a2 = Button(
    x=5,
    y=190,
    width=310,
    height=100,
    label="Animation 2",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_ORANGE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_a2)  # adding this button to the buttons group
page1.append(buttons[1])

button_a3 = Button(
    x=5,
    y=300,
    width=310,
    height=100,
    label="Animation 3",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_ORANGE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_a3)  # adding this button to the buttons group
page1.append(buttons[2])

# Three message selection buttons (page 2)
button_m1 = Button(
    x=5,
    y=80,
    width=100,
    height=100,
    label="Message 1",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_m1)  # adding this button to the buttons group
page2.append(buttons[3])

button_m2 = Button(
    x=110,
    y=80,
    width=100,
    height=100,
    label="Message 2",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_m2)  # adding this button to the buttons group
page2.append(buttons[4])

button_m3 = Button(
    x=215,
    y=80,
    width=100,
    height=100,
    label="Message 3",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_m3)  # adding this button to the buttons group
page2.append(buttons[5])

# Ten color selection buttons (page 2)
button_c1 = Button(
    x=5,
    y=190,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=RED,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c1)  # adding this button to the buttons group
page2.append(buttons[6])

button_c2 = Button(
    x=68,
    y=190,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=ORANGE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c2)  # adding this button to the buttons group
page2.append(buttons[7])

button_c3 = Button(
    x=131,
    y=190,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=YELLOW,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c3)  # adding this button to the buttons group
page2.append(buttons[8])

button_c4 = Button(
    x=194,
    y=190,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=GREEN,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c4)  # adding this button to the buttons group
page2.append(buttons[9])

button_c5 = Button(
    x=257,
    y=190,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_GREEN,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c5)  # adding this button to the buttons group
page2.append(buttons[10])

button_c6 = Button(
    x=5,
    y=243,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=CYAN,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c6)  # adding this button to the buttons group
page2.append(buttons[11])

button_c7 = Button(
    x=68,
    y=243,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=BLUE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c7)  # adding this button to the buttons group
page2.append(buttons[12])

button_c8 = Button(
    x=131,
    y=243,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c8)  # adding this button to the buttons group
page2.append(buttons[13])

button_c9 = Button(
    x=194,
    y=243,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PINK,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c9)  # adding this button to the buttons group
page2.append(buttons[14])

button_c10 = Button(
    x=257,
    y=243,
    width=58,
    height=47,
    label="",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=WHITE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_c10)  # adding this button to the buttons group
page2.append(buttons[15])

# Speed selection buttons (page 2)
button_s1 = Button(
    x=5,
    y=300,
    width=100,
    height=100,
    label="Slow",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_s1)  # adding this button to the buttons group
page2.append(buttons[16])

button_s2 = Button(
    x=110,
    y=300,
    width=100,
    height=100,
    label="Medium",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_s2)  # adding this button to the buttons group
page2.append(buttons[17])

button_s3 = Button(
    x=215,
    y=300,
    width=100,
    height=100,
    label="Fast",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=PURPLE,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_s3)  # adding this button to the buttons group
page2.append(buttons[18])

# Animation and Custom buttons used to switch pages
button_page11 = Button(
    x=5,
    y=5,
    width=150,
    height=65,
    label="Animations",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=GRAY,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_page11)  # adding this button to the buttons group
page1.append(buttons[19])

button_page21 = Button(
    x=165,
    y=5,
    width=150,
    height=65,
    label="Custom",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_GRAY,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_page21)  # adding this button to the buttons group
page1.append(buttons[20])

# exact copy for second display group
button_page12 = Button(
    x=5,
    y=5,
    width=150,
    height=65,
    label="Animations",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=GRAY,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_page12)  # adding this button to the buttons group
page2.append(buttons[21])

button_page22 = Button(
    x=165,
    y=5,
    width=150,
    height=65,
    label="Custom",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_GRAY,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_page22)  # adding this button to the buttons group
page2.append(buttons[22])

# Send button for tranmitting message to ESP32
button_send1 = Button(
    x=5,
    y=410,
    width=310,
    height=65,
    label="Send",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_GRAY,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_send1)  # adding this button to the buttons group
page1.append(buttons[23])

# exact copy for second display group
button_send2 = Button(
    x=5,
    y=410,
    width=310,
    height=65,
    label="Send",
    label_font=font,
    label_color=0xFFFFFF,
    fill_color=DARK_GRAY,
    outline_color=OUTLINE_COLOR,
)
buttons.append(button_send2)  # adding this button to the buttons group
page2.append(buttons[24])

# ---------Button Execution-----------
data1 = "0"  # animation number
data2 = {
    "message": "0",
    "color": "0",
    "speed": "0",
}  # json struct {message, color, speed}
check = "0"
page = 1  # used to keep track of page
while True:  # loop forever
    touch = ts.touch_point
    if touch:  # if touch deteccted
        for i, b in enumerate(buttons):  # for every button in the button list
            if b.contains(touch):  # if touch resides in the button
                if page == 1:  # page 1 button actions listed below
                    # each if statement corresponds to a button
                    # fill colors are changed to indicate button selection
                    if i == 0:
                        print("Animation 1")
                        data1 = "a1"
                        button_a1.fill_color = LIGHT_ORANGE
                        button_a2.fill_color = DARK_ORANGE
                        button_a3.fill_color = DARK_ORANGE
                        while ts.touch_point:
                            pass
                    if i == 1:
                        print("Animation 2")
                        data1 = "a2"
                        button_a1.fill_color = DARK_ORANGE
                        button_a2.fill_color = LIGHT_ORANGE
                        button_a3.fill_color = DARK_ORANGE
                        while ts.touch_point:
                            pass
                    if i == 2:
                        print("Animation 3")
                        data1 = "a3"
                        button_a1.fill_color = DARK_ORANGE
                        button_a2.fill_color = DARK_ORANGE
                        button_a3.fill_color = LIGHT_ORANGE
                        while ts.touch_point:
                            pass
                    if i == 19:
                        print("Page 1")
                        page = 1
                        button_page12.fill_color = GRAY
                        button_page22.fill_color = DARK_GRAY
                        while ts.touch_point:
                            pass
                    if i == 20:
                        print("Page 2")
                        page = 2
                        button_page12.fill_color = DARK_GRAY
                        button_page22.fill_color = GRAY
                        while ts.touch_point:
                            pass
                    if i == 23:
                        print("Send")
                        print(data1)
                        data1 = "0"
                        button_a1.fill_color = DARK_ORANGE
                        button_a2.fill_color = DARK_ORANGE
                        button_a3.fill_color = DARK_ORANGE
                        # code below is used to connect to ESP32
                        # print("POSTing data to {0}: {1}".format(URL, data1))
                        # try:
                        # response = requests.post(URL, data=data1)
                        # except BaseException as error:
                        # traceback.print_exception(error,error,error.__traceback__)
                        # print(response.text)
                        # print('-'*40)
                        # json_resp = response.json()
                        # Parse out the 'data' key from json_resp dict.
                        # print("Data received from server:", json_resp['data'])
                        # print('-'*40)
                        # response.close()
                        while ts.touch_point:
                            pass
                # page 2 buttons actions listed below
                if page == 2:
                    if i == 3:
                        print("Message 1")
                        data2["message"] = "m1"
                        button_m1.fill_color = LIGHT_PURPLE
                        button_m2.fill_color = PURPLE
                        button_m3.fill_color = PURPLE
                        while ts.touch_point:
                            pass
                    if i == 4:
                        print("Message 2")
                        button_m1.fill_color = PURPLE
                        button_m2.fill_color = LIGHT_PURPLE
                        button_m3.fill_color = PURPLE
                        data2["message"] = "m2"
                        while ts.touch_point:
                            pass
                    if i == 5:
                        print("Message 3")
                        button_m1.fill_color = PURPLE
                        button_m2.fill_color = PURPLE
                        button_m3.fill_color = LIGHT_PURPLE
                        data2["message"] = "m3"
                        while ts.touch_point:
                            pass
                    # for color buttons, outline color is changed to indicate selection
                    if i == 6:
                        print("Red")
                        data2["color"] = "255,0,0"
                        button_c1.outline_color = BLACK
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 7:
                        print("Orange")
                        data2["color"] = "255,128,0"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = BLACK
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 8:
                        print("Yellow")
                        data2["color"] = "255,255,0"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = BLACK
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 9:
                        print("Green")
                        data2["color"] = "0,255,0"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = BLACK
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 10:
                        print("Dark Green")
                        data2["color"] = "0,153,0"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = BLACK
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 11:
                        print("CYAN")
                        data2["color"] = "0,255,255"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = BLACK
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 12:
                        print("Blue")
                        data2["color"] = "0,0,255"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = BLACK
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 13:
                        print("Purple")
                        data2["color"] = "128,0,128"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = BLACK
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 14:
                        print("Pink")
                        data2["color"] = "255,0,127"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = BLACK
                        button_c10.outline_color = WHITE
                        while ts.touch_point:
                            pass
                    if i == 15:
                        print("White")
                        data2["color"] = "255,255,255"
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = BLACK
                        while ts.touch_point:
                            pass
                    if i == 16:
                        print("Slow")
                        button_s1.fill_color = LIGHT_PURPLE
                        button_s2.fill_color = PURPLE
                        button_s3.fill_color = PURPLE
                        data2["speed"] = "slow"
                        while ts.touch_point:
                            pass
                    if i == 17:
                        print("Medium")
                        button_s1.fill_color = PURPLE
                        button_s2.fill_color = LIGHT_PURPLE
                        button_s3.fill_color = PURPLE
                        data2["speed"] = "medium"
                        while ts.touch_point:
                            pass
                    if i == 18:
                        print("Fast")
                        data2["speed"] = "fast"
                        button_s1.fill_color = PURPLE
                        button_s2.fill_color = PURPLE
                        button_s3.fill_color = LIGHT_PURPLE
                        while ts.touch_point:
                            pass
                    if i == 21:
                        print("Page 1")
                        page = 1
                        button_page12.fill_color = GRAY
                        button_page22.fill_color = DARK_GRAY
                        while ts.touch_point:
                            pass
                    if i == 22:
                        print("Page 2")
                        page = 2
                        button_page12.fill_color = DARK_GRAY
                        button_page22.fill_color = GRAY
                        while ts.touch_point:
                            pass
                    if i == 24:
                        print("Send")
                        print(data2)
                        data2 = {"message": "0", "color": "0", "speed": "0"}
                        button_m1.fill_color = PURPLE
                        button_m2.fill_color = PURPLE
                        button_m3.fill_color = PURPLE
                        button_c1.outline_color = WHITE
                        button_c2.outline_color = WHITE
                        button_c3.outline_color = WHITE
                        button_c4.outline_color = WHITE
                        button_c5.outline_color = WHITE
                        button_c6.outline_color = WHITE
                        button_c7.outline_color = WHITE
                        button_c8.outline_color = WHITE
                        button_c9.outline_color = WHITE
                        button_c10.outline_color = WHITE
                        button_s1.fill_color = PURPLE
                        button_s2.fill_color = PURPLE
                        button_s3.fill_color = PURPLE
                        # this code is used to send json data to ESP32
                        # print("POSTing data to {0}: {1}".format(URL, data2))
                        # try:
                        # response = requests.post(URL, json=data2)
                        # except BaseException as error:
                        # traceback.print_exception(error,error,error.__traceback__)
                        # print(response.text)
                        # print('-'*40)
                        # json_resp = response.json()
                        # Parse out the 'data' key from json_resp dict.
                        # print("Data received from server:", json_resp['data'])
                        # print('-'*40)
                        # response.close()
                        while ts.touch_point:
                            pass
    # displays the current page
    if page == 1:
        board.DISPLAY.show(page1)
    if page == 2:
        board.DISPLAY.show(page2)
