echo "File Directory at $1"
for file in 17 432 499 880 1355 1908 2670 3540 5315 6288 7552; do
    for type in 3npn_mcnc 3npn mcnc_q mcnc xmg; do
        python3 convert2qasm.py --qc "$1/q${file}_${type}.qc" 
    done
done