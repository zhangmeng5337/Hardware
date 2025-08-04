#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
	FILE *fPtrKeys;
	char header[] = "#include <stdint.h>\n";
	char key[128] = "uint8_t key[16] = \"";
	char iv[128] = "uint8_t iv[16] = \"";
	
	if ( argc < 3) {
         printf("Usage:\n %s Input File\n",argv[0]);
    } 
    else {
		 fPtrKeys = fopen("key.c", "w");
		 strcat(key,argv[1]);
		 strcat(key,"\";\n");
		 strcat(iv,argv[2]);
		 strcat(iv,"\";\n");
		 
		 // generate C file for bootloader
		 fputs(header,fPtrKeys);
		 fputs(key,fPtrKeys);
		 fputs(iv,fPtrKeys);
		 fclose(fPtrKeys);
		 
		 // generate ket for release builds
		 fPtrKeys = fopen("key.enc", "w");
		 fputs(argv[1],fPtrKeys);
		 fputs(argv[2],fPtrKeys);
		 fclose(fPtrKeys);
		 
    }
	
	
	
  return 0;
}