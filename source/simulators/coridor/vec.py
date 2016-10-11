import math

def vec2_from_direction(angle, length):
    return Vec2(math.cos(angle), math.sin(angle)) * length

def radians_to_degrees(angle):
    return (angle / math.pi) * 180

class Vec2:

    def __init__(self, x, y = None):
        if y is None:
            #This is a tuple
            self.x = x[0]
            self.y = x[1]
        else:
            self.x = x
            self.y = y

    def __add__(self, other):
        return Vec2(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vec2(self.x - other.x, self.y - other.y)

    def __iadd__(self, other):
        new_x = self.x + other.x
        new_y = self.y + other.y
        return Vec2(new_x, new_y)
    
    def __mul__(self, other):
        return Vec2(self.x * other, self.y * other)

    def __abs__(self):
        return math.sqrt(self.x ** 2 + self.y ** 2)

    def is_within_bounds(self, center_point, size):
        """Checks whether this vector is within size distance from center_point"""
        return self.distance_to(center_point) <= size

    def angle(self):
        return math.atan2(self.y, self.x)

    def relative_angle_to(self, other):
        return (self - other).angle()

    def to_tuple(self):
        return (self.x, self.y)

    def distance_to(self, other):
        return abs(self - other)

