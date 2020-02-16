package main

import (
	"bufio"
	"errors"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"

	"time"

	"github.com/akamensky/argparse"
)

const hybridThreshold = 11

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

func fromFile(file *os.File) (matrix, error) {
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

func fromPath(path string) (matrix, error) {
	file, err := os.Open(path)
	if err != nil {
		return [][]float64{}, err
	}
	defer file.Close()

	return fromFile(file)
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

func (m matrix) print(pretty bool) {
	n := m.dimension()

	max := 0
	for j := 0; j < n; j++ {
		for i := 0; i < n; i++ {
			v := int64(m[j][i])
			s := len(fmt.Sprintf("%d", v))
			if s > max {
				max = s
			}
		}
	}

	var format string
	if pretty {
		format = fmt.Sprintf("%%%dd ", max)
	} else {
		format = "%d "
	}

	for j := 0; j < n; j++ {
		for i := 0; i < n; i++ {
			v := int64(m[j][i])
			fmt.Printf(format, v)
		}
		fmt.Printf("\n")
	}
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

	if hybrid && n < hybridThreshold {
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

func main() {
	parser := argparse.NewParser("multiply", "Matrices Multiplicator")
	useNaive := parser.Flag("1", "naive",
		&argparse.Options{
			Required: false,
			Help:     "use naive algorithm",
		})

	useStrassen := parser.Flag("2", "strassen",
		&argparse.Options{
			Required: false,
			Help:     "use strassen algorithm",
		})

	useHybrid := parser.Flag("3", "hybrid",
		&argparse.Options{
			Required: false,
			Help:     "use hybrid algorithm",
		})

	doTime := parser.Flag("", "time",
		&argparse.Options{
			Required: false,
			Help:     "benchmark execution time",
		})

	doPrint := parser.Flag("p", "print",
		&argparse.Options{
			Required: false,
			Help:     "print the resulting matrix",
		})

	doPrettyPrint := parser.Flag("", "pretty",
		&argparse.Options{
			Required: false,
			Help:     "enable pretty-printing of the resulting matrix",
		})

	matrixAFile := parser.File("a", "matrix-a", os.O_RDONLY, 0600,
		&argparse.Options{
			Required: true,
			Help:     "the first matrix to multiply",
		})

	matrixBFile := parser.File("b", "matrix-b", os.O_RDONLY, 0600,
		&argparse.Options{
			Required: true,
			Help:     "the second matrix to multiply",
		})

	err := parser.Parse(os.Args)
	if err != nil {
		fmt.Fprint(os.Stderr, parser.Usage(err))
		os.Exit(1)
	}

	count := 0
	if *useNaive {
		count++
	}

	if *useStrassen {
		count++
	}

	if *useHybrid {
		count++
	}

	if count > 1 {
		fmt.Fprintln(os.Stderr, "error: multiple algorithms selected")
		os.Exit(1)
	}

	if count == 0 {
		*useNaive = true
	}

	matrixA, err := fromFile(matrixAFile)
	if err != nil {
		fmt.Fprintln(os.Stderr, err.Error())
		os.Exit(1)
	}

	matrixB, err := fromFile(matrixBFile)
	if err != nil {
		fmt.Fprintln(os.Stderr, err.Error())
		os.Exit(1)
	}

	var result matrix

	start := time.Now()
	if *useNaive {
		if result, err = matrixA.mulNaive(matrixB); err != nil {
			fmt.Fprintln(os.Stderr, err.Error())
			os.Exit(1)
		}
	}

	if *useStrassen {
		if result, err = matrixA.mulStrassen(matrixB, false); err != nil {
			fmt.Fprintln(os.Stderr, err.Error())
			os.Exit(1)
		}
	}

	if *useHybrid {
		if result, err = matrixA.mulStrassen(matrixB, true); err != nil {
			fmt.Fprintln(os.Stderr, err.Error())
			os.Exit(1)
		}
	}
	elapsed := time.Since(start)

	if *doPrettyPrint {
		*doPrint = true
	}

	if *doPrint {
		result.print(*doPrettyPrint)
	}

	if *doTime {
		fmt.Println(float64(elapsed) / 1e6)
	}
}
