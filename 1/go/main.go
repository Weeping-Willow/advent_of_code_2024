package main

import (
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"
)

func main() {
	d, err := os.ReadFile("./input.txt")
	if err != nil {
		panic(err)
	}

	lines := strings.Split(string(d), "\n")
	rowA, rowB := make([]int, 0, len(lines)), make([]int, 0, len(lines))

	for i, line := range lines {
		values := strings.Split(line, " ")

		numA, err := strconv.Atoi(values[0])
		if err != nil {
			panic(fmt.Sprintf("invalid input line a #%d", i))
		}

		numB, err := strconv.Atoi(values[len(values)-1])
		if err != nil {
			panic(fmt.Sprintf("invalid input line b #%d", i))
		}

		rowA = append(rowA, numA)

		rowB = append(rowB, numB)
	}

	sort.Ints(rowA)
	sort.Ints(rowB)

	diff := 0
	for i, a := range rowA {
		b := rowB[i]

		tempDiff := a - b
		if tempDiff < 0 {
			tempDiff *= -1
		}

		diff += tempDiff
	}

	println(diff)
}
