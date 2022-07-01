from argparse import ArgumentParser, Namespace
from pathlib import Path
from typing import Any, Dict, List


def getQasmBit(bits: List[str],qcBit2qasmBit: Dict[str,int], qasmBit: int) -> List[int]: 
    result = []
    for bit in bits:
        if bit not in qcBit2qasmBit.keys():
            qcBit2qasmBit[bit] = qasmBit
            result.append(qasmBit)
            qasmBit +=1
        else:
            result.append(qcBit2qasmBit[bit])
    return result, qasmBit

def convertCCX(w, qasmBits):
    assert(len(qasmBits)==3)
    # Depth 0
    w.append("h q[{}];\n".format(qasmBits[2]))
    # Depth 1
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[1],targ=qasmBits[2]))
    # Depth 2
    w.append("tdg q[{}];\n".format(qasmBits[2]))
    # Depth 3
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[0],targ=qasmBits[2]))
    # Depth 4
    w.append("t q[{}];\n".format(qasmBits[2]))
    # Depth 5
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[1],targ=qasmBits[2]))
    # Depth 6
    w.append("tdg q[{}];\n".format(qasmBits[2]))
    # Depth 7
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[0],targ=qasmBits[2]))
    # Depth 8
    w.append("t q[{}];\n".format(qasmBits[1]))
    w.append("t q[{}];\n".format(qasmBits[2]))
    # Depth 9
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[0],targ=qasmBits[1]))
    w.append("h q[{}];\n".format(qasmBits[2]))
    # Depth 10
    w.append("t q[{}];\n".format(qasmBits[0]))
    w.append("tdg q[{}];\n".format(qasmBits[1]))
    # Depth 11
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[0],targ=qasmBits[1]))

def convertCX(w, qasmBits):
    assert(len(qasmBits)==2)
    w.append("cx q[{ctrl}], q[{targ}];\n".format(ctrl=qasmBits[0],targ=qasmBits[1]))

def convertX(w, qasmBits):
    assert(len(qasmBits)==1)
    w.append("x q[{}];\n".format(qasmBits[0]))

def main(args):
    qcBit2qasmBit = dict()
    qasmBit=0
    buff = []
    with open(args.qc, "r") as qcf:
        for i in range(5):
            line = qcf.readline()
        while line != "":
            line = line.split()[1:]
            gtype = line[0]
            bits = line[1:]
            bits = [bit[:-1] for bit in bits]
            qasmBits, qasmBit = getQasmBit(bits,qcBit2qasmBit,qasmBit)
            if(gtype=="cx"):
                convertCX(buff, qasmBits)
            elif(gtype=="x"):
                convertX(buff, qasmBits)
            elif(gtype=="ccx"):
                convertCCX(buff, qasmBits)
            line = qcf.readline()
    outfile = args.qc.split("/")[-1].split(".")[0]
    
    with open("{root}/{file}.qasm".format(root=args.output_root,file=outfile), "w") as qasmf:
        qasmf.write('OPENQASM 2.0;\ninclude "qelib1.inc";\n')
        qasmf.write("qreg q[{}];\n".format(len(qcBit2qasmBit)))
        for item in buff:
            qasmf.write(item)       
    print("Save file to {root}/{file}.qasm".format(root=args.output_root,file=outfile))       


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--qc", type=str, help="Input qc file", required=True
    )
    parser.add_argument(
        "--output_root", type=Path, help="Output file directory", default="./"
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
