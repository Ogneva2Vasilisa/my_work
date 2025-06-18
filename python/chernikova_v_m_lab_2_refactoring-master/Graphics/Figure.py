import abc
from abc import abstractmethod

class Figure(abc.ABC):

    @abstractmethod
    def draw(self, font, color, bg_color, center_x, center_y, surface):
        pass