//
// Created by abdurashidov.aak on 21.06.2018.
//

#ifndef LAB2AER2_SYNTAXTREE_H
#define LAB2AER2_SYNTAXTREE_H

#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define MAX_CHAR_SIZE 127

#define OK 0
#define ERR_SIGNS 3
#define END_STRING 6
#define ERROR ERR_SIGNS
#define STEP 10
#define VAR_SIZE 26

typedef enum {IS_NUM, IS_FUN, IS_VAR, IS_OPR} t_type;
typedef enum {IS_DEFLT, IS_SETED, IS_EXIST} t_type_var;
typedef struct Tree_Node {
    t_type type;
    union {
        double num;
        int ind;
        char opr;
    } t_data;
    struct Tree_Node *CHILD[2];
} t_node;

typedef struct Vars {
    double data;
    t_type_var type;
} t_var;

typedef struct {
    t_node *root;
    t_var Vars[26];
} t_tree;

t_tree* Create_Tree(char**, double*);
double Solve_Tree(t_tree*);
int Set_Var(t_tree* ,size_t ind, const double*);
t_tree* Copy_Tree(t_tree*);
int Delete_Tree(t_tree**);
void Print_Tree(t_node*);
t_tree* Diff_Tree(t_tree*);
#endif //LAB2AER2_SYNTAXTREE_H
