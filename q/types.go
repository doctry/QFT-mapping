package main

import (
	"fmt"
	"sort"
	"strings"

	"github.com/samber/lo"
)

type operations struct {
	Operations [][1]generalGate
}

func (op operations) asSlice() []generalGate {
	gg := make([]generalGate, len(op.Operations))

	for i, o := range op.Operations {
		gg[i] = o[0]
	}

	return gg
}

type generalGate struct {
	Operator string
	Qubits   []int
	Duration [2]int
}

type pair struct {
	left, right int
}

type cNotGate struct {
	pair

	startTime int
	endTime   int
}

func filterCNot(gg []generalGate) []cNotGate {
	gates := make([]cNotGate, 0)

	for _, g := range gg {
		if strings.ToLower(g.Operator) != "cnot" {
			continue
		}

		gates = append(gates,
			cNotGate{
				pair:      pair{g.Qubits[0], g.Qubits[1]},
				startTime: g.Duration[0],
				endTime:   g.Duration[1],
			},
		)
	}

	return gates
}

type swapGate struct {
	cNots [3]cNotGate
}

type rGate struct {
	cNots [2]cNotGate
}

func parseSwap(starts *[]int, pairs map[int]pair) bool { return false }
func parseR(starts *[]int, pairs map[int]pair)         {}
func parseOnce(starts *[]int, pairs map[int]pair) {
	for parseSwap(starts, pairs) {
	}

	parseR(starts, pairs)
}
func parse(starts *[]int, pairs map[int]pair) {
	for len(*starts) > 0 {
		parseOnce(starts, pairs)
	}
}

func orderByTime(cnots []cNotGate) (startTimes []int, gatePairs map[int]pair) {
	starts := make([]int, len(cnots))
	gatePairs = make(map[int]pair)

	sort.Slice(cnots, func(i, j int) bool {
		return cnots[i].startTime < cnots[j].startTime
	})

	for i, cnot := range cnots {
		if cnot.endTime != cnot.startTime+1 {
			fmt.Printf("I am very surprised that start=%d and end=%d.\n", cnot.startTime, cnot.endTime)
		}

		starts[i] = cnot.startTime
		gatePairs[i] = cnot.pair
	}

	length := lo.Max(starts)
	startTimes = make([]int, length+1)

	for _, s := range starts {
		startTimes[s]++
	}

	return
}
