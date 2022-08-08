import os
import sys

from parse import *

folder = sys.argv[1]
output = sys.argv[2]

single_gate_time = 1
double_gate_time = 2
for file in os.listdir(folder):
    if file.split(".")[-1] != "qasm":
        continue
    with open("%s/%s" % (folder, file), "r") as f:
        f.readline()
        f.readline()
        line = f.readline()
        qubit_number = int(parse("qreg q[{}];\n", line)[0])
        depth = [0 for _ in range(qubit_number)]
        g = 0
        while True:
            line = f.readline()
            if line == "":
                # print("end")
                break
            elif "creg" in line:
                pass
            else:
                g += 1
                [gate, qubit] = parse("{} {};\n", line)
                if gate == "cx":
                    [q1, q2] = parse("q[{}],q[{}]", qubit)
                    q1, q2 = int(q1), int(q2)
                    if depth[q1] > depth[q2]:
                        start_time = depth[q1]
                        depth[q2] = depth[q1]
                    else:
                        start_time = depth[q2]
                        depth[q1] = depth[q2]
                    depth[q1] += double_gate_time
                    depth[q2] += double_gate_time
                else:
                    q = int(parse("q[{}]", qubit)[0])
                    start_time = depth[q]
                    depth[q] += single_gate_time
        print(
            file + " & " + str(qubit_number) + " & " + str(max(depth)) + " & " + str(g)
        )
        with open(output, "a") as w:
            w.write(
                file
                + " & "
                + str(qubit_number)
                + " & "
                + str(max(depth))
                + " & "
                + str(g)
                + "\n"
            )
