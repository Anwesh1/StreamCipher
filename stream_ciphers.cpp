#include <iostream>
#include <cmath>

char* encode(char *plaintext, unsigned long key);
char* decode(char *ciphertext, unsigned long key);

//Encoding
char* encode(char *plaintext, unsigned long key) {

	unsigned int i = 0;
	unsigned int j = 0;
	unsigned char S[256];

//loop for array
	for (int a = 0; a < 256; a++) {
		S[a] = a;
	}

//Scramble
	for (int b = 0; b < 256; b++) {
		unsigned char temp;
		int k = i % 64;
		int kth = (key >> k) & 1;
		j = (j + S[i] + kth) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		i = (i + 1) % 256;
	}

//Length of plaintext and making length a multiple of 4
	int length = 0;
	while (true) {
		if (plaintext[length] == 0) {
			break;
		}
		length++;
	}
	int length2 = length;
	while (length2 % 4 != 0){
		length2++;
	}

	unsigned char exclusive[length2];
	for(int a= 0; a < length2; a ++)
	{
		exclusive[a] = 0;
	}

//Exclusive-OR
	for (int b = 0; b < length2; b++) {
		char temp;
		int r;
		unsigned char R;
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		r = (S[i] + S[j]) % 256;
		R = S[r];
		if (b >= length){
		exclusive[b] = (0 ^ R);
		}
		else {
		exclusive[b] = (plaintext[b] ^ R);
		}

	}


//ASCII Armour
	int grp = (length2 / 4);
	char *array = new char[(int) (5 * ((length2 / 4)) + 1)];
	for (int count = 0; count < grp; count++) {
		unsigned char w = exclusive[4 * count];
		unsigned char x = exclusive[4 * count + 1];
		unsigned char y = exclusive[4 * count + 2];
		unsigned char z = exclusive[4 * count + 3];
		unsigned int binary = ((w << 24) | (x << 16) | (y << 8) | (z));
		unsigned char a = ((binary % 85) + '!');
		unsigned char b = (((binary / 85) % 85) + '!');
		unsigned char c = ((((binary / 85) / 85) % 85) + '!');
		unsigned char d = (((((binary / 85) / 85) / 85) % 85) + '!');
		unsigned char e = ((((((binary / 85) / 85) / 85) / 85) % 85) + '!');
		array[5 * count + 4] = a;
		array[(5 * count) + 3] = b;
		array[(5 * count) + 2] = c;
		array[(5 * count) + 1] = d;
		array[(5 * count)] = e;
	}
	return array;
}

//Decoding
char* decode(char *ciphertext, unsigned long key){

//Calculating length of ciphertext
	int length = 0;
		while (true) {
			if (ciphertext[length] == 0) {
				break;
			}
			length++;
		}

//Reversing ASCII Armour
	int grp = (length/5);
	char *array = new char[(int) (4 * ((length / 5)) + 1)];
	for(int i = 0; i < grp; i++){
		unsigned char v = ciphertext[5 * i];
		unsigned char w = ciphertext[5 * i + 1];
		unsigned char x = ciphertext[5 * i + 2];
		unsigned char y = ciphertext[5 * i + 3];
		unsigned char z = ciphertext[5 * i + 4];
		unsigned int a = ( z - '!');
		unsigned int b = (((y - '!') * 85) + a);
		unsigned int c = ((((x - '!') * 85) * 85) + b);
		unsigned int d = (((((w - '!') * 85) * 85) * 85) + c);
		unsigned int e = (((((((v - '!') * 85) * 85) * 85) * 85) + d));
		unsigned char c4 = e >> 24;
		unsigned char c3 = (e >> 16) & 255;
		unsigned char c2 = (e >> 8) & 255;
		unsigned char c1 = e & 255;
		array[4 * i + 3] = c1;
		array[4 * i + 2] = c2;
		array[4 * i + 1] = c3;
		array[4 * i] = c4;
	}

//loop for array
	unsigned char S[256];
	for (int a = 0; a < 256; a++) {
		S[a] = a;
	}
//Scramble
	unsigned int i = 0;
	unsigned int j = 0;
	for (int b = 0; b < 256; b++) {
		unsigned char temp;
		int k = i % 64;
		int kth = (key >> k) & 1;
		j = (j + S[i] + kth) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		i = (i + 1) % 256;
	}
//Exclusive-OR
	for (int b = 0; b < (4 * (length / 5)); b++) {
		char temp;
		int r;
		unsigned char R;
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		r = (S[i] + S[j]) % 256;
		R = S[r];
		array[b] = (array[b] ^ R);

		}
	return array;
	}



