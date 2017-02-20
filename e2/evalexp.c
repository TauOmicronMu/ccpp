#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "evalexp.h"

struct Env {
    struct Env *next;
    char   var[8];
    int    constant;
};

void add(struct Env *curr, char var[8], int constant) {
    // Update if var is already in here
    if(strcmp(curr -> var,var)) {
        curr -> constant = constant;
        return;
    }
    // Otherwise create a new one
    if(curr -> next) add(curr -> next, var, constant);
    else {
        // Create new list element
        struct Env *new = calloc(sizeof(struct Env), 1);
        // Add to the current element
        curr -> next     = new;
        curr -> constant = constant;
        strncpy(curr -> var, var, 8);
    }
}

struct Env *copy_a(struct Env *curr, struct Env *new);

struct Env *copy(struct Env *curr) {
    struct Env *new = calloc(sizeof(struct Env), 1);
    return copy_a(curr, new);
}

struct Env *copy_a(struct Env *curr, struct Env *new) {
    if(!curr -> next) return new;
    add(new, curr -> var, curr -> constant);
    return copy_a(curr -> next, new);
}

// Return the value of a given constant, var, or
// -1 if var has no associated constant
int val(struct Env *curr, char var[8]) {
    if(strcmp(curr -> var, var)) return curr -> constant;
    if(curr -> next)       return val(curr -> next, var);
    return -1; // var doesn't have an associated constant
} 

int evalexplistenv(struct explist *es, enum op op, struct Env *env);

int evalexpenv(struct exp *e, struct Env *env) {
    enum exptag c_tag = e -> tag;
    if(c_tag == islet) {
        // copy current env and add to that
        struct Env *cpy = copy(env);
        add(cpy, e -> bvar, evalexpenv(e -> bexp, cpy));
        return evalexpenv(e -> body, cpy);
    }
    if(c_tag == isconstant) return e -> constant;
    if(c_tag == isvar) {
        int c_val = val(env, e -> var);
        if(c_val >= 0) return c_val;
        return c_val; // This is erroneous and should never be reached.
    }
    if(c_tag == isopapp) return evalexplistenv(e -> exps, e -> op, env);
    return -1; // we should never reach this
}


int evalexplistenv(struct explist *es, enum op op, struct Env *env) {
    if(!es -> tail) return evalexpenv(es -> head, env);
    switch(op) {
        case isplus:
            return evalexpenv(es -> head, env) + evalexplistenv(es -> tail, op, env);
        case ismult:
            return evalexpenv(es -> head, env) * evalexplistenv(es -> tail, op, env);
    }
}

int evalexplist(struct explist *es, enum op op) {
    if(!es -> tail) return evalexp(es -> head);
    switch(op) {
        case isplus:
            return evalexp(es -> head) + evalexplist(es -> tail, op);
        case ismult:  
            return evalexp(es -> head) * evalexplist(es -> tail, op);
    }
}

int evalexp(struct exp *e) {
    // Get the tag from the expression
    enum exptag c_tag = e -> tag;
        if(c_tag == islet) {
            // create an environment
            struct Env *env = calloc(sizeof(struct Env), 1);
            add(env, e -> bvar, evalexp(e -> bexp));
            return evalexpenv(e -> body, env);
        }
        if(c_tag == isconstant) return e -> constant;
        if(c_tag == isvar) return 0;
        if(c_tag == isopapp) return evalexplist(e -> exps, e -> op);
    return -1; // we should never reach this! 
}
