import abc
import sys
from abc import abstractmethod

from Buttons.Button import Button
from Field import InterfaceField
from Graphics import InterfaceGraphics

class InterfaceEvent(abc.ABC):
    current_x = None
    current_y = None
    dig_box = False
    flag_box = False

    @abstractmethod
    def handle(self, h_field: InterfaceField, gr: InterfaceGraphics):
        pass

    @abstractmethod
    def check_but(self, ch_b_field: InterfaceField, but:Button, current_x, current_y):
        pass

    @abstractmethod
    def get_box(self, g_b_field: InterfaceField, current_x, current_y, gr: InterfaceGraphics):
        pass