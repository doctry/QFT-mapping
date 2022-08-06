# Move run_oracle.sh run_benchmark.sh modify.py to build/

export LD_PRELOAD=/usr/local/lib/libmimalloc.so
export OMP_NUM_THREADS=8

repo_root='/home/chinyi0523/QFT-mapping'
data_root='/home/chinyi0523/QFT-mapping/benchmark/original/large/'
depth=1
candidates=-1
exec_single=false

echo "Results to benchmarks" > result.log
echo "Running times to benchmarks" > time.log

for name in 9symml_195 cycle10_2_110 inc_237 d53_251 sqrt8_260 z4_268 adr4_197 dc2_222 life_238 rd73_252 square_root_7 cm42a_207 dist_223 mlp4_245 rd84_253 urf1_149 cm82a_208 ham15_107 pm1_249 root_255 urf1_278 cm85a_209 hwb8_113 qft_10 sqn_258 urf2_277; do
    python3 modify.py --direc_root $repo_root --data_root $data_root --filename $name --oracle 'f' --depth $depth --candidates $candidates --exec_single $exec_single
    echo $name >> result.log
    echo $name >> time.log
    # { time ./Duostra ../config.json >> result.log; } 2>> time.log
    echo "" >> result.log
    echo "" >> time.log
done    