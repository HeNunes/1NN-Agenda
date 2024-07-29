// Trabalho 2 de ICC1 - Agenda inteligente
// Enzo Nunes Sedenho & Pedro Augusto Monteiro Delgado
// BCC - Junho de 2022.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_TAM_STRING 200 

// Declarando registros padrões do programa.
struct data_t
{
    int dia;
    int mes;
    int ano;
};
struct preferencias_t
{
    float rock;
    float funk;
    float mpb;
};
struct pessoa_t
{
    char nome[MAX_TAM_STRING];
    struct data_t data_c;
    char cidade[MAX_TAM_STRING];
    char uf[4];
    struct preferencias_t preferencias_c;
    int grupo;
};

void checar_vetor(struct pessoa_t *v)
{
    // Verificando se a memória foi alocada corretamente.
    if(v == NULL)
    {
        printf("Erro ao alocar memória.\n");
        free(v);
        exit(1);
    }
}
void checar_arquivo(FILE *arq, struct pessoa_t *v)
{
    // Verificando se o arquivo foi aberto com êxito.
    if(arq == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        if(v != NULL) free(v);
        exit(1);
    }
}
void printar_tabela(){
    // Printando cabeçalho da tabela
    printf("POS |       NOME       |    DATA    |      CIDADE      | UF | ROCK | FUNK | MPB  | GRUPO\n");
    printf("----+------------------+------------+------------------+----+------+------+------+-------\n");
}
void printar_pessoa(struct pessoa_t *v, int indice)
{
    // Imprimindo as informações de cada registro ativo.
    // Pertencer ao grupo -2 indica que o registro foi excluído.
    if(v[indice].grupo != -2)
    {
        printf(" %2d |", indice);
        printf(" %-16s |", v[indice].nome);
        printf(" %02d/%02d/%4d |", v[indice].data_c.dia, v[indice].data_c.mes, v[indice].data_c.ano);
        printf(" %-16s |", v[indice].cidade);
        printf(" %s |", v[indice].uf);
        printf(" %.2f | %.2f | %.2f |", v[indice].preferencias_c.rock, v[indice].preferencias_c.funk, v[indice].preferencias_c.mpb);
        if(v[indice].grupo == -1) printf(" -\n");
        else printf(" %d\n", v[indice].grupo);   
    }
}
struct pessoa_t *cadastrar_pessoa()
{
    // Alocando memória para um ponteiro de uma struct a ser cadastrada.
    struct pessoa_t *p = NULL;
    p = malloc (sizeof(struct pessoa_t));
    checar_vetor(p);

    // Solicitando as informações do registro a ser cadastrado.
    printf("Insira o nome: ");
    scanf("%*[\r\n]s");
    scanf("%[^\r\n]s", p->nome);
    printf("Insira o dia de nascimento: ");
    scanf("%d", &p->data_c.dia);
    printf("Insira o mes de nascimento: ");
    scanf("%d", &p->data_c.mes);
    printf("Insira o ano de nascimento: ");
    scanf("%d", &p->data_c.ano);
    printf("Insira a cidade: ");
    scanf("%*[\r\n]s");
    scanf("%[^\r\n]s", p->cidade);
    printf("Insira a UF: ");
    scanf("%s", p->uf);
    printf("Insira a preferência por rock: ");
    scanf("%f", &p->preferencias_c.rock);
    printf("Insira a preferência por funk: ");
    scanf("%f", &p->preferencias_c.funk);
    printf("Insira a preferência por mpb: ");
    scanf("%f", &p->preferencias_c.mpb);

    // Todo registro cadastrado pertence, inicialmente, ao grupo -1. 
    // Isso significa que ele ainda não foi agrupado de acordo com suas preferências.
    p->grupo = -1;

    return p;
}
int contar_pessoas(FILE *arq)
{
    int total = 0;
    struct pessoa_t *p = NULL;
    
    // Alocando memória para um ponteiro de struct e verificando se a memória foi alocada.
    p = malloc(sizeof(struct pessoa_t));
    checar_vetor(p);

    // Contando o total de structs escritas no arquivo.
    while(fread(p, sizeof(struct pessoa_t), 1, arq))
    {
        total++;
    }
    free(p);

    return total;
}
void editar_pessoa(struct pessoa_t *v, int indice)
{
    int op;
    do
    {
        // Imprimindo o menu padrão de edição de registros.
        printf("\nSelecione:\n[1] Para editar o nome\n[2] Para editar a data de nascimento\n");
        printf("[3] Para editar o local de nascimento\n[4] Para editar as preferências\n[0] Para finalizar\n");
        scanf("%d", &op);

        switch(op)
        {
        case 1: // Editando o nome.
            printf("Insira o novo nome: ");
            scanf("%*[\r\n]s");
            scanf("%[^\r\n]s", v[indice].nome);
            break;
        case 2: // Editando a data de nascimento.
            printf("Insira o novo dia de nascimento: ");
            scanf("%d", &v[indice].data_c.dia);
            printf("Insira o novo mes de nascimento: ");
            scanf("%d", &v[indice].data_c.mes);
            printf("Insira o novo ano de nascimento: ");
            scanf("%d", &v[indice].data_c.ano);
            break;
        case 3: // Editando o local de nascimento.
            printf("Insira a nova cidade: ");
            scanf("%*[\r\n]s");
            scanf("%[^\r\n]s", v[indice].cidade);
            printf("Insira a nova UF: ");
            scanf("%*[\r\n]s");
            scanf("%[^\r\n]s", v[indice].uf);
            break;
        case 4: // Editando as preferências.
            printf("Insira a nova preferência por rock: ");
            scanf("%f", &v[indice].preferencias_c.rock);
            printf("Insira a nova preferência por funk: ");
            scanf("%f", &v[indice].preferencias_c.funk);
            printf("Insira a nova preferência por mpb: ");
            scanf("%f", &v[indice].preferencias_c.mpb);
            break;
        case 0: // Finalizando edição.
            break;
        default:
            printf("Insira uma opção válida!\n");
            break;
        }
    } while(op != 0);
}
void buscar_pessoa(struct pessoa_t *v, int tam)
{
    int op = 0, n_dia, n_mes, n_ano, n_grupo;
    char aux[MAX_TAM_STRING];
    do
    {
        // Imprimindo menu padrão de busca de registros.
        printf("\nSelecione:\n[1] Para buscar por nome completo\n[2] Para buscar por parte do nome\n");
        printf("[3] Para buscar pela data de nascimento\n[4] Para buscar pelo grupo\n[0] Para finalizar\n");
        scanf("%d", &op);
        switch(op)
        {
        case 1: // Buscando pelo nome completo e imprimindo, caso exista.
            printf("Informe o nome completo da pessoa: ");
            scanf("%*[\r\n]s");
            scanf("%[^\r\n]s", aux);

            printf("\n> Os registros encontrados foram:\n\n");  
            printar_tabela();
            for(int i = 0; i < tam; i++)
            {
                if(strcmp(v[i].nome, aux) == 0) 
                    printar_pessoa(v, i);
            }
            break;
        case 2: // Buscando por uma parte do nome e imprimindo, caso exista.
            printf("Informe uma parte do nome da pessoa: ");
            scanf("%*[\r\n]s");
            scanf("%[^\r\n]s", aux);

            printf("\n> Os registros encontrados foram:\n\n");  
            printar_tabela();
            for(int i = 0; i < tam; i++)
            {
                if(strstr(v[i].nome, aux)) 
                    printar_pessoa(v, i);
            }
            break;
        case 3: // Buscando pela data de nascimento e imprimindo, caso exista.
            printf("\nInforme o dia de nascimento da pessoa: ");
            scanf("%d", &n_dia);
            printf("Informe o mes de nascimento da pessoa: ");
            scanf("%d", &n_mes);
            printf("Informe o ano de nascimento da pessoa: ");
            scanf("%d", &n_ano);

            printf("\n> Os registros encontrados foram:\n\n");  
            printar_tabela();
            for(int i = 0; i < tam; i++)
            {
                if((v[i].data_c.dia == n_dia) && (v[i].data_c.mes == n_mes) && (v[i].data_c.ano == n_ano)) 
                    printar_pessoa(v, i);
            }
            break;
        case 4: // Buscando pelo grupo e imprimindo, caso exista.
            printf("\nInforme o grupo: ");
            scanf("%d", &n_grupo);
            printf("\n> Os registros encontrados foram:\n\n");  
            printar_tabela();
            for(int i = 0; i < tam; i++)
            {
                if(v[i].grupo == n_grupo) 
                    printar_pessoa(v, i);
            }
            break;
        case 0: // Finalizando a busca.
            break;
        default:
            printf("Informe um opção válida!\n");
            break;
        }
    } while(op != 0);
}
void trocar(struct pessoa_t *v, int indice)
{
    struct pessoa_t temp;
    temp = v[indice];
    v[indice] = v[indice + 1];
    v[indice + 1] = temp;
}
void ordenar_pessoas(struct pessoa_t *v, int tam)
{
    int op;
    struct pessoa_t aux;
    printf("\nSelecione:\n[1] Para ordenar pelo nome\n[2] Para ordenar pela data de nascimento\n[3] Para ordenar pelo grupo\n");
    scanf("%d", &op);

    // Ordenando as structs por bubble sort.
    switch (op)
    {
    case 1: // Ordenando, em ordem alfabética, as structs pelo nome.
        for(int contador = tam; contador > 0; contador--)
        {
            for(int i = 0; i < contador - 1; i++)
            {
                if(strcmp(v[i].nome, v[i + 1].nome) > 0) 
                    trocar(v, i);
            }
        }
        break;
    case 2: // Ordenando, em ordem crescente, os registros pela data de nascimento.
        for(int contador = 0; contador < tam; contador++)
        {
            for(int i = 0; i < tam - 1; i++)
            {
                if(v[i].data_c.ano > v[i + 1].data_c.ano) trocar(v, i);

                else if((v[i].data_c.ano == v[i + 1].data_c.ano) && 
                        (v[i].data_c.mes > v[i + 1].data_c.mes)) 
                            trocar(v, i);

                else if((v[i].data_c.ano == v[i + 1].data_c.ano) && 
                        (v[i].data_c.mes == v[i + 1].data_c.mes) && 
                        (v[i].data_c.dia > v[i + 1].data_c.dia)) 
                            trocar(v, i);
            }
        }
        break;
    case 3: // Ordenando, em ordem crescente, as structs pelo grupo.
        for(int contador = 0; contador < tam; contador++)
        {   
            for(int i = 0; i < tam - 1; i++)
            {
                if(v[i].grupo > v[i + 1].grupo) 
                    trocar(v, i);
            }
        }
        break;
    default:
        printf("Insira um opção válida!\n");
        break;
    }
}
void agrupar_pessoas(struct pessoa_t *v, int tam)
{
    // Escolhemos, arbitrariamente, a distância limite entre registros de um mesmo grupo de 0.45.
    const float limite_distancia = 0.45;
    float dist = 0, menor_dist = 2;
    int n_grupos = 1, indice_mais_proximo;

    // A primeira struct do vetor é, obrigatoriamente, do grupo 1.
    v[0].grupo = 1;

    // Percorrendo o vetor de structs, a partir da segunda posição, e atribuindo a cada registro ativo o grupo -1 (sem grupo).
    // Pertencer ao grupo -2 indica que o registro foi excluído.
    for(int i = 1; i < tam; i++)
    {
        if(v[i].grupo != -2) 
            v[i].grupo = -1;
    }

    // Percorrendo o vetor de structs, a partir da segunda posição.
    for(int i = 1; i < tam; i++)
    {
        menor_dist = 2;

        // Verificando se o registro não foi excluído.
        if(v[i].grupo != -2)
        {
            // Calculando a distância euclidiana de cada registro ativo com os demais registros ativos.
            for(int j = 0; j < tam; j++)
            {
                dist = 0;
                if((v[j].grupo != -2) && (i != j))
                {
                    dist += pow((v[i].preferencias_c.rock - v[j].preferencias_c.rock), 2);
                    dist += pow((v[i].preferencias_c.funk - v[j].preferencias_c.funk), 2);
                    dist += pow((v[i].preferencias_c.mpb - v[j].preferencias_c.mpb), 2);
                    dist = sqrt(dist);
                }
                // Encontrando a menor distância e armazenando o índice do registro mais próximo.
                if((dist < menor_dist) && (i != j) && (v[j].grupo > -1))
                {
                    menor_dist = dist;
                    indice_mais_proximo = j;
                }
            }
            // Verificando se a menor distância encontrada está dentro do limite de distância estabelecido.
            // Caso a distância seja maior que o limite, um novo grupo é criado, tendo o registro analisado como primeiro membro.
            // Caso contrário, o registro analisado entra no mesmo grupo do registro com o qual apresenta a menor distância.
            if(menor_dist > limite_distancia)
            {
                n_grupos++;
                v[i].grupo = n_grupos;
            }
            else
                v[i].grupo = v[indice_mais_proximo].grupo;
        }   
    }
}
int main()
{
    int menu, n, n_pessoas = 0;
    struct pessoa_t *vetor_pessoas = NULL, *nova_pessoa = NULL, *aux = NULL;
    char arquivo_entrada[MAX_TAM_STRING], arquivo_saida[MAX_TAM_STRING];
    FILE *arq_entrada = NULL, *arq_saida = NULL;

    do
    {
        // Imprimindo o menu padrão principal do programa.
        printf("\nSelecione:\n[1] Para importar registros de um arquivo\n[2] Para cadastrar um novo registro\n");
        printf("[3] Para editar um registro\n[4] Para excluir um registro\n");
        printf("[5] Para buscar um registro\n[6] Para ordenar os registros\n");
        printf("[7] Para agrupar os registros\n[8] Para exportar os dados para um arquivo\n");
        printf("[9] Para imprimir todos os registros\n[0] Para sair\n");
    
        scanf("%d", &menu);

        switch(menu)
        {
        case 1: // Importando registros de um arquivo informado pelo usuário.
            printf("Insira o nome do arquivo de dados: ");
            scanf("%s", arquivo_entrada);
            
            // Abrindo arquivo de entrada e verificando se foi aberto.
            arq_entrada = fopen(arquivo_entrada, "rb");
            checar_arquivo(arq_entrada, vetor_pessoas);
            printf("> Arquivo aberto com sucesso!\n");

            n = contar_pessoas(arq_entrada);

            // Alocando memória para um vetor auxiliar de registros e verificando se foi alocada.
            aux = malloc (n * sizeof(struct pessoa_t));
            checar_vetor(aux);
            vetor_pessoas = realloc (vetor_pessoas,(n_pessoas + n) * sizeof(struct pessoa_t));
            checar_vetor(vetor_pessoas);
            
            // Lendo o arquivo e armazenando os registros em um vetor auxiliar.
            fseek(arq_entrada, 0, SEEK_SET);
            fread(aux, sizeof(struct pessoa_t), n, arq_entrada);

            // Atribuindo o vetor auxiliar ao vetor_pessoas.
            for(int i = 0; i < n; i++)
                vetor_pessoas[n_pessoas + i] = aux[i];
            n_pessoas += n;

            // Liberando memória alocada.
            free(aux);
            fclose(arq_entrada);
            break;

        case 2: // Cadastrando um novo registro.
            // Alocando memória para um registro e verificando se foi alocada.
            nova_pessoa = malloc (sizeof(struct pessoa_t));
            checar_vetor(nova_pessoa);

            // Preenchendo as informações do registro.
            nova_pessoa = cadastrar_pessoa();

            // Realocando memória para o vetor de registros, que receberá um novo registro, e verificando se a memória foi alocada.
            vetor_pessoas = realloc (vetor_pessoas, (n_pessoas + 1) * sizeof(struct pessoa_t));
            vetor_pessoas[n_pessoas] = *nova_pessoa;
            n_pessoas++;            
            printf("> Registro cadastrado com sucesso!\n");

            // Liberando memória alocada.
            free(nova_pessoa);
            break;

        case 3: // Editando um registro de acordo com sua posição no vetor, informada pelo usuário.
            printf("Insira a posição do registro: ");
            scanf("%d", &n);
            editar_pessoa(vetor_pessoas, n);            
            printf("> Registro editado com sucesso!\n");
            break;

        case 4: // Excluindo um registro de acordo com sua posição no vetor, informada pelo usuário.
            printf("Insira a posição do registro: ");
            scanf("%d", &n);

            // Para fins práticos, os usuários excluídos pertencem ao grupo -2.
            vetor_pessoas[n].grupo = -2;            
            printf("> Registro excluído com sucesso!\n");
            break;

        case 5: // Buscando um registro de acordo com dados informados pelo usuário.
            buscar_pessoa(vetor_pessoas, n_pessoas);
            break;

        case 6: // Ordenando registros de acordo com dados informados pelo usuário e imprimindo-os.
            ordenar_pessoas(vetor_pessoas, n_pessoas);            
            printf("> Registros ordenados com sucesso!\n");
            break;

        case 7: // Agrupando registros de acordo com suas preferências e imprimindo-os.
            agrupar_pessoas(vetor_pessoas, n_pessoas);            
            printf("> Registros agrupados com sucesso!\n");
            break;

        case 8: // Exportando os registros para um arquivo informado pelo usuário.
            printf("Insira o nome do arquivo: ");
            scanf("%s", arquivo_saida);

            // Abrindo arquivo de saída e verificando se foi aberto.
            arq_saida = fopen(arquivo_saida, "wb");
            checar_arquivo(arq_saida, vetor_pessoas);

            // Escrevendo os usuários ativos no arquivo.
            for(int i = 0; i < n_pessoas; i++)
                if(vetor_pessoas[i].grupo != -2) fwrite(&vetor_pessoas[i], sizeof(struct pessoa_t), 1, arq_saida);

            // Fechando o arquivo de saída.
            fclose(arq_saida);
            printf("> Arquivo salvo com sucesso!\n");

            break;
        case 9:
            printf("\n> Os registros cadastrados são:\n\n");            
            printar_tabela();
            for(int i = 0; i < n_pessoas; i++)
                printar_pessoa(vetor_pessoas, i);
    
            break;
        case 0: // Finalizando o programa.
            // Liberando memória alocada e fechando arquivo de entrada.
            if(vetor_pessoas != NULL) free(vetor_pessoas);            
            printf("> Programa encerrado com sucesso!\n");
            break;
        
        default:
            printf("> Insira um opção válida!\n");
            break;
        }
    } while(menu != 0);

    return 0;
}