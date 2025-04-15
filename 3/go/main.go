package main

import (
	"fmt"
	"os"
	"regexp"
	"strconv"
)

type Interval struct {
	Start   int
	End     int
	Allowed bool
}

func main() {
	mulExp := regexp.MustCompile(`mul\((\d+),(\d+)\)`)
	doExp := regexp.MustCompile(`do\(\)`)
	dontExpt := regexp.MustCompile(`don't\(\)`)

	d, err := os.ReadFile("./input.txt")
	if err != nil {
		panic(err)
	}

	numMatches := mulExp.FindAllStringSubmatch(string(d), -1)
	numIndexMatches := mulExp.FindAllStringSubmatchIndex(string(d), -1)
	doMatches := doExp.FindAllStringIndex(string(d), -1)
	dontMatches := dontExpt.FindAllStringIndex(string(d), -1)

	intervalRange := buildInterval(nil, doMatches, dontMatches)
	fmt.Println(intervalRange)

	total := 0
	totalWithInterval := 0
	for i, match := range numMatches {
		num1, err := strconv.Atoi(match[1])
		if err != nil {
			panic(err)
		}

		num2, err := strconv.Atoi(match[2])
		if err != nil {
			panic(err)
		}

		total += num1 * num2

		allowed := inInterval(numIndexMatches[i][0], intervalRange)
		fmt.Println(fmt.Sprintf("(%d,%d) index:%d %b", num1, num2, numIndexMatches[i][0], allowed))
		if allowed {
			totalWithInterval += num1 * num2
		}
	}

	println("Total:", total)
	println("Total with interval:", totalWithInterval)
}

func buildInterval(intervalRange []Interval, do, dont [][]int) []Interval {
	if len(do) == 0 && len(dont) == 0 {
		return intervalRange
	}

	if len(do) != 0 && len(dont) == 0 {
		intervalRange, do = mapOneInterval(intervalRange, do, true)
		return buildInterval(intervalRange, do, dont)
	}

	if len(do) == 0 && len(dont) != 0 {
		intervalRange, dont = mapOneInterval(intervalRange, dont, false)
		return buildInterval(intervalRange, do, dont)
	}

	doVal := do[0][0]
	dontVal := dont[0][0]
	if doVal < dontVal {
		intervalRange, do = mapOneInterval(intervalRange, do, true)
		return buildInterval(intervalRange, do, dont)
	}

	intervalRange, dont = mapOneInterval(intervalRange, dont, false)
	return buildInterval(intervalRange, do, dont)
}

func mapOneInterval(intervalRange []Interval, arr [][]int, allowed bool) ([]Interval, [][]int) {
	if !allowed && len(intervalRange) == 0 {
		return []Interval{{
			Start:   0,
			End:     arr[0][0] - 1,
			Allowed: true,
		}}, arr
	}

	if allowed && len(intervalRange) == 0 {
		return nil, arr[1:]
	}

	prev := intervalRange[len(intervalRange)-1]
	if prev.Allowed == allowed {
		return intervalRange, arr[1:]
	}

	prev.End = arr[0][0] - 1
	intervalRange[len(intervalRange)-1] = prev
	intervalRange = append(intervalRange, Interval{
		Start:   arr[0][0],
		End:     arr[0][1],
		Allowed: allowed,
	})

	return intervalRange, arr[1:]
}

func inInterval(i int, intervalRange []Interval) bool {
	for j, interval := range intervalRange {
		if i >= interval.Start && (i <= interval.End || j == len(intervalRange)-1) {
			return interval.Allowed
		}
	}

	return false
}
