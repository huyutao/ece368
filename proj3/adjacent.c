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
void find_the_path(int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination,
                   Dijkstra *dijkstra, int * prev, int * dist, int * dijkstra_place);
int calculate_dist(Vertice vertice1, Vertice vertice2);
void swap(Dijkstra * dijkstra, int ind1, int ind2, int * dijkstra_place);
void downward_heapify(Dijkstra * dijkstra, int i, int n, int * dijkstra_place);
void upward_heapify(Dijkstra * dijkstra, int n, int * dijkstra_place);
void print_result(int destination, int num_vertices, int dist, int *prev);
void free_memory(int num_vertices, Dijkstra *dijkstra, int* prev, int* dist, int* dijkstra_place, Vertice* vertices, Link_edge* edges);

int main(int argc, char ** argv)
{
    int num_vertices;
    int num_edge;
    int num_query;
    int source;
    int destination;
    int i = 0;
    FILE * map_fp;
    FILE * query_fp;
    map_fp = fopen (argv[1], "r");
    query_fp = fopen (argv[2], "r");
    fscanf(map_fp,"%d %d", &num_vertices, &num_edge);
    Vertice *vertices = malloc(sizeof(Vertice) * num_vertices);
    Link_edge *edges = malloc(sizeof(Link_edge) * num_vertices);
    read_map(vertices,edges,num_vertices,num_edge,map_fp);
    fscanf(query_fp,"%d", &num_query);
    Dijkstra *dijkstra = malloc(sizeof(Dijkstra) * num_vertices);
    int * prev = malloc(sizeof(int) * num_vertices);
    int * dist = malloc(sizeof(int) * num_vertices);
    int * dijkstra_place = malloc(sizeof(int) * num_vertices);
    for(i = 0; i< num_query; i++){
        fscanf(query_fp,"%d %d", &source, &destination);
        if(source >= num_vertices || destination >= num_vertices){
            printf("INF\n");
            printf("%d %d\n",source,destination);
        }else{
            find_the_path(num_vertices,vertices, edges, source, destination,dijkstra,prev,dist,dijkstra_place);
        }
    }
    free_memory(num_vertices, dijkstra, prev, dist, dijkstra_place, vertices, edges);
    fclose(map_fp);
    fclose(query_fp);
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
        fscanf(fp,"%d %d", &edge1->node, &edge2->node);
        temp = &edges[edge1->node];
        edge2->next = temp->next;
        temp->next = edge2;
        temp = &edges[edge2->node];
        edge1->next = temp->next;
        temp->next = edge1;
    }
}

void find_the_path(int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination,
                   Dijkstra *dijkstra, int * prev, int * dist, int * dijkstra_place){
    int i;
    int unvisited = num_vertices;
    int que = 1;
    for(i = 0; i < num_vertices; i++){        //initialize
        dist[i] = 2147483647;   //infinity
        dijkstra_place[i] = i;
        dijkstra[i].node = i;
        dijkstra[i].dist = &dist[i];
    }
    prev[source] = -1;
    swap(dijkstra,0,source,dijkstra_place);
    dist[source] = 0;
    while(dist[dijkstra[0].node] != 2147483647){
        int cur_node = dijkstra[0].node;
        if(cur_node == destination){
            break;
        }
        unvisited --;
        if(que > 0){
            que--;
        }
        swap(dijkstra,0,que,dijkstra_place);
        swap(dijkstra,que,unvisited,dijkstra_place);
        downward_heapify(dijkstra,0,que,dijkstra_place);
        Link_edge * temp = &edges[cur_node];
        while(temp->next != NULL){         //for each neighbor update distance
            temp = temp -> next;
            int update_node = temp->node;    //update distance
            int alt = dist[cur_node] + calculate_dist(vertices[cur_node],vertices[update_node]);
            if (alt < dist[update_node]){
                if(dist[update_node] == 2147483647){
                    swap(dijkstra,que,dijkstra_place[update_node],dijkstra_place);
                    que++;
                }
                dist[update_node] = alt;
                prev[update_node] = cur_node;
                upward_heapify(dijkstra,dijkstra_place[update_node],dijkstra_place);
            }
        }
    }
    print_result(destination, num_vertices, dist[destination], prev);
}

int calculate_dist(Vertice vertice1, Vertice vertice2){
    int x_d = vertice2.x_pos - vertice1.x_pos;
    int y_d = vertice2.y_pos - vertice1.y_pos;
    int dist = sqrt(x_d*x_d + y_d*y_d);
    return dist;
}

void swap(Dijkstra * dijkstra, int ind1, int ind2, int * dijkstra_place){
    Dijkstra temp = dijkstra[ind1];
    dijkstra[ind1] = dijkstra[ind2];
    dijkstra[ind2] = temp;
    dijkstra_place[dijkstra[ind1].node] = ind1;
    dijkstra_place[temp.node] = ind2;
}

void downward_heapify(Dijkstra * dijkstra, int i, int n, int * dijkstra_place){
    Dijkstra temp = dijkstra[i];
    int temp_parent_place = dijkstra_place[dijkstra[i].node];
    int temp_child_place;
    while((i+1) <= n/2){
        int j = 2*(i+1);
        if((j < n && *dijkstra[j-1].dist < *dijkstra[j].dist) || j == n){
            j = j - 1;
        }
        if(*temp.dist > *dijkstra[j].dist){
            temp_child_place = dijkstra_place[dijkstra[j].node];
            dijkstra_place[dijkstra[j].node] = temp_parent_place;
            temp_parent_place = temp_child_place;
            dijkstra[i] = dijkstra[j];
            i = j;
        }else{
            break;
        }
    }
    dijkstra[i] = temp;
    dijkstra_place[dijkstra[i].node] = temp_parent_place;
}

void upward_heapify(Dijkstra * dijkstra, int n, int * dijkstra_place) {
    int child = n;
    Dijkstra temp = dijkstra[child];
    int temp_child_place = dijkstra_place[temp.node];
    int temp_parent_place;
    int parent = (child - 1) / 2;
    while (*dijkstra[parent].dist > *temp.dist && child > 0) {
        temp_parent_place = dijkstra_place[dijkstra[parent].node];
        dijkstra_place[dijkstra[parent].node] = temp_child_place;
        temp_child_place = temp_parent_place;
        dijkstra[child] = dijkstra[parent];
        child = parent;
        parent = (child - 1) / 2;
    }
    dijkstra[child] = temp;
    dijkstra_place[dijkstra[child].node] = temp_child_place;
}

void print_result(int destination, int num_vertices, int dist, int *prev){
    int i = -1;
    int temp_dest = destination;
    int result_array[num_vertices];
    printf("%d\n",dist);
    while(temp_dest != -1){
        i++;
        result_array[i] = temp_dest;
        temp_dest = prev[temp_dest];
    }
    for(;i>=0;i--){
        printf("%d ",result_array[i]);
    }
    printf("\n");
}

void free_memory(int num_vertices, Dijkstra *dijkstra, int* prev, int* dist, int* dijkstra_place, Vertice* vertices, Link_edge* edges){
    int i;
    free(dijkstra);
    free(prev);
    free(dist);
    free(dijkstra_place);
    free(vertices);
    Link_edge *temp;
    for(i=0; i<num_vertices; i++){    //print edges
        temp = &edges[i];
        temp = temp -> next;
        while(temp->next != NULL){
            free(temp);
            temp = temp -> next;
        }
        free(temp);
    }
    free(edges);
}
