from argparse import ArgumentParser, BooleanOptionalAction, Namespace
from os.path import exists

benchmark2qubit = {
    "9symml_195": 11,
    "cycle10_2_110": 12,
    "inc_237": 16,
    "rd53_251": 8,
    "sqrt8_260": 12,
    "adr4_197": 13,
    "dc2_222": 15,
    "life_238": 11,
    "rd73_252": 10,
    "square_root_7": 15,
    "cm42a_207": 14,
    "dist_223": 13,
    "mlp4_245": 16,
    "rd84_253": 12,
    "urf1_149": 9,
    "cm82a_208": 8,
    "ham15_107": 15,
    "pm1_249": 14,
    "root_255": 13,
    "urf1_278": 9,
    "cm85a_209": 14,
    "hwb8_113": 9,
    "qft_10": 10,
    "sqn_258": 10,
    "urf2_277": 8,
    "z4_268": 11,
}
table = dict()


def checkfiles():
    if not (exists("result_TOQM.log") & exists("time_TOQM.log")):
        print("TOQM result and/or time not exist")
        exit()
    if not (exists("result_search.log") & exists("time_search.log")):
        print("Search-based result and/or time not exist")
        exit()
    if not (exists("result_greedy.log") & exists("time_greedy.log")):
        print("Search-based result and/or time not exist")
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
    return name


def main(args):
    checkfiles()

    with open("result_TOQM.log") as f:
        f.readline()
        benchmark = f.readline()[:-1]
        while benchmark != "":
            gate = int(f.readline().split()[0][2:])
            ideal = int(f.readline().split()[0][2:])
            cost_TOQM = int(f.readline().split()[0][2:])
            table[benchmark] = {
                "qubit": benchmark2qubit[benchmark],
                "gate": gate,
                "ideal": ideal,
                "cost_TOQM": cost_TOQM,
            }
            f.readline()
            benchmark = f.readline()[:-1]
    f.close()

    with open("time_TOQM.log") as f:
        f.readline()
        benchmark = f.readline()[:-1]
        while benchmark != "":
            f.readline()
            f.readline()
            time = f.readline().split()[-1]
            sec = time2sec(time)
            table[benchmark]["time_TOQM"] = sec
            f.readline()
            f.readline()
            benchmark = f.readline()[:-1]
    f.close()

    for scheduler in ["greedy", "search"]:
        with open("result_{}.log".format(scheduler)) as f:
            f.readline()
            benchmark = f.readline()[:-1]
            while benchmark != "":
                for _ in range(8):
                    f.readline()
                if scheduler == "greedy":
                    for _ in range(3):
                        f.readline()
                cost = int(f.readline().split()[-1])
                table[benchmark]["cost_{}".format(scheduler)] = cost
                for _ in range(3):
                    f.readline()
                benchmark = f.readline()[:-1]

        with open("time_{}.log".format(scheduler)) as f:
            f.readline()
            benchmark = f.readline()[:-1]
            while benchmark != "":
                f.readline()
                time = f.readline().split()[-1]
                sec = time2sec(time)
                table[benchmark]["time_{}".format(scheduler)] = sec
                for _ in range(3):
                    f.readline()
                benchmark = f.readline()[:-1]

    table.pop("qft_10", None)
    tables = {k: v for k, v in sorted(table.items(), key=lambda item: item[1]["gate"])}
    tabless = {
        k: v for k, v in sorted(tables.items(), key=lambda item: item[1]["qubit"])
    }
    with open("table.txt", "w") as w:
        w.write(
            "{bench}  &  {qubit}  &  {gate}  &  {ideal}  &  {cost_TOQM}  &  {time_TOQM}  &  {cost_greedy}  &  {time_greedy}  &  {C_greedy}  &  {T_greedy}  &  {cost_search}  &  {time_search}  &  {C_search}  &  {T_search}  \\\\\n".format(
                bench="% benchmark".ljust(16, " "),
                qubit="#Q".rjust(2, " "),
                gate="#Gate".center(7, " "),
                ideal="Ideal".center(7, " "),
                cost_TOQM="cost_TO".center(7, " "),
                time_TOQM="time_TO".center(7, " "),
                cost_greedy="cost_GR".center(7, " "),
                time_greedy="time_GR".center(7, " "),
                C_greedy="CIR_GR".center(7, " "),
                T_greedy="TIx_GR".center(7, " "),
                cost_search="cost_SE".center(7, " "),
                time_search="time_SE".center(7, " "),
                C_search="CIR_SE".center(7, " "),
                T_search="TIx_SE".center(7, " "),
            )
        )
        count = 0
        for benchmark in tabless.keys():
            count += 1
            if args.shading:
                if count % 2 == 0:
                    w.write("\\rowcolor{Gray}\n")

            w.write(
                "{bench}  &  {qubit}  &  {gate}  &  {ideal}  &  {cost_TOQM}  &  {time_TOQM}  &  {cost_greedy}  &  {time_greedy}  &  {C_greedy}  &  {T_greedy}  &  {cost_search}  &  {time_search}  &  {C_search}  &  {T_search}  \\\\\n".format(
                    bench=satisfyOverleaf(benchmark).ljust(16, " "),
                    qubit=str(table[benchmark]["qubit"]).rjust(2, " "),
                    gate="{:,}".format(table[benchmark]["gate"]).rjust(7, " "),
                    ideal="{:,}".format(table[benchmark]["ideal"]).rjust(7, " "),
                    cost_TOQM="{:,}".format(table[benchmark]["cost_TOQM"]).rjust(
                        7, " "
                    ),
                    time_TOQM="{:,.2f}".format(table[benchmark]["time_TOQM"]).rjust(
                        7, " "
                    ),
                    cost_greedy="{:,}".format(table[benchmark]["cost_greedy"]).rjust(
                        7, " "
                    ),
                    time_greedy="{:,.2f}".format(table[benchmark]["time_greedy"]).rjust(
                        7, " "
                    ),
                    C_greedy="{:,.2f}".format(
                        (
                            table[benchmark]["cost_TOQM"]
                            - table[benchmark]["cost_greedy"]
                        )
                        * 100
                        / table[benchmark]["cost_TOQM"]
                    ).rjust(7, " "),
                    T_greedy="{:,.1f}".format(
                        (
                            table[benchmark]["time_TOQM"]
                            / table[benchmark]["time_greedy"]
                        )
                    ).rjust(7, " "),
                    cost_search="{:,}".format(table[benchmark]["cost_search"]).rjust(
                        7, " "
                    ),
                    time_search="{:,.2f}".format(table[benchmark]["time_search"]).rjust(
                        7, " "
                    ),
                    C_search="{:,.2f}".format(
                        (
                            table[benchmark]["cost_TOQM"]
                            - table[benchmark]["cost_search"]
                        )
                        * 100
                        / table[benchmark]["cost_TOQM"]
                    ).rjust(7, " "),
                    T_search="{:,.1f}".format(
                        (
                            table[benchmark]["time_TOQM"]
                            / table[benchmark]["time_search"]
                        )
                    ).rjust(7, " "),
                )
            )


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--transform2HHMMSS", default=False, action=BooleanOptionalAction
    )
    parser.add_argument("--shading", default=False, action=BooleanOptionalAction)
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
