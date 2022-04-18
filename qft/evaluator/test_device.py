import sys

from qft.evaluator.physical import Device

if __name__ == "__main__":
    filename = sys.argv[1]
    device = Device(filename)
    print("passing test")
