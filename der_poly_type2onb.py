from sympy import isprime
import argparse


def make_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("p", help="Characteristic p", type=int)
    parser.add_argument("m", help="Extension degree m", type=int)
    return parser


# Function to display a polynomial to standard output
def display_polynomial(s, m: int):
    print(s)

    print("x^{}".format(m), end=' ')
    for i in reversed(range(1, m)):
        if s[i] == 0:
            continue
        elif s[i] == 1:
            if i == 1:
                print("+ x", end=' ')
            else:
                print("+ x^{}".format(i), end=' ')
        else:
            if i == 1:
                print("+ {}x".format(s[i]), end=' ')
            else:
                print("+ {}x^{}".format(s[i], i), end=' ')
    print("+ {}".format(s[0]))


# Function to check if Type-II ONB conditions are met
def is_satisfy_condition(p: int, m: int) -> bool:
    r = int(2*m + 1)

    # Condition 1
    if (not isprime(p)) or (not isprime(r)) or (p == r):
        return False

    # Precalculation of condition2
    tmp = int(1)
    p_order = int(0)
    double_m = int(2 * m)

    for i in range(1,r):
        tmp = (tmp * p) % r
        p_order += 1
        if(tmp == 1):
            break

    # Condition 2a
    if p_order == double_m:
        return True 

    # Condition 2b
    if (p_order == m) and (m & 1):
        return True 

    return False


# Function to derive polynomial by the proposed method (Algorithm 1)
def derive_poly_by_trace(p: int, m: int):
    m_prime = int(m + 1)
    s = [int(0)] * (m + 1)
    c = [int(1)] * (m - 2 + 1)
    t = [int(0)] * (m + 1)

    t[0] = 1
    t[1] = p - 1

    for i in range(2, m+1):
        j = i - 2
        k_max = int(i/2)
        for k in range(1, k_max+1):
            c[j] = int((c[j] * (m_prime-j-k))/k)
            t[i] = t[i] + (c[j] * t[j])
            j = j - 2

        if i % 2 == 0:
            t[i] = (1 - t[i]) % p
            s[m - i] = t[i]
        else:
            s[m - i] = (1 + t[i]) % p
            t[i] = (-s[m - i]) % p
    
    s[m - 1] = 1
    s[m] = 1

    return s


def main(p, m):
    if is_satisfy_condition(p, m) is False:
        print("Input does not meet the conditions.")
        return 0
    elif m == 1:
        print("1 cannot be used for the extension degree m.")
        return 0
    else:
        poly = derive_poly_by_trace(p, m)
        display_polynomial(poly, m)

    return 0


if __name__ == "__main__":
    args = make_parser().parse_args()
    main(args.p, args.m)