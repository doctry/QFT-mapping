from argparse import ArgumentParser, BooleanOptionalAction, Namespace
from os.path import exists

table = dict()


def checkfiles(single, depth):
    if not (exists("info.txt") & exists("result_oracle_greedy.log") & exists("time_oracle_greedy.log")):
        print("info.txt / Greedy result and/or time not exist")
        exit()
    for i in range(depth):
        if not (exists("result_oracle_{dep}_{sin}.log".format(dep=i+1,sin='true' if single else 'false')) & exists("time_oracle_{dep}_{sin}.log".format(dep=i+1,sin='true' if single else 'false'))):
            print("Search-based with depth {} result and/or time not exist".format(i+1))
            exit()


def time2sec(time):
    temp = time.split("h")
    hour = int(temp[0]) if len(temp) > 1 else 0
    temp = temp[-1].split("m")
    min = int(temp[0])
    sec = float(temp[1][:-1])
    return hour * 3600 + min * 60 + sec


def satisfyOverleaf(name):
    name = name.split("_")
    name = "\\_".join(name)
    return "\\quad "+name


def main(args):
    checkfiles(args.exec_single, args.max_depth)
    with open("info.txt") as f:
        f.readline()
        while True:
            line = f.readline()
            if line == "":
                break
            else:
                benchmark, qubit, ideal, gate = line.split(" & ")
            
                table[benchmark] = {
                        "qubit": int(qubit),
                        "gate": int(gate[:-1]),
                        "ideal": int(ideal)
                }


    for scheduler in ["greedy", "search"]:
        depth = 1 if scheduler == "greedy" else args.max_depth
        
        for i in range(depth):
            with open("result_oracle_{}.log".format("greedy" if scheduler == "greedy" else "{dep}_{sin}".format(dep=i+1, sin='true' if args.exec_single else 'false'))) as f:
                f.readline()
                f.readline()
                benchmark = f.readline()[:-1]
                while benchmark != "":
                    for _ in range(8):
                        f.readline()
                    if scheduler == "greedy":
                        for _ in range(3):
                            f.readline()
                    cost = int(f.readline().split()[-1])
                    if scheduler == "greedy":
                        table[benchmark]["cost_{}".format(scheduler)] = cost
                    else:
                        table[benchmark]["cost_{sch}_{dep}".format(sch=scheduler,dep=i+1)] = cost
                    for _ in range(3):
                        f.readline()
                    benchmark = f.readline()[:-1]

            with open("time_oracle_{}.log".format("greedy" if scheduler == "greedy" else "1_{}".format('true' if args.exec_single else 'false'))) as f:
                f.readline()
                f.readline()
                benchmark = f.readline()[:-1]
                while benchmark != "":
                    f.readline()
                    time = f.readline().split()[-1]
                    sec = time2sec(time)
                    if scheduler == "greedy":
                        table[benchmark]["time_{}".format(scheduler)] = sec
                    else:
                        table[benchmark]["time_{sch}_{dep}".format(sch=scheduler,dep=i+1)] = sec
                    for _ in range(3):
                        f.readline()
                    benchmark = f.readline()[:-1]
    
    tables = {k: v for k, v in sorted(table.items(), key=lambda item: item[1]["gate"])}
    tabless = {
        k: v for k, v in sorted(tables.items(), key=lambda item: item[1]["qubit"])
    }

    with open("table.txt", "w") as w:
        w.write(
            "{bench}  &  {qubit}  &  {gate}  &  {ideal}  &  {cost_greedy}  &  {ratio_greedy}  &  {time_greedy}  &  {cost_d1}  &  {ratio_d1}  &  {time_d1}  &  {C_d1}  &  {T_d1}  \\\\\n".format(
                bench="% oracle".ljust(16, " "),
                qubit="#Q".rjust(6, " "),
                gate="#Gate".center(7, " "),
                ideal="Ideal".center(7, " "),
                cost_greedy="cost_GR".center(8, " "),
                time_greedy="time_GR".center(8, " "),
                ratio_greedy="ratio_GR".center(10, " "),
                C_greedy="CIR_GR".center(8, " "),
                T_greedy="TIx_GR".center(8, " "),
                cost_d1="cost_SE1".center(8, " "),
                time_d1="time_SE1".center(8, " "),
                ratio_d1="ratio_SE1".center(10, " "),
                C_d1="CIR_SE1".center(8, " "),
                T_d1="TIx_SE1".center(8, " "),
            )
        )
        for benchmark in tabless.keys():
            w.write(
                "{bench}  &  {qubit}  &  {gate}  &  {ideal}  &  {cost_greedy}  &  {ratio_greedy}  &  {time_greedy}  &  {cost_d1}  &  {ratio_d1}  &  {time_d1}  &  {C_d1}  &  {T_d1}  \\\\\n".format(
                    bench=satisfyOverleaf(benchmark).ljust(16, " "),
                    qubit="{:,}".format(table[benchmark]["qubit"]).rjust(6, " "),
                    gate="{:,}".format(table[benchmark]["gate"]).rjust(7, " "),
                    ideal="{:,}".format(table[benchmark]["ideal"]).rjust(7, " "),
                    cost_greedy="{:,}".format(table[benchmark]["cost_greedy"]).rjust(
                        8, " "
                    ),
                    time_greedy="{:,.2f}".format(table[benchmark]["time_greedy"]).rjust(
                        8, " "
                    ),
                    ratio_greedy="{:,.2f}".format(
                        (
                            table[benchmark]["cost_greedy"]
                            / table[benchmark]["ideal"]
                        )
                    ).rjust(10, " "),
                    cost_d1="{:,}".format(table[benchmark]["cost_search_1"]).rjust(
                        8, " "
                    ),
                    time_d1="{:,.2f}".format(table[benchmark]["time_search_1"]).rjust(
                        8, " "
                    ),
                    ratio_d1="{:,.2f}".format(
                        (
                            table[benchmark]["cost_search_1"]
                            / table[benchmark]["ideal"]
                        )
                    ).rjust(10, " "),
                    C_d1="{:,.2f}".format(
                        (
                            table[benchmark]["cost_greedy"]
                            - table[benchmark]["cost_search_1"]
                        )
                        * 100
                        / table[benchmark]["cost_greedy"]
                    ).rjust(8, " "),
                    T_d1="{:,.1f}".format(
                        (
                            table[benchmark]["time_search_1"]
                            / table[benchmark]["time_greedy"]
                        )
                    ).rjust(8, " "),
                )
            )


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--transform2HHMMSS", default=False, action=BooleanOptionalAction
    )
    parser.add_argument("--shading", default=False, action=BooleanOptionalAction)
    parser.add_argument("--exec_single", default=True, action=BooleanOptionalAction)
    parser.add_argument("--max_depth", type=int, default=0)
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
