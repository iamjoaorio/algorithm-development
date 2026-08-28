#include <stdio.h>
#include <stdlib.h>

int main(){
/* Escreva um programa que lê dois números e que pergunta qual a
operação você quer realizar. Você pode escolher soma(0),
subtração(1), multiplicação(2) ou divisão(3).Exiba o resultado da
operação escolhida ou uma mensagem dizendo que a operação
escolhida não é válida.*/

	float n1, n2, resultado;
 	int operacao;
	printf("Digite o Primeiro Numero: \n");
	scanf("%f", &n1);
    printf("Digite o Segundo Numero: \n");
	scanf("%f", &n2);

	printf("Qual operação você deseja fazer: \n");
	scanf("%d", &operacao);
	if (operacao == 1) {
	resultado = n1 + n2;
	}
	else if (operacao == 2){
	resultado = n1 - n2;
	}
	else if (operacao == 3){
	resultado = n1 * n2;
	}
	else if(operacao == 4) {
	resultado = n1 / n2;
	}
	
	else {
	printf("Operação invalida!");
		}
	printf("Seu resultado eh: %.2f\n", resultado);
	
	system("pause");	
	return 0;
}
