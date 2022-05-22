from argparse import ArgumentParser, Namespace
from pathlib import Path


def calculate(w, h):
    neighbor = dict()
    print("Width: {wi} Height: {he}".format(wi=w, he=h))
    ## row start:   0       1(4w-1)+1w-2+1  2(4w-1)+2w-2+1
    ## row end  :   4w-1-2  2(4w-1)+1w-2    3(4w-1)+2w-2
    id = 0
    for i in range(h):
        base = i * (4 * w - 1) + (i - 1) * w - 2
        end = (i + 1) * (4 * w - 1) + (i) * w - 2
        for j in range(4 * w - 1):
            if i == 0:
                if j == 4 * w - 2:
                    break
                neighbor[id] = []
                if j > 0:
                    neighbor[id].append(id - 1)
                if j != 4 * w - 3:
                    neighbor[id].append(id + 1)
                if j % 4 == 0:
                    neighbor[id].append(int(4 * w - 2 + j / 4))
            elif i == h - 1:
                if j == 4 * w - 2:
                    break
                neighbor[id] = []
                if j % 4 == 1:
                    neighbor[id].append(int(base + 1 + j / 4))
                if j > 0:
                    neighbor[id].append(id - 1)
                if j < 4 * w - 3:
                    neighbor[id].append(id + 1)
            else:
                neighbor[id] = []
                if i % 2 == 1:
                    if j % 4 == 0:
                        neighbor[id].append(int(base + 1 + j / 4))
                    if j % 4 == 2:
                        neighbor[id].append(int(end + 1 + j / 4))
                else:
                    if j % 4 == 2:
                        neighbor[id].append(int(base + 1 + j / 4))
                    if j % 4 == 0:
                        neighbor[id].append(int(end + 1 + j / 4))
                if j > 0:
                    neighbor[id].append(id - 1)
                if j < 4 * w - 2:
                    neighbor[id].append(id + 1)
            id += 1
        for j in range(w):
            if i == 0:
                neighbor[id] = []
                neighbor[id].append(4 * j)
                neighbor[id].append(5 * w - 2 + 4 * j)
                id += 1
            elif i == h - 2:
                neighbor[id] = []
                neighbor[id].append(base + w + 3 + 4 * j)
                neighbor[id].append(end + w + 2 + 4 * j)
                id += 1
            elif i < h - 1:
                if i % 2 == 1:
                    neighbor[id] = []
                    neighbor[id].append(end - 4 * (w - j - 1))
                    neighbor[id].append(end + w + 3 + 4 * j)
                else:
                    neighbor[id] = []
                    neighbor[id].append(base + w + 1 + 4 * j)
                    neighbor[id].append(end + w + 1 + 4 * j)
                id += 1
    return neighbor


def main(args):
    w = args.width
    h = args.height
    if h % 2 == 0:
        print("Error: Height should be a odd number")
        return
    neighbor = calculate(w, h)
    with open(
        "{root}/topo_{n}.txt".format(root=args.output_root, n=len(neighbor)), "w"
    ) as f:
        f.write(str(len(neighbor)) + "\n")
        for i in neighbor:
            neighbor[i].sort()
            neighbor[i] = [str(j) for j in neighbor[i]]
            f.write(
                "{i} {l} {n}\n".format(i=i, l=len(neighbor[i]), n=" ".join(neighbor[i]))
            )


def parse_args() -> Namespace:
    parser = ArgumentParser()
    parser.add_argument(
        "--width", type=int, help="Numbers of blocks in width", required=True
    )
    parser.add_argument(
        "--height",
        type=int,
        help="Numbers of rows in height (should be odd)",
        required=True,
    )
    parser.add_argument(
        "--output_root", type=Path, help="Output file directory", default="./"
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()
    main(args)
