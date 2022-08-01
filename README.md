# Provable good and Scalable Qubit Mapping
This project want to map quantum circuit with more than 1000 qubits on real-world device. Current providing Quantum Fourier Transform scalable mapping.

## Compile and Run
```bash=
# Clone the repository
git clone https://github.com/doctry/qft-mapping
cd qft-mapping
git submodule update --init --recursive

# Install pytorch
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip

# Compile cmake
git submodule update --init --recursive
mkdir QFT-mapping/qft/duostra/build
cd QFT-mapping/qft/duostra/build
cmake -DCMAKE_PREFIX_PATH=<Path to libtorch> -DCMAKE_BUILD_TYPE=<Release|Debug> ..
cmake --build .

# Run
./Duostra ../config.json

# Profiling
valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes ./Duostra ../config.json
```

## Settings
The following settings needs to be given in qft/duostra/config.json
Please construct the config.json by the provided config_sample.json
### Scheduler
* Static:  The first item in the wait list (longest time in wait list).
* Random:  Random from wait list.
* Greedy:  Find minimum cost (APSP+max(sup(Gate)))
### Placer
* Static: Match ID with the IDs in IBM topology.
* DFS: Match ID with the DFS topological order.
* Random: Just Random place.
### Router
* Orientation: Small topology first.
* Naive: Without considering any orientation.
### Algo
* Directly give a number, e.g. 127 would provide a 127 qubit QFT.
* Give the path of logical circuit.
* The provided benchmark of logical circuit is given in  ``` benchmark/ ```
### Device
* The device are provided in  ``` device/ ```, please give the path of txt file.
## Result

### Experiment 1: Without Synthesis
* Placer: DFS
* Scheduler: Static/Greedy
* Router: Orientation
* Upperbound: $O(n^{3})$
* Realistic: $\approx n^{1.5}$

|  Machine   | #Qubit | Static | Greedy |
| ---------- | ------ | ------ | ------ |
| IBM-????   |  11969 | 2514327 | 5713536 |
| IBM-????   |   5105 |  878169 | 1659527 |
| IBM-????   |   3457 |  541048 |  869439 |
| IBM-????   |   2129 |  304439 |  422602 |
| IBM-2023   |   1121 | 139918 | 178256 |
| IBM-2022   |    433 |  41326 |  43592 |
| Washington |    127 |   8369 |   8020 |
| Brooklyn   |     65 |   3436 |   2834 |
| Kolkata    |     27 |    904 |    759 |
| Guadalupe  |     16 |    366 |    334 |
| Perth      |      7 |     99 |     82 |
| Quito      |      5 |     39 |     44 |

### Experiment 2: With Synthesis
* Initial: IBM Synthesis
* Placer: DFS
* Scheduler: Static/Greedy
* Router: Orientation

|  Machine   | #Qubit | Static | Greedy |
| ---------- | ------ | ------ | ------ |
| Washington |    127 |   3877 |   3046 |
| Brooklyn   |     65 |   1969 |   1687 |
| Kolkata    |     27 |    777 |    745 |
| Guadalupe  |     16 |    370 |    366 |
| Perth      |      7 |     85 |     82 |

## IBM Result
### Synthesis and Mapping

* #CNOT: Numbers of CNOT after synthesis
* Cost: Cost after mapping

| Machine    | #Qubit | #CNOT | Cost |
| --------   | ------ | ----- | ---- | 
| Washington |    127 |  4012 | 9022 | 
| Brooklyn   |     65 |  1904 | 3584 |
| Kolkata    |     27 |   612 | 1001 |
| Guadalupe  |     16 |   240 |  358 |
| Perth      |      7 |    42 |  103 |
| Quito      |      5 |    20 |   32 |
