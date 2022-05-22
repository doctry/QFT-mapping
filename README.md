# QFT-mapping
This project want to map QFT with more than 1000 qubits on real-world device.

## Compile and Run
```bash=
# Install pytorch
wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip
unzip libtorch-shared-with-deps-latest.zip

# Compile cmake
mkdir QFT-mapping/qft/duostra/build
cd QFT-mapping/qft/duostra/build
cmake -DCMAKE_PREFIX_PATH=<Path to libtorch> ..
cmake --build . --config Release

# Run
./Duostra ../config.json
```

## Settings
### Scheduler
* Static:  The first item in the wait list (longest time in wait list).
* Old: Follow the sequence.
* Random:  Random from wait list.
* Greedy:  Find minimum cost (APSP+max(sup(Gate)))
### Placer
* Static: Match ID with the IDs in IBM topology.
* DFS: Match ID with the DFS topological order.
* Random: Just Random place.
### Router
* Orientation: Small topology first.
* Naive: Without considering any orientation.

## Result

### Experiment 1: Without Synthesis
* Placer: DFS
* Scheduler: Static/Greedy
* Router: Orientation
* Upperbound: $O(n^{3})$
* Realistic: $\approx n^{1.5}$

|  Machine   | #Qubit | Static | Greedy |
| ---------- | ------ | ------ | ------ |
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