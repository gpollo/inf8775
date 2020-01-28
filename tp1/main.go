package main

import (
	"bufio"
	"errors"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"
	"sync"
	"time"
)

const hybridThreshold = 10

type matrix [][]float64

func create(n int) (matrix, error) {
	if n%2 != 0 && n != 1 {
		return [][]float64{}, errors.New("Power must a multiple of 2")
	}

	m := make([][]float64, n)
	for j := 0; j < n; j++ {
		m[j] = make([]float64, n)
	}

	return m, nil
}

func fromFile(path string) (matrix, error) {
	file, err := os.Open(path)
	if err != nil {
		return [][]float64{}, err
	}
	defer file.Close()

	reader := bufio.NewReader(file)

	str, err := reader.ReadString('\n')
	if err != nil {
		return [][]float64{}, err
	}

	n, err := strconv.Atoi(str[0 : len(str)-1])
	if err != nil {
		return [][]float64{}, err
	}
	n = int(math.Pow(2, float64(n)))

	m, err := create(n)
	if err != nil {
		return [][]float64{}, err
	}

	for j := 0; j < n; j++ {
		str, err = reader.ReadString('\n')
		if err != nil {
			return [][]float64{}, err
		}

		splitted := strings.Split(strings.TrimSpace(str), "\t")
		if len(splitted) != n {
			return [][]float64{}, errors.New("Unexpected values count in line")
		}

		for i := 0; i < n; i++ {
			v, err := strconv.Atoi(splitted[i])
			if err != nil {
				return [][]float64{}, err
			}

			m[j][i] = float64(v)
		}
	}

	return m, nil
}

func (m matrix) dimension() int {
	sy := len(m)
	if sy == 0 {
		panic("Matrix Y dimension is 0")
	}

	sx := len(m[0])
	if sx == 0 {
		panic("Matrix X dimension is 0")
	}

	if sx != sy {
		panic("Matrix must be square")
	}

	if sx%2 != 0 && sx != 1 {
		panic("Matrix dimension must be a power of 2")
	}

	return sx
}

func (m matrix) dimensionEqual(o matrix) bool {
	return m.dimension() == o.dimension()
}

func (m matrix) add(o matrix) (matrix, error) {
	if !m.dimensionEqual(o) {
		return [][]float64{}, errors.New("Matrices must be of equal dimension")
	}
	n := m.dimension()

	r, err := create(n)
	if err != nil {
		return [][]float64{}, err
	}

	for j := 0; j < n; j++ {
		for i := 0; i < n; i++ {
			r[j][i] = m[j][i] + o[j][i]
		}
	}

	return r, nil
}

func (m matrix) sub(o matrix) (matrix, error) {
	if !m.dimensionEqual(o) {
		return [][]float64{}, errors.New("Matrices must be of equal dimension")
	}
	n := m.dimension()

	r, err := create(n)
	if err != nil {
		return [][]float64{}, err
	}

	for j := 0; j < n; j++ {
		for i := 0; i < n; i++ {
			r[j][i] = m[j][i] - o[j][i]
		}
	}

	return r, nil
}

func (m matrix) mulNaive(o matrix) (matrix, error) {
	if !m.dimensionEqual(o) {
		return [][]float64{}, errors.New("Matrices must be of equal dimension")
	}
	n := m.dimension()

	r, err := create(n)
	if err != nil {
		return [][]float64{}, err
	}

	for j := 0; j < n; j++ {
		for i := 0; i < n; i++ {
			r[j][i] = 0
			for k := 0; k < n; k++ {
				r[j][i] += m[j][k] * o[k][i]
			}
		}
	}

	return r, nil
}

func (m matrix) extract(quadrant int) (matrix, error) {
	n := m.dimension()

	var sx, sy int
	switch quadrant {
	case 1:
		sx = n / 2
		sy = 0
	case 2:
		sx = 0
		sy = 0
	case 3:
		sx = 0
		sy = n / 2
	case 4:
		sx = n / 2
		sy = n / 2
	default:
		return [][]float64{}, errors.New("Invalid quadrant")
	}

	r, err := create(n / 2)
	if err != nil {
		return [][]float64{}, err
	}

	for j := 0; j < n/2; j++ {
		for i := 0; i < n/2; i++ {
			r[j][i] = m[sy+j][sx+i]
		}
	}

	return r, nil
}

func (m matrix) insert(quadrant int, o matrix) error {
	if m.dimension() != 2*o.dimension() {
		return errors.New("Inserted matrix be half the size of the other")
	}
	n := m.dimension()

	var sx, sy int
	switch quadrant {
	case 1:
		sx = n / 2
		sy = 0
	case 2:
		sx = 0
		sy = 0
	case 3:
		sx = 0
		sy = n / 2
	case 4:
		sx = n / 2
		sy = n / 2
	default:
		return errors.New("Invalid quadrant")
	}

	for j := 0; j < n/2; j++ {
		for i := 0; i < n/2; i++ {
			m[sy+j][sx+i] = o[j][i]
		}
	}

	return nil
}

func (m matrix) mulStrassen(o matrix, hybrid bool) (matrix, error) {
	if !m.dimensionEqual(o) {
		return [][]float64{}, errors.New("Matrices must be of equal dimension")
	}
	n := m.dimension()

	if hybrid {
		return m.mulNaive(o)
	}

	if n == 1 {
		return [][]float64{{m[0][0] * o[0][0]}}, nil
	}

	// extract A11, A12, A21 and A22 submatrices
	a12, err := m.extract(1)
	if err != nil {
		return [][]float64{}, err
	}

	a11, err := m.extract(2)
	if err != nil {
		return [][]float64{}, err
	}

	a21, err := m.extract(3)
	if err != nil {
		return [][]float64{}, err
	}

	a22, err := m.extract(4)
	if err != nil {
		return [][]float64{}, err
	}

	// extract B11, B12, B21 and B22 submatrices
	b12, err := o.extract(1)
	if err != nil {
		return [][]float64{}, err
	}

	b11, err := o.extract(2)
	if err != nil {
		return [][]float64{}, err
	}

	b21, err := o.extract(3)
	if err != nil {
		return [][]float64{}, err
	}

	b22, err := o.extract(4)
	if err != nil {
		return [][]float64{}, err
	}

	// computes intermediate addition/substraction matrices
	t0, err := a11.add(a22)
	if err != nil {
		return [][]float64{}, err
	}

	t1, err := b11.add(b22)
	if err != nil {
		return [][]float64{}, err
	}

	t2, err := a21.add(a22)
	if err != nil {
		return [][]float64{}, err
	}

	t3, err := b12.sub(b22)
	if err != nil {
		return [][]float64{}, err
	}

	t4, err := b21.sub(b11)
	if err != nil {
		return [][]float64{}, err
	}

	t5, err := a11.add(a12)
	if err != nil {
		return [][]float64{}, err
	}

	t6, err := a21.sub(a11)
	if err != nil {
		return [][]float64{}, err
	}

	t7, err := b11.add(b12)
	if err != nil {
		return [][]float64{}, err
	}

	t8, err := a12.sub(a22)
	if err != nil {
		return [][]float64{}, err
	}

	t9, err := b21.add(b22)
	if err != nil {
		return [][]float64{}, err
	}

	// compute intermediate multiplication matrices
	m1, err := t0.mulStrassen(t1, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	m2, err := t2.mulStrassen(b11, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	m3, err := a11.mulStrassen(t3, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	m4, err := a22.mulStrassen(t4, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	m5, err := t5.mulStrassen(b22, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	m6, err := t6.mulStrassen(t7, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	m7, err := t8.mulStrassen(t9, hybrid)
	if err != nil {
		return [][]float64{}, err
	}

	// compute C11 result submatrix
	c11, err := m1.add(m4)
	if err != nil {
		return [][]float64{}, err
	}

	if c11, err = c11.sub(m5); err != nil {
		return [][]float64{}, err
	}

	if c11, err = c11.add(m7); err != nil {
		return [][]float64{}, err
	}

	// compute C12 result submatrix
	c12, err := m3.add(m5)
	if err != nil {
		return [][]float64{}, err
	}

	// compute C21 result submatrix
	c21, err := m2.add(m4)
	if err != nil {
		return [][]float64{}, err
	}

	// compute C22 result submatrix
	c22, err := m1.sub(m2)
	if err != nil {
		return [][]float64{}, err
	}

	if c22, err = c22.add(m3); err != nil {
		return [][]float64{}, err
	}

	if c22, err = c22.add(m6); err != nil {
		return [][]float64{}, err
	}

	// create result matrix
	c, err := create(n)
	if err != nil {
		return [][]float64{}, err
	}

	if err = c.insert(1, c12); err != nil {
		return [][]float64{}, err
	}

	if err = c.insert(2, c11); err != nil {
		return [][]float64{}, err
	}

	if err = c.insert(3, c21); err != nil {
		return [][]float64{}, err
	}

	if err = c.insert(4, c22); err != nil {
		return [][]float64{}, err
	}

	return c, nil
}

func benchmark(m matrix) (uint64, uint64, uint64, error) {
	resultNaive := uint64(0)
	resultStrassen := uint64(0)
	resultHybrid := uint64(0)

	var err error

	count := 1
	for i := 0; i < count; i++ {
		start := time.Now()
		_, err = m.mulNaive(m)
		resultNaive += uint64(time.Since(start))
		if err != nil {
			return 0, 0, 0, err
		}

		start = time.Now()
		_, err = m.mulStrassen(m, false)
		resultStrassen += uint64(time.Since(start))
		if err != nil {
			return 0, 0, 0, err
		}

		start = time.Now()
		_, err = m.mulStrassen(m, true)
		resultHybrid += uint64(time.Since(start))
		if err != nil {
			return 0, 0, 0, err
		}
	}

	resultNaive /= uint64(count)
	resultStrassen /= uint64(count)
	resultHybrid /= uint64(count)

	return resultNaive, resultStrassen, resultHybrid, nil
}

type dataset struct {
	pathfmt    string
	dimension  int
	variations []int

	resultNaive    uint64
	resultStrassen uint64
	resultHybrid   uint64
}

func (d *dataset) run() error {
	d.resultNaive = 0
	d.resultStrassen = 0
	d.resultHybrid = 0

	for i := 0; i < len(d.variations); i++ {
		filename := fmt.Sprintf(d.pathfmt, d.dimension, d.variations[i])
		fmt.Println("Running test", filename, "...")

		m, err := fromFile(filename)
		if err != nil {
			return err
		}

		r1, r2, r3, err := benchmark(m)
		if err != nil {
			return err
		}

		d.resultNaive += r1
		d.resultStrassen += r2
		d.resultHybrid += r3
	}

	d.resultNaive /= uint64(len(d.variations))
	d.resultStrassen /= uint64(len(d.variations))
	d.resultHybrid /= uint64(len(d.variations))

	return nil
}

var datasets []dataset = []dataset{
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  5,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  6,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  7,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  8,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  9,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  10,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  11,
		variations: []int{1, 2, 3, 4, 5},
	},
	dataset{
		pathfmt:    "gen_matrix/ex_%d.%d",
		dimension:  12,
		variations: []int{1, 2, 3, 4, 5},
	},
}

func main() {
	var wg sync.WaitGroup

	ch := make(chan *dataset, 1000)
	for i := 0; i < 4; i++ {
		go func() {
			for {
				d, ok := <-ch
				if !ok {
					break
				}

				if err := d.run(); err != nil {
					fmt.Println(err.Error())
					break
				}
			}

			wg.Done()
		}()
	}
	wg.Add(4)

	for i := 0; i < len(datasets); i++ {
		ch <- &datasets[i]
	}
	wg.Wait()
	close(ch)

	for i := 0; i < len(datasets)/3; i++ {
		fmt.Printf("%d,%d,%d,%d\n",
			datasets[i].dimension,
			datasets[i].resultNaive,
			datasets[i].resultStrassen,
			datasets[i].resultHybrid,
		)
	}
}
