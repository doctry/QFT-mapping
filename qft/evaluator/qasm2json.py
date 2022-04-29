#!/usr/bin/env python
# coding: utf-8

# In[40]:


import json

from parse import *
from qiskit import *

# In[206]:


qubit_number = 27
single_gate_time = 0
double_gate_time = 1


# In[207]:


qc = QuantumCircuit.from_qasm_file("circuit\\real_device\q_%d.qasm" % qubit_number)


# In[208]:


dic = {"Operations": [], "final_cost": 0}
depth = [0 for _ in range(qubit_number)]
for i in range(len(qc)):
    gate = parse("<qiskit.circuit.library.standard_gates.{}.{} at {}", str(qc[i][0]))
    if gate[1] == "CXGate object":
        qubit = parse(
            "[Qubit(QuantumRegister(%d, 'q'), {}), Qubit(QuantumRegister(%d, 'q'), {})]"
            % (qubit_number, qubit_number),
            str(qc[i][1]),
        )
        if depth[int(qubit[0])] > depth[int(qubit[1])]:
            start_time = depth[int(qubit[0])]
            depth[int(qubit[1])] = depth[int(qubit[0])]
        else:
            start_time = depth[int(qubit[1])]
            depth[int(qubit[0])] = depth[int(qubit[1])]
        depth[int(qubit[0])] += double_gate_time
        depth[int(qubit[1])] += double_gate_time
        d = {
            "Operator": "Cnot",
            "Qubits": [int(qubit[0]), int(qubit[1])],
            "duration": [start_time, start_time + double_gate_time],
        }
    else:
        start_time = depth[int(qubit[0])]
        qubit = parse(
            "[Qubit(QuantumRegister(%d, 'q'),{})]" % qubit_number, str(qc[i][1])
        )
        depth[int(qubit[0])] += single_gate_time
        d = {
            "Operator": str(gate[0]),
            "Qubits": [int(qubit[0])],
            "duration": [start_time, start_time + single_gate_time],
        }
    dic["Operations"].append([d])
dic["final_cost"] = max(depth)


# In[209]:


# dic
print(dic["final_cost"])


# In[190]:


json_object = json.dumps(dic, indent=4)
with open("circuit\json\q_%d.json" % qubit_number, "w") as outfile:
    outfile.write(json_object)


# In[210]:


qc.depth()


# In[ ]:
