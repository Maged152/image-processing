# Function Configuration

Types that parameterize the behavior of the library functions.

## TermCriteria

Stop condition of the iterative algorithms (e.g. OpticalFlowPyrLK Newton iterations).
Defined in [`include/common/types.hpp`](../../../../include/common/types.hpp).

```c++
struct TermCriteria
{
    int    max_count = 0;   // maximum number of iterations
    double epsilon  = 0.0;  // convergence tolerance
};
```


