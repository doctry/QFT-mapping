import sys

from .physical import Device

if __name__ == "__main__":
    filename = sys.argv[1]
    device = Device(filename)
