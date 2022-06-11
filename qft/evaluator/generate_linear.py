import sys

if __name__ == "__main__":
    size = int(sys.argv[1])
    outname = sys.argv[2]
    output = []
            
    with open(outname,"w") as w:
        w.write(str(size)+" "+str(size)+"\n")
        for i in range(size):
            w.write(str(i)+" "+str((i+1)%(size))+"\n")
