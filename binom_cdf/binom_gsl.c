#include <gsl/gsl_cdf.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
        int n = 0, k = 0;
        double p = 0, Q;
        
        if(argc != 4){
                printf("usage: %s <n> <k> <p>\n", argv[0]);
                return 1;
        }
        
        n = atoi(argv[1]);
        k = atoi(argv[2]);
        p = atof(argv[3]);
        
        Q = gsl_cdf_binomial_Q(k - 1, p, n);
        printf("%.10f\n", Q);
        
        return 0;
}
