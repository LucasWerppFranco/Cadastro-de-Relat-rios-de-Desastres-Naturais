# Cadastro-de-Relat-rios-de-Desastres-Naturais

## Descrição
Este sistema em C permite cadastrar, listar, buscar e ordenar relatos de catástrofes naturais com geolocalização (latitude e longitude). Os relatos são persistidos em arquivo e podem ser consultados por proximidade geográfica (até 10 km).

---

## Funcionalidades
- Cadastro de relatos com:
  - Nome
  - CPF (11 dígitos numéricos)
  - Descrição
  - Latitude (-90 a 90)
  - Longitude (-180 a 180)

- Listagem de relatos próximos (até 10 km).
- Ordenação de relatos por nome.
- Busca de relatos por CPF.
- Persistência em arquivo `relatos.txt` (salvar e carregar).
- Validação de entradas e tratamento de erros.

---

## Como usar

### 1. Compilar
```
gcc main.c -O -lm
```

### Compilando pelo Docker
```
docker build -t relatos-app .
docker run -it relatos-app
```
