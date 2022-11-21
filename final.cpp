#include<string.h>
#include<stdlib.h>
#include <Windows.h>
#include<locale.h>
#include<iostream>
#include <stdio.h>

//modelo do carro
struct modelo {
	char modelo[10];
	char cor[10];
	char status;
	int id;
};

int tam(FILE*);
void cadastrarCarro(FILE *);
void consultarCarro(FILE *);
void gerarArquivo(FILE *);
void excluir(FILE *);


int main (void) {
	
	setlocale(LC_ALL, "");
	int op;
	FILE *arq;
	
	if ((arq = fopen("locadora-carro", "rb+")) == NULL)
	{
		if ((arq = fopen("locadora-carro", "wb+")) == NULL)
		{
			printf("Não foi possível abrir arquivo!\n");
			system("pause");
		}
	}
	
	do{
		system("cls");
		printf("----------Locadora de Automóveis----------\n");
		printf("1-Cadastrar carro :\n");
		printf("2- Consultar carro: \n");
		printf("3- Gerar arquivo:\n");
		printf("4- Excluir: \n");
		printf("5-Sair: \n");
		printf("Informe sua opção:\n");
		scanf("%d",&op);
		switch (op)
		{
		case 1:
			cadastrarCarro(arq);
			break;
		case 2:
			consultarCarro(arq);
			break;
		case 3:
			gerarArquivo(arq);
			break;
		case 4:
			excluir(arq);
			break;
		case 5: fclose(arq);
		break;
		}
	}while(op!=5);	
	
	
	return 1;
}

void cadastrarCarro (FILE*arq)
{
	int i;
	modelo carro;
	carro.status = 'D';
	char confirma;
	fflush(stdin);

	printf("Código do carro: %i \n",tam(arq)+1);
	printf("Informe a marca do carro: \n");
	fflush(stdin);
	gets(carro.modelo);
	
	printf("Informe a cor do carro:\n");
	fflush(stdin);
	gets(carro.cor);
	
	carro.id = tam(arq) + 1;
	printf("\nCadastrar?(S/N):");
	fflush(stdin);
	scanf("%c", &confirma);
	
	if (toupper(confirma) == 'S')
	{
		printf("\Carro cadastrado com sucesso!\n\n");
		fseek(arq, 0, SEEK_END);
		fwrite(&carro, sizeof(modelo), 1, arq);
	}	
}
void consultarCarro (FILE*arq) {
	
	modelo carro;
	int numero;
	
	printf("\nInforme o código a ser consultado: ");
	scanf("%d", &numero);

	
	if ((numero <= tam(arq)) && (numero>0))
	{
		fseek(arq, (numero - 1) * sizeof(modelo), SEEK_SET);
		fread(&carro, sizeof(modelo), 1, arq);
		if(carro.status == 'D')
		{
			printf("\n modelo: %s", carro.modelo);
			printf("\n cor: %s", carro.cor);
			printf("\n status: %c\n\n",carro.status);
		}
		else
		{
			printf("Não existe carro cadastro com o código informado!\n");
		}
		
	}
	else
	{
		printf("\nCódigo inválido!\n");
	}
	system("pause");	
}

void gerarArquivo(FILE *arq)
{
	char nome[20];
	int numero;
	modelo carro;
	printf("Informe o nome do arquivo a ser gerado:");
	scanf("%s", nome);
	strcat(nome, ".txt");
	FILE *arqtxt = fopen(nome, "w");
	if (!arqtxt)
	{
		printf("Erro ao criar arquivo. Tente novamente!\n");
		system("pause");
	}
	fprintf(arqtxt, "Marca                Cor               Status\n");
	fprintf(arqtxt, "----------------------------------------------\n");

	for (numero = 0; numero<tam(arq); numero++)
	{
		fseek(arq, numero* sizeof(modelo), SEEK_SET);
		fread(&carro, sizeof(modelo), 1, arq);
		fprintf(arqtxt, "%-20s%-22s%-25c\n", carro.modelo, carro.cor, carro.status);
	}
	fprintf(arqtxt, "----------------------------------------------\n");
	fclose(arqtxt);
}

void excluir(FILE *arq)
{
	modelo carro;
	char confirma;
	int numero;

	printf("\nInforme o código do carro a ser excluído:\n");
	fflush(stdin);
	scanf("%d", &numero);
	
	if ((numero <= tam(arq)) && (numero>0))
	{
		fseek(arq, (numero - 1) * sizeof(modelo), SEEK_SET);
		fread(&carro, sizeof(modelo), 1, arq);
		fflush(stdin);
		
		if (carro.status == 'D')
		{
			printf("\nMarca: %s", carro.modelo);
			printf("\nCor: %s", carro.cor);
			printf("\nStatus: %c\n",carro.status);
			printf("\nDeseja excluir(S/N)\n");
			getchar();
			scanf("%s", &confirma);

			if (toupper(confirma) == 'S')
			{
				printf("\nExcluido com sucesso\n\n");
				carro.status = 'I';
				fseek(arq, (numero - 1) * sizeof(modelo), SEEK_SET);
				fwrite(&carro, sizeof(modelo), 1, arq);
			}
		}
		else
			printf("Não foi encontrado registro com o código informado!\n");
	}
	else
	{
		printf("\nCódigo inválido. Favor inserir um código válido!\n");
	}
	system("pause");

}
int tam(FILE *arq)
{
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(modelo);
}

