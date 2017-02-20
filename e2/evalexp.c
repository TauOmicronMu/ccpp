#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "evalexp.h"

/* Holds an environment for a let statement */
struct Env {
    struct Env *next;
    char   var[8];
    int    constant;
}; 

int evalexpenv(struct exp *e, struct Env *env);

/* Allocates space for a new Env and returns a pointer to it */
struct Env *newEnv() {
    struct Env *new = calloc(sizeof(struct Env), 1);
    new->next     = NULL;
    return new;
} 

/* Adds a new item to the front of an Env */
struct Env *add(struct Env *env, char var[8], int constant) {
    struct Env *new = newEnv();
    strncpy(new->var, var, 8);
    new->constant = constant;
    new->next     = env;
    return new;
}

/* Returns the value of a variable in an Env */
int val(struct Env *env, char var[8]) {
    if(strcmp(env->var, var) == 0) return env->constant;
    if(env->next) return val(env->next, var);
    return -1;
}

/* Pops the topmost Env from an Env*/
struct Env *pop(struct Env *env) { return env->next; }

/* Eval an explist */
int evalexplist(struct explist *exps, struct Env *env, enum op op) {
    int val = evalexpenv(exps->head, env);
    if(!exps->tail)  return val;
    if(op == ismult) return evalexpenv(exps->head, env) * evalexplist(exps->tail, env, op);
    if(op == isplus) return evalexpenv(exps->head, env) + evalexplist(exps->tail, env, op);
    return -1;
}

/* Start evalexpenv with an empty Env */
int evalexp(struct exp *e) { return evalexpenv(e, newEnv()); } 

/* Eval an exp in a given Env */
int evalexpenv(struct exp *e, struct Env *env) {
    if(e->tag == islet) {
        int res = evalexpenv(e->body, add(env, e->bvar, evalexpenv(e->bexp, env)));
        pop(env);
        return res; 
    }
    if(e->tag == isvar)      return val(env, e->var);
    if(e->tag == isconstant) return e->constant;
    if(e->tag == isopapp)    return evalexplist(e->exps, env, e->op);
    return -1;
}
