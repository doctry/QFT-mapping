from argparse import ArgumentParser, Namespace
from pathlib import Path


def main(args):
    storage = []
    print(args.device)
    with open(args.device) as f:
        line = f.readline()
        qubit_num = int(line)
        line = f.readline()
        while line != "":
            info = line.split()
            qubit = int(info[0])
            connect = [int(c) for c in info[2:]]
            for qc in connect:
                if qc > qubit:
                    storage.append("{a} {b}\n".format(a=qubit, b=qc))
            line = f.readline()

    with open(
        "{root}/{name}".format(root=args.output_root, name=args.device), "w"
    ) as f:
        f.write(str(qubit_num) + " " + str(len(storage)) + "\n")
        for i in storage:
            f.write(i)


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument("--device", type=str, help="device", required=True)
    parser.add_argument(
        "--output_root", type=Path, help="Output file directory", default="./"
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
