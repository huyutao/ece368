//
// Created by yutao on 11/13/16.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int node;
    int x_pos;
    int y_pos;
    int dist_to_source;
}Vertice;

typedef struct Link_edge{
    int node;
    struct Link_edge * next;
}Link_edge;

typedef struct {
    int node;
    int dist;
    int prev;
}Dijkstra;

void read_map(Vertice * vertices, Link_edge * edges, int num_vertices, int num_edge, FILE * fp);
void find_the_path(Dijkstra * dijkstra, int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination);
void swap(Dijkstra * dijkstra, int ind1, int ind2);

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
        printf("source is %d, and destination is %d\n",source,destination);
        Dijkstra * dijkstra = malloc(sizeof(Dijkstra) *num_vertices);
        find_the_path(dijkstra,num_vertices,vertices, edges, source, destination);
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
        //printf(" %d, %d, %d\n", vertices[i].node, vertices[i].x_pos, vertices[i].y_pos);
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
    for(i=0; i<num_vertices; i++){    //print edges
        temp = &edges[i];
        while(temp->next != NULL){
            printf(" %d ", temp->node);
            temp = temp -> next;
        }
        printf(" %d ", temp->node);
        printf("\n");
    }
}

void find_the_path(Dijkstra * dijkstra, int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination){
    //int first = 0;
    int last = num_vertices - 1;
    int i = 0;
    for(i = 0; i < num_vertices; i++){
        dijkstra[i].node = i;
        dijkstra[i].dist = 150000;
        dijkstra[i].prev = -1;
    }
    dijkstra[source].dist = 0;
    swap(dijkstra,0,source);
    /*
    for(i = 0; i < num_vertices; i++){
        printf("distance is %d\n", dijkstra[i].dist);
    }*/

    while(last > 0){
        update_distance();

        swap(dijkstra,0,last);
        downward_heapify(dijkstra,1,last);
    }
}


void swap(Dijkstra * dijkstra, int ind1, int ind2){
    int temp_node = dijkstra[ind1].node;
    int temp_dist = dijkstra[ind1].dist;
    int temp_prev = dijkstra[ind1].prev;
    dijkstra[ind1].node = dijkstra[ind2].node;
    dijkstra[ind1].dist = dijkstra[ind2].dist;
    dijkstra[ind1].prev = dijkstra[ind2].prev;
    dijkstra[ind2].node = temp_node;
    dijkstra[ind2].dist = temp_dist;
    dijkstra[ind2].prev = temp_prev;
}

void update_distance(Dijkstra * dijkstra, Link_edge *edges){
    while(1){
        int current = dijkstra[0].node;
        
        int neighbor;
        Link_edge *temp = &edges[current];
        while(temp->next != NULL){
            neighbor = temp->node;
            temp = temp -> next;
        }
    }
}

