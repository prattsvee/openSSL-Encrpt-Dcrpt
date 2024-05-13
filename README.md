Use the openssl library to write functions for encryption and decryption.


fscrypt.cc is implemented with block cipher method in CBC (Cipher Block Chain) mode including other specifications mentioned in the project.


The code is tested on Ubuntu 14.04 and remote.cs , works goood.



To run the programs, you should set up OpenSSL environment.
Here is examples:

To compile your C/C++ code using OpenSSL on a Linux system, including on a system like the remote.cs server at Binghamton University where OpenSSL is pre-installed, you can follow similar steps. Here's how you would rewrite the instructions for Linux:

Step 1: Download the OpenSSL source tarball from https://www.openssl.org/source/ and extract it.

Step 2: Build OpenSSL using the following steps:

```
bash
./config --prefix=/path/to/installation/directory
make
make install
```

Replace `/path/to/installation/directory` with the directory where you want OpenSSL to be installed. You may need to use `sudo` if installing to system directories like `/usr/local`.

Step 3: Compile your C/C++ code with OpenSSL by specifying the include path and library path:


```bash
gcc -I/path/to/installation/directory/include -L/path/to/installation/directory/lib main.cc fscrypt.cc -lcrypto -o ./exec
```

Replace `/path/to/installation/directory` with the actual path where OpenSSL is installed. For example, if you installed OpenSSL to `/usr/local/ssl`, the command would be:


```bash
gcc -I/usr/local/ssl/include -L/usr/local/ssl/lib main.cc fscrypt.cc -lcrypto -o ./exec
```


This command will compile your code and link it against the OpenSSL libraries (`-lcrypto`). The `-I` flag specifies the path to the OpenSSL header files, and the `-L` flag specifies the path to the OpenSSL libraries.

If you're using a system like the remote.cs server at Binghamton University, where OpenSSL is pre-installed and available system-wide, you may not need to specify custom paths. In that case, you can simply compile your code like this:

```bash
gcc main.cc fscrypt.cc -lcrypto -o exec
./exec
```

This command will use the default system paths for OpenSSL headers and libraries.



================================================	


        Use the openssl library (www.openssl.org) to write the following 
        two functions for encryption and decryption in a file fscrypt.cc. 
	You should use block cipher method 
	blowfish for encryption. Blowfish uses 64-bit blocks and typically 
	128-bit keys.
	
	// put the following lines in fscrypt.h
	
	#include "openssl/blowfish.h"

	// encrypt plaintext of length bufsize. Use keystr as the key.
	const int BLOCKSIZE = 8;           // Block size for blowfish
	void *fs_encrypt(void *plaintext, int bufsize, char *keystr, 
			int *resultlen);

	// decrypt ciphertext of length bufsize. Use keystr as the key.
	void *fs_decrypt(void *ciphertext, int bufsize, char *keystr, 
			int *resultlen);
	
	Both functions allocate the result buffer of at least the required
	size (using new()) and return a pointer to it. Both functions 
	also return the number of valid bytes in the result buffer in resultlen.
	The application code is responsible for deleting the buffer.

	Use CBC mode of encryption. For padding, pad with length of the pad
	in all the padded characters.

	Assume that the initialization vector contains NULL characters 
	(all 0's).

	Description of blowfish functions can be found at:
	https://www.openssl.org/docs/man1.0.2/man3/blowfish.html

	Use the following functions to faciliate your work:

	BF_set_key: use all characters of the keystr, excluding NULL 
		    terminator.  Valid keystr is assumed to be a string.

	BF_cbc_encrypt and BF_ecb_encrypt	    

	You should use BF_ecb_encrypt to implement the CBC mode	on your own.

	You will need to include "openssl/blowfish.h" from the
	openssl package) and link with the "crypto" library.

	Below is a small test code (main.cc).

	You can compile it with your code in fscrypt.cc using
	gcc (or g++) main.cc fscrypt.cc -lcrypto

        Submit your fscrypt.cc, which uses only BF_ecb_encrypt.

================================================

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fscrypt.h"

int main()
{
  char s[] = "hello world";
  char *outbuf, *recvbuf;
  char pass[] = "top secret";
  int len = 0;
  int recvlen = 0;

  outbuf = (char *) fs_encrypt((void *) s, strlen(s)+1, pass, &len);
  printf("%s %d\n", "length after encryption = ", len);

  int i = 0;
  printf("ciphertext = ");
  for (i = 0; i < len; i++)
      printf("%02x", outbuf[i]);
  printf("\n");

  recvbuf  = (char *) fs_decrypt((void *) outbuf, len, pass, &recvlen);
  assert(memcmp(s, recvbuf, recvlen) == 0);
  assert(recvlen == (strlen(s) + 1));
  printf("plaintext = %s\n", recvbuf);
}

================================================
