class Gate:
    def __init__(self, id, qubits, prev):
        self.id = id
        self.qubits = qubits
        self.prev = []
        self.next = []
        if prev[0]["id"]!= -1:
            self.prev.append(prev[0])
        if prev[0]["id"]!=prev[1]["id"]:
            if prev[1]["id"]!= -1:  
                self.prev.append(prev[1])
                
    def is_avail(self):
        avail = True
        for item in self.prev:
            if item["done"] == False:
                avail = False
                break
        return avail
    
    def finished(self, prevId):
        for item in self.prev:
            if item["id"] == prevId:
                item["done"] = True
        
    def add_next(self,id):
        if id!=-1:
            self.next.append(id)
    
    def get_next(self):
        return self.next

class Topology:
    def __init__(self, file):
        self.qubit2gate = qubit2gate = dict()
        self.gateList = list()
        self.avail_gates = list()
        self.parseQasm(file)

    def get_avail_gates(self):
        return self.avail_gates

    def update_avail_gates(self, executedId):
        self.avail_gates.remove(executedId)
        nexts = self.gateList[executedId].get_next()
        for item in nexts:
            self.gateList[item].finished(executedId)
            
            if(self.gateList[item].is_avail()):
                self.avail_gates.append(item)

    def parseQasm(self,file):
        with open(file,'r') as f:
            for i in range(3):
                line = f.readline()
            qubit_num = int(line.split()[1].split('[')[1].split(']')[0])
            for i in range(qubit_num):
                self.qubit2gate[i] = -1
            id = 0
            while line != "":
                line = f.readline()
                li = line.split(" ")
                gtype = li[0]
                if gtype == 'cx':
                    [q1,q2] = li[1].split(',')
                    q1 = int(q1[2:-1])
                    q2 = int(q2[2:-3])
                    temp = Gate(id,[q1,q2],[{"id":self.qubit2gate[q1],"done":False},{"id":self.qubit2gate[q2],"done":False}])
                    if self.qubit2gate[q1]!=-1:
                        self.gateList[self.qubit2gate[q1]].add_next(id)
                    if self.qubit2gate[q1]!=self.qubit2gate[q2]:
                        if self.qubit2gate[q2]!=-1:
                            self.gateList[self.qubit2gate[q2]].add_next(id)

                    self.qubit2gate[q1] = id
                    self.qubit2gate[q2] = id
                    self.gateList.append(temp)
                    id += 1
        
        for gate in self.gateList:
            if gate.is_avail():
                self.avail_gates.append(gate.id)
        
        