root=$1
echo "Output at root $1"
for name in quito_5 perth_7 guadalupe_16 kolkata_27 brooklyn_65 washington_127 topo_433 topo_1121 topo_2129 topo_3457 topo_5105 topo_7073 topo_11969; do
    python3 convert2TOQM.py --device ${name}.txt --output_root $1
done