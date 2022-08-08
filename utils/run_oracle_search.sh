# Move run_oracle.sh run_benchmark.sh modify.py to build/

export LD_PRELOAD=/usr/local/lib/libmimalloc.so
export OMP_NUM_THREADS=8

repo_root='/home/chinyi0523/QFT-mapping'
data_root='/home/chinyi0523/QFT-mapping/benchmark/original/EDA_traditional/'
depth=2
candidates=-1
exec_single=true
result_f=result_oracle_${depth}_${exec_single}.log
time_f=time_oracle_${depth}_${exec_single}.log

echo "Results to benchmarks" > $result_f
echo "Running times to benchmarks" > $time_f
echo "Exec_single: $exec_single, Depth: $depth" >> $result_f
echo "Exec_single: $exec_single, Depth: $depth" >> $time_f
for name in q17_xmg q432_xmg q499_xmg q880_xmg q1355_xmg q1908_xmg q2670_xmg q3540_xmg q5315_xmg q6288_xmg q7552_xmg; do
    python3 modify.py --direc_root $repo_root --data_root $data_root --filename $name --oracle 't' --depth $depth --candidates $candidates --exec_single $exec_single
    echo $name >> $result_f
    echo $name >> $time_f
    { time ./Duostra ../config.json >> $result_f; } 2>> $time_f
    echo "" >> $result_f
    echo "" >> $time_f
done    
