#include <NTL/ZZ.h>
#include <NTL/ZZ_pEX.h>
#include <gmpxx.h>

using std::cout;
using std::endl;
using std::floor;
using namespace NTL;

// Function to perform mpz_init() on an array
void InitMPZArray(mpz_t *a, unsigned long long int size) {
    for (unsigned long long int i = 0; i < size; i++) {
        mpz_init(a[i]);
    }
}

// Function to display a polynomial to standard output
void DisplayPolynomial(ZZ_pX poly, unsigned long long int m) {
    unsigned long long int i = m;
    cout << poly << endl;
    for (i = m; i > 0; i--) {
        if (coeff(poly, i) == 0) {
            continue;
        } else if (coeff(poly, i) == 1) {
            if (i == 1) {
                cout << "x + ";
            } else {
                cout << "x^" << i << " + ";
            }
        } else {
            if (i == 1) {
                cout << coeff(poly, i) << "x + ";
            } else {
                cout << coeff(poly, i) << "x^" << i << " + ";
            }
        }
    }
    cout << coeff(poly, 0) << endl;
}

// Function to check if Type-II ONB conditions are met
bool IsSatisfyCondition(char *char_p, char *char_m) {
    mpz_t p, m, r;
    mpz_init(p);
    mpz_init(m);
    mpz_init(r);
    mpz_set_str(p, char_p, 10);
    mpz_set_str(m, char_m, 10);
    mpz_mul_ui(r, m, 2);
    mpz_add_ui(r, r, 1);

    // Condition 1
    if ((mpz_probab_prime_p(p, 50) == 0) or (mpz_probab_prime_p(r, 50) == 0) or
        (mpz_cmp(p, r) == 0))
        return 0;

    // Precalculation of condition2
    mpz_t tmp, p_order, double_m;
    mpz_init(tmp);
    mpz_init(p_order);
    mpz_init(double_m);
    mpz_set(tmp, p);
    mpz_set_ui(p_order, 1);
    mpz_mul_ui(double_m, m, 2);

    mpz_t i;
    mpz_init(i);

    for (mpz_set_ui(i, 0); mpz_cmp(i, r) <= 0; mpz_add_ui(i, i, 1)) {
        mpz_mul(tmp, tmp, p);
        mpz_mod(tmp, tmp, r);
        mpz_add_ui(p_order, p_order, 1);
        if (mpz_cmp_ui(tmp, 1) == 0) break;
    }

    // condition 2a
    if ((mpz_cmp(p_order, double_m) == 0)) {
        return 1;
    }
    // condition 2b
    if ((mpz_cmp(p_order, m) == 0) and (mpz_odd_p(m) != 0)) {
        return 1;
    }

    return 0;
}

// Function to derive polynomial by the proposed method (Algorithm 1)
ZZ_pX DerivePolyByTrace(mpz_t *s, char *char_p, char *char_m) {
    unsigned long long int m = strtoull(char_m, NULL, 10);
    unsigned long long int mprime, i, j, k, k_max;

    mpz_t p, c[m - 2 + 1], t[m + 1], tmp;
    mpz_init(p);
    mpz_init(tmp);
    mpz_set_str(p, char_p, 10);
    InitMPZArray(c, m - 2 + 1);
    InitMPZArray(t, m + 1);

    mprime = m + 1;
    mpz_set_ui(t[0], 1);
    mpz_sub_ui(t[1], p, 1);
    for (i = 0; i <= m - 2; i++) {
        mpz_set_ui(c[i], 1);
        mpz_set_ui(t[i + 2], 0);
    }

    for (i = 2; i <= m; i++) {
        j = i - 2;
        k_max = floor(i / 2);
        for (k = 1; k <= k_max; k++) {
            mpz_mul_ui(c[j], c[j], (mprime - j - k));
            mpz_tdiv_q_ui(c[j], c[j], k);
            mpz_addmul(t[i], c[j], t[j]);
            j = j - 2;
        }
        if (i % 2 == 0) {
            mpz_ui_sub(t[i], 1, t[i]);
            mpz_mod(t[i], t[i], p);
            mpz_set(s[m - i], t[i]);
        } else {
            mpz_add_ui(s[m - i], t[i], 1);
            mpz_mod(s[m - i], s[m - i], p);
            mpz_sub(t[i], p, s[m - i]);
            mpz_mod(t[i], t[i], p);
        }
    }
    mpz_set_ui(s[m - 1], 1);

    ZZ_p::init(conv<ZZ>(char_p));
    ZZ_pX poly;
    poly.SetLength(m);
    char char_s[1000] = "0";
    for (i = 0; i < m; i++) {
        mpz_get_str(char_s, 10, s[i]);
        SetCoeff(poly, i, to_ZZ_p(conv<ZZ>(char_s)));
    }
    SetCoeff(poly, m, to_ZZ_p(conv<ZZ>("1")));

    return poly;
}

int main(int argc, char *argv[]) {
    char *p_val = argv[1], *m_val = argv[2];

    if (argc != 3) {
        cout << "Usage: [object file] p m" << endl;
        return 0;
    }

    if (IsSatisfyCondition(p_val, m_val) == 0) {
        cout << "Input does not meet the conditions." << endl;
        return 0;
    }

    if (atoi(m_val) == 1) {
        cout << "1 cannot be used for the extension degree m." << endl;
        return 0;
    }

    unsigned long long int m = strtoull(m_val, NULL, 10);
    mpz_t s[m];
    ZZ_pX poly;
    InitMPZArray(s, m);

    poly = DerivePolyByTrace(s, p_val, m_val);
    DisplayPolynomial(poly, m);

    return 0;
}