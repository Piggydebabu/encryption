/* ************************************************************************
> File Name:     Diffie-Hellman.h
> Author:        Scaramouche
> Created Time:  Thursday, April 18, 2024 PM06:48:02
> Description:   
 ************************************************************************/
#ifndef _DIFFIE_HELLMAN_H_
#define _DIFFIE_HELLMAN_H_

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <gmp.h>
#include <gmpxx.h>
using namespace std;
// void mpz_powm a^e mod b
// int mpz_probab_prime_p  if a num is prime return 2 yes return 1 probab return 0 no
// void mpz_nextprime   a prime greater than present num
// void mpz_gcd  Euclid
// void mpz_urandomb  random num from 0 to 2^n-1
// void gmp_randinit_default initialize random number functions
// int mpz_cmp return > 0 op1>op2, return 0 op1==op2, return <0 op1<op2

const int MAXLEN = 32;
// big prime table
// map<mpz_class, unsigned int> p_gs;
// / for(int i=0; i<primes.size(); i++) {
//     p_gs.insert(make_pair(primes[i], roots[i]));
// }
unsigned int get_primitive_root(const mpz_class p);
mpz_class generate_large_prime(unsigned int i);
bool is_prime(const mpz_class num);
void init(mpz_class& p, unsigned int& g);
mpz_class generate_private_key(size_t n);
mpz_class generate_public_key(mpz_class prikey, unsigned int g, 
                            mpz_class p);
mpz_class get_shared_key(const mpz_class& pubkeya, const mpz_class prikeyb, 
			const mpz_class& p); 
bool is_key_equal(const mpz_class& pubkeya, const mpz_class& pubkeyb, const mpz_class& prikeya, 
                        const mpz_class& prikeyb, const mpz_class& p);
#endif
