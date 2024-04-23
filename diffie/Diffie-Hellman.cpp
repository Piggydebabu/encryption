/* ************************************************************************
> File Name:     Diffie-Hellman.c
> Author:        Scaramouche

> Created Time:  Thursday, April 18, 2024 PM08:14:02
> Description:   
 ************************************************************************/

#include "Diffie-Hellman.h"
vector<mpz_class> primes = {
    998244353, 1004535809, 1998585857, 2013265921, 2281701377,
    3221225473, 75161927681, 77309411329, 206158430209
};
vector<unsigned int> roots = {3, 3, 3, 31, 3, 5, 3, 7, 22};

// check if n is primitive
bool is_prime(const mpz_class n) {
    return mpz_probab_prime_p(n.get_mpz_t(), 10) > 0;
}

// // find a prime greater than start
// void find_next_prime(mpz_class& prime, mpz_class& start) {
//     mpz_nextprime(prime.get_mpz_t(), start.get_mpz_t());
// }

// Init random state
void init_random_state(gmp_randstate_t state) {
 	gmp_randinit_mt(state);   
	std::ifstream urandom("/dev/urandom", std::ios::binary);
    if (urandom) {
        unsigned long int seed;
        urandom.read(reinterpret_cast<char *>(&seed), sizeof(seed));
        gmp_randseed_ui(state, seed);
    } else {
        gmp_randseed_ui(state, time(NULL));
    }
	urandom.close();
}

// // Generate a large prime
// mpz_class generate_large_prime(size_t bits) {
//     mpz_class candidate;
//     gmp_randstate_t state;
// 	init_random_state(state);
//     mpz_urandomb(candidate.get_mpz_t(), state, bits);
//     mpz_setbit(candidate.get_mpz_t(), bits - 1);

//     if (!is_prime(candidate)) {
//         find_next_prime(candidate, candidate);
//     }
//     gmp_randclear(state);
// 	cout << "large prime: " << candidate << endl;
// 	return candidate;
// }

// // mpz_class powm(mpz_class base, mpz_class exp, mpz_class mod) {
// 	mpz_class ret;
// 	mpz_powm(ret.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
// 	return ret;
// }

// // 检查g是否是p的原根
// bool is_primitive_root(const mpz_class g, const mpz_class p) {
//     mpz_class phi = p - 1;
// 	phi = phi / 2;
//     mpz_class gp;// = powm(g, phi / 2, p); // 检查g的平方根次幂
// 	mpz_powm(gp.get_mpz_t(), g.get_mpz_t(), phi.get_mpz_t(), p.get_mpz_t());
// 	for (mpz_class k = 1; k <= phi / 2; ++k) {
//         if (gp == 1) return false; // 如果gp在中间某步变为1，则g不是原根
		
//     }
//     return true; // 如果gp不为1，则g是原根
// }

// // 查找一个素数p的最小原根
// mpz_class get_primitive_root(mpz_class p) {
// 	if (p < 3 || !is_prime(p)) {
//         throw std::runtime_error("The number must be a prime greater than 2.");
//     }
//     mpz_class g = 2; // 从最小的正整数开始尝试
//     while (true) {
//         if (is_primitive_root(g, p)) {
// 			cout << "primitive root: " << g <<endl;
// 			return g;
//         }
//         g += 2; // 只检查奇数
//         if (g * g > p - 1) { // 如果g的平方大于p-1，则g不可能是原根
//             break;
//         }
//     }
// 	return -1;

// }
int generate_random(int min, int max) {
    // 使用当前时间作为随机数生成器的种子
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

    // 设置随机数的分布范围
    std::uniform_int_distribution<int> distribution(min, max);

    // 生成并返回一个随机数
    return distribution(rng);
}

mpz_class generate_large_prime(unsigned int i) {
	cout << "Big prime: " << primes[i] << endl;
    return primes[i];
}
unsigned int get_primitive_root(unsigned int i) {
	// auto ret = find(primes.begin(), primes.end(), p);
	cout << "It's primitive root is " << roots[i] << endl;
    return roots[i];
}
void init(mpz_class& p, unsigned int& g) {
	// while(true) {
    unsigned int i = generate_random(0, 8);
    p = generate_large_prime(i);
	g = get_primitive_root(i);
		// if(g>0) {
		// 	return;
		// }
		// else mpz_nextprime(p.get_mpz_t(), p.get_mpz_t());
	// }
}

// generate a big number as private key
mpz_class generate_private_key(size_t n) {
	gmp_randstate_t state;
	init_random_state(state);
	mpz_class random;
	mpz_urandomb(random.get_mpz_t(), state, n);
	gmp_randclear(state);
	cout << "prikey: " << random << endl;
	return random;
}

mpz_class generate_public_key(mpz_class prikey, 
					 	unsigned int g, mpz_class p) {
	mpz_class pubkey;
	mpz_class gg;
    mpz_set_ui(gg.get_mpz_t(), g);
	// pubkey = powm(g, prikey, p);
	mpz_powm(pubkey.get_mpz_t(), gg.get_mpz_t(), prikey.get_mpz_t(), p.get_mpz_t());
	cout << "pubkey: " << pubkey << endl;
	return pubkey;
}

//get shared key
mpz_class get_shared_key(const mpz_class& pubkeya, const mpz_class prikeyb, 
			const mpz_class& p) 
{
	mpz_class S;
	mpz_powm(S.get_mpz_t(), pubkeya.get_mpz_t(), prikeyb.get_mpz_t(), p.get_mpz_t());
	return S;
}


bool is_key_equal(const mpz_class& pubkeya, const mpz_class& pubkeyb, 
const mpz_class& prikeya, const mpz_class& prikeyb, const mpz_class& p ) 
{
	mpz_class S1 = get_shared_key(pubkeya, prikeyb, p);
	mpz_class S2 = get_shared_key(pubkeyb, prikeya, p);
	if(S1==S2) {
		return true;
	}
	else return false;
}
