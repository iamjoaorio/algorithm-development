#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para os dados do cliente
typedef struct {
    char documento[12];
    char senha[7];
    char nomeCompleto[100];
    float saldoReais;
    float qtdBtc;
    float qtdEth;
    float qtdXrp;
} Cliente;

// Estrutura para o Administrador
typedef struct {
    char documento[12];
    char senha[7];
    char senhaMestra[7]; 
    char nomeCompleto[100];
} Gestor;

// Estrutura para o Mercado
typedef struct {
    float precoBtc;
    float precoEth;
    float precoXrp;
} MercadoCripto;

// Protótipos das funções
void lerMercado(MercadoCripto *mercado);
void gravarMercado(MercadoCripto mercado);
int carregarClientes(Cliente lista[]);
void salvarClientes(Cliente lista[], int qtd);
int carregarGestor(Gestor *admin);
void salvarGestor(Gestor admin);
void gravarLogExtrato(Cliente c, char *sinal, float valor, char *ativo, float cotacao, float taxa);
void painelAdmin();
void menuCliente(int indiceCliente);

int main() {
    srand(time(NULL));

    char docDigitado[12];
    char senhaDigitada[7];

    printf("=========================================\n");
    printf("     EXCHANGE DE CRIPTOMOEDAS - DUPLA     \n");
    printf("=========================================\n");
    printf("Digite seu CPF (ou digite 'admin'): ");
    scanf("%s", docDigitado);

    // Fluxo do Administrador
    if (strcmp(docDigitado, "admin") == 0) {
        Gestor admin;
        int temAdmin = carregarGestor(&admin);

        if (!temAdmin) {
            printf("\n--- PRIMEIRO ACESSO: CADASTRO DO ADMIN ---\n");
            strcpy(admin.documento, "admin");
            printf("Digite o nome completo do Admin: ");
            scanf(" %[^\n]", admin.nomeCompleto);
            printf("Digite a senha de acesso: ");
            scanf("%s", admin.senha);
            strcpy(admin.senhaMestra, "0000"); 
            salvarGestor(admin);
            printf("Admin cadastrado com sucesso! Senha geral definida como: 0000\n");
        }

        printf("Digite a senha geral da carteira (ou senha do admin): ");
        scanf("%s", senhaDigitada);

        if (strcmp(senhaDigitada, admin.senhaMestra) == 0 || strcmp(senhaDigitada, admin.senha) == 0) {
            painelAdmin();
        } else {
            printf("Senha incorreta!\n");
        }
        return 0;
    }

    // Fluxo do Cliente
    Cliente listaClientes[50];
    int totalClientes = carregarClientes(listaClientes);
    int posicaoCliente = -1;

    for (int i = 0; i < totalClientes; i++) {
        if (strcmp(listaClientes[i].documento, docDigitado) == 0) {
            posicaoCliente = i;
            break;
        }
    }

    if (posicaoCliente == -1) {
        printf("\nCPF nao cadastrado no sistema!\n");
        printf("Vamos realizar o seu cadastro.\n");

        strcpy(listaClientes[totalClientes].documento, docDigitado);
        printf("Digite o seu nome completo: ");
        scanf(" %[^\n]", listaClientes[totalClientes].nomeCompleto);
        printf("Digite uma senha numerica (6 digitos): ");
        scanf("%s", listaClientes[totalClientes].senha);

        listaClientes[totalClientes].saldoReais = 0.0;
        listaClientes[totalClientes].qtdBtc = 0.0;
        listaClientes[totalClientes].qtdEth = 0.0;
        listaClientes[totalClientes].qtdXrp = 0.0;

        totalClientes++;
        salvarClientes(listaClientes, totalClientes);
        posicaoCliente = totalClientes - 1;
        printf("Cadastro realizado com sucesso! Voce ja esta logado.\n");
    } else {
        printf("Digite sua senha de 6 digitos: ");
        scanf("%s", senhaDigitada);

        if (strcmp(listaClientes[posicaoCliente].senha, senhaDigitada) != 0) {
            printf("Senha incorreta!\n");
            return 0;
        }
    }

    menuCliente(posicaoCliente);
    return 0;
}

// Funções de Arquivo
void lerMercado(MercadoCripto *mercado) {
    FILE *arq = fopen("mercado.txt", "r");
    if (arq == NULL) {
        mercado->precoBtc = 150000.00;
        mercado->precoEth = 8000.00;
        mercado->precoXrp = 3.50;
        gravarMercado(*mercado);
    } else {
        fscanf(arq, "%f %f %f", &mercado->precoBtc, &mercado->precoEth, &mercado->precoXrp);
        fclose(arq);
    }
}

void gravarMercado(MercadoCripto mercado) {
    FILE *arq = fopen("mercado.txt", "w");
    if (arq != NULL) {
        fprintf(arq, "%.2f %.2f %.2f\n", mercado.precoBtc, mercado.precoEth, mercado.precoXrp);
        fclose(arq);
    }
}

int carregarClientes(Cliente lista[]) {
    FILE *arq = fopen("investidores.txt", "r");
    if (arq == NULL) return 0;

    int qtd = 0;
    while (fscanf(arq, "%s %s %[^\n]", lista[qtd].documento, lista[qtd].senha, lista[qtd].nomeCompleto) == 3) {
        fscanf(arq, "%f %f %f %f", &lista[qtd].saldoReais, &lista[qtd].qtdBtc, &lista[qtd].qtdEth, &lista[qtd].qtdXrp);
        qtd++;
        if (qtd >= 50) break;
    }
    fclose(arq);
    return qtd;
}

void salvarClientes(Cliente lista[], int qtd) {
    FILE *arq = fopen("investidores.txt", "w");
    if (arq != NULL) {
        for (int i = 0; i < qtd; i++) {
            fprintf(arq, "%s %s %s\n", lista[i].documento, lista[i].senha, lista[i].nomeCompleto);
            fprintf(arq, "%.2f %.4f %.4f %.4f\n", lista[i].saldoReais, lista[i].qtdBtc, lista[i].qtdEth, lista[i].qtdXrp);
        }
        fclose(arq);
    }
}

int carregarGestor(Gestor *admin) {
    FILE *arq = fopen("admin.txt", "r");
    if (arq == NULL) return 0;
    fscanf(arq, "%s %s %s %[^\n]", admin->documento, admin->senha, admin->senhaMestra, admin->nomeCompleto);
    fclose(arq);
    return 1;
}

void salvarGestor(Gestor admin) {
    FILE *arq = fopen("admin.txt", "w");
    if (arq != NULL) {
        fprintf(arq, "%s %s %s %s\n", admin.documento, admin.senha, admin.senhaMestra, admin.nomeCompleto);
        fclose(arq);
    }
}

// Extrato detalhado no formato exigido pela especificação da FEI
void gravarLogExtrato(Cliente c, char *sinal, float valor, char *ativo, float cotacao, float taxa) {
    FILE *arq = fopen("extrato.txt", "a");
    if (arq != NULL) {
        time_t agora = time(NULL);
        struct tm t = *localtime(&agora);
        fprintf(arq, "[CPF: %s] %02d-%02d-%d %02d:%02d %s %.2f %s CT: %.2f TX: %.2f REAL: %.2f BTC: %.4f ETH: %.4f XRP: %.4f\n",
                c.documento, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900,
                t.tm_hour, t.tm_min, sinal, valor, ativo,
                cotacao, taxa, c.saldoReais, c.qtdBtc, c.qtdEth, c.qtdXrp);
        fclose(arq);
    }
}

// Painel do Administrador
void painelAdmin() {
    Cliente listaClientes[50];
    MercadoCripto mercado;
    int escolha;

    lerMercado(&mercado);

    do {
        printf("\n=========================================\n");
        printf("           PAINEL DO ADMINISTRADOR       \n");
        printf("=========================================\n");
        printf("1. Cadastrar novo investidor\n");
        printf("2. Excluir investidor\n");
        printf("3. Consultar saldo de um investidor\n");
        printf("4. Consultar extrato de um investidor\n");
        printf("5. Atualizar cotacao das criptomoedas\n");
        printf("6. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        int totalClientes = carregarClientes(listaClientes);

        if (escolha == 1) {
            char novoDoc[12];
            printf("Digite o CPF (sem pontos e tracos): ");
            scanf("%s", novoDoc);

            int jaExiste = 0;
            for (int i = 0; i < totalClientes; i++) {
                if (strcmp(listaClientes[i].documento, novoDoc) == 0) { jaExiste = 1; break; }
            }

            if (jaExiste) {
                printf("Erro: CPF ja cadastrado!\n");
            } else {
                strcpy(listaClientes[totalClientes].documento, novoDoc);
                printf("Digite o nome completo: ");
                scanf(" %[^\n]", listaClientes[totalClientes].nomeCompleto);
                printf("Digite a senha numerica (6 digitos): ");
                scanf("%s", listaClientes[totalClientes].senha);
                listaClientes[totalClientes].saldoReais = 0.0;
                listaClientes[totalClientes].qtdBtc = 0.0;
                listaClientes[totalClientes].qtdEth = 0.0;
                listaClientes[totalClientes].qtdXrp = 0.0;

                salvarClientes(listaClientes, totalClientes + 1);
                printf("Investidor cadastrado com sucesso!\n");
            }

        } else if (escolha == 2) {
            char cpfBusca[12];
            printf("Digite o CPF do investidor a ser excluido: ");
            scanf("%s", cpfBusca);

            int idx = -1;
            for (int i = 0; i < totalClientes; i++) {
                if (strcmp(listaClientes[i].documento, cpfBusca) == 0) { idx = i; break; }
            }

            if (idx == -1) {
                printf("CPF nao encontrado.\n");
            } else {
                printf("\nNome: %s | CPF: %s\n", listaClientes[idx].nomeCompleto, listaClientes[idx].documento);
                printf("Deseja realmente excluir? (1-Sim / 0-Nao): ");
                int confirmacao;
                scanf("%d", &confirmacao);

                if (confirmacao == 1) {
                    for (int i = idx; i < totalClientes - 1; i++) { listaClientes[i] = listaClientes[i + 1]; }
                    salvarClientes(listaClientes, totalClientes - 1);
                    printf("Investidor excluido com sucesso!\n");
                }
            }

        } else if (escolha == 3) {
            char cpfBusca[12];
            printf("Digite o CPF do investidor: ");
            scanf("%s", cpfBusca);

            int idx = -1;
            for (int i = 0; i < totalClientes; i++) {
                if (strcmp(listaClientes[i].documento, cpfBusca) == 0) { idx = i; break; }
            }

            if (idx != -1) {
                printf("\n--- SALDO DO INVESTIDOR ---\n");
                printf("Nome: %s\n", listaClientes[idx].nomeCompleto);
                printf("CPF: %s\n", listaClientes[idx].documento);
                printf("Reais: R$ %.2f\n", listaClientes[idx].saldoReais);
                printf("Bitcoin: %.4f\n", listaClientes[idx].qtdBtc);
                printf("Ethereum: %.4f\n", listaClientes[idx].qtdEth);
                printf("Ripple: %.4f\n", listaClientes[idx].qtdXrp);
            } else {
                printf("CPF nao encontrado.\n");
            }

        } else if (escolha == 4) {
            char cpfBusca[12];
            printf("Digite o CPF do investidor: ");
            scanf("%s", cpfBusca);

            FILE *fExt = fopen("extrato.txt", "r");
            if (fExt == NULL) {
                printf("Nenhum extrato registrado.\n");
            } else {
                printf("\n--- EXTRATO (CPF: %s) ---\n", cpfBusca);
                char linha[256];
                int achou = 0;
                while (fgets(linha, sizeof(linha), fExt) != NULL) {
                    if (strstr(linha, cpfBusca) != NULL) {
                        printf("%s", linha);
                        achou = 1;
                    }
                }
                if (!achou) printf("Nenhuma operacao para este CPF.\n");
                fclose(fExt);
            }

        } else if (escolha == 5) {
            float variacaoBtc = ((rand() % 11) - 5) / 100.0;
            float variacaoEth = ((rand() % 11) - 5) / 100.0;
            float variacaoXrp = ((rand() % 11) - 5) / 100.0;

            mercado.precoBtc += mercado.precoBtc * variacaoBtc;
            mercado.precoEth += mercado.precoEth * variacaoEth;
            mercado.precoXrp += mercado.precoXrp * variacaoXrp;
            gravarMercado(mercado);

            printf("\nCotacoes atualizadas!\nBTC: R$ %.2f | ETH: R$ %.2f | XRP: R$ %.2f\n", mercado.precoBtc, mercado.precoEth, mercado.precoXrp);

        } else if (escolha == 6) {
            printf("Saindo do painel...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (escolha != 6);
}

// Menu do Cliente com Validação de Senha em Consultas
void menuCliente(int indiceCliente) {
    Cliente listaClientes[50];
    MercadoCripto mercado;
    int escolha;

    lerMercado(&mercado);
    int totalClientes = carregarClientes(listaClientes);

    do {
        printf("\n=========================================\n");
        printf("           MENU DO INVESTIDOR            \n");
        printf("=========================================\n");
        printf("1. Consultar saldo\n");
        printf("2. Consultar extrato\n");
        printf("3. Depositar reais\n");
        printf("4. Sacar reais\n");
        printf("5. Comprar criptomoedas\n");
        printf("6. Vender criptomoedas\n");
        printf("7. Atualizar cotacao\n");
        printf("8. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);

        if (escolha == 1) {
            char senhaConf[7];
            printf("Digite sua senha de 6 digitos para continuar: ");
            scanf("%s", senhaConf);

            if (strcmp(senhaConf, listaClientes[indiceCliente].senha) == 0) {
                printf("\n--- SEU SALDO ---\n");
                printf("Nome: %s\n", listaClientes[indiceCliente].nomeCompleto);
                printf("CPF: %s\n", listaClientes[indiceCliente].documento);
                printf("Reais: R$ %.2f\n", listaClientes[indiceCliente].saldoReais);
                printf("Bitcoin: %.4f\n", listaClientes[indiceCliente].qtdBtc);
                printf("Ethereum: %.4f\n", listaClientes[indiceCliente].qtdEth);
                printf("Ripple: %.4f\n", listaClientes[indiceCliente].qtdXrp);
            } else {
                printf("Senha incorreta!\n");
            }

        } else if (escolha == 2) {
            char senhaConf[7];
            printf("Digite sua senha de 6 digitos para continuar: ");
            scanf("%s", senhaConf);

            if (strcmp(senhaConf, listaClientes[indiceCliente].senha) == 0) {
                FILE *fExt = fopen("extrato.txt", "r");
                if (fExt == NULL) {
                    printf("Nenhum extrato registrado.\n");
                } else {
                    printf("\n--- SEU EXTRATO ---\n");
                    char linha[256];
                    while (fgets(linha, sizeof(linha), fExt) != NULL) {
                        if (strstr(linha, listaClientes[indiceCliente].documento) != NULL) {
                            printf("%s", linha);
                        }
                    }
                    fclose(fExt);
                }
            } else {
                printf("Senha incorreta!\n");
            }

        } else if (escolha == 3) {
            float valorDep;
            printf("Valor para deposito (R$): ");
            scanf("%f", &valorDep);
            if (valorDep > 0) {
                listaClientes[indiceCliente].saldoReais += valorDep;
                salvarClientes(listaClientes, totalClientes);
                gravarLogExtrato(listaClientes[indiceCliente], "+", valorDep, "REAL", 0.0, 0.0);
                printf("Deposito realizado!\n");
            } else { printf("Valor invalido!\n"); }

        } else if (escolha == 4) {
            float valorSaque;
            printf("Valor para saque (R$): ");
            scanf("%f", &valorSaque);
            if (valorSaque > 0 && valorSaque <= listaClientes[indiceCliente].saldoReais) {
                listaClientes[indiceCliente].saldoReais -= valorSaque;
                salvarClientes(listaClientes, totalClientes);
                gravarLogExtrato(listaClientes[indiceCliente], "-", valorSaque, "REAL", 0.0, 0.0);
                printf("Saque realizado!\n");
            } else { printf("Saldo insuficiente ou valor invalido!\n"); }

        } else if (escolha == 5) {
            int tipoMoeda;
            float qtdMoeda, custoTotal, taxaCompra;
            printf("\n1. BTC (R$ %.2f | Taxa 2%%)\n2. ETH (R$ %.2f | Taxa 1%%)\n3. XRP (R$ %.2f | Taxa 1%%)\n", mercado.precoBtc, mercado.precoEth, mercado.precoXrp);
            printf("Escolha a moeda: ");
            scanf("%d", &tipoMoeda);
            printf("Quantidade: ");
            scanf("%f", &qtdMoeda);

            if (tipoMoeda == 1) {
                custoTotal = qtdMoeda * mercado.precoBtc; taxaCompra = custoTotal * 0.02;
                if (listaClientes[indiceCliente].saldoReais >= (custoTotal + taxaCompra)) {
                    listaClientes[indiceCliente].saldoReais -= (custoTotal + taxaCompra);
                    listaClientes[indiceCliente].qtdBtc += qtdMoeda;
                    salvarClientes(listaClientes, totalClientes);
                    gravarLogExtrato(listaClientes[indiceCliente], "+", qtdMoeda, "BTC", mercado.precoBtc, taxaCompra);
                    printf("Compra realizada!\n");
                } else { printf("Saldo insuficiente!\n"); }
            } else if (tipoMoeda == 2) {
                custoTotal = qtdMoeda * mercado.precoEth; taxaCompra = custoTotal * 0.01;
                if (listaClientes[indiceCliente].saldoReais >= (custoTotal + taxaCompra)) {
                    listaClientes[indiceCliente].saldoReais -= (custoTotal + taxaCompra);
                    listaClientes[indiceCliente].qtdEth += qtdMoeda;
                    salvarClientes(listaClientes, totalClientes);
                    gravarLogExtrato(listaClientes[indiceCliente], "+", qtdMoeda, "ETH", mercado.precoEth, taxaCompra);
                    printf("Compra realizada!\n");
                } else { printf("Saldo insuficiente!\n"); }
            } else if (tipoMoeda == 3) {
                custoTotal = qtdMoeda * mercado.precoXrp; taxaCompra = custoTotal * 0.01;
                if (listaClientes[indiceCliente].saldoReais >= (custoTotal + taxaCompra)) {
                    listaClientes[indiceCliente].saldoReais -= (custoTotal + taxaCompra);
                    listaClientes[indiceCliente].qtdXrp += qtdMoeda;
                    salvarClientes(listaClientes, totalClientes);
                    gravarLogExtrato(listaClientes[indiceCliente], "+", qtdMoeda, "XRP", mercado.precoXrp, taxaCompra);
                    printf("Compra realizada!\n");
                } else { printf("Saldo insuficiente!\n"); }
            }

        } else if (escolha == 6) {
            int tipoMoeda;
            float qtdMoeda, valorBruto, taxaVenda;
            printf("Escolha a moeda para vender (1-BTC, 2-ETH, 3-XRP): ");
            scanf("%d", &tipoMoeda);
            printf("Quantidade: ");
            scanf("%f", &qtdMoeda);

            if (tipoMoeda == 1 && listaClientes[indiceCliente].qtdBtc >= qtdMoeda) {
                valorBruto = qtdMoeda * mercado.precoBtc; taxaVenda = valorBruto * 0.03;
                listaClientes[indiceCliente].qtdBtc -= qtdMoeda; 
                listaClientes[indiceCliente].saldoReais += (valorBruto - taxaVenda);
                salvarClientes(listaClientes, totalClientes);
                gravarLogExtrato(listaClientes[indiceCliente], "-", qtdMoeda, "BTC", mercado.precoBtc, taxaVenda);
                printf("Venda realizada!\n");
            } else if (tipoMoeda == 2 && listaClientes[indiceCliente].qtdEth >= qtdMoeda) {
                valorBruto = qtdMoeda * mercado.precoEth; taxaVenda = valorBruto * 0.02;
                listaClientes[indiceCliente].qtdEth -= qtdMoeda; 
                listaClientes[indiceCliente].saldoReais += (valorBruto - taxaVenda);
                salvarClientes(listaClientes, totalClientes);
                gravarLogExtrato(listaClientes[indiceCliente], "-", qtdMoeda, "ETH", mercado.precoEth, taxaVenda);
                printf("Venda realizada!\n");
            } else if (tipoMoeda == 3 && listaClientes[indiceCliente].qtdXrp >= qtdMoeda) {
                valorBruto = qtdMoeda * mercado.precoXrp; taxaVenda = valorBruto * 0.01;
                listaClientes[indiceCliente].qtdXrp -= qtdMoeda; 
                listaClientes[indiceCliente].saldoReais += (valorBruto - taxaVenda);
                salvarClientes(listaClientes, totalClientes);
                gravarLogExtrato(listaClientes[indiceCliente], "-", qtdMoeda, "XRP", mercado.precoXrp, taxaVenda);
                printf("Venda realizada!\n");
            } else { printf("Quantidade insuficiente!\n"); }

        } else if (escolha == 7) {
            float variacaoBtc = ((rand() % 11) - 5) / 100.0;
            float variacaoEth = ((rand() % 11) - 5) / 100.0;
            float variacaoXrp = ((rand() % 11) - 5) / 100.0;
            mercado.precoBtc += mercado.precoBtc * variacaoBtc;
            mercado.precoEth += mercado.precoEth * variacaoEth;
            mercado.precoXrp += mercado.precoXrp * variacaoXrp;
            gravarMercado(mercado);
            printf("\nCotacoes atualizadas!\nBTC: R$ %.2f | ETH: R$ %.2f | XRP: R$ %.2f\n", mercado.precoBtc, mercado.precoEth, mercado.precoXrp);

        } else if (escolha == 8) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (escolha != 8);
}