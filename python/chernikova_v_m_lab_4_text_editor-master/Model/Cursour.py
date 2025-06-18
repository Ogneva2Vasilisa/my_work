class Cursor:
    __x: int
    __y: int

    def __init__(self):
        self.__x = 0
        self.__y = 0

    def getx(self) -> int:
        return self.__x

    def gety(self) -> int:
        return self.__y

    def move(self, x: int, y: int):
        if x >= 0:
            self.__x = x
        if y >= 0:
            self.__y = y

