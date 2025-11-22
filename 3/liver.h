#ifndef LIVER_H
#define LIVER_H

#include <stdio.h>

typedef struct {
    char surname[50];
    char name[50];
    char patronymic[50];
    int b_day, b_mon, b_year;
    char gender;
    double income;
} Liver;

#define LIST_TYPE Liver

#endif