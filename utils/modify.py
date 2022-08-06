# Move run_oracle.sh run_benchmark.sh modify.py to build/

import json
from argparse import ArgumentParser, Namespace

q2device = {
    "q17_xmg": "guadalupe_16.txt",
    "q432_xmg": "topo_433.txt",
    "q499_xmg": "topo_433.txt",
    "q880_xmg": "topo_433.txt",
    "q1355_xmg": "topo_433.txt",
    "q1908_xmg": "topo_433.txt",
    "q2670_xmg": "topo_1121.txt",
    "q3540_xmg": "topo_1121.txt",
    "q5315_xmg": "topo_2129.txt",
    "q6288_xmg": "topo_1121.txt",
    "q7552_xmg": "topo_2129.txt",
}


def main(args):
    root = args.data_root
    name = args.filename
    f = open("../config.json")
    conf = json.load(f)

    conf["device"] = "{}/device/guadalupe_16.txt".format(args.direc_root)
    conf["algo"] = root + "/" + name + ".qasm"

    if args.oracle == "t":
        dev = q2device[name]
        conf["device"] = "/home/chinyi0523/QFT-mapping/device/" + dev

    conf["mapper"]["greedy_conf"]["depth"] = int(args.depth)
    conf["mapper"]["greedy_conf"]["candidates"] = int(args.candidates)
    conf["mapper"]["greedy_conf"]["exec_single"] = (
        False if args.exec_single == "false" else True
    )
    f.close()
    f = open("../config.json", "w")

    json.dump(conf, f, indent=4)

    f.close()


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument("--direc_root", type=str, help="repository root", required=True)
    parser.add_argument("--data_root", type=str, help="data root", default="./")
    parser.add_argument("--oracle", type=str, help="run oracle benchmark", default="t")
    parser.add_argument("--filename", type=str, help="filename", required=True)
    parser.add_argument("--depth", type=str, help="data root", default="1")
    parser.add_argument(
        "--exec_single",
        type=str,
        help="execute single-qubit gate directly",
        default="false",
    )
    parser.add_argument("--candidates", type=str, help="keep top k", default="-1")
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
