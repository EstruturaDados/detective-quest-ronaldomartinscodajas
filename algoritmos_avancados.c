#include <stdio.h>
#include <stdlib.h>

typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria dinamicamente uma sala
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    snprintf(nova->nome, 50, "%s", nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Monta o mapa fixo da mansão
Sala* montarMansao() {
    Sala *hall = criarSala("Hall de Entrada");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *salaJantar = criarSala("Sala de Jantar");
    Sala *escritorio = criarSala("Escritório");
    Sala *cozinha = criarSala("Cozinha");

    // Conexões (esquerda e direita)
    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = NULL; // Folha

    salaJantar->esquerda = NULL;
    salaJantar->direita = cozinha;

    // Escritório e Cozinha são folhas
    return hall;
}

// Função interativa para explorar os cômodos
void explorarSalas(Sala *atual) {
    char escolha;

    while (atual != NULL) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🔍 Este é um cômodo sem saídas... Fim do caminho!\n");
            break;
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda)
            printf("  (e) → Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita)
            printf("  (d) → Ir para a direita (%s)\n", atual->direita->nome);
        printf("  (s) → Sair da exploração\n");

        printf("\nDigite sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        }
        else if (escolha == 's') {
            printf("👋 Saindo da mansão...\n");
            break;
        }
        else {
            printf("❌ Caminho inválido! Tente novamente.\n");
        }
    }
}

int main() {
    Sala *mansao = montarMansao();

    printf("🔎 Detective Quest - Bem-vindo à Mansão Misteriosa!\n");
    explorarSalas(mansao);

    printf("\n🎯 Fim da exploração!\n");
    return 0;
}
