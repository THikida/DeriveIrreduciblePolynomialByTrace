<script type="text/javascript" async src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.7/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
 MathJax.Hub.Config({
 tex2jax: {
 inlineMath: [['$', '$'] ],
 displayMath: [ ['$$','$$'], ["\\[","\\]"] ]
 }
 });
</script>

# DeriveIrreduciblePolynomialByTrace
Thank you for viewing.  Here is an example of a C++ implementation of the proposed method (Algorithm 1) introduced in "Trace Values Calculation of Type-II ONB Elements and Its Application for Deriving an Irreducible Polynomial".

## Usage

1. Please install [GMP](https://gmplib.org/) and [NTL](https://libntl.org/doc/tour-gmp.html)
1. Please compile with the following
    ```
    g++ -Wall -g -O2 -std=c++17 -pthread -march=native -mtune=native der_poly_type2onb.cpp -o der_poly_type2onb.o -lgmp -lgmpxx -lntl  
    ```
1. If you enter the parameters $p$ and $m$ that satisfy the Type-II ONB conditions as shown below, the irreducible polynomial will display on the standard output.
    ```
    der_poly_type2onb.o p m
    ```
    In addition, if the conditions are not met, the following will be displayed.
    ```
    Input does not meet the conditions.
    ```

### Example

If you want to derive an irreducible polynomial with $p=2$ and $m=5$, input the following.
```
der_poly_type2onb.o 2 5
```
The output should be as follows.
```
[1 1 1 0 1 1]
x^5 + x^4 + x^2 + x + 1
```