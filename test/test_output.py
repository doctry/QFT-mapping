import json
import sys
from test_device import Device

if __name__ == '__main__':
    device_filename = sys.argv[1]
    output_filename = sys.argv[2]
    device = Device(device_filename)
    output_file = open(output_filename, 'r')
