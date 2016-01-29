#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<stdbool.h>
#include<omp.h>
struct pathNode
{
    int nodeNumber;
    struct pathNode *next;
};
struct path
{
    struct pathNode *head;
    int lastNode;
};


struct tour
{   
    int cityCount; 
    struct path *thePath;
    int totalCost;
};
struct AdjListNode
{
    int dest;
    int cost;
    struct AdjListNode* next;
};
 
// A structure to represent an adjacency liat
struct AdjList
{
    struct AdjListNode *head;  // pointer to head node of list
};
 
// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    struct AdjList* array;
};
typedef struct Stack
{
        int capacity;
        int size;
        struct tour* elements;
}Stack;
int minCost=INT_MAX;
//struct tour* bestTour=(struct tour*) malloc(sizeof(struct tour));
/* crateStack function takes argument the maximum number of elements the stack can hold, creates
   a stack according to it and returns a pointer to the stack. */
Stack * createStack(int maxElements)
{
        /* Create a Stack */
        Stack *S;
        S = (Stack *)malloc(sizeof(Stack));
        /* Initialise its properties */
        S->elements = (struct tour*) malloc(sizeof(struct tour)*maxElements);
        S->size = 0;
        S->capacity = maxElements;
        /* Return the pointer */
        return S;
}
struct tour top(Stack *S)
{
        if(S->size==0)
        {
                printf("Stack is Empty\n");
                exit(0);
        }
        /* Return the topmost element */
        return S->elements[S->size-1];
}
struct tour pop(Stack *S)
{       struct tour temp;
        /* If stack size is zero then it is empty. So we cannot pop */
        if(S->size==0)
        {
                printf("Stack is Empty\n");
                return;
        }
        /* Removing an element is equivalent to reducing its size by one */
        else
        {
                temp=top(S);
                S->size--;
        }
        return temp;
}


void push(Stack *S,struct tour element)
{
        /* If the stack is full, we cannot push an element into it as there is no space for it.*/
        if(S->size == S->capacity)
        {
                printf("Stack is Full\n");
        }
        else
        {
                /* Push an element on the top of it and increase its size by one*/ 
                //printf("%d\n",S->size);
                S->elements[S->size]=element;
                //printf("%d\n",S->size);
                S->size++;
        }
        return;
}

bool isEmpty(Stack *s)
{
    if(s->size==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}



 
// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest,int cost)
{
    struct AdjListNode* newNode =(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    newNode->cost=cost;
    return newNode;
}
 
// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));
 
     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 
// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest,int cost,int *costMatrix,int V)
{
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src.  The node is added at the begining
    struct AdjListNode* newNode = newAdjListNode(dest,cost);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
 
    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src,cost);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
    *(costMatrix+V*src+dest)=cost;
    *(costMatrix+V*dest+src)=cost;
}



void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d %d", pCrawl->dest,pCrawl->cost);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}


void addCity(struct tour *t,int nodeNumber,int V,int *costMatrix)
{
    
    if(t->thePath==NULL)
    {
        //printf("Asdsad\n");
        t->totalCost=0;
        t->cityCount=1;
        t->thePath=(struct path*) malloc(sizeof(struct path));
        t->thePath->head=(struct pathNode*) malloc(sizeof(struct pathNode));
        t->thePath->head->nodeNumber=nodeNumber;
        t->thePath->head->next=NULL;
        t->thePath->lastNode=nodeNumber;

    }
    else
    {
        struct pathNode *temp=t->thePath->head;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }
        struct pathNode *newNode=(struct pathNode*) malloc(sizeof(struct pathNode));
        newNode->nodeNumber=nodeNumber;
        newNode->next=NULL;
        temp->next=newNode;
        t->totalCost=t->totalCost+*(costMatrix+V*(temp->nodeNumber)+nodeNumber);
        t->cityCount=t->cityCount+1;
        t->thePath->lastNode=nodeNumber;
        
    }
    
}
void removeLastCity(struct tour *t,int *costMatrix,int V)
{
        struct pathNode *temp=t->thePath->head;
        struct pathNode *temp1=t->thePath->head;
        while(temp->next!=NULL)
        {
            temp1=temp;
            temp=temp->next;
        }
        temp1->next=NULL;
        t->cityCount=t->cityCount-1;
        t->totalCost=t->totalCost-*(costMatrix+V*(temp1->nodeNumber)+temp->nodeNumber);
        free(temp);
}
bool feasible(struct tour *t,int nodeNumber)
{
    struct pathNode *temp=t->thePath->head;
    while(temp!=NULL)
    {
        if(temp->nodeNumber==nodeNumber)
        {
            return false;
        }
        temp=temp->next;
    }  
    return true; 
}

void printTour(struct tour *t)
{
    struct pathNode *temp=t->thePath->head;
    while(temp!=NULL)
    {
        printf("%d->",temp->nodeNumber);
        temp=temp->next;
    }
    printf("\n");
}
// Driver program to test above functions


int TreeSearch(struct Graph *graph,int src,int V,int * costMatrix)
{
    struct tour temp3;
    Stack *s=createStack(1000);
    struct tour bestTour;
    //printf("asdsad\n");
    struct tour t={0,NULL,0};
    struct pathNode *temp11=NULL;
    struct tour temp={0,NULL,0};
     //printf("asdsad\n");
    addCity(&t,src,V,costMatrix);
    //printf("asdsad\n");
    push(s,t);
    //printf("asdsad\n");
    while(!isEmpty(s))
    {
        
        t=pop(s);
        
        
        if(t.cityCount==V)
        {
            int lastCost=*(costMatrix+V*t.thePath->lastNode+t.thePath->head->nodeNumber);
            if(t.totalCost+lastCost<minCost)
            {
                minCost=t.totalCost+lastCost;
                //printTour(&t);
            }
            struct pathNode *temp44=t.thePath->head;
            struct pathNode *temp55=temp44;
            while(temp44!=NULL)
            {
                temp44=temp44->next;
                free(temp55);
                temp55=temp44;

            }
            free(t.thePath);
        }
        else
        {
            int i=0;
            for(i=V-1;i>0;i--)
            {
                
                if(feasible(&t,i))
                {
                     
                    addCity(&t,i,V,costMatrix);
                    
                    temp3.thePath=NULL;
                    temp3.totalCost=t.totalCost;
                    temp11=t.thePath->head;
                    
                    while(temp11->next!=NULL)
                    {
                        
                        addCity(&temp3,temp11->nodeNumber,V,costMatrix);
                        temp11=temp11->next;
                       
                    }
                    //printf("asdasdsada\n");
                    addCity(&temp3,temp11->nodeNumber,V,costMatrix);
                    temp3.cityCount=t.cityCount;
                    push(s,temp3);
                    //printf("Asdsadasdsad\n");
                    removeLastCity(&t,costMatrix,V);
                    //printf("%lu\n",sizeof(struct tour));
                }
            }
            struct pathNode *temp44=t.thePath->head;
            struct pathNode *temp55=temp44;
            while(temp44!=NULL)
            {
                temp44=temp44->next;
                free(temp55);
                temp55=temp44;

            }
            free(t.thePath);
        }
    }
    return minCost;
}




int main()
{
    // create the graph given in above fugure
    int V = 0;
    int src;
    //printf("Enter the number of nodes\n");
    scanf("%d",&V);
    int *costMatrix=(int *) malloc(sizeof(int)*V*V);
    struct Graph* graph = createGraph(V);
    //printf("Enter the Number of Edges you want to add\n");
    int i=0,E=0;
    scanf("%d",&E);
    //printf("Enter the links and their costs\n");
    int n1,n2,cost;
    for(i=0;i<E;i++)
    {
        
        scanf("%d %d %d",&n1,&n2,&cost);
        addEdge(graph,n1,n2,cost,costMatrix,V);
    }
    //printf("Enter the source Node\n");
    scanf("%d",&src);
    int k=0,l=0;
    double x=omp_get_wtime();
    TreeSearch(graph,src,V,costMatrix);
    double y=omp_get_wtime();
    printf("%f\n",y-x);

    return 0;}
