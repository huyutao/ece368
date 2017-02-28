//
// Created by yutao on 11/13/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    int node;
    int x_pos;
    int y_pos;
}Vertice;

typedef struct Link_edge{
    int node;
    struct Link_edge * next;
}Link_edge;

typedef struct {
    int node;
    int * dist;
}Dijkstra;

void read_map(Vertice * vertices, Link_edge * edges, int num_vertices, int num_edge, FILE * fp);
void find_the_path(int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination);
int calculate_dist(Vertice vertice1, Vertice vertice2);
void swap(Dijkstra * dijkstra, int ind1, int ind2, int * dijkstra_place);
void downward_heapify(Dijkstra * dijkstra, int i, int n, int * dijkstra_place);
void upward_heapify(Dijkstra * dijkstra, int n, int * dijkstra_place);

int main(int argc, char ** argv)
{
    int num_vertices;
    int num_edge;
    int num_query;
    int i = 0;
    FILE * map_fp;
    map_fp = fopen (argv[1], "r");
    fscanf(map_fp,"%d %d", &num_vertices, &num_edge);
    Vertice *vertices = malloc(sizeof(Vertice) * num_vertices);
    Link_edge *edges = malloc(sizeof(Link_edge) * num_vertices);
    read_map(vertices,edges,num_vertices,num_edge,map_fp);
    fclose(map_fp);
    FILE * query_fp;
    query_fp = fopen (argv[2], "r");
    fscanf(query_fp,"%d", &num_query);
    for(i = 0; i< num_query; i++){
        int source;
        int destination;
        fscanf(query_fp,"%d %d", &source, &destination);
        //printf("source is %d, and destination is %d\n",source,destination);
        if(source > num_vertices || destination > num_vertices){
            printf("INF\n");
            printf("%d %d\n",source,destination);
        }else{
            find_the_path(num_vertices,vertices, edges, source, destination);
        }
    }
    return(0);
}

void read_map(Vertice * vertices, Link_edge * edges, int num_vertices, int num_edge, FILE * fp){
    int i;
    Link_edge *temp;
    for(i=0; i<num_vertices;i++){    //initialize the vertices
        edges[i].next = NULL;
        edges[i].node = i;
    }
    for(i=0; i<num_vertices;i++){
        fscanf(fp,"%d %d %d", &vertices[i].node, &vertices[i].x_pos, &vertices[i].y_pos);
    }
    for(i=0; i<num_edge; i++){
        Link_edge *edge1 = malloc(sizeof(Link_edge));
        Link_edge *edge2 = malloc(sizeof(Link_edge));
        edge1->next = NULL;
        edge2->next = NULL;
        fscanf(fp,"%d %d", &edge1->node, &edge2->node);
        temp = &edges[edge1->node];
        while (temp -> next != NULL) {
            temp = temp -> next;
        }
        temp->next = edge2;

        temp = &edges[edge2->node];
        while (temp -> next != NULL) {
            temp = temp -> next;
        }
        temp->next = edge1;
    }
}

void find_the_path(int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination){
    Dijkstra dijkstra[num_vertices];
    int i = 0;
    int prev[num_vertices];
    int dist[num_vertices];
    int dijkstra_place[num_vertices];
    for(i = 0; i < num_vertices; i++){
        dist[i] = 150000;
        dijkstra_place[i] = i;
        dijkstra[i].node = i;
        dijkstra[i].dist = &dist[i];
    }
    prev[source] = -1;
    swap(dijkstra,0,source,dijkstra_place);
    dist[source] = 0;
    int unvisited = num_vertices;
    while(unvisited > 1){
        int cur_node = dijkstra[0].node;
        unvisited --;
        swap(dijkstra,0,unvisited,dijkstra_place);
        downward_heapify(dijkstra,0,unvisited,dijkstra_place);

        Link_edge * temp = &edges[cur_node];
        while(temp->next != NULL){         //for each neighbor update distance
            temp = temp -> next;
            int update_node = temp->node;    //update distance
            int alt = dist[cur_node] + calculate_dist(vertices[cur_node],vertices[update_node]);
            if (alt < dist[update_node]){
                dist[update_node] = alt;
                prev[update_node] = cur_node;
                upward_heapify(dijkstra,dijkstra_place[update_node],dijkstra_place);
            }

        }
    }
    printf("%d\n",dist[destination]);
    int temp = destination;
    while(temp != -1){
        printf("%d ",temp);
        temp = prev[temp];
    }
    printf("\n");
}

int calculate_dist(Vertice vertice1, Vertice vertice2){
    double dist = sqrt(pow((vertice2.x_pos - vertice1.x_pos), 2) + pow((vertice2.y_pos - vertice1.y_pos), 2));
    return (int)dist;
}


void swap(Dijkstra * dijkstra, int ind1, int ind2, int * dijkstra_place){
    int temp_node = dijkstra[ind1].node;
    int * temp_dist = dijkstra[ind1].dist;
    dijkstra[ind1].node = dijkstra[ind2].node;
    dijkstra[ind1].dist = dijkstra[ind2].dist;
    dijkstra[ind2].node = temp_node;
    dijkstra[ind2].dist = temp_dist;
    dijkstra_place[dijkstra[ind1].node] = ind1;
    dijkstra_place[temp_node] = ind2;

}


void downward_heapify(Dijkstra * dijkstra, int i, int n, int * dijkstra_place){
    int temp_node = dijkstra[i].node;
    int * temp_dist = dijkstra[i].dist;
    int temp_place = dijkstra_place[dijkstra[i].node];
    while((i+1) <= n/2){
        int j = 2*(i+1);
        if((j < n && *dijkstra[j-1].dist < *dijkstra[j].dist) || j == n){
            j = j - 1;
        }
        if(*temp_dist > *dijkstra[j].dist){
            temp_place = dijkstra_place[dijkstra[j].node];
            dijkstra_place[dijkstra[j].node] = dijkstra_place[dijkstra[i].node];
            dijkstra[i].node = dijkstra[j].node;
            dijkstra[i].dist = dijkstra[j].dist;
            i = j;
        }else{
            break;
        }
    }
    dijkstra[i].node = temp_node;
    dijkstra[i].dist = temp_dist;
    dijkstra_place[dijkstra[i].node] = temp_place;
}

void upward_heapify(Dijkstra * dijkstra, int n, int * dijkstra_place){
    int temp_node = dijkstra[n].node;
    int * temp_dist = dijkstra[n].dist;
    int temp_place = dijkstra_place[temp_node];
    int child = n;
    int parent = (child - 1) / 2;
    while(*dijkstra[parent].dist > *temp_dist && child > 0){
        temp_place = dijkstra_place[dijkstra[parent].node];
        dijkstra_place[dijkstra[parent].node] = dijkstra_place[dijkstra[child].node];
        dijkstra[child].node = dijkstra[parent].node;
        dijkstra[child].dist = dijkstra[parent].dist;
        child = parent;
        parent = (child - 1) / 2;
    }
    dijkstra[child].node = temp_node;
    dijkstra[child].dist = temp_dist;
    dijkstra_place[dijkstra[child].node] = temp_place;
}