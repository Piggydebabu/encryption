/* ************************************************************************
> File Name:     dhmain.cpp
> Author:        Scaramouche
> Created Time:  Thursday, April 18, 2024 PM08:20:09
> Description:   
 ************************************************************************/

#include "Diffie-Hellman.h"
#include "des.h"

int main() {
    //变量声明与初始化
    mpz_class prikeya;
    mpz_class prikeyb;
    mpz_class pubkeya;
    mpz_class pubkeyb;
    mpz_class p;
    mpz_class S;
    unsigned int g;
    //生成大素数及其原根
    // p = generate_large_prime(MAXLEN);
    // g = get_primitive_root(p); 
    while(true) {
        init(p, g);
        cout << endl;
        //生成公钥与私钥
        cout << "Generating A's Private Key..." << endl;
        prikeya = generate_private_key(64);

        cout << "Generating B's Private Key..." << endl;
        prikeyb = generate_private_key(64);
        cout << "Generating A's Public Key..." << endl;
        pubkeya = generate_public_key(prikeya, g, p);
        cout << "Generating B's Public Key..." << endl;
        pubkeyb = generate_public_key(prikeyb, g, p);
        // mpz_powm_sec(pubkeya.get_mpz_t(), g.get_mpz_t(), prikeyb.get_mpz_t(), p.get_mpz_t());
        //判断密钥交换是否成功
        cout << endl;
        cout << "Calculating Shared Keys..." << endl;
        if(is_key_equal(pubkeya, pubkeyb, prikeya, prikeyb, p)) {

            cout << "Successfully exchanged keys!" << endl;
            S = get_shared_key(pubkeya, prikeyb, p);
            cout << "shared key: " << S << endl;
            string key = S.get_str();
            string str1 = "";
            cout << endl;
            cout << "Using DES to encrypt..." << endl;
            cout << "Please input plaintext:";
            getline(cin, str1);
            cout << "Encrypting..." <<endl;
            string rs = des_encrypt(str1, key);
            cout << "Cipher text:" << rs << endl;
            cout << "Decrypting..." << endl;
            rs = des_decrypt(rs);
            cout << "Plaintext:" << rs << endl;
            return 0;
        }
        else {
            cout << "Error!" << endl;
            cout << "Retrying... " << endl;
        }   
        
    }
    return 0;
}

