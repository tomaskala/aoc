package main

import (
	"bufio"
	"errors"
	"fmt"
	"math"
	"os"
)

var (
	errQueueFull  = errors.New("Queue full")
	errQueueEmpty = errors.New("Queue empty")
)

type queue struct {
	channel chan position
}

func newQueue(capacity int) *queue {
	return &queue{
		channel: make(chan position, capacity),
	}
}

func (q *queue) enqueue(pos position) error {
	select {
	case q.channel <- pos:
		return nil
	default:
		return errQueueFull
	}
}

func (q *queue) dequeue() (position, error) {
	select {
	case val := <-q.channel:
		return val, nil
	default:
		return position{0, 0}, errQueueEmpty
	}
}

type position struct {
	row, col int
}

func bfs(grid [][]byte, start, end position) (int, error) {
	rows := len(grid)
	cols := len(grid[0])

	directions := [][]int{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}
	visited := make([][]bool, rows)
	costs := make([][]int, rows)

	for row := range grid {
		visited[row] = make([]bool, cols)
		costs[row] = make([]int, cols)
	}

	queue := newQueue(rows * cols)
	queue.enqueue(start)

	for {
		curr, err := queue.dequeue()
		if err != nil {
			return math.MaxInt, err
		}

		if curr == end {
			return costs[curr.row][curr.col], nil
		}

		for _, dir := range directions {
			next := position{curr.row + dir[0], curr.col + dir[1]}
			if next.row == -1 || next.row == rows ||
				next.col == -1 || next.col == cols ||
				visited[next.row][next.col] ||
				int(grid[next.row][next.col])-int(grid[curr.row][curr.col]) > 1 {
				continue
			}

			costs[next.row][next.col] = costs[curr.row][curr.col] + 1
			visited[next.row][next.col] = true

			err = queue.enqueue(next)
			if err != nil {
				return math.MaxInt, err
			}
		}
	}
}

func main() {
	var grid [][]byte
	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		line := []byte(scanner.Text())
		grid = append(grid, line)
	}

	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}

	var start, end position

	for i, row := range grid {
		for j, pos := range row {
			if pos == 'S' {
				start = position{i, j}
				grid[i][j] = 'a'
			} else if pos == 'E' {
				end = position{i, j}
				grid[i][j] = 'z'
			}
		}
	}

	cost, err := bfs(grid, start, end)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
	}

	fmt.Println(cost)
}
