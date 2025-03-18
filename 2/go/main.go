package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	a := []int{1, 2, 3, 4, 5, 6, 7, 8, 9}

	b := remove(a, 4)
	c := remove(a, 0)
	g := remove(a, len(a)-1)
	fmt.Println("Original slice:", a)
	fmt.Println("Slice after removal:", b)
	fmt.Println("Slice after removal:", c)
	fmt.Println("Slice after removal:", g)

	d, err := os.ReadFile("./input.txt")
	if err != nil {
		panic(err)
	}

	lines := strings.Split(string(d), "\n")
	rows := make([][]int, 0, len(lines))

	for i, line := range lines {
		values := strings.Split(line, " ")

		row := make([]int, 0, len(values))
		for _, val := range values {
			if val == "" {
				continue
			}

			num, err := strconv.Atoi(val)
			if err != nil {
				panic(fmt.Sprintf("Error converting line %d: %v", i, err))
			}

			row = append(row, num)
		}

		rows = append(rows, row)
	}

	count := 0
	countWithLooserRules := 0
	for _, row := range rows {
		initialSafety := validRow(row)
		if initialSafety {
			count++
			countWithLooserRules++
			continue
		}

		for i := 0; i < len(row); i++ {
			newRow := remove(row, i)
			safe := validRow(newRow)
			if safe {
				countWithLooserRules++
				break
			}
		}
	}

	fmt.Println("Valid rows:", count)
	fmt.Println("Valid rows with looser rules:", countWithLooserRules)
	fmt.Println("Total rows:", len(rows))
}

func remove(slice []int, s int) []int {
	newSlice := make([]int, len(slice))
	copy(newSlice, slice)
	return append(newSlice[:s], newSlice[s+1:]...)
}

func validRow(row []int) bool {
	var prevStatus *int
	for i := 0; i < len(row)-1; i++ {
		status := 0

		if row[i] == row[i+1] {
			status = 1
		}

		if row[i] > row[i+1] {
			status = 2
		}

		if prevStatus == nil {
			prevStatus = &status
		}

		diff := row[i] - row[i+1]

		allowedDiff := (diff >= -3 && diff <= -1) || (diff >= 1 && diff <= 3)

		if !allowedDiff {
			return false
		}

		if status != *prevStatus {
			return false
		}
	}

	return true
}
