#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define infinito 1000000

int **Grafo;
int numNodos;
int numArcos;
char **nombres;

int nodo1;
int nodo2;

bool *visitados;
int *costoDijkstra;
int *predecesorDijkstra;
int *camino;

/*
Entrada: nada.
Salida: nada.
Proceso: función que recibe la cantidad de nodos del grafo, y arma una matriz de 0.
*/
void crearMatrizGrafo()
{
    int i;

    Grafo= calloc(numNodos,sizeof(int *));

    for(i=0;i<numNodos;i++)
    {
        Grafo[i]= calloc(numNodos,sizeof(int));
    }
}

/*
Entrada: nada.
Salida: nada.
Proceso: función que recibe la cantidad de nodos del grafo, y arma una matriz de 0.
*/
void crearMatrizNombres()
{
    int i;

    nombres= calloc(numNodos, sizeof(char*));

    for(i=0;i<numNodos;i++)
    {
        nombres[i]= calloc(30, sizeof(char));
    }
}

/*
Entrada: nada.
Salida: nada.
Proceso: busca espacio en la ram para cada puntero necesario para la función Dijkstra.
*/
void creaArreglosParaDijkstra()
{
    visitados= calloc(numNodos,sizeof(bool));
    costoDijkstra= calloc(numNodos,sizeof(int));
    predecesorDijkstra= calloc(numNodos,sizeof(int));
    camino= calloc(numNodos,sizeof(int));
}
/*
Entrada: recibe un nombre.
Salida: la fila en que se encuentra el nombre en la matriz "nombres".
Proceso: revisa la matriz "nombres", comparando el nombre ingresado con los almacenados en esta.
*/
int posicionDeNombre(char nombrePersonas[30])
{
    int i;

    for(i=0;i<numNodos;i++)
    {
        if(strcmp(nombres[i],nombrePersonas)==0)
        {
            return i;
        }
    }
    printf("El nombre no existe o esta mal escrito (distinguir las mayusculas).");
    exit(-1);
}

/*
Entrada: nombre del archivo.
Salida: nada.
Proceso: lee los datos del archivo; nodos, arcos, nombres y relaciones. Estos los guarda en variables y rellena la matriz Grafo con las relaciones.
*/
void leerGrafoArchivo(char nombreArchivo[50])
{
    FILE *archivo;
    int i, j;
    char linea[150];
    char *persona1, *persona2;

    archivo= fopen(nombreArchivo, "r");

    if(archivo== NULL)
    {
        printf("No existe este archivo. Se cerrara el programa.");
        exit(-1);
    }

    fscanf(archivo,"%i",&numNodos);
    crearMatrizGrafo();
    crearMatrizNombres();
    fscanf(archivo,"%i\n",&numArcos);

    for(i=0;i<numNodos;i++)
    {
        fgets(linea,150,archivo);
        strtok(linea,"\n");
        strcpy(nombres[i],linea);
    }

    for(j=0;j<numArcos;j++)
    {
       fgets(linea,100,archivo);
       strtok(linea,"\n");
       persona1= strtok(linea,",");
       persona2= strtok(NULL,",");
       nodo1= posicionDeNombre(persona1);
       nodo2= posicionDeNombre(persona2);
       Grafo[nodo1][nodo2]= 1;
       Grafo[nodo2][nodo1]= 1;
    }
    fclose(archivo);
}

/*
Entrada: nada.
Salida: cantidad de nodos sin visitar.
Proceso: pasa por cada nodo para saber si fue visitado.
*/
int nodosSinVisitar()
{
    int i, cont;

    cont = 0;
    for (i=0; i<numNodos; i++)
    {
        if (visitados[i] == false){
            cont++;
        }
    }
   return cont;
}

/*
Entrada: nada.
Salida: el nodo de menor costo.
Proceso: de todos los nodos sin marcar elige el de menor costo.
*/
int nodoCostoMinimoSinVisitar()
{
    int i, nodo, minimo;
    bool esElPrimero;

    esElPrimero = true;
    for (i=0; i<numNodos; i++)
    {
        if (visitados[i] == false)
        {
            if (esElPrimero)
            {
                minimo = costoDijkstra[i];
                nodo = i;
                esElPrimero = false;
            }
            else
            {
                if (costoDijkstra[i] < minimo)
                {
                    minimo = costoDijkstra[i];
                    nodo = i;
                }
            }
        }
    }
    return nodo;
}

/*
Entrada: matriz Grafo, número de nodos y el nombre por el que se parte el recorrido.
Salida: nada.
Proceso: utiliza el algoritmo de dijkstra para analizar los posibles caminos para llegar a cada nombre.
*/
void Dijkstra(int **Grafo,int numNodos,int v0)
{
    int i, j, w;
    w= v0;

    visitados[v0]= true;

    for(i=0;i<numNodos;i++)
    {
        if(Grafo[v0][i]==0)
        {
            costoDijkstra[i]= infinito;
        }
        else
        {
            costoDijkstra[i]= Grafo[v0][i];
        }
        predecesorDijkstra[i]= v0;
    }

    while(nodosSinVisitar()>1)
    {
        w= nodoCostoMinimoSinVisitar();
        visitados[w]= true;

        for(j=0;j<numNodos;j++)
        {
            if(Grafo[w][j]!= 0 && visitados[j] == false)
            {
                if(costoDijkstra[w] + Grafo[w][j] < costoDijkstra[j])
                {
                    predecesorDijkstra[j]= w;
                    costoDijkstra[j] = costoDijkstra[w] + Grafo[w][j];
                }
            }
        }
    }
}

/*
Entrada: recibe el nodo inicial por el que partir.
Salida: nada.
Proceso: retorna el recorrido de personas necesarias para llegar a la persona indicada.
*/
void ImprimeCaminos(int v0)
{
    int i, j, x, nodo, nodoNombre;
    char usuario2[30];

    printf("A quien deseas conocer?: ");
    fgets(usuario2,30,stdin);
    strtok(usuario2,"\n");
    x= posicionDeNombre(usuario2);

    if(numArcos== 0)
    {
        printf("\nNinguna persona tiene amigos en esta red social. No se creara el archivo .aec\n");
        exit(-1);
    }
    else
    {
        if(Grafo[v0][x]!= 0)
        {
            printf("\nUstedes ya son amigos.\n");
            exit(-1);
        }
        if(v0== x)
        {
            printf("\nConsigue amigos...\n");
            exit(-1);
        }

        if(costoDijkstra[x]== infinito)
        {
            printf("\nNo puedes conocer a %s.\n" ,nombres[x]);
            exit(-1);
        }

        if (x != v0)
        {
            camino[0] = x;
            nodo = predecesorDijkstra[x];

            for (i=1; nodo!=v0; i++)
            {
                camino[i] = nodo;
                nodo = predecesorDijkstra[nodo];
            }
            camino[i] = nodo;

            printf("\nLa secuencia de nombres es: \n");

            for (; i>=0; i--)
            {
                printf("%s\n", nombres[camino[i]]);
            }
        }
    }
}

/*
Entrada: dos nombres.
Salida: cantidad de amigos en común.
Proceso: cuenta los amigos en común entre dos personas.
*/
int numAmigosEnComun(int nodo1, int nodo2)
{
    int cont;
    int i;

    cont = 0;
    for (i=0; i<numNodos; i++)
    {
        if (Grafo[nodo1][i] != 0 && Grafo[nodo2][i] != 0)
        {
            cont++;
        }
    }
    return cont;
}

/*
Entrada: nombre del archivo, matriz Grafo, matriz nombres y la cantidad de nodos.
Salida: nada.
Proceso: genera un archivo .aec que almacene las relaciones de amistad entre todas las las personas.
*/
void generarArchivo(char nombreArchivo[50],int **Grafo,char **nombres,int numNodos)
{
    FILE *nuevoArchivo;

    char nombreSalidaArchivo[50];
    int i, j;

    strcpy(nombreSalidaArchivo, nombreArchivo);
    strtok(nombreSalidaArchivo, ".");
    strcat(nombreSalidaArchivo, ".aec");

    nuevoArchivo= fopen(nombreSalidaArchivo,"w");

    if(nuevoArchivo== NULL)
    {
        printf("No se puede abrir el archivo.");
        printf("\nEste programa se cerrara.");
        exit(-1);
    }

    for(i=0;i<numNodos;i++)
    {
        for(j=i;j<numNodos;j++)
        {
            if(Grafo[i][j]!= 0)
            {
                fprintf(nuevoArchivo,"%s,%s: %i\n" ,nombres[i], nombres[j], numAmigosEnComun(i, j));
            }
        }
    }

    fclose(nuevoArchivo);
}

/*
Entrada: nada.
Salida: nada.
Proceso: acciona todas las funciones necesarias para el proceso de búsqueda entre personas.
*/
void iniciaProceso()
{
    char nombreArchivo[50];
    int v0;
    int x;
    char usuario1[30];

    printf("Ingrese nombre del archivo: ");
    fgets(nombreArchivo,50,stdin);
    strtok(nombreArchivo,"\n");

    leerGrafoArchivo(nombreArchivo);
    creaArreglosParaDijkstra();

    printf("\nIngresa tu nombre: ");
    fgets(usuario1,30,stdin);
    strtok(usuario1,"\n");
    v0= posicionDeNombre(usuario1);

    Dijkstra(Grafo,numNodos,v0);
    //generarArchivo(nombreArchivo,Grafo,nombres,numNodos);
    ImprimeCaminos(v0);
    generarArchivo(nombreArchivo,Grafo,nombres,numNodos);
}

int main()
{
    iniciaProceso();
    return 0;
}
