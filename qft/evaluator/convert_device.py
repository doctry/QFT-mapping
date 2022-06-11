import sys

if __name__ == "__main__":
    filename = sys.argv[1]
    outname = sys.argv[2]
    output = []
    with open(filename,"r") as f:
        line = f.readline()
        qubit_n = line
        line = f.readline()

        while(line):
            line = line.split()
            start = int(line[0])
            connect = line[2:]
            # print(line)
            for qubit in connect:
                if start < int(qubit):
                    output.append(str(start)+" "+qubit+"\n")
            line = f.readline()
            
    with open(outname,"w") as w:
        w.write(qubit_n[:-1]+" "+str(len(output))+"\n")
        for pri in output:
            w.write(pri)

