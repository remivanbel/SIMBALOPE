import analogio
from time import sleep
from board import *

with analogio.AnalogIn(IO4) as pin:
    while True:
        print(pin.value)
        sleep(0.5)