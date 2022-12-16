#include "../include/graph.h"
#include "../include/delaunay.h"
#include "../include/sdebug.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdbool.h>
#include <string.h>

double dist(data_t p1, data_t p2, enum DISTANCE_TYPE type){
    switch(type){
        case EUCLIDEAN:
            return sqrt((p1.latitude - p2.latitude)*(p1.latitude - p2.latitude) +
                        (p1.longitude - p2.longitude)*(p1.longitude - p2.longitude));
        case HAVESINE:
            double radius = 6371.0;
            double plat = p1.latitude * M_PI / 180.0;
            double mlat = p2.latitude * M_PI / 180.0;
            double dlat = (p2.latitude - p1.latitude) * M_PI / 180.0;
            double dlong = (p2.longitude - p1.longitude) * M_PI / 180.0;
            double a = sin(dlat/2) * sin(dlat/2) +
                    cos(plat) * cos(mlat) *
                    sin(dlong/2) * sin(dlong/2);
            double c = 2 * atan2(sqrt(a), sqrt(1-a));
            return radius * c;
        default:
            return sqrt((p1.latitude - p2.latitude)*(p1.latitude - p2.latitude) +
                        (p1.longitude - p2.longitude)*(p1.longitude - p2.longitude));
    }
}

void free_graph(graph_t* graph){
    for (int i = 0; i < graph->size_vertices; i++){
        node_adj_t* node = graph->arr[i].head;
        while(node != NULL){
            node_adj_t* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(graph->arr);
    free(graph);
}

graph_t* create_graph(int size_vertices){
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    graph->size_vertices = size_vertices;
    graph->size_edges = 0;
    graph->arr = (adj_list_t*)malloc(size_vertices * sizeof(adj_list_t));
    for (int i = 0; i < size_vertices; i++)
        graph->arr[i].head = NULL;
    return graph;
}

node_adj_t* create_node_adj(int id, data_t* data, double weight){
    node_adj_t* node = (node_adj_t*)malloc(sizeof(node_adj_t));
    node->id = id;
    node->data = data;
    node->weight = weight;
    node->next = NULL;
    return node;
}

void add_edge(graph_t* graph, data_t* data1, data_t* data2){
    int id1 = data1->id;
    int id2 = data2->id;
    double weight = dist(*data1, *data2, HAVESINE);
    // add edge from data1 to data2
    node_adj_t* node1 = create_node_adj(id2, data2, weight);
    node1->next = graph->arr[id1].head;
    graph->arr[id1].head = node1;
    // add edge from data2 to data1
    node_adj_t* node2 = create_node_adj(id1, data1, weight);
    node2->next = graph->arr[id2].head;
    graph->arr[id2].head = node2;
    // increment number of edges (undirected graph)
    graph->size_edges++;
}

void show_graph_adj(graph_t* g){
    for (int i = 0; i < g->size_vertices; i++)
    {
        node_adj_t* node = g->arr[i].head;
        printf("Vertex %d: ", i);
        while(node != NULL)
        {
            printf("%d(%f) ", node->id, node->weight);
            node = node->next;
        }
        printf("\n");
    }
}

void show_graph_adj_at(graph_t* g, int id){
    node_adj_t* node = g->arr[id].head;
    printf("Vertex %d: ", id);
    while(node != NULL)
    {
        printf("%d(%f) ", node->id, node->weight);
        node = node->next;
    }
    printf("\n");
}

void save_mst(int* parent, int size_vertices, char* path){
    FILE* file = fopen(path, "w+");
    if(file == NULL){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    for (int i = 1; i < size_vertices; i++){
        if(parent[i] == -1){
            deprintf("Error save_mst: parent[%d] = -1\n Vertex not connected \
            (Probably another vertex having same coordinate is already connected)\n", i);
        }
        fprintf(file, "%d - %d\n", parent[i], i);
    }
    fclose(file);
    deprintf("MST saved in file: %s\n", path);
}

int* prim_mst(graph_t* graph, char* path){
    int size_vertices = graph->size_vertices;
    int* parent = malloc(size_vertices * sizeof(int));
    double* key = malloc(size_vertices * sizeof(double));

    min_heap_t* min_heap = create_min_heap(size_vertices);
    // Initialize a min heap with all vertices (except first vertex)
    for (int i = 1; i < size_vertices; i++){
        parent[i] = -1;
        key[i] = DBL_MAX;
        min_heap->array[i] = create_min_heap_node(i, key[i]);
        min_heap->pos[i] = i;
    }

    // Make key value of first vertex as 0 so that it is extracted first
    key[0] = 0;
    min_heap->array[0] = create_min_heap_node(0, key[0]);
    min_heap->pos[0] = 0;

    // Initially size of min heap is equal to V
    min_heap->size = size_vertices;

    // In the following loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!is_empty(min_heap)){
        // Extract the vertex with minimum distance value
        min_heap_node_t* min_heap_node = extract_min(min_heap);
        int u = min_heap_node->v;

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        node_adj_t* node = graph->arr[u].head;
        while (node != NULL){
            int v = node->data->id;
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (is_in_min_heap(min_heap, v) && node->weight < key[v]){
                key[v] = node->weight;
                parent[v] = u;
                decrease_key(min_heap, v, key[v]);
            }
            node = node->next;
        }
        free(min_heap_node);
    }
    // save MST in file if path is not empty
    if(strcmp(path, "")){
        save_mst(parent, size_vertices, path);
    }
    // free memory
    free(key);
    free_min_heap(min_heap);
    deprintf("Prim MST done\n");
    return parent;
}

bool is_edge_in_graph(graph_t* graph, data_t* d1, data_t* d2){
    node_adj_t* node = graph->arr[d1->id].head;
    while(node != NULL){
        if(node->id == d2->id)
            return true;
        node = node->next;
    }
    return false;
}


void delaunay_to_graph(triangle_t** triangles, graph_t* graph){
    for (int i = 1; i < triangles[0][0].s1->latitude; i++){
        triangle_t* t = triangles[i];
        if(t->s1->id != -1 && t->s2->id != -1 && t->s3->id != -1){
            if(!is_edge_in_graph(graph, t->s1, t->s2))
                add_edge(graph, t->s1, t->s2);
            if(!is_edge_in_graph(graph, t->s2, t->s3))
                add_edge(graph, t->s2, t->s3);
            if(!is_edge_in_graph(graph, t->s3, t->s1))
                add_edge(graph, t->s3, t->s1);
        }
    }
    deprintf("Delaunay to graph done\n");
}

double sum_weight_graph(int* mst, list_t* nodes){
    double sum = 0;
    for (int i = 1; i < list_size(nodes); i++){
        if(mst[i] == -1)
            continue;
        data_t* data1 = list_get(nodes, mst[i]);
        data_t* data2 = list_get(nodes, i);
        sum += dist(*data1, *data2, HAVESINE);
    }
    return sum;
}