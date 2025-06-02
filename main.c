#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define PI 3.14159265358979323846
#define RAIO_TERRA 6371.0  // km

typedef struct {
    char nome[50];
    char cpf[15];
    char descricao[200];
    double latitude;
    double longitude;
} Relato;

Relato *relatos = NULL;
int totalRelatos = 0;
int capacidadeRelatos = 0;

// Função para validar CPF simples (11 dígitos numéricos)
int validarCPF(char *cpf) {
    if (strlen(cpf) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) return 0;
    }
    return 1;
}

// Função para validar latitude (-90 a 90)
int validarLatitude(double lat) {
    return lat >= -90.0 && lat <= 90.0;
}

// Função para validar longitude (-180 a 180)
int validarLongitude(double lon) {
    return lon >= -180.0 && lon <= 180.0;
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
    if (totalRelatos >= capacidadeRelatos) {
        int novaCapacidade = capacidadeRelatos == 0 ? 10 : capacidadeRelatos * 2;
        Relato *novoPtr = realloc(relatos, novaCapacidade * sizeof(Relato));
        if (!novoPtr) {
            printf("Erro de alocacao de memoria!\n");
            return;
        }
        relatos = novoPtr;
        capacidadeRelatos = novaCapacidade;
    }

    Relato r;

    printf("Nome: ");
    fgets(r.nome, sizeof(r.nome), stdin);
    r.nome[strcspn(r.nome, "\n")] = 0;

    printf("CPF (somente numeros): ");
    fgets(r.cpf, sizeof(r.cpf), stdin);
    r.cpf[strcspn(r.cpf, "\n")] = 0;

    if (!validarCPF(r.cpf)) {
        printf("CPF invalido! Deve conter exatamente 11 digitos numericos.\n");
        return;
    }

    printf("Descricao do relato: ");
    fgets(r.descricao, sizeof(r.descricao), stdin);
    r.descricao[strcspn(r.descricao, "\n")] = 0;

    printf("Latitude (-90 a 90): ");
    if (scanf("%lf", &r.latitude) != 1 || !validarLatitude(r.latitude)) {
        printf("Latitude invalida!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Longitude (-180 a 180): ");
    if (scanf("%lf", &r.longitude) != 1 || !validarLongitude(r.longitude)) {
        printf("Longitude invalida!\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    relatos[totalRelatos++] = r;

    printf("Relato cadastrado com sucesso!\n");
}

// Função para listar relatos em até 10 km de uma posição
void listarRelatosProximos() {
    double lat, lon;
    printf("Informe sua Latitude: ");
    if (scanf("%lf", &lat) != 1 || !validarLatitude(lat)) {
        printf("Latitude invalida!\n");
        while (getchar() != '\n');
        return;
    }
    printf("Informe sua Longitude: ");
    if (scanf("%lf", &lon) != 1 || !validarLongitude(lon)) {
        printf("Longitude invalida!\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    printf("\nRelatos em até 10 km:\n");
    int encontrados = 0;
    for (int i = 0; i < totalRelatos; i++) {
        double distancia = calcularDistancia(lat, lon, relatos[i].latitude, relatos[i].longitude);
        if (distancia <= 10.0) {
            printf("Relato %d:\n", i + 1);
            printf("Nome: %s\n", relatos[i].nome);
            printf("CPF: %s\n", relatos[i].cpf);
            printf("Descricao: %s\n", relatos[i].descricao);
            printf("Local: (%.6lf, %.6lf)\n", relatos[i].latitude, relatos[i].longitude);
            printf("Distancia: %.2lf km\n\n", distancia);
            encontrados++;
        }
    }
    if (!encontrados) {
        printf("Nenhum relato encontrado em até 10 km.\n");
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
    if (totalRelatos == 0) {
        printf("Nenhum relato para ordenar.\n");
        return;
    }
    qsort(relatos, totalRelatos, sizeof(Relato), compararRelatos);
    printf("Relatos ordenados por nome!\n");
}

// Função para buscar relato pelo CPF
void buscarRelatoPorCPF() {
    char cpf[15];
    printf("Informe o CPF para busca: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = 0;

    if (!validarCPF(cpf)) {
        printf("CPF invalido! Deve conter 11 digitos numericos.\n");
        return;
    }

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
        printf("Erro ao abrir arquivo para escrita!\n");
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

    Relato r;
    while (fscanf(f, "%49[^|]|%14[^|]|%199[^|]|%lf|%lf\n",
                  r.nome, r.cpf, r.descricao, &r.latitude, &r.longitude) == 5) {
        if (totalRelatos >= capacidadeRelatos) {
            int novaCapacidade = capacidadeRelatos == 0 ? 10 : capacidadeRelatos * 2;
            Relato *novoPtr = realloc(relatos, novaCapacidade * sizeof(Relato));
            if (!novoPtr) {
                printf("Erro de alocacao de memoria ao carregar!\n");
                fclose(f);
                return;
            }
            relatos = novoPtr;
            capacidadeRelatos = novaCapacidade;
        }
        relatos[totalRelatos++] = r;
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
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Por favor insira um numero de 1 a 6.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

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

    free(relatos);  // Libera memória alocada

    return 0;
}

