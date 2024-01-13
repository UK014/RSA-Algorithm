#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int findPublicKey(int phi) {
    int e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1)
            break;
        e++;
    }
    return e;
}

int modInverse(int e, int phi) {
    int d;
    for (d = 1; d < phi; d++) {
        if ((e * d) % phi == 1)
            break;
    }
    return d;
}

int modExp(int base, int exponent, int modulus) {
    int result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        exponent = exponent / 2;
    }
    return result;
}


void generateKeys(int *publicKey, int *privateKey, int *n) {
   
    int p = 11;
    int q = 13;

    *n = p * q;

    int phi = (p - 1) * (q - 1);

    *publicKey = findPublicKey(phi);

    *privateKey = modInverse(*publicKey, phi);
}

int* encrypt(const char *message, int publicKey, int n) {
    int length = strlen(message);
    int *encryptedMessage = (int *)malloc(length * sizeof(int));

    for (int i = 0; i < length; i++) {
        encryptedMessage[i] = modExp(message[i], publicKey, n);
    }

    return encryptedMessage;
}

char* decrypt(const int *encryptedMessage, int privateKey, int n, int length) {
    char *decryptedMessage = (char *)malloc(length + 1);

    for (int i = 0; i < length; i++) {
        decryptedMessage[i] = modExp(encryptedMessage[i], privateKey, n);
    }
    decryptedMessage[length] = '\0';

    return decryptedMessage;
}

int main() {
    int publicKey, privateKey, n;

    
    generateKeys(&publicKey, &privateKey, &n);

    printf("Public Key (e): %d\n", publicKey);
    printf("Private Key (d): %d\n", privateKey);
    printf("n: %d\n", n);

    const char *message = "hello";

    int *encryptedMessage = encrypt(message, publicKey, n);

    printf("Original Message: %s\n", message);
    printf("Encrypted Message: ");
    for (int i = 0; i < strlen(message); i++) {
        printf("%d ", encryptedMessage[i]);
    }
    printf("\n");

    char *decryptedMessage = decrypt(encryptedMessage, privateKey, n, strlen(message));

    printf("Decrypted Message: %s\n", decryptedMessage);

    free(encryptedMessage);
    free(decryptedMessage);

    return 0;
}

