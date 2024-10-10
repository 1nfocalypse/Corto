<p align="center">
  <a href="https://github.com/1nfocalypse/Corto">
	<img alt="Corto" src="https://i.imgur.com/W35JDVZ.png"/>
  </a>
</p>
<p align="center">
  <a href="https://choosealicense.com/licenses/mit/">
  	<img alt="License: MIT" src="https://img.shields.io/github/license/1nfocalypse/Corto"/>
  </a>
</p>
<h2 align="center">Corto</h3>
<h3 align="center">
  Software Defined Implementations of Kuznyechik, Magma (GOST), and Streebog
</h2>
<p align="center">
  By <a href="https://github.com/1nfocalypse">1nfocalypse</a>
</p>

## TL;DR
Corto is an implementation of the contemporary cryptographic standards of the Russian Federation, GOST R 34.12-2015 (Kuznyechik, Magma/GOST) and GOST R 34.11-2012 (Streebog). Additionally, it contains a custom BigInt class called "BinNum" that allows for arbitrary, finite length registers to be created supporting unsigned integers.
It allows the user to encrypt data with the block ciphers in either CBC or multithreaded EBC mode, along with allows for the production of hash digests via Streebog-512 or Streebog-256. While all of the algorithms here are considered cryptographically inferior to other available algorithms, they are all still officially supported 
by the Russian Federation, and are thus of interest. For usage information, please see [usage](#usage). Additionally, please note that while this implementation meets the standards and tests set forth in the specifications, it has not been peer reviewed, and thus may contain errors.

## Ciphers
Both ciphers are able to operate in multithreaded ECB mode (where each block is treated as independent) or iterative CBC mode (where the result of the previous block is XOR'd with the incoming data for the current block). 
### Kuznyechik
Kuznyechik is the newest cipher to be adopted by the Russian Federation, first being established in 2015. It is based on a Substitution Permutation network, similar to Rijndael, but utilizes a Feistel Network for key generation, thus obtaining better diffusion at the cost
of markedly slower key generation. It utilizes a block size of 128 bits, twice that of it's predecessor, Magma (otherwise previously known as GOST), with the same key size of 256 bits and 10 rounds. The standard maintains a number of transformations, which are as follows:
- $X[k] : V_{128} \rightarrow V_{128} : X[k]\(a) = k \bigoplus a$, where $V_{128}$ is a vector of bits with length 128.
- $S : V_{128} \rightarrow V_{128}$, where the S box $\pi$ is applied to each byte of the vector.
- $S^{-1} :  V_{128} \rightarrow V_{128}$, the inverse by application of $\pi^{-1}$ to each byte of the vector.
- $R : V_{128} \rightarrow V_{128}$, the application of the linear function in addition to a 120 bit left shift to the result of the linear function XOR'd with an 8 bit right shift to the given value.
- $R^{-1} : V_{128} \rightarrow V_{128}$, the inverse of R.
- $L : V_{128} \rightarrow V_{128}$, the application of R 16 times to a given value.
- $L^{-1} : V_{128} \rightarrow V_{128}$, the application of the inverse of R 16 times to a given value.l
- $F[k] : V_{128} \rightarrow V_{128} : F[k](a_{0}, a_{1}) = LSX[k]\(a_{1}) \bigoplus a_{0}, a_{1}$, which is part of the key scheduler.

Additionally, similar to Rijndael, we are provided a constant binary polynomial $x^8 + x^7 + x^6 + x + 1$, representable as 111000011, which is used as the characteristic for the finite field employed in the linear function.

With these transforms in mind, the operation of the cipher is as follows:

To encrypt:

Perform key generation
- $G : V_{256} \rightarrow \[K_1, K_2, ..., K_{10}]$, which is accomplished by first masking off the 128 MSB and LSB digits (key 1 and 2) and adding them to the key list, then looping 32 times. For each loop iteration, we take the iteration number, and apply L to it. Then, copy the previous final key 1 result, and apply L(S(key 1 ^ <iter_transform_result>)) ^ key2. Key 2 then is assigned to the previous value of key 1. Every 8 rounds, key 1 and key 2's values are pushed into the key list.
- Upon obtaining keys, perform 9 loops such that for each iteration, $value := L(S(value \bigoplus key_{i}))$, then return $value \bigoplus keys_{9}$
- L is simply as described in the transform table.
- R calls the linear function to obtain an 8 bit value, which is then prepended to the original 128 bit value after the original value is right shifted by 8.
- The linear function masks off a byte at a time, from LSB to MSB, applying Kuznyechik multiplication of each byte with a given constant, of which there are 16. This value is then XOR'd with its previous value.
- Kuznyechik multiplication treats the two passed arguments (each one byte) as binary polynomials. It first multiplies them together, then obtains the result over a finite field of the previously mentioned binary polynomial constant. It then returns a single byte resulting from this.
- The resultant value of the second step is the ciphertext.

To Decrypt:
Perform key generation in the same manner, then apply L inverse, which applies the respective inverse functions to the calls L makes.

### So is it any good?
Kuznyechik has a known vulnerability in the form of a Meet in the Middle attack on round 5. However, it is considered computationally infeasible to crack the cipher even with this method due to the large computational cost. Regardless, the S box maintains an internal structure that shows it is not randomly generated, as the authors claim.
An excellent showcase of this is [GOSTBox](https://github.com/mjosaarinen/gostbox) by Markku-Juhani O. Saarinen. This creates a degree of suspicion that communications may be monitored by the Russian Federation, and as such, caution when utilizing this cipher is encouraged.

### Magma/GOST
Magma, previously known as GOST, is a 32 round [Feistel Network](https://github.com/1nfocalypse/CryptoWriteup?tab=readme-ov-file#feistel-networks) with a block size of 64 bits and a key size of 256 bits. Corto once again uses the 256 bit digests of Streebog to key the cipher. Magma is substantially simpler than Kuznyechik,
employing a round function that consists of adding 32 bits of the block to a 32 bit subkey, modding off any overflows, then performing a substitution with its s box. After this, it simply performs a binary rot 11 by right shifting (>>>11). Its key schedule is also very simple, deriving 8 32 bit subkeys from the given key, using them in order
three times, then reversing them for the last 8 to obtain 32 subkeys.

General cipher operation is described below.

To encrypt:
- Generate keys, such that the key is broken into 8 consecutive 32 bit numbers. These numbers in this order form the first 24 subkeys. Reverse them to obtain the last 8 for a total of 32.
- Obtain a 64 bit block of cleartext, and split it into two 32 bit numbers.
- Perform 32 rounds of a balanced Feistel Network, with the initial left being the 32 MSB of the cleartext and the right being the 32 LSB. The right value is then assigned to the new left value, the new right value is set to the left value XOR the result of the round function of the right and that iteration's subkey.
- The round function adds the subkey and the cleartext together, modding off any overflows. It is then sent to the s box for substitution. Then, perform binary rot11 via right shift. The left and right are then joined to form the ciphertext.

To decrypt:
- Generate keys in the same manner.
- Seperate the ciphertext in the same manner to obtain two 32 bit numbers.
- Perform 32 rounds of a balanced Feistel Network in the same manner.
- Join the resultant values together to form a 64 bit number to obtain the resulting cleartext.

### So is it any good?
Magma enjoyed a high degree of success for several decades due to the ease of implementation it offered and the high degree of security it was purported to have. However, the cipher was completely broken in 2011, rendering it insecure and not recommended for use. Additionally, due to the low block size, it needs to be rekeyed approximately
every 30 gigabytes of data, as otherwise it is susceptible to the birthday paradox.

## Hash (Streebog)
Streebog is the cryptographic hash of the Russian Federation. Established in 2012, it uses a Merkle-Damgard construction as the basis for its operation, with a Miyaguchi-Preneel compression function. It utilizes two substitution boxes along with mixes in defined constants in order to satisfy the preimage resistance requirement. Streebog has
two modes of operation, one providing a 512 bit digest and the other a 256. They are extremely similar, with the only real difference being that the 256 bit mode utilizes a different initial value and truncates the digest to the 256 MSB. While there have been both successful showcasings of preimage and collision attacks, they are once again
considered infeasible due to the complexity required to induce them.

The operation of Streebog is as follows.

- Initialize variable H with the initial value, dependent on mode
- Variables N, $\sigma$ are initialized to zero.
- If the passed data is larger than 512 bits, the last 512 bits are obtained as the subvector.
- The compression/round function is then called with arguments N, H, and the subvector.
- 512 and variable N are added together, and N is set as the resulting 512 LSB
- $\sigma$ is then set as the 512 LSB of its own addition with the subvector.
- This process is repeated until the passed data is less than or equal to 512 bits.
- If the data is less than 512 bits, its size is recorded and it is padded by prepending a variable number of zeroes followed by a one to the front of the data.
- the variable H is then set equal to the value of the round function with arguments N, H, and the 512 bits of data.
- N is then set to the result of sum of itself and the size of the given data after it is reduced.
- $\sigma$ is then set to the value of the sum of itself and the given data after it is reduced.
- H is then assigned the result of the compression/round function with arguments 0, H, N
- H is then assigned the result of the compression function with arguments 0, H, Sigma
- If the operating mode is 256, the digest is the 256 MSB
- Otherwise, return the entire digest.

The compression function operates by accepting three arguments N, H, M, then returning the value of calling another function E(shift(pBox(sBox(H ^ N))), m) ^ H ^ M. The s box is the same as used in Kuznyechik and is employed in the same manner for 512 bit numbers. The p box works in a similar way with a different set of values, Tau.
The shift is accomplished by breaking the 512 bit value down into 64 bytes, then performing a triple nested loop of $8^3$ iterations with iterators i, j, k. A variable tmp is then declared to hold the value of the shift. If the value of the byte stored at index i * 8 + j is masked with 1 left shifted by 7 - k is 1, tmp is XOR'd with the j * 8 
\+ k'th value of A, a given set of constants. Every 64 iterations, another loop is run 8 times with iterator j, which replaces 8 bytes from the original value with the result of tmp AND 0xFF left shifted by (7 - j) * 8, which is then right shifted by 7 - j * 8. The resultant 512 bit value is then returned. E, upon obtaining the resultant
values, uses the result of shift(pBox(sBox(H ^ N))) to generate keys via a key generation function. The key generation function takes the passed value, appends it to a vector, and iterates 13 times, obtaining an iterative constant provided by the spec on each iteration. With each value, it then obtains the last value in the vector, and XORs it
with the current constant. It then applies the s box, p box, and shift, in that order, pushing back the result from the shift as a new key. This yields a total of 3 keys. Once the keys are obtained, E establishes a value tmp, which is the result of shift(pBox(sBox(keys\[0] ^ \<right argument>))). It then reassigns tmp in the same manner by 
iterating through the provided keys, returning tmp ^ keys\[12], which is then XOR'd with H and M in order to yield the compressed values.

### So is it any good?
As previously mentioned, Streebog has had some attacks shown to be possible, but they are computationally infeasible to produce. As such, it is acceptable to use in current application. For a bit of comedy, Streebog was a response to the US's SHA3 competition, from which Keccak was selected. Notably, while MD5, the SHA1 hashes, and the SHA2 
hashes all utilize a Merkel-Damgard construction, Keccak does not. While SHA2 also remains safe at the time of writing, the longevity of such a hash should be considered when being adopted, and as such, it may be better to employ Keccak or SHA3.

## Conclusion
Corto, while a functional implementation of the GOST cryptographic standards, is primarily more meant for fun and education. It is purely software defined and does not enjoy the hardware acceleration offered by more nuanced implementations. Additionally, while it complies with the tests set forth in the standards, it is not peer-reviewed, and
as such, may contain errors. Please see the contributing file if you spot any, and I'll be happy to merge any PRs that fix them. Lastly, I would like to reference the videos by JacksonInfoSec on both [Kuznyechik](https://www.youtube.com/watch?v=DhWNFEa7alM) and [Magma](https://www.youtube.com/watch?v=Aq5y9pnbCSM) 
(unfortunately he does not have one for Streebog), as they are excellent resources. Additionally, a C implementation of Magma is available in [Applied Cryptography by Bruce Schneier](https://www.amazon.com/Applied-Cryptography-Protocols-Algorithms-Source/dp/0471117099) if you would like another reference. Thank you for reading!


## Usage
General
- -h/-help/--help: displays help menu
- -i/-info/--info: displays info menu
- --verify: verifies Kuznyechik, Streebog, and Magma are to specifications


Ciphers
- Proper call method -> ./corto -\[kuz/mag] -\[cbc/ecb=N] -\[e/d] -k="<KEY_HERE>" -tgt="<TRGT_PATH>" -out="<OUTPUT_PATH>"
- -\[kuz/mag]: Denotes Kuznyechik (.kuz) or Magma (.mag) operations.
- -\[cbc/ecb=N]: Denotes block cipher mode of operation. Uses N threads if ECB is selected.
- -\[e/d]: Denotes encrypt or decrypt operation.
- -k="<KEY_HERE>": Key for operation provided in quotes.
- -tgt="<ENC_TRGT_PTH>": Path to file targeted for encryption in quotes.
- -out="<OUTPUT_PTH>": Path to output target directory in quotes.
- EX: ./corto -kuz -cbc -e -k="my super secret key" -tgt="./secretinfo.txt" -out="./"
- EX: ./corto -mag -ecb=6 -d -k="terriblekey" -tgt="./secretpic.png.mag-ecb" -out="./"


Hash
- Proper call method -> ./corto -str -\[256/512] -\[stl="\<STRING_LITERAL>"/tgt="<HASH_TGT_PATH>] -out=\["<OUTPUT_PTH>"/0]
- -str: Denotes Streebog operation.
- -[256/512\]: Denotes operating mode/digest size.
- -stl="<STRING_LITERAL>": Denotes passing a string literal, with the string in quotes.
- -tgt="<HASH_TGT_PTH>": Denotes passing a file to hash, with the path in quotes.
- -out="<PATH_TO_OUTPUT>"/0: Outputs to a text file in the given directory, or if 0 is provided, in the terminal.
- EX: ./corto -str -512 -stl="This is my target!" -out=0
- EX: ./corto -str -256 -tgt="./myfile.png" -out="./"

