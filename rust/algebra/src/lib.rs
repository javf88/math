struct Matrix {
    rows: usize,
    cols: usize,
    // what is better a long vector or a vec of vectors?
    val: Vec<f32>
}

impl Matrix {
    fn new(rows: usize, cols: usize) -> Matrix {
        Matrix {
            rows,
            cols,
            val: Vec::with_capacity(rows * cols)
        }
    }

    fn from(col: Vec<f32>) -> Matrix {
        Matrix {
            rows: col.len(),
            cols: 1,
            val: col.to_vec()
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn empty() {
        let a = Matrix::new(0,  0);
        assert_eq!(a.rows, 0);
        assert_eq!(a.cols, 0);
        assert_eq!(a.val.capacity(), 0);

        let b = Matrix::new(3,  0);
        assert_eq!(b.rows, 3);
        assert_eq!(b.cols, 0);
        assert_eq!(b.val.capacity(), 0);

        let c = Matrix::new(0,  3);
        assert_eq!(c.rows, 0);
        assert_eq!(c.cols, 3);
        assert_eq!(c.val.capacity(), 0);
    }

    #[test]
    fn malloc() {
        let d = Matrix::new(3, 3);
        assert_eq!(d.rows, 3);
        assert_eq!(d.cols, 3);
        assert_eq!(d.val.capacity(), 9);
        // memory has only be allocated
        assert_eq!(d.val.len(), 0);
    }

    #[test]
    fn from_col_vector() {
        // TODO: maybe to change from int to f32?
        let src = vec![1f32, 2f32, 3f32, 4f32, 5f32];
        let dst = Matrix::from(src);
        assert_eq!(dst.rows, 5);
        assert_eq!(dst.cols, 1);
        assert_eq!(dst.val.len(), 5);
    }
}
