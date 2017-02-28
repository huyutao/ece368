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

void read_map(Vertice * vertices, Link_edge * edges, int num_vertices, int num_edge, FILE * fp);
void find_the_path(int num_vertices,Vertice * vertices, Link_edge *edges, int source, int destination);
int calculate_dist(Vertice vertice1, Vertice vertice2);

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
    int i = 0;
    int inf = 0;
    int dist[num_vertices];
    int status[num_vertices];
    int prev[num_vertices];
    for(i = 0; i < num_vertices; i++){
        dist[i] = 150000;
        status[i] = 0;
    }
    dist[source] = 0;
    prev[source] = -1;
    while(status[destination] != 150000){
        int min = 150000;
        int cur_node = -1;
        for(i = 0; i < num_vertices; i++){    //find min
            if((dist[i] + status[i]) < min){
                min = dist[i];
                cur_node = i;
            }
        }
        if(min == 150000){
            inf = 1;
            break;
        }
        status[cur_node] = 150000;
        Link_edge * temp = &edges[cur_node];
        while(temp->next != NULL){
            temp = temp -> next;
            int update_node = temp->node;    //update distance
            int alt = dist[cur_node] + calculate_dist(vertices[cur_node],vertices[update_node]);
            if (alt < dist[update_node]){
                dist[update_node] = alt;
                prev[update_node] = cur_node;
            }

        }
    }
    if (inf == 1){
        printf("INF\n");
        printf("%d %d\n",source,destination);
    }else{
        printf("%d\n",dist[destination]);
        int temp = destination;
        while(temp != -1){
            printf("%d ",temp);
            temp = prev[temp];
        }
        printf("\n");
    }

}

int calculate_dist(Vertice vertice1, Vertice vertice2){
    double dist = sqrt(pow((vertice2.x_pos - vertice1.x_pos), 2) + pow((vertice2.y_pos - vertice1.y_pos), 2));
    return (int)dist;
}
