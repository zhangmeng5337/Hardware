#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../../Application/src/crypto/aes.h"

int8_t encrypt(char *filename, char *keyfile) {
	FILE *pKeyFile;
	FILE *pFile;
	FILE *pEncFile;
	size_t result;
	char buffer[64];
	char encbuffer[64];
	char *allfile;
	long lSize = 64;
	long tSize = 0, pSize = 0;
	long i;
	
	uint8_t key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	uint8_t iv[16]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
	
	pKeyFile = fopen(keyfile,"r");
	memset(key,0,16);
	fread(key,1,16,pKeyFile);
	//printf("%s\n", key);
	
	memset(iv,0,16);
	fread(iv,1,16,pKeyFile);
	//printf("%s\n", iv);
	// get the key that is shared between the firmware file and the encryption app
	fclose(pKeyFile);
	
	pFile = fopen(filename,"rb");
	if (pFile != NULL) {
		printf("Encrypting %s\n", filename);
		pEncFile = fopen("encrypted", "wb");

		// obtain file size:
		fseek (pFile , 0 , SEEK_END);
		tSize = ftell (pFile);
		rewind (pFile);	
		printf("Total file size %i\n", tSize);
		
		allfile = (char*) malloc (sizeof(char)*tSize);
		result = fread (allfile,1,tSize,pFile);
		if (result != tSize) {fputs ("Reading error",stderr); printf("%i %i\n",result,tSize);exit (3);}
		fclose(pFile);
	
		while (pSize < tSize) {
			memset(buffer,0xff,64);
			memcpy(buffer, &allfile[pSize], lSize);
			// encrypt here
			AES128_CBC_encrypt_buffer(encbuffer, buffer, 64, key, iv);		
			// write here
			fwrite (encbuffer , 1, 64, pEncFile);
			
			pSize += lSize; 
			if ((tSize - pSize) < 64) {
				lSize = tSize - pSize;
			}
			//printf("bytes processed %i\n", pSize);
		}
		
		free(allfile);
		fclose(pEncFile);
	} else {
		printf("%s does not exist", filename);
		return 1;
	}
	
	printf("Encryption done\n");
	return 0;
}

int main(int argc, char *argv[]) {
    
	if ( argc < 3) {
         printf("Usage:\n %s Input File\n",argv[0]);
    } 
    else {
		 encrypt(argv[1], argv[2]);
    }
	
	
	
  return 0;
}