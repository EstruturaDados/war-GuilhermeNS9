#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ===================== STRUCT ======================
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ===================== FUNÇÕES ======================

// Função para cadastrar os territórios
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do território %d ---\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();
    }
}

// Exibe todos os territórios
void exibirMapa(Territorio* mapa, int n) {
    printf("\n=== MAPA ATUAL ===\n");
    for (int i = 0; i < n; i++) {
        printf("%d. %-15s | Cor: %-10s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("==================\n");
}

// Função de ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n🎯 %s (%s) está atacando %s (%s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("Dado atacante: %d 🎲 | Dado defensor: %d 🎲\n", dadoA, dadoD);

    if (dadoA > dadoD) {
        printf("✅ Ataque bem-sucedido! %s conquistou %s!\n",
               atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("❌ O ataque falhou! %s resistiu.\n", defensor->nome);
        atacante->tropas--;
        if (atacante->tropas < 0)
            atacante->tropas = 0;
    }
}

// ===================== MISSÕES ======================

// Sorteia e atribui missão ao jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Exibe a missão do jogador
void exibirMissao(char* missao) {
    printf("\n🎯 Sua missão secreta é:\n👉 %s\n\n", missao);
}

// Verifica se o jogador cumpriu sua missão
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    int cont = 0;
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0)
            cont++;
    }

    if (strstr(missao, "3 territórios") != NULL && cont >= 3)
        return 1;

    if (strstr(missao, "eliminar cor vermelha") != NULL) {
        int existe = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0)
                existe = 1;
        }
        return (existe == 0);
    }

    if (strstr(missao, "10 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10)
                return 1;
        }
    }

    return 0;
}

// Libera memória alocada
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

// ===================== MAIN ======================
int main() {
    srand(time(NULL));

    int n;
    printf("Digite o número de territórios: ");
    scanf("%d", &n);
    getchar();

    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirMapa(mapa, n);

    // Missões pré-definidas
    char* missoes[] = {
        "Conquistar 3 territórios seguidos.",
        "Eliminar todas as tropas da cor vermelha.",
        "Dominar metade dos territórios do mapa.",
        "Conquistar todos os territórios azuis.",
        "Ter pelo menos 10 tropas em um território."
    };
    int totalMissoes = 5;

    // Alocação dinâmica da missão
    char* missaoJogador = (char*) malloc(120 * sizeof(char));
    if (!missaoJogador) {
        printf("Erro ao alocar memória para missão.\n");
        free(mapa);
        return 1;
    }

    atribuirMissao(missaoJogador, missoes, totalMissoes);
    exibirMissao(missaoJogador);

    // Guarda a cor inicial do jogador atacante
    char corJogador[10];
    printf("Digite a cor do seu exército (para verificar missão): ");
    fgets(corJogador, 10, stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';

    // ========== MENU DE AÇÕES ==========
    int opcao;
    do {
        printf("\n=== MENU DE AÇÕES ===\n");
        printf("1. Exibir mapa\n");
        printf("2. Atacar território\n");
        printf("3. Verificar missão\n");
        printf("4. Finalizar jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                exibirMapa(mapa, n);
                break;

            case 2: {
                int atacante, defensor;
                printf("Escolha território atacante: ");
                scanf("%d", &atacante);
                printf("Escolha território defensor: ");
                scanf("%d", &defensor);
                getchar();

                if (atacante < 1 || atacante > n || defensor < 1 || defensor > n) {
                    printf("⚠️ Território inválido!\n");
                    break;
                }

                if (strcmp(mapa[atacante - 1].cor, mapa[defensor - 1].cor) == 0) {
                    printf("⚠️ Um território não pode atacar outro da mesma cor!\n");
                } else {
                    atacar(&mapa[atacante - 1], &mapa[defensor - 1]);
                }
                break;
            }

            case 3:
                if (verificarMissao(missaoJogador, mapa, n, corJogador)) {
                    printf("\n🏆 MISSÃO CUMPRIDA! O exército %s venceu o jogo!\n", corJogador);
                    opcao = 4; // encerra o jogo automaticamente
                } else {
                    printf("\n⏳ Missão ainda não foi cumprida...\n");
                }
                break;

            case 4:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("⚠️ Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 4);

    liberarMemoria(mapa, missaoJogador);
    printf("Memória liberada. Fim do jogo.\n");
    return 0;
}

