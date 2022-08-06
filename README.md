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
make -j

# Run
./Duostra ../config.json

# Profiling
valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes ./Duostra ../config.json
```

## Settings
The following settings needs to be given in qft/duostra/config.json
Please construct the config.json by the provided config_sample.json
### Mapper
#### Scheduler
* static:  The first item in the wait list (longest time in wait list).
* random:  Random from wait list.
* greedy:  Find minimum cost (APSP+max(sup(Gate)))
* dora (alias cks): Search based scheduler
##### Greedy Conf (change name to scheduler conf ?)
* candidates: ``positive int or -1, default -1``. Keep top k results, -1 denotes all.
* apsp_coef: ``positive int, default 1``. $\alpha$ in $C_{greedy}=\max(v_0.occu, v_1.occu)+\dfrac{APSP(v_0,v_1)}{\alpha}$
* cost: ``min/max, default min``. Schedule based on min/max cost
* depth: ``positive int, default 1`` Search depth.
* exec_single: ``bool, default false``. true for directly execute single=qubit gates when available; false for schedule with double-qubits gates
* never_cache: ``bool, default true``,
#### Placer
* static: Match ID with the IDs in IBM topology.
* dfs: Match ID with the DFS topological order.
* random: Just Random place.
#### Router
* Duostra:
### Algo
* Directly give a number, e.g. 127 would provide a 127 qubit QFT.
* Give the path of logical circuit.
    * The provided benchmark of logical circuit is given in  ``` benchmark/ ```
### Device
* The device are provided in  ``` device/ ```, please give the path of txt file.

