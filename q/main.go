package main

import (
	"encoding/json"
	"fmt"
	"io"
	"os"
)

const fileName = "data/q_5.json"

func main() {
	var ops operations

	file := checkAssign(os.Open(fileName))
	data := checkAssign(io.ReadAll(file))

	json.Unmarshal(data, &ops)

	cnots := filterCNot(ops.asSlice())

	starts, pairs := orderByTime(cnots)

	fmt.Println(starts, pairs)
}
