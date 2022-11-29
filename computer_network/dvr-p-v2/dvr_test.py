import os
import sys
import time
import math 
import threading
from socket import *

#  The link between any two nodes that are directly connected to each other. 
class PathLink:
    def __init__(self, distance: int, nextHop) -> None:
        self.distance = distance
        self.nextHop = nextHop

 