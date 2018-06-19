#ifndef EXR1_DEFINES_H
#define EXR1_DEFINES_H

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

t_node* Create_Tree(char**, double*);
double Solve_Tree(t_node*);
int Set_Var(size_t ind, const double*);
t_var Variables[VAR_SIZE];
#endif //EXR1_DEFINES_H