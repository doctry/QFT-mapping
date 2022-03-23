import json
from argparse import ArgumentParser, Namespace

from hydra import main

from evaluator import Device


def main(flags: Namespace):
    fname = flags.file
    input_name = f"{fname}.txt"
    output_name = f"{fname}.json"

    device = Device(input_name)
    output = device.to_json()

    output_file = open(output_name, "w")
    json.dump(output, output_file)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("file", type=str)
    flags = parser.parse_args()

    main(flags)
