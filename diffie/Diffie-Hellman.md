# Diffie-Hellman(cpp)

基于大整数库GMP实现Diffie-Hellman密钥交换算法，协商出的密钥作为DES算法的加密密钥，对字符串进行加解密。

---

- 文件结构
1. des.h：实现DES算法，读入字符串并对其进行加密解密
   
   ```C++
   // 主要函数实现
   // 加密函数
   string encrypt(string str1, string str2); 
   // 解密函数
   string decrypt(string str);
   // 明文分组和填充（64位一组）， 返回01字符串
   string chardeel(string str1, string str2);
   // 生成16个子密钥
   string des_generateKeys(string str);
   // 明文初始置换
   string plaintext_initial_permutation(string str1);
   // 末置换
   string final_premutation(string str);
   // f轮函数
   string des_f(string str1, string str2);
   // S盒
   string S_box(string str1);
   // P盒
   string P_box(string str1);
   ```

2. Diffie-Hellman.h：实现DH算法所必要的函数声明
   
   ```C++
   // 主要函数实现
   // 定义生成随机数的位数
   const int MAXLEN = size;
   // 计算大素数的本原根
   mpz_class get_primitive_root(const mpz_class p);
   // 素性检测，调用mpz_probab_prime_p，实现Miller-Robin素性检测
   bool is_prime();
   // 生成一个n位大素数
   mpz_class generate_large_prime(unsigned int n);
   // 初始化大素数及其生成元
   void init(mpz_class& p, mpz_class & g);
   // 生成n位的随机数作为私钥
   mpz_class generate_private_key(const unsigned int& n);
   // 利用生成的私钥计算自身公钥
   mpz_class generate_public_key(const mpz_class& prikey, const mpz_class& g, 
                                   const mpz_class& g);
   // 计算共享密钥
   mpz_class get_shared_key(const mpz_class& pubkeya, const mpz_class& prikeyb, 
                               const mpz_class& p);
   // 调用get_shared_key, 分别计算双方密钥，并判断是否相等
   bool is_key_equal(const mpz_class& pubkeya, const mpz_class& pubkeyb, const mpz_class prikeya, 
                       const mpz_class& prikeyb, const mpz_class& p);
   ```

3. Diffie-Hellman.cpp：上述函数实现以及一些辅助函数的实现
   
   ```C++
   // 辅助函数
   // 因无高效率的计算大素数原根的方法，这里采用原根表
   // 简单的计算原根的方法在代码中给出
   vector<mpz_class> primes = {
       998244353, 1004535809, 1998585857, 2013265921, 2281701377,
       3221225473, 75161927681, 77309411329, 206158430209
   };
   vector<unsigned int> roots = {3, 3, 3, 31, 3, 5, 3, 7, 22};
   // 利用Linux系统文件/dev/random生成真随机数种子
   void init_random_state(gmp_randstate_t grt);
   // 封装mpz_powm，使其可用于C++
   mpz_class powm(const mpz_class& base, const mpz_class& exp, const mpz_class& mod);
   // 判断g是否为p的原根
   bool is_primitive_root(const mpz_class& g, const mpz_class& p);
   // 利用上述随机数种子生成随机数
   mpz_class generate_random(gmp_randstate_t grt); 
   ```

4. dhmain.cpp：主函数，模拟两方交换密钥
   
   ```C++
   /* ************************************************************************
   > File Name:     dhmain.cpp
   > Author:        Scaramouche
   > Created Time:  Thursday, April 18, 2024 PM08:20:09
   > Description:   
    ************************************************************************/
   
   #include "Diffie-Hellman.h"
   #include "des.h"
   
   int main() {
       // 变量声明与初始化
       mpz_class prikeya;
       mpz_class prikeyb;
       mpz_class pubkeya;
       mpz_class pubkeyb;
       mpz_class p;
       mpz_class S;
       unsigned int g;
       // 生成大素数及其原根
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
   ```

---

- 编译选项：
  
  ```
  # 链接gmp库并添加调试选项
  gcc -g dhmain.cpp Diffie-Hellman.cpp -o dhmain -lgmp -lgmpxx -lstdc++
  ```

---

- 运行结果
  
  ```
  cjw@cjw-Magic:~/codespace/encryption$ ./dhmain 
  Big prime: 77309411329
  It's primitive root is 7
  
  Generating A's Private Key...
  prikey: 8858952238899014485
  Generating B's Private Key...
  prikey: 6493059684422211184
  Generating A's Public Key...
  pubkey: 53751488064
  Generating B's Public Key...
  pubkey: 43848608461
  
  Calculating Shared Keys...
  Successfully exchanged keys!
  shared key: 22388782779
  
  Using DES to encrypt...
  Please input plaintext:123123123
  Encrypting...
  Cipher text:00001100101001100101101010011001011110110011000110110000110100100111110011101010110101101010000000011000011001111011010101010001
  Decrypting...
  Plaintext:123123123
  ```

---

- 收获
  
  学习了大整数库GMP的使用，了解了Diffie-Hellman密钥交换算法的过程。
