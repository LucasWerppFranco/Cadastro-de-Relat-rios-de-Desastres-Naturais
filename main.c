#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_RELATOS 100
#define PI 3.14159265358979323846
#define RAIO_TERRA 6371.0  // km

typedef struct {
    char nome[50];
    char cpf[15];
    char descricao[200];
    double latitude;
    double longitude;
} Relato;

Relato relatos[MAX_RELATOS];
int totalRelatos = 0;

// Função para validar CPF simples (11 dígitos)
int validarCPF(char *cpf) {
    return strlen(cpf) == 11;
}

// Função para calcular distância entre duas coordenadas (Haversine)
double calcularDistancia(double lat1, double lon1, double lat2, double lon2) {
    double dLat = (lat2 - lat1) * PI / 180.0;
    double dLon = (lon2 - lon1) * PI / 180.0;

    lat1 = lat1 * PI / 180.0;
    lat2 = lat2 * PI / 180.0;

    double a = sin(dLat / 2) * sin(dLat / 2) +
               sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return RAIO_TERRA * c;
}

// Função para cadastrar um relato
void cadastrarRelato() {
    if (totalRelatos >= MAX_RELATOS) {
        printf("Limite de relatos atingido!\n");
        return;
    }

    Relato r;

    printf("Nome: ");
    fgets(r.nome, sizeof(r.nome), stdin);
    r.nome[strcspn(r.nome, "\n")] = 0;  // Remove '\n'

    printf("CPF (somente numeros): ");
    fgets(r.cpf, sizeof(r.cpf), stdin);
    r.cpf[strcspn(r.cpf, "\n")] = 0;

    if (!validarCPF(r.cpf)) {
        printf("CPF invalido!\n");
        return;
    }

    printf("Descricao do relato: ");
    fgets(r.descricao, sizeof(r.descricao), stdin);
    r.descricao[strcspn(r.descricao, "\n")] = 0;

    printf("Latitude: ");
    scanf("%lf", &r.latitude);

    printf("Longitude: ");
    scanf("%lf", &r.longitude);

    getchar(); // Limpa '\n' pendente

    relatos[totalRelatos++] = r;

    printf("Relato cadastrado com sucesso!\n");
}

// Função para listar relatos em até 10 km de uma posição
void listarRelatosProximos() {
    double lat, lon;
    printf("Informe sua Latitude: ");
    scanf("%lf", &lat);
    printf("Informe sua Longitude: ");
    scanf("%lf", &lon);

    getchar(); // Limpa '\n'

    printf("\nRelatos em até 10 km:\n");
    for (int i = 0; i < totalRelatos; i++) {
        double distancia = calcularDistancia(lat, lon, relatos[i].latitude, relatos[i].longitude);
        if (distancia <= 10.0) {
            printf("Relato %d:\n", i + 1);
            printf("Nome: %s\n", relatos[i].nome);
            printf("CPF: %s\n", relatos[i].cpf);
            printf("Descricao: %s\n", relatos[i].descricao);
            printf("Local: (%.6lf, %.6lf)\n", relatos[i].latitude, relatos[i].longitude);
            printf("Distancia: %.2lf km\n\n", distancia);
        }
    }
}

// Função de comparação para ordenar relatos por nome
int compararRelatos(const void *a, const void *b) {
    Relato *r1 = (Relato *)a;
    Relato *r2 = (Relato *)b;
    return strcmp(r1->nome, r2->nome);
}

// Função para ordenar relatos por nome
void ordenarRelatos() {
    qsort(relatos, totalRelatos, sizeof(Relato), compararRelatos);
    printf("Relatos ordenados por nome!\n");
}

// Função para buscar relato pelo CPF
void buscarRelatoPorCPF() {
    char cpf[15];
    printf("Informe o CPF para busca: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    for (int i = 0; i < totalRelatos; i++) {
        if (strcmp(relatos[i].cpf, cpf) == 0) {
            printf("Relato encontrado:\n");
            printf("Nome: %s\n", relatos[i].nome);
            printf("Descricao: %s\n", relatos[i].descricao);
            printf("Local: (%.6lf, %.6lf)\n", relatos[i].latitude, relatos[i].longitude);
            return;
        }
    }
    printf("Relato nao encontrado para o CPF informado.\n");
}

// Função para salvar relatos em arquivo
void salvarRelatos() {
    FILE *f = fopen("relatos.txt", "w");
    if (!f) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    for (int i = 0; i < totalRelatos; i++) {
        fprintf(f, "%s|%s|%s|%.6lf|%.6lf\n",
                relatos[i].nome,
                relatos[i].cpf,
                relatos[i].descricao,
                relatos[i].latitude,
                relatos[i].longitude);
    }

    fclose(f);
    printf("Relatos salvos em 'relatos.txt'\n");
}

// Função para carregar relatos de arquivo
void carregarRelatos() {
    FILE *f = fopen("relatos.txt", "r");
    if (!f) return;

    while (!feof(f)) {
        Relato r;
        if (fscanf(f, "%49[^|]|%14[^|]|%199[^|]|%lf|%lf\n",
                   r.nome, r.cpf, r.descricao, &r.latitude, &r.longitude) == 5) {
            relatos[totalRelatos++] = r;
        }
    }
    fclose(f);
}

void menu() {
    printf("\nSistema de Cadastro de Relatos de Catastrofes Naturais\n");
    printf("1. Cadastrar relato\n");
    printf("2. Listar relatos proximos (ate 10 km)\n");
    printf("3. Ordenar relatos por nome\n");
    printf("4. Buscar relato por CPF\n");
    printf("5. Salvar relatos em arquivo\n");
    printf("6. Sair\n");
    printf("Escolha: ");
}

int main() {
    carregarRelatos();

    int opcao;
    do {
        menu();
        scanf("%d", &opcao);
        getchar();  // Limpa '\n'

        switch (opcao) {
            case 1: cadastrarRelato(); break;
            case 2: listarRelatosProximos(); break;
            case 3: ordenarRelatos(); break;
            case 4: buscarRelatoPorCPF(); break;
            case 5: salvarRelatos(); break;
            case 6: salvarRelatos(); printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n");
        }

    } while (opcao != 6);

    return 0;
}

