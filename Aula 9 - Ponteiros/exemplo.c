#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(){
	setlocale(LC_ALL,"Portuguese");

	char letra = 'A';
	char letra2 = 'B';
	int numero = 37;
	int numero2 = 25;
	float numero3 = 6.7;
	

	printf("A variável %c tem o endereço %p\n",  letra, &letra);
	printf("A variável %c tem o endereço %p\n",  letra2, &letra2);
	printf("A variável %d tem o endereço %p\n", numero, &numero);
	printf("A variável %d tem o endereço %p\n", numero2, &numero2);
	printf("A variável %f tem o endereço %p\n", numero3, &numero3);
	

	system("pause");	
	return 0;
}
