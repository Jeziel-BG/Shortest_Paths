/*
Author: Jeziel Banos Gonzalez
Course: CSC 352
Description: Program reads a file and creates a undirected graph based on the
values read on each line. Once the graph is created users are able to type the
name of two cities. The program will then print out the minDistance between the
two entered cities.


*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

// the paths / edges connecting the nodes on the graph
struct edge
{
    struct node *to;
    struct edge *next;
    int weight;
};

// Nodes of the graph
struct node
{
    struct edge *edges;
    struct edge *edgeTail;
    struct node *next;
    char name[65];
    bool visited;
    int minDist;
};

struct node *start = NULL;
struct node *tail = NULL;

bool checkAlpha(char *character)
{
    /*checkAlpha(char*)- requires a string, looks through each character to
    verify all chars are letters. returns true if this is the case, false
    otherwise
    */
    for (int i = 0; i < strlen(character); i++)
    {
        if (isalpha(character[i]) == false)
        {
            return false;
        }
    }
    return true;
}

void freeHelper(struct edge *head)
{
    /*
    freeHelper(edge) - helper of the freeAll function, this function frees the edge linked
    list of the node that called freeHelper. No returns, justs free memory
    */
    struct edge *state1, *state2;
    state1 = head;
    while (state1 != NULL)
    {
        state2 = state1->next;
        free(state1);
        state1 = state2;
    }
}

void freeAll()
{
    /*
    freeAll() - frees all the nodes structs, no mems or args. Does use a helper
    to free the edges of a node.
    */
    struct node *state1, *state2;
    state1 = start;
    while (state1 != NULL)
    {
        state2 = state1->next;
        // free the edges
        freeHelper(state1->edges);
        free(state1);
        state1 = state2;
    }
}

void printGraph()
{
    /*
    printGraph() - no params or returns. This function is for testing only, just
    prints out all the cities and the edges connected to the cities
    */
    struct node *cur = start;
    while (cur != NULL)
    {
        printf("%s edges: \n", cur->name);
        struct edge *curEdge = cur->edges;
        while (curEdge != NULL)
        {
            printf("%s ", curEdge->to->name);
            curEdge = curEdge->next;
        }
        cur = cur->next;
        printf("\n");
    }
}

struct node *findNode(char *vector1)
{
    /*
    findNode(char) - param is a string, function looks through the linked list
    of cities and checks if a city's name matches the param, if a match occurs
    then a pointer to the city is returned.
    */
    struct node *cur = start;
    while (strcmp(cur->name, vector1) != 0)
    {
        cur = cur->next;
        if (cur == NULL)
        {
            break;
        }
    }

    return cur;
}

void createEdge(struct node *vectorA, struct node *vectorB, int weight)
{
    /*
    createEdge(node, node, int) - function creates an edge from vectorA to B
    and vectorB to A, then adds them to the edge list of both cities. no returns
    */
    // edge that will be added to vectorA
    struct edge *tempA = malloc(sizeof(struct edge));
    tempA->next = NULL;
    tempA->to = vectorB;
    tempA->weight = weight;
    bool addedA = false;

    // edge that will be added to vectorB
    struct edge *tempB = malloc(sizeof(struct edge));
    tempB->next = NULL;
    tempB->to = vectorA;
    tempB->weight = weight;
    bool addedB = false;

    // no edges yet
    if (vectorA->edges == NULL)
    {
        vectorA->edges = tempA;
        vectorA->edgeTail = tempA;
        addedA = true;
    }
    if (vectorB->edges == NULL)
    {
        vectorB->edges = tempB;
        vectorB->edgeTail = tempB;
        addedB = true;
    }

    // adding if there already exists edges
    if (!addedA)
    {
        vectorA->edgeTail->next = tempA;
        vectorA->edgeTail = vectorA->edgeTail->next;
    }

    if (!addedB)
    {
        vectorB->edgeTail->next = tempB;
        vectorB->edgeTail = vectorB->edgeTail->next;
    }
}

void createGraph(char *vector1, char *vector2, int weight)
{
    /*
    createGraph(char*, char*, int )- function is used to create a graph, two
    city names are passed and two node structs are created with their name,
    then an edge between the two nodes is created. No returns
    */
    // creating the linked list of nodes in case none exist
    if (start == NULL)
    {
        start = malloc(sizeof(struct node));
        start->edges = NULL;
        strcpy(start->name, vector1);
        start->visited = false;
        start->edgeTail = NULL;
        start->minDist = INT_MAX;

        struct node *temp = malloc(sizeof(struct node));
        temp->edges = NULL;
        temp->next = NULL;
        temp->visited = false;
        strcpy(temp->name, vector2);
        temp->edgeTail = NULL;
        temp->minDist = INT_MAX;

        start->next = temp;
        tail = temp;

        createEdge(start, temp, weight);
    }
    else
    {
        // will need to find the two
        struct node *vectorA = findNode(vector1);
        struct node *vectorB = findNode(vector2);
        // node A or B does not exist
        if (vectorA == NULL)
        {
            struct node *temp = malloc(sizeof(struct node));
            temp->edges = NULL;
            temp->edgeTail = NULL;
            temp->next = NULL;
            temp->visited = false;
            temp->minDist = INT_MAX;
            strcpy(temp->name, vector1);
            // updating tail of node linked list
            tail->next = temp;
            tail = tail->next;
            vectorA = temp;
        }
        if (vectorB == NULL)
        {
            struct node *temp2 = malloc(sizeof(struct node));
            temp2->edges = NULL;
            temp2->edgeTail = NULL;
            temp2->next = NULL;
            temp2->visited = false;
            temp2->minDist = INT_MAX;
            strcpy(temp2->name, vector2);
            // updating tail of node linked list
            tail->next = temp2;
            tail = tail->next;
            vectorB = temp2;
        }

        createEdge(vectorA, vectorB, weight);
    }
}

void resetMinDist()
{
    /*
    restMinDist()- function is used to set up the initialize step of the search
    alg. just loops through the cities and resets the minDist and visited vals.
    No returns.
    */
    struct node *cur = start;
    while (cur != NULL)
    {
        cur->minDist = INT_MAX;
        cur->visited = false;
        cur = cur->next;
    }
}

bool exists(char *vector)
{
    /*
    exists(char)- a string is passed and the function checks if there exists
    a city whose name matches the passed string. returns a bool
    */
    struct node *cur = start;
    while (cur != NULL)
    {
        if (strcmp(vector, cur->name) == 0)
        {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

bool fullyVisited()
{
    /*
    fullyVisited() - no params, returns true if all cities have a field val of
    true in visited, false otherwise
    */
    struct node *cur = start;
    while (cur != NULL)
    {
        if (cur->visited == false)
        {
            return false;
        }
        cur = cur->next;
    }
    return true;
}

struct node *smallestDist()
{
    /*
    smallestDist()- function loops through all the cities and returns a
    pointer to the city with the smallest minDist field val.
    */
    struct node *toReturn = start;
    struct node *cur = start;
    int min = INT_MAX;
    while (cur != NULL)
    {
        if (cur->visited == false && cur->minDist < min)
        {
            min = cur->minDist;
            toReturn = cur;
        }
        cur = cur->next;
    }

    return toReturn;
}

int pathFinder(char *vector1, char *vector2)
{
    /*
    pathFinder(char, char) - function uses the alg described in the spec. function
    will calc the smallest dist possible between the cities that have a name field
    val equal to strings vector1 and vector2

    */
    struct node *sourceNode = findNode(vector1);
    // initialize step
    resetMinDist();
    // same city, you are already there
    if (strcmp(vector1, vector2) == 0)
    {
        return 0;
    }

    sourceNode->minDist = 0;
    while (!fullyVisited())
    {
        struct node *cur = smallestDist();
        cur->visited = true;
        struct edge *curEdge = cur->edges;
        while (curEdge != NULL)
        {
            int n = cur->minDist + curEdge->weight;
            if (n < curEdge->to->minDist)
            {
                curEdge->to->minDist = n;
            }

            curEdge = curEdge->next;
        }
    }

    return findNode(vector2)->minDist;
}

int main(int argc, char **argv)
{
    FILE *fileName;
    int errorStatus = 0;
    fileName = fopen(argv[1], "r");

    if (fileName == NULL)
    {
        fprintf(stderr, "There was an error opening the file.\n");
        exit(1);
    }

    // read the file
    char *line = NULL;
    size_t size = 0;

    while (getline(&line, &size, fileName) != EOF)
    {

        char vector1[65] = {0};
        char vector2[65] = {0};
        int weight = INT_MIN;
        char overflow[1] = {0};

        sscanf(line, "%64s %64s %d %s", vector1, vector2, &weight, overflow);
        if (vector1[0] == '\0' || vector2[0] == '\0' || weight == INT_MIN || overflow[0] != '\0')
        {
            fprintf(stderr, "Illegal edge detected.\n");
            errorStatus = 1;
        }
        else if (!checkAlpha(vector1) || !checkAlpha(vector2))
        {
            fprintf(stderr, "Illegal node name detected.\n");
            errorStatus = 1;
        }
        else
        {
            createGraph(vector1, vector2, weight);
        }
    }
    fclose(fileName);

    FILE *inputSource = stdin;
    // read from standard in
    while (getline(&line, &size, inputSource) != EOF)
    {
        char vectorA[65] = {0};
        char vectorB[65] = {0};
        char overflow[1] = {0};

        sscanf(line, "%64s %64s %s", vectorA, vectorB, overflow);
        if (overflow[0] != '\0')
        {
            fprintf(stderr, "Too many input were entered, please try again.\n");
            errorStatus = 1;
        }
        else if (vectorA[0] == '\0' || vectorB[0] == '\0')
        {
            fprintf(stderr, "Not enough inputs were entered.\n");
            errorStatus = 1;
        }

        else if (!(exists(vectorA) && exists(vectorB)))
        {
            fprintf(stderr, "One of the entered vectors does not exists, try again.\n");
            errorStatus = 1;
        }
        else
        {

            int shortestPath = pathFinder(vectorA, vectorB);
            printf("%d\n", shortestPath);
        }
    }
    freeAll();
    free(line);

    return errorStatus;
}