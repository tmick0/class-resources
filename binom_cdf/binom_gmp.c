#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

/** calculates the product from n*(n-1)*(n-2)*...*(k+1)*(k), equivalent
    to n!/(k-1)!.
 
    @param res: GMP integer to store resulting product in
    @param n:   high end of product sequence
    @param k:   low end of product sequence
 */
void partial_factorial(mpz_t res, unsigned n, unsigned k){
        mpz_set_ui(res, k);
        while(k < n){
                mpz_mul_ui(res, res, ++k);
        }
}

/** calculates the factorial of n.
 
    @param res: GMP integer to store resulting factorial in
    @param n:   integer to compute factorial of
 */
void factorial(mpz_t res, unsigned n){
        partial_factorial(res, n, 1);
}

/** calculates the binomial coefficient, n-choose-k, which gives the number of
    ways k elements can be chosen from a set of n elements.
 
    @param res: GMP integer to store resulting coefficient in
    @param n:   number of elements in the full set
    @param k:   number of elements to choose from the set
 */
void binom(mpz_t res, unsigned n, unsigned k){
        mpz_t num, den;
        mpz_inits(num, den, 0);
        
        /* numerator is the product n*...*(k+1), which is equivalent to n!/k!
         */
        partial_factorial(num, n, k+1);
        
        /* denominator is (n-k)!
         */
        factorial(den, n-k);
        
        /* calculate numerator/denominator. we use divexact because the binomial
           coefficient is always guaranteed to be an integer.
         */
        mpz_divexact(res, num, den);
        
        mpz_clears(num, den, 0);
}

/** calculates the value of the binomial probability mass function, or the
    probability that k out of n bernoulli trials are a success.

    @param res: GMP float to store resulting probability in
    @param n:   number of bernoulli trials
    @param k:   number of successes to find the probability of
    @param p:   probability of a success in each bernoulli trial
 */
void binpdf(mpf_t res, unsigned n, unsigned k, double p){
        mpz_t bin;
        mpf_t tmp;
        
        /* we will use bin to store the binomial coefficient as an integer. we
           only need it briefly, because we will then store it as a float.
         */
        mpz_init(bin);
        
        /* tmp will be used to store p^k, then later (1-p)^(n-k), prior to
           applying those factors to the result
         */
        mpf_init(tmp);
        
        /* first calculate the binomial coefficient
         */
        binom(bin, n, k);
        mpf_set_z(res, bin);
        
        /* now calculate p^k
         */
        mpf_set_d(tmp, p);
        mpf_pow_ui(tmp, tmp, k);
        
        /* multiply the binomial coefficient by p^k and store it as our 
           intermediate result
         */
        mpf_mul(res, res, tmp);
        
        /* calculate (1-p)^(n-k)
         */
        mpf_set_d(tmp, 1.0 - p);
        mpf_pow_ui(tmp, tmp, n - k);
        
        /* now finally multiply our intermediate result by (1-p)^(n-k)
           to get the final value of the pmf
         */
        mpf_mul(res, res, tmp);

        mpz_clear(bin);
        mpf_clear(tmp);
}

/** calculates the value of the binomial cumulative distribution function, or
    the probability that the binomial random variable has a value less than
    or equal to x.
 
    @param res: GMP float to store resulting probability in
    @param n:   the number of bernoulli trials in our binomial experiment
    @param x:   the point at which we are interested in value of the cdf
    @param p:   the probability of success for each bernoulli trial
 */
void bincdf(mpf_t res, unsigned n, unsigned x, double p){
        mpf_t tmp;
        mpf_init(tmp);
        mpf_set_ui(res, 0);
        
        /* simply sum up the values of the pdf from 0 to x.
         */
        while(x < n){
                binpdf(tmp, n, x--, p);
                mpf_add(res, res, tmp);
        }
        
        mpf_clear(tmp);
}

/** calculates the value of 1 - cdf(n, k, p)
 */
int main(int argc, char** argv){
        mpf_t res;
        double p;
        unsigned n, k;
        mpf_init(res);
        
        if(argc != 4){
                printf("usage: %s <n> <k> <p>\n", argv[0]);
                return 1;
        }
        
        n = atoi(argv[1]);
        k = atoi(argv[2]);
        p = atof(argv[3]);
        
        /* first calculate P(X <= k-1)
         */
        bincdf(res, n, k-1, p);
        
        /* now we use the identity P(X >= k) = P(X > k-1) = 1 - P(X <= k-1)
         */
        mpf_neg(res, res);
        mpf_add_ui(res, res, 1);
        
        gmp_printf("%.10Ff\n", res);
        
        mpf_clear(res);
        return 0;
}
