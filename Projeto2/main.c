#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura do Investidor
typedef struct {
    char cpf[12];
    char senha[7];
    char nome[100];
    float reais;
    float btc;
    float eth;
    float xrp;
} Investidor;

// Estrutura do Administrador
typedef struct {
    char cpf[12];
    char senha[7];
    char senhaGeral[7]; // Padrão "0000"
    char nome[100];
} Administrador;

// Estrutura do Mercado (Cotações)
typedef struct {
    float cot_btc;
    float cot_eth;
    float cot_xrp;
} Mercado;

// Protótipos
void carregarMercado(Mercado *m);
void salvarMercado(Mercado m);
int carregarInvestidores(Investidor invs[]);
void salvarInvestidores(Investidor invs[], int total);
int carregarAdmin(Administrador *adm);
void salvarAdmin(Administrador adm);
void registrarExtrato(char *cpfAlvo, char *detalhes, float valor, char *moeda);
void menuAdmin();
void menuInvestidor(int indiceLogado);

int main() {
    srand(time(NULL));

    char cpfDigitado[12];
    char senhaDigitada[7];

    printf("=========================================\n");
    printf("   EXCHANGE DE CRIPTOMOEDAS - DUPLA     \n");
    printf("=========================================\n");
    printf("Digite seu CPF (ou digite 'admin'): ");
    scanf("%s", cpfDigitado);

    // ==========================================
    // FLUXO DO ADMINISTRADOR
    // ==========================================
    if (strcmp(cpfDigitado, "admin") == 0) {
        Administrador adm;
        int existeAdmin = carregarAdmin(&adm);

        if (!existeAdmin) {
            printf("\n--- PRIMEIRO ACESSO: CADASTRO DO ADMIN ---\n");
            strcpy(adm.cpf, "admin");
            printf("Digite o nome completo do Admin: ");
            scanf(" %[^\n]", adm.nome);
            printf("Digite a senha de acesso: ");
            scanf("%s", adm.senha);
            strcpy(adm.senhaGeral, "0000"); // Senha padrão solicitada
            salvarAdmin(adm);
            printf("Admin cadastrado com sucesso! Senha geral da carteira definida como: 0000\n");
        }

        printf("Digite a senha geral da carteira (ou senha do admin): ");
        scanf("%s", senhaDigitada);

        if (strcmp(senhaDigitada, adm.senhaGeral) == 0 || strcmp(senhaDigitada, adm.senha) == 0) {
            menuAdmin();
        } else {
            printf("Senha incorreta!\n");
        }
        return 0;
    }

    // ==========================================
    // FLUXO DO INVESTIDOR COMUM
    // ==========================================
    Investidor invs[50];
    int total = carregarInvestidores(invs);
    int encontrado = -1;

    for (int i = 0; i < total; i++) {
        if (strcmp(invs[i].cpf, cpfDigitado) == 0) {
            encontrado = i;
            break;
        }
    }

    // Se o CPF não existir, faz o cadastro automático na hora
    if (encontrado == -1) {
        printf("\nCPF nao cadastrado no sistema!\n");
        printf("Vamos realizar o seu cadastro.\n");

        strcpy(invs[total].cpf, cpfDigitado);
        printf("Digite o seu nome completo: ");
        scanf(" %[^\n]", invs[total].nome);
        printf("Digite uma senha numerica (6 digitos): ");
        scanf("%s", invs[total].senha);

        // Saldos iniciais zerados
        invs[total].reais = 0.0;
        invs[total].btc = 0.0;
        invs[total].eth = 0.0;
        invs[total].xrp = 0.0;

        total++;
        salvarInvestidores(invs, total);
        encontrado = total - 1; // Já define o índice como o recém-cadastrado
        printf("Cadastro realizado com sucesso! Voce ja esta logado.\n");
    } else {
        // Se já existe, pede apenas a senha
        printf("Digite sua senha de 6 digitos: ");
        scanf("%s", senhaDigitada);

        if (strcmp(invs[encontrado].senha, senhaDigitada) != 0) {
            printf("Senha incorreta!\n");
            return 0;
        }
    }

    menuInvestidor(encontrado);
    return 0;
}

// ==========================================
// FUNÇÕES DE ARQUIVO
// ==========================================

void carregarMercado(Mercado *m) {
    FILE *f = fopen("mercado.txt", "r");
    if (f == NULL) {
        m->cot_btc = 150000.00;
        m->cot_eth = 8000.00;
        m->cot_xrp = 3.50;
        salvarMercado(*m);
    } else {
        fscanf(f, "%f %f %f", &m->cot_btc, &m->cot_eth, &m->cot_xrp);
        fclose(f);
    }
}

void salvarMercado(Mercado m) {
    FILE *f = fopen("mercado.txt", "w");
    if (f != NULL) {
        fprintf(f, "%.2f %.2f %.2f\n", m.cot_btc, m.cot_eth, m.cot_xrp);
        fclose(f);
    }
}

int carregarInvestidores(Investidor invs[]) {
    FILE *f = fopen("investidores.txt", "r");
    if (f == NULL) return 0;

    int total = 0;
    while (fscanf(f, "%s %s %[^\n]", invs[total].cpf, invs[total].senha, invs[total].nome) == 3) {
        fscanf(f, "%f %f %f %f", &invs[total].reais, &invs[total].btc, &invs[total].eth, &invs[total].xrp);
        total++;
        if (total >= 50) break;
    }
    fclose(f);
    return total;
}

void salvarInvestidores(Investidor invs[], int total) {
    FILE *f = fopen("investidores.txt", "w");
    if (f != NULL) {
        for (int i = 0; i < total; i++) {
            fprintf(f, "%s %s %s\n", invs[i].cpf, invs[i].senha, invs[i].nome);
            fprintf(f, "%.2f %.4f %.4f %.4f\n", invs[i].reais, invs[i].btc, invs[i].eth, invs[i].xrp);
        }
        fclose(f);
    }
}

int carregarAdmin(Administrador *adm) {
    FILE *f = fopen("admin.txt", "r");
    if (f == NULL) return 0;
    fscanf(f, "%s %s %s %[^\n]", adm->cpf, adm->senha, adm->senhaGeral, adm->nome);
    fclose(f);
    return 1;
}

void salvarAdmin(Administrador adm) {
    FILE *f = fopen("admin.txt", "w");
    if (f != NULL) {
        fprintf(f, "%s %s %s %s\n", adm.cpf, adm.senha, adm.senhaGeral, adm.nome);
        fclose(f);
    }
}

void registrarExtrato(char *cpfAlvo, char *detalhes, float valor, char *moeda) {
    FILE *f = fopen("extrato.txt", "a");
    if (f != NULL) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(f, "[CPF: %s] %02d-%02d-%d %02d:%02d %s %.2f %s\n",
                cpfAlvo, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                tm.tm_hour, tm.tm_min, detalhes, valor, moeda);
        fclose(f);
    }
}

// ==========================================
// PAINEL DO ADMINISTRADOR
// ==========================================

void menuAdmin() {
    Investidor invs[50];
    Mercado m;
    int opcao;

    carregarMercado(&m);

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
        scanf("%d", &opcao);

        int total = carregarInvestidores(invs);

        if (opcao == 1) {
            char novoCpf[12];
            printf("Digite o CPF (sem pontos e tracos): ");
            scanf("%s", novoCpf);

            int existe = 0;
            for (int i = 0; i < total; i++) {
                if (strcmp(invs[i].cpf, novoCpf) == 0) { existe = 1; break; }
            }

            if (existe) {
                printf("Erro: CPF ja cadastrado!\n");
            } else {
                strcpy(invs[total].cpf, novoCpf);
                printf("Digite o nome completo: ");
                scanf(" %[^\n]", invs[total].nome);
                printf("Digite a senha numerica (6 digitos): ");
                scanf("%s", invs[total].senha);
                invs[total].reais = 0.0;
                invs[total].btc = 0.0;
                invs[total].eth = 0.0;
                invs[total].xrp = 0.0;

                salvarInvestidores(invs, total + 1);
                printf("Investidor cadastrado com sucesso!\n");
            }

        } else if (opcao == 2) {
            char cpfBusca[12];
            printf("Digite o CPF do investidor a ser excluido: ");
            scanf("%s", cpfBusca);

            int pos = -1;
            for (int i = 0; i < total; i++) {
                if (strcmp(invs[i].cpf, cpfBusca) == 0) { pos = i; break; }
            }

            if (pos == -1) {
                printf("CPF nao encontrado.\n");
            } else {
                printf("\nNome: %s | CPF: %s\n", invs[pos].nome, invs[pos].cpf);
                printf("Deseja realmente excluir? (1-Sim / 0-Nao): ");
                int confirm;
                scanf("%d", &confirm);

                if (confirm == 1) {
                    for (int i = pos; i < total - 1; i++) { invs[i] = invs[i + 1]; }
                    salvarInvestidores(invs, total - 1);
                    printf("Investidor excluido com sucesso!\n");
                }
            }

        } else if (opcao == 3) {
            char cpfBusca[12];
            printf("Digite o CPF do investidor: ");
            scanf("%s", cpfBusca);

            int pos = -1;
            for (int i = 0; i < total; i++) {
                if (strcmp(invs[i].cpf, cpfBusca) == 0) { pos = i; break; }
            }

            if (pos != -1) {
                printf("\n--- SALDO DO INVESTIDOR ---\n");
                printf("Nome: %s\n", invs[pos].nome);
                printf("CPF: %s\n", invs[pos].cpf);
                printf("Reais: R$ %.2f\n", invs[pos].reais);
                printf("Bitcoin: %.4f\n", invs[pos].btc);
                printf("Ethereum: %.4f\n", invs[pos].eth);
                printf("Ripple: %.4f\n", invs[pos].xrp);
            } else {
                printf("CPF nao encontrado.\n");
            }

        } else if (opcao == 4) {
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

        } else if (opcao == 5) {
            float var_btc = ((rand() % 11) - 5) / 100.0;
            float var_eth = ((rand() % 11) - 5) / 100.0;
            float var_xrp = ((rand() % 11) - 5) / 100.0;

            m.cot_btc += m.cot_btc * var_btc;
            m.cot_eth += m.cot_eth * var_eth;
            m.cot_xrp += m.cot_xrp * var_xrp;
            salvarMercado(m);

            printf("\nCotacoes atualizadas!\nBTC: R$ %.2f | ETH: R$ %.2f | XRP: R$ %.2f\n", m.cot_btc, m.cot_eth, m.cot_xrp);

        } else if (opcao == 6) {
            printf("Saindo do painel...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 6);
}

// ==========================================
// PAINEL DO INVESTIDOR
// ==========================================

void menuInvestidor(int indiceLogado) {
    Investidor invs[50];
    Mercado m;
    int opcao;

    carregarMercado(&m);
    int total = carregarInvestidores(invs);

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
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("\n--- SEU SALDO ---\n");
            printf("Nome: %s\n", invs[indiceLogado].nome);
            printf("CPF: %s\n", invs[indiceLogado].cpf);
            printf("Reais: R$ %.2f\n", invs[indiceLogado].reais);
            printf("Bitcoin: %.4f\n", invs[indiceLogado].btc);
            printf("Ethereum: %.4f\n", invs[indiceLogado].eth);
            printf("Ripple: %.4f\n", invs[indiceLogado].xrp);

        } else if (opcao == 2) {
            FILE *fExt = fopen("extrato.txt", "r");
            if (fExt == NULL) {
                printf("Nenhum extrato registrado.\n");
            } else {
                printf("\n--- SEU EXTRATO ---\n");
                char linha[256];
                while (fgets(linha, sizeof(linha), fExt) != NULL) {
                    if (strstr(linha, invs[indiceLogado].cpf) != NULL) {
                        printf("%s", linha);
                    }
                }
                fclose(fExt);
            }

        } else if (opcao == 3) {
            float valor;
            printf("Valor para deposito (R$): ");
            scanf("%f", &valor);
            if (valor > 0) {
                invs[indiceLogado].reais += valor;
                salvarInvestidores(invs, total);
                registrarExtrato(invs[indiceLogado].cpf, "+", valor, "REAL");
                printf("Deposito realizado!\n");
            } else { printf("Valor invalido!\n"); }

        } else if (opcao == 4) {
            float valor;
            printf("Valor para saque (R$): ");
            scanf("%f", &valor);
            if (valor > 0 && valor <= invs[indiceLogado].reais) {
                invs[indiceLogado].reais -= valor;
                salvarInvestidores(invs, total);
                registrarExtrato(invs[indiceLogado].cpf, "-", valor, "REAL");
                printf("Saque realizado!\n");
            } else { printf("Saldo insuficiente ou valor invalido!\n"); }

        } else if (opcao == 5) {
            int tipo;
            float qtd, custo, taxa;
            printf("\n1. BTC (R$ %.2f | Taxa 2%%)\n2. ETH (R$ %.2f | Taxa 1%%)\n3. XRP (R$ %.2f | Taxa 1%%)\n", m.cot_btc, m.cot_eth, m.cot_xrp);
            printf("Escolha a moeda: ");
            scanf("%d", &tipo);
            printf("Quantidade: ");
            scanf("%f", &qtd);

            if (tipo == 1) {
                custo = qtd * m.cot_btc; taxa = custo * 0.02;
                if (invs[indiceLogado].reais >= (custo + taxa)) {
                    invs[indiceLogado].reais -= (custo + taxa);
                    invs[indiceLogado].btc += qtd;
                    salvarInvestidores(invs, total);
                    registrarExtrato(invs[indiceLogado].cpf, "+", qtd, "BTC");
                    printf("Compra realizada!\n");
                } else { printf("Saldo insuficiente!\n"); }
            } else if (tipo == 2) {
                custo = qtd * m.cot_eth; taxa = custo * 0.01;
                if (invs[indiceLogado].reais >= (custo + taxa)) {
                    invs[indiceLogado].reais -= (custo + taxa);
                    invs[indiceLogado].eth += qtd;
                    salvarInvestidores(invs, total);
                    registrarExtrato(invs[indiceLogado].cpf, "+", qtd, "ETH");
                    printf("Compra realizada!\n");
                } else { printf("Saldo insuficiente!\n"); }
            } else if (tipo == 3) {
                custo = qtd * m.cot_xrp; taxa = custo * 0.01;
                if (invs[indiceLogado].reais >= (custo + taxa)) {
                    invs[indiceLogado].reais -= (custo + taxa);
                    invs[indiceLogado].xrp += qtd;
                    salvarInvestidores(invs, total);
                    registrarExtrato(invs[indiceLogado].cpf, "+", qtd, "XRP");
                    printf("Compra realizada!\n");
                } else { printf("Saldo insuficiente!\n"); }
            }

        } else if (opcao == 6) {
            int tipo;
            float qtd, bruto, taxa;
            printf("Escolha a moeda para vender (1-BTC, 2-ETH, 3-XRP): ");
            scanf("%d", &tipo);
            printf("Quantidade: ");
            scanf("%f", &qtd);

            if (tipo == 1 && invs[indiceLogado].btc >= qtd) {
                bruto = qtd * m.cot_btc; taxa = bruto * 0.03;
                invs[indiceLogado].btc -= qtd; invs[indiceLogado].reais += (bruto - taxa);
                salvarInvestidores(invs, total);
                registrarExtrato(invs[indiceLogado].cpf, "-", qtd, "BTC");
                printf("Venda realizada!\n");
            } else if (tipo == 2 && invs[indiceLogado].eth >= qtd) {
                bruto = qtd * m.cot_eth; taxa = bruto * 0.02;
                invs[indiceLogado].eth -= qtd; invs[indiceLogado].reais += (bruto - taxa);
                salvarInvestidores(invs, total);
                registrarExtrato(invs[indiceLogado].cpf, "-", qtd, "ETH");
                printf("Venda realizada!\n");
            } else if (tipo == 3 && invs[indiceLogado].xrp >= qtd) {
                bruto = qtd * m.cot_xrp; taxa = bruto * 0.01;
                invs[indiceLogado].xrp -= qtd; invs[indiceLogado].reais += (bruto - taxa);
                salvarInvestidores(invs, total);
                registrarExtrato(invs[indiceLogado].cpf, "-", qtd, "XRP");
                printf("Venda realizada!\n");
            } else { printf("Quantidade insuficiente!\n"); }

        } else if (opcao == 7) {
            float var_btc = ((rand() % 11) - 5) / 100.0;
            float var_eth = ((rand() % 11) - 5) / 100.0;
            float var_xrp = ((rand() % 11) - 5) / 100.0;
            m.cot_btc += m.cot_btc * var_btc;
            m.cot_eth += m.cot_eth * var_eth;
            m.cot_xrp += m.cot_xrp * var_xrp;
            salvarMercado(m);
            printf("\nCotacoes atualizadas!\nBTC: R$ %.2f | ETH: R$ %.2f | XRP: R$ %.2f\n", m.cot_btc, m.cot_eth, m.cot_xrp);

        } else if (opcao == 8) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida!\n");
        }

    } while (opcao != 8);
}