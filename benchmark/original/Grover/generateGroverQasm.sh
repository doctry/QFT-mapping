for file in 5 7 15 27 65 127 433 1121 2129 3457 5105 7073 9361 11969; do
    echo "Generating size = $file"
    python3 generateGroverQasm.py --qnum ${file}                                    
    echo "Generating size = $file Clifford + T"
    python3 generateGroverQasm.py --qnum ${file} --type Cli
    echo "Generating size = $file for IBM"
    python3 generateGroverQasm.py --qnum ${file} --type IBM                      
done