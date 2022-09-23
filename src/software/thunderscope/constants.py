from pyqtgraph.Qt import QtCore, QtGui

LINE_WIDTH = 3

# TODO (#2683) get these values from robot constants
MAX_LINEAR_SPEED_MPS = 2.0
MIN_LINEAR_SPEED_MPS = -2.0

MAX_ANGULAR_SPEED_RAD_PER_S = 20
MIN_ANGULAR_SPEED_RAD_PER_S = -20

MAX_DRIBBLER_RPM = 10000
MIN_DRIBBLER_RPM = -10000

MIN_POWER = 1000
MAX_POWER = 20000

class Colors(object):

    FIELD_COLOR = "w"
    FIELD_LINE_COLOR = "w"

    BLUE_ROBOT = QtGui.QColor(255, 100, 0, 255)
    BALL_COLOR = QtGui.QColor(255, 100, 0, 255)
    SIM_BALL_COLOR = QtGui.QColor(255, 100, 0, 150)
    YELLOW_ROBOT_COLOR = QtGui.QColor(255, 255, 0, 255)
    BLUE_ROBOT_COLOR = QtGui.QColor(0, 75, 255, 255)

    ROBOT_SPEED_SLOW_COLOR = "black"
    NAVIGATOR_PATH_COLOR = "green"
    NAVIGATOR_OBSTACLE_COLOR = "orange"

    VALIDATION_PASSED_COLOR = "g"
    VALIDATION_FAILED_COLOR = "r"
