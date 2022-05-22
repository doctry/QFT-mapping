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
