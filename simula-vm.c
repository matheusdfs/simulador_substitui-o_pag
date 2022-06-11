#include <stdio.h>
#include <stdlib.h>

#define NUM_REF_MAX 30

#define FALSE 0
#define TRUE 1

int n_ref = 0;

void print_ram(int tam_ram, int* ram)
{
    for(int i = 0; i < tam_ram; i++)
    {
        printf("%d ", ram[i]);
    }
}

int FIFO(int ram_size, int* ref)
{
    int page_fault = 0;

    //initializing the ram array
    int* ram = malloc(sizeof(int)*ram_size);

    for(int i = 0; i < ram_size; i++)
    {
        ram[i] = -1; //-1 means empty memory
    }

    int count_next = 0;

    for(int i = 0; i < n_ref; i++)
    {
        int isAlreadyInMemory = FALSE;
        
        for(int j = 0; j < ram_size; j++)
        {
            if(ram[j] == ref[i])
            {
                isAlreadyInMemory = TRUE;
            }
        }
        
        if(!isAlreadyInMemory)
        {
            if(ram[count_next%ram_size] != -1)
            {
                page_fault++;
            }

            ram[count_next%ram_size] = ref[i];
            count_next++;
        }
    }

    return page_fault;
}

int next_LRU_position(int* ram, int* aux, int ram_size)
{
    int next_position = 0;
    int aux_value_next = aux[0];
    
    for(int i = 0; i < ram_size; i++)
    {
        if(ram[i] == -1)
        {
            return i;
        }

        if(aux_value_next > aux[i])
        {
            next_position = i;
            aux_value_next = aux[i];
        }
    }

    return next_position;
}

int LRU(int ram_size, int* ref)
{
    int page_fault = 0;

    //initializing the ram array
    int* ram = malloc(sizeof(int)*ram_size);

    int* aux = malloc(sizeof(int)*ram_size);

    for(int i = 0; i < ram_size; i++)
    {
        ram[i] = -1; //-1 means empty memory
        aux[i] = 0;
    }

    for(int i = 0; i < n_ref; i++)
    {   
        int isAlreadyInMemory = FALSE;
        
        for(int j = 0; j < ram_size; j++)
        {
            if(ram[j] == ref[i])
            {
                isAlreadyInMemory = TRUE;
            }
        }

        if(!isAlreadyInMemory)
        {
            int next_pos = next_LRU_position(ram, aux, ram_size);

            if(ram[next_pos] != -1)
                page_fault++;

            ram[next_pos] = ref[i];
            aux[next_pos] = i;
        }
    }

    return page_fault;
}

int OPT(int ram_size, int* ref)
{
    int page_fault = 0;

    //initializing the ram array
    int* ram = malloc(sizeof(int)*ram_size);

    for(int i = 0; i < ram_size; i++)
    {
        ram[i] = -1; //-1 means empty memory
    }

    for(int i = 0; i < n_ref; i++)
    {   
        int isAlreadyInMemory = FALSE;
        
        for(int j = 0; j < ram_size; j++)
        {
            if(ram[j] == ref[i])
            {
                isAlreadyInMemory = TRUE;
            }
        }

        if(!isAlreadyInMemory)
        {
            int next_pos = next_LRU_position(ram, ref,ram_size);

            if(ram[next_pos] != -1)
                page_fault++;

            ram[next_pos] = ref[i];
            aux[next_pos] = i;
        }
    }

    return page_fault;
}

int* get_ref_pag()
{
    //opening the document to count the lines
    FILE* file = fopen("referencias.txt", "r");
    char line[2];

    int n = 1;

    while (fgets(line, sizeof(line), file) && n < NUM_REF_MAX)
        if(*line == '\n')
            n++;

    fclose(file);

    //allocate array with size
    int *ref = malloc(sizeof(int)*n);
    n_ref = n;

    //running the document to get the info
    file = fopen("referencias.txt", "r");

    int i = 0;

    while (fgets(line, sizeof(line), file))
        if(*line != '\n')
        {
            ref[i] = *line - '0'; 
            i++;
        }

    fclose(file);

    return ref;
}

int main(int argc, char *argv[])
{
    if(argv[1] == NULL)
        puts("Error: precisa inserir o número de quadros disponíveis na RAM");

    int tam_ram = (int)*argv[1] - '0';

    int *ref = get_ref_pag();

    printf ("%5d quadros, %7d refs: FIFO: %5d PFs, LRU: %5d PFs, OPT: %5d PFs\n", tam_ram, n_ref, FIFO(tam_ram, ref), LRU(tam_ram, ref), 0) ;

    return 0;
}

