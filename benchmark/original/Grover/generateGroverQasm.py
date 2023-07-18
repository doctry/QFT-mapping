from argparse import ArgumentParser, Namespace
from pathlib import Path


def decomposeCRZ(w, ang, ctrl, targ):
    w.write("p(pi/{ang}) q[{ctrl}];\n".format(ang=format(ang * 2, ".1E"), ctrl=ctrl))
    w.write("p(pi/{ang}) q[{targ}];\n".format(ang=format(ang * 2, ".1E"), targ=targ))
    w.write("cx q[{ctrl}],q[{targ}];\n".format(ctrl=ctrl, targ=targ))
    w.write("p(-pi/{ang}) q[{targ}];\n".format(ang=format(ang * 2, ".1E"), targ=targ))
    w.write("cx q[{ctrl}],q[{targ}];\n".format(ctrl=ctrl, targ=targ))


def HGate(w, qb, type):
    if type == "IBM":
        w.write("rz(pi/2) q[{}];\n".format(qb))
        w.write("sx q[{}];\n".format(qb))
        w.write("rz(pi/2) q[{}];\n".format(qb))
    else:
        w.write("h q[{}];\n".format(qb))


def CCXGate(w, qb, type):
    if type == "":
        w.write("ccx q[{c0}],q[{c1}],q[{t}];\n".format(c0=qb[0], c1=qb[1], t=qb[2]))
    else:
        HGate(w, qb[2], type)
        w.write("cx q[{c}],q[{t}];\n".format(c=qb[1], t=qb[2]))
        w.write("tdg q[{}];\n".format(qb[2]))
        w.write("cx q[{c}],q[{t}];\n".format(c=qb[0], t=qb[2]))
        w.write("t q[{}];\n".format(qb[2]))
        w.write("cx q[{c}],q[{t}];\n".format(c=qb[1], t=qb[2]))
        w.write("t q[{}];\n".format(qb[1]))
        w.write("tdg q[{}];\n".format(qb[2]))
        w.write("cx q[{c}],q[{t}];\n".format(c=qb[0], t=qb[2]))
        w.write("cx q[{c}],q[{t}];\n".format(c=qb[0], t=qb[1]))
        w.write("t q[{}];\n".format(qb[2]))
        w.write("t q[{}];\n".format(qb[0]))
        w.write("tdg q[{}];\n".format(qb[1]))
        HGate(w, qb[2], type)
        w.write("cx q[{c}],q[{t}];\n".format(c=qb[0], t=qb[1]))


def CCXGateSet1(w, peak, type):
    CCXGate(w, [0, 1, peak], type)
    for i in range(2, peak):
        CCXGate(w, [i, i + peak - 2, i + peak - 1], type)
    for i in range(peak - 2, 1, -1):
        CCXGate(w, [i, i + peak - 2, i + peak - 1], type)
    if peak > 2:
        CCXGate(w, [0, 1, peak], type)


def CCXGateSet2(w, peak, type):
    CCXGate(w, [0, 1, peak], type)
    for i in range(2, peak - 2):
        CCXGate(w, [i, i + peak - 2, i + peak - 1], type)
    CCXGate(w, [peak - 2, 2 * (peak - 2), peak - 1], type)
    for i in range(peak - 3, 1, -1):
        CCXGate(w, [i, i + peak - 2, i + peak - 1], type)
    if peak > 2:
        CCXGate(w, [0, 1, peak], type)


def main(args):
    if args.qnum % 2 == 0 or args.qnum < 3:
        print("Qubit number should be an odd number and bigger than 2")
        return
    if args.type not in ["IBM", "Cli", ""]:
        print("Wrong type. Type should be either IBM, Cli or '' (empty)")
    with open(
        "{root}/grover_{num}{type}.qasm".format(
            root=args.output_root, num=args.qnum, type=args.type
        ),
        "w",
    ) as qasmf:
        qasmf.write('OPENQASM 2.0;\ninclude "qelib1.inc";\n')
        qasmf.write("qreg q[{}];\n".format(args.qnum))
        halfSize = int((args.qnum + 1) / 2)
        for i in range(halfSize):
            HGate(qasmf, i, args.type)
        HGate(qasmf, args.qnum - 1, args.type)
        CCXGateSet1(qasmf, halfSize, args.type)
        for i in range(halfSize):
            HGate(qasmf, i, args.type)
        for i in range(halfSize):
            qasmf.write("x q[{}];\n".format(i))
        HGate(qasmf, halfSize - 1, args.type)
        if args.qnum == 3:
            qasmf.write("cx q[0],q[1];\n")
        elif args.qnum == 5:
            CCXGate(qasmf, [0, 1, 2], args.type)
        else:
            CCXGateSet2(qasmf, halfSize, args.type)
        HGate(qasmf, halfSize - 1, args.type)
        for i in range(halfSize):
            qasmf.write("x q[{}];\n".format(i))
        for i in range(halfSize):
            HGate(qasmf, i, args.type)
        HGate(qasmf, (args.qnum) - 1, args.type)


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--qnum", type=int, help="Size of Grover. Odd and >=3", required=True
    )
    parser.add_argument("--type", type=str, help="Decompose type", default="")
    parser.add_argument(
        "--output_root", type=Path, help="Output file directory", default="./"
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
