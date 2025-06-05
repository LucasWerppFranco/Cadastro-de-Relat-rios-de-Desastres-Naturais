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

// Function prototype
void carregarRelatos();

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

// Função para aguardar o usuário pressionar Enter para continuar
void aguardarEnter() {
    printf("Pressione Enter para continuar...");
    while (getchar() != '\n');
}

// Função para mostrar um cabeçalho claro e visualmente agradável
void mostrarCabecalho(const char *titulo) {
    printf("\n========================================\n");
    printf("  %s\n", titulo);
    printf("========================================\n");
}

// Função para ler CPF garantindo que somente números serão aceitos (11 dígitos)
int lerCPF(char *cpfBuffer, size_t size) {
    printf("CPF (somente numeros): ");
    if (fgets(cpfBuffer, size, stdin) == NULL) return 0;
    cpfBuffer[strcspn(cpfBuffer, "\n")] = 0; // Remove newline

    if (strlen(cpfBuffer) != 11) {
        printf("CPF deve conter exatamente 11 digitos.\n");
        return 0;
    }

    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpfBuffer[i])) {
            printf("CPF deve conter apenas digitos numericos.\n");
            return 0;
        }
    }
    return 1;
}

// Função para cadastrar um relato
void cadastrarRelato() {
    mostrarCabecalho("Cadastro de Relato de Catastrofe");
    if (totalRelatos >= capacidadeRelatos) {
        int novaCapacidade = capacidadeRelatos == 0 ? 10 : capacidadeRelatos * 2;
        Relato *novoPtr = realloc(relatos, novaCapacidade * sizeof(Relato));
        if (!novoPtr) {
            printf("Erro de alocacao de memoria!\n");
            aguardarEnter();
            return;
        }
        relatos = novoPtr;
        capacidadeRelatos = novaCapacidade;
    }

    Relato r;

    printf("Nome: ");
    fgets(r.nome, sizeof(r.nome), stdin);
    r.nome[strcspn(r.nome, "\n")] = 0;
    if(strlen(r.nome) == 0){
        printf("Nome nao pode ser vazio.\n");
        aguardarEnter();
        return;
    }

    if (!lerCPF(r.cpf, sizeof(r.cpf))) {
        aguardarEnter();
        return;
    }

    printf("Descricao do relato: ");
    fgets(r.descricao, sizeof(r.descricao), stdin);
    r.descricao[strcspn(r.descricao, "\n")] = 0;
    if(strlen(r.descricao) == 0){
        printf("Descricao nao pode ser vazia.\n");
        aguardarEnter();
        return;
    }

    printf("Latitude (-90 a 90): ");
    if (scanf("%lf", &r.latitude) != 1 || !validarLatitude(r.latitude)) {
        printf("Latitude invalida!\n");
        while (getchar() != '\n');
        aguardarEnter();
        return;
    }

    printf("Longitude (-180 a 180): ");
    if (scanf("%lf", &r.longitude) != 1 || !validarLongitude(r.longitude)) {
        printf("Longitude invalida!\n");
        while (getchar() != '\n');
        aguardarEnter();
        return;
    }

    while (getchar() != '\n');

    relatos[totalRelatos++] = r;

    printf("Relato cadastrado com sucesso!\n");
    aguardarEnter();
}

// Função para listar relatos em até 10 km de uma posição
void listarRelatosProximos() {
    mostrarCabecalho("Listar Relatos Proximos (ate 10 km)");

    double lat, lon;
    printf("Informe sua Latitude: ");
    if (scanf("%lf", &lat) != 1 || !validarLatitude(lat)) {
        printf("Latitude invalida!\n");
        while (getchar() != '\n');
        aguardarEnter();
        return;
    }
    printf("Informe sua Longitude: ");
    if (scanf("%lf", &lon) != 1 || !validarLongitude(lon)) {
        printf("Longitude invalida!\n");
        while (getchar() != '\n');
        aguardarEnter();
        return;
    }

    while (getchar() != '\n');

    printf("\nRelatos em ate 10 km:\n");
    int encontrados = 0;
    for (int i = 0; i < totalRelatos; i++) {
        double distancia = calcularDistancia(lat, lon, relatos[i].latitude, relatos[i].longitude);
        if (distancia <= 10.0) {
            printf("----------------------------------------\n");
            printf("Relato %d:\n", i + 1);
            printf("Nome: %s\n", relatos[i].nome);
            printf("CPF: %s\n", relatos[i].cpf);
            printf("Descricao: %s\n", relatos[i].descricao);
            printf("Local: (%.6lf, %.6lf)\n", relatos[i].latitude, relatos[i].longitude);
            printf("Distancia: %.2lf km\n", distancia);
            printf("----------------------------------------\n\n");
            encontrados++;
        }
    }
    if (!encontrados) {
        printf("Nenhum relato encontrado em ate 10 km.\n");
    }
    aguardarEnter();
}

// Função de comparação para ordenar relatos por nome
int compararRelatos(const void *a, const void *b) {
    Relato *r1 = (Relato *)a;
    Relato *r2 = (Relato *)b;
    return strcmp(r1->nome, r2->nome);
}

// Função para ordenar relatos por nome
void ordenarRelatos() {
    mostrarCabecalho("Ordenar Relatos por Nome");

    if (totalRelatos == 0) {
        printf("Nenhum relato para ordenar.\n");
        aguardarEnter();
        return;
    }
    qsort(relatos, totalRelatos, sizeof(Relato), compararRelatos);
    printf("Relatos ordenados por nome!\n");
    aguardarEnter();
}

// Função para buscar relato pelo CPF
void buscarRelatoPorCPF() {
    mostrarCabecalho("Buscar Relato por CPF");

    char cpf[15];
    if (!lerCPF(cpf, sizeof(cpf))) {
        aguardarEnter();
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < totalRelatos; i++) {
        if (strcmp(relatos[i].cpf, cpf) == 0) {
            printf("\nRelato encontrado:\n");
            printf("Nome: %s\n", relatos[i].nome);
            printf("Descricao: %s\n", relatos[i].descricao);
            printf("Local: (%.6lf, %.6lf)\n", relatos[i].latitude, relatos[i].longitude);
            encontrado = 1;
            break;
        }
    }
    if(!encontrado) {
        printf("Relato nao encontrado para o CPF informado.\n");
    }
    aguardarEnter();
}

// Função para salvar relatos em arquivo
void salvarRelatos() {
    mostrarCabecalho("Salvar Relatos em Arquivo");

    FILE *f = fopen("relatos.txt", "w");
    if (!f) {
        printf("Erro ao abrir arquivo para escrita!\n");
        aguardarEnter();
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
    printf("Relatos salvos em 'relatos.txt' com sucesso!\n");
    aguardarEnter();
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
    printf("\n========================================\n");
    printf(" Sistema de Cadastro de Relatos de Catastrofes Naturais\n");
    printf("========================================\n");
    printf("1. Cadastrar relato\n");
    printf("2. Listar relatos proximos (ate 10 km)\n");
    printf("3. Ordenar relatos por nome\n");
    printf("4. Buscar relato por CPF\n");
    printf("5. Salvar relatos em arquivo\n");
    printf("6. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opcao (1-6): ");
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
            case 6: salvarRelatos(); printf("Saindo... Obrigado por usar o sistema.\n"); break;
            default: printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 6);

    free(relatos);  // Libera memória alocada

    return 0;
}


