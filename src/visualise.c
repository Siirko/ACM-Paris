#include <stdio.h>
#include <stdlib.h>
#include <tps.h>
#include "../include/visualise.h"
#include "../include/array_list.h"
#include "../include/serializer.h"
#include "../include/delaunay.h"


double normalize_to_screen(float coord, float coord_max, float coord_min){
    return (coord - coord_min) / (coord_max - coord_min);
}

void xy_min_max(node_t n, float* x_min, float* x_max, float* y_min, float* y_max){
    if(n.latitude < *x_min && n.latitude != 0 && n.latitude < 249){
        *x_min = n.latitude;
    }
    if(n.latitude > *x_max && n.latitude != 0 && n.latitude < 249){
        *x_max = n.latitude;
    }
    if(n.longitude < *y_min && n.latitude != 0 && n.latitude < 249){
        *y_min = n.longitude;
    }
    if(n.longitude > *y_max && n.latitude != 0 && n.latitude < 249){
        *y_max = n.longitude;
    }
}

void get_xy_min_max(triangle** triangles, float* x_max, float* x_min, float* y_max, float* y_min){
    for(size_t i = 1; i < triangles[0][0].s1->latitude; i++){
        if(triangles[i]->s1!=NULL){
            node_t* s1 = triangles[i]->s1;
            node_t* s2 = triangles[i]->s2;
            node_t* s3 = triangles[i]->s3;
            xy_min_max(*s1, x_min, x_max, y_min, y_max);
        //second node
            xy_min_max(*s2, x_min, x_max, y_min, y_max);
        //third node
            xy_min_max(*s3, x_min, x_max, y_min, y_max);
        }

    }   
}

void show_data(int width, int height, triangle** node_list){
    tps_createWindow("Tree of Paris", width, height);
    float x_max;
    float x_min = 50;
    float y_max;
    float y_min = 10;
    get_xy_min_max(node_list, &x_max, &x_min, &y_max, &y_min);
    printf("%f %f %f %f\n", x_max, x_min, y_max, y_min);

    while(tps_isRunning()) {
        tps_background(255,255,255);
        tps_setColor(0,0,0);
        for(size_t i = 1; i <  node_list[0][0].s1->latitude; i++){
            //printf("ok in showdata");
            triangle* t = node_list[i];
            // normalize to screen
            if(t->s1!=NULL){
                float x_s1 = normalize_to_screen(t->s1->latitude, x_max, x_min);
                float x_s2 = normalize_to_screen(t->s2->latitude, x_max, x_min);
                float x_s3 = normalize_to_screen(t->s3->latitude, x_max, x_min);
                float y_s1 = normalize_to_screen(t->s1->longitude, y_max, y_min);
                float y_s2 = normalize_to_screen(t->s2->longitude, y_max, y_min);
                float y_s3 = normalize_to_screen(t->s3->longitude, y_max, y_min);
                int diveded = 1;
                // draw nodes
                tps_drawEllipse(x_s1 * width/diveded, y_s1 * height/diveded, 1, 1);
                tps_drawEllipse(x_s2 * width/diveded, y_s2 * height/diveded, 1, 1);
                tps_drawEllipse(x_s3 * width/diveded, y_s3 * height/diveded, 1, 1);
                // draw edges
                if(t->s1->latitude != 0 && t->s1->latitude < 249){
                    tps_drawLine(normalize_to_screen(t->s1->latitude, x_max, x_min) * width, normalize_to_screen(t->s1->longitude, y_max, y_min) * height, normalize_to_screen(t->s2->latitude, x_max, x_min) * width, normalize_to_screen(t->s2->longitude, y_max, y_min) * height);
                    tps_drawLine(normalize_to_screen(t->s2->latitude, x_max, x_min) * width, normalize_to_screen(t->s2->longitude, y_max, y_min) * height, normalize_to_screen(t->s3->latitude, x_max, x_min) * width, normalize_to_screen(t->s3->longitude, y_max, y_min) * height);
                    tps_drawLine(normalize_to_screen(t->s3->latitude, x_max, x_min) * width, normalize_to_screen(t->s3->longitude, y_max, y_min) * height, normalize_to_screen(t->s1->latitude, x_max, x_min) * width, normalize_to_screen(t->s1->longitude, y_max, y_min) * height);
                }
            }
        }
        tps_render();
    }
    tps_closeWindow();
    /*
    for(size_t i = 0; i < list_size(node_list); i++)
        free(list_get(node_list, i));
    list_free(node_list);
    */
}