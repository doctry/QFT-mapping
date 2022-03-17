from test_device import Device
import sys
import json

if __name__ == "__main__":
    filename = sys.argv[1]
    device = Device(filename)
    output = device.to_json()

    output_file = open(sys.argv[2], 'w')
    json.dump(output, output_file)