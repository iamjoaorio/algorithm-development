#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	float distancia, preco;
	
	printf("Digite a quantidade de KM: \n");
	scanf("%f", &distancia);
	if (distancia >= 200){
		preco = (distancia * 0.45);
		printf("Preco da corrida: %f\n", preco);
	}
	else {
		preco = (distancia * 0.50);
		printf("Preco da corrida: %.2f\n", preco);
	}

	system("pause");	
	return 0;
}
