#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 4294967296

//linked list to store index and weight of each grafh from AggiungiGrafo
struct node {
    int index;
    long long int weight;
    struct node *next;
};

//used to sort the list, i convert the list into an array of struct then sort it and takes the first K
typedef struct {
    int index;
    long long int weight;
} leaderBoard;

typedef struct node *list;

//normal insert head function
list insert_head(list l, int index, long long int weight){
    struct node *temp = malloc(sizeof(struct node));
    temp->index = index;
    temp->weight = weight;
    temp->next = l;
    return temp;
}

//insert into end
list insert_tail (list l, int index, long long int weight){
    if (l == NULL)
        return insert_head(l, index, weight);
    else{
        l->next = insert_tail(l->next, index, weight);
        return l;
    }
}

//print the list
void printer(list l){
    while (l != NULL) {
        if(l->index == 0)
            printf("%d", l->index);
        else
            printf("%d ", l->index);
        l = l->next;
    }
}

//merge function for merge sort
void merge(leaderBoard lead[], int l, int m, int r){
    int i, j, k;
    //dimension for our sub array
    int n1 = m - l + 1;
    int n2 = r - m;
    //sub arrays
    leaderBoard l1[n1], l2[n2];
    //putting data to l1 and l2
    for(i = 0; i < n1; i++){
        l1[i].index = lead[l + i].index;
        l1[i].weight = lead[l + i].weight;
    }
    for(j = 0; j < n2; j++){
        l2[j].index = lead[m + 1 + j].index;
        l2[j].weight = lead[m + 1 + j].weight;
    }
    //merge back
    i = 0;
    j = 0;
    k = l;
    while(i < n1 && j < n2){
        if(l1[i].weight < l2[j].weight){
            lead[k].weight = l1[i].weight;
            lead[k].index = l1[i].index;
            i++;
        }
        else if(l1[i].weight == l2[j].weight){
            if(l1[i].index < l2[j].index){
                lead[k].weight = l1[i].weight;
                lead[k].index = l1[i].index;
                i++;
            }
            else{
                lead[k].weight = l2[j].weight;
                lead[k].index = l2[j].index;
                j++;
            }
        }
        else{
            lead[k].weight = l2[j].weight;
            lead[k].index = l2[j].index;
            j++;
        }
        k++;
    }
    //copy the rest of l1 elements
    while(i < n1){
        lead[k].weight = l1[i].weight;
        lead[k].index = l1[i].index;
        i++;
        k++;
    }
    //copy the rest of l2 elements
    while(j < n2){
        lead[k].weight = l2[j].weight;
        lead[k].index = l2[j].index;
        j++;
        k++;
    }
}

//l is for l1 index and r is l2 index
void mergeSort(leaderBoard lead[], int l, int r){
    if (l < r){
        int m = l + (r - l) / 2;
        //sort the first and second sub arrays
        mergeSort(lead, l, m);
        mergeSort(lead, m + 1, r);
        merge(lead, l, m, r);
    }
}

void TopK(list l, int n, int graph_index){
    list temp;
    if(l == NULL)
        return;
    int i = 0;
    leaderBoard lead[graph_index];
    temp = l;
    while(temp!= NULL){
        lead[i].index = temp->index;
        lead[i].weight = temp->weight;
        temp = temp->next;
        i++;
    }

    /*for(i =0; i < graph_index; i++){
        printf("[%d %lli] ",lead[i].index,lead[i].weight);
    }
    printf("\n");*/
    //merge sort the lead array then take the first K;
    mergeSort(lead, 0, graph_index - 1);

    /*for(i =0; i < graph_index; i++){
        printf("[%d %lli] ",lead[i].index,lead[i].weight);
    }
    printf("\n");*/

    for(i=0; i<n; i++){
        if(i == 0)
            printf("%d", lead[i].index);
        else
            printf(" %d", lead[i].index);
    }
}

long int AggiungiGrafo(int d){
    char *unused __attribute__((unused));
    long long int a[d][d];
    long long int weight = 0;
    long long int num = 0;
    int i,j, k = 0;
    char buffer[10*d+d];

    /*num = 0;
    num = num;*/

    for(i = 0; i < d; i++){
        //reads 1 line
        unused = fgets(buffer, 10*d+d, stdin);
        j=0;
        //parsing into the matrix
        while(buffer[j] != '\n'){
            //initializing "num"
            if(j == 0){
                num = 0;
                k = 0;
            }
            //counting the numbers and parsing them into INT
            if(buffer[j] != ','){
                num = num*10 + buffer[j] - 48;
            }
            //in encounter "," put num into the matrix
            if(buffer[j+1] == ',' || buffer[j+1] == '\n'){
                a[i][k] = num;
                num = 0;
                k++;
            }
            j++;
        }
    }

    /*for(i=0; i<d; i++){
        for(j=0; j<d; j++){
           printf("%lli ", a[i][j]);
        }
        printf("\n");
    }*/
    //Calculate the weight of the graph that is recived in input DIJKSTRA

    //matrix to store the weights
    long long int weights[d][d];
    //array to store the distance
    long long int distance[d];
    //array to store the prev of each node
    long long int visited[d], prev[d], count, min, next=0;
    for(i=0; i<d; i++){
        for(j=0; j<d; j++){
            if(a[i][j] == 0)
                weights[i][j] = INF;
            else
                weights[i][j] = a[i][j];
        }
    }
    //to avoid Warning
    prev[0] = 0;
    prev[0] = prev[0];

    //initializing the varialbles
    for(i=0; i<d; i++){
        distance[i] = weights[0][i];
        prev[i] = 0;
        visited[i] = 0;
    }
    distance[0] = 0;
    visited[0] = 1;
    count = 1;

    while(count < d-1){
        min = INF;
        for(i = 0; i< d; i++){
            if(distance[i] < min && !visited[i]){
                min = distance[i];
                next = i;
            }
        }
        //check better path
        visited[next] = 1;
        for(i = 0; i < d; i++){
            if(!visited[i]){
                if(min + weights[next][i] < distance[i]){
                    distance[i] = min + weights[next][i];
                    prev[i] = next;
                }
            }
        }
        count++;
    }

    for(i = 0; i<d ; i++){
        if(distance[i] == INF)
            weight = weight + 0;
        else
            weight = weight + distance[i];
    }
    //END DIJKSTRA
    return weight;
}

int main(){
    int d, k, graph_index=0;
    int unused __attribute__((unused));
    long long int weight;
    char instruction[14];
    list l = NULL;

    //take the first 2 inputs: "d" stands for the graph's length and "k" for the leaderboard's length
    unused = scanf("%d %d", &d, &k);
    //looping for the inputs
    while((scanf("%s\n", instruction)) != EOF){
        if(strcmp(instruction, "AggiungiGrafo") == 0){
            graph_index++;
            weight = AggiungiGrafo(d);
            //printf("%lli ", weight);
            l = insert_head(l, graph_index-1,weight);
            //printer(l);
        }
        else if(strcmp(instruction, "TopK") == 0) {

            //when the number of the given graphs is lower than "k" we print "graph_index"n times
            if (graph_index <= k) {
                printer(l);
                printf("\n");
            }

                //when the number of the given graphs is greater than "k" we print "k"n times
            else {
                TopK(l, k, graph_index);
                printf("\n");
            }
        }
        else;
    }
    return 0;
}
