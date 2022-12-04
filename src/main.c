#include <stdio.h>
#include <stdlib.h>
#include "../include/args_parser.h"
#include "../include/array_list.h"
#include "../include/csv_parser.h"
#include "../include/bin_builder.h"
#include "../include/visualise.h"
#include "../include/prim.h"
#include "../include/delaunay.h"



void free_list_n(list_t* data_list){
    size_t longueur = list_size(data_list);
    for(size_t i = 0; i < longueur; i++){
        data_t* a = list_take(data_list,list_size(data_list)-1);
        free(a);
    }
    list_free(data_list);
}


void free_list_t(list_t* data_list){
    size_t longueur = list_size(data_list);
    for(size_t i = 0; i < longueur; i++){
        triangle* a = list_take(data_list,list_size(data_list)-1);
        free(a);
    }
    list_free(data_list);
}

int main(int argc, char* argv[]){
    // Initiate arguments for args parsing
    struct arguments arguments;
    arguments.output_file = "-";
    arguments.input_file = "-";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    // Open CSV file
    FILE* fp = open_file(arguments.input_file);
    // Build bin file based on fp
    int result = build_bin(fp, arguments.output_file);
    if(result == EXIT_FAILURE){ fclose(fp); exit(1); }
    // Open bin file to read it
    FILE* fp_bin = open_file(arguments.output_file);
    list_t* data_list = get_data_bin(fp_bin);
    triangle** triangles = delaunay_bowyer_watson(data_list);
    graph_t* g = create_graph(list_size(data_list));
    convert_to_graph(triangles, g);
    //show_graph_ajd(g);
    int* mst = prim_mst(g);

    free_graph(g);
    //show_mst(1300, 900, data_list, mst);
    free(mst);
    show_delaunay(1300, 900, data_list, triangles);
    for(int i = 0; i < list_size(data_list); i++){
        free(triangles[i]);
    }
    free(triangles);
    // Free content of the list and the list itself
    free_list_n(data_list);
    fclose(fp_bin);
    return 0;
}