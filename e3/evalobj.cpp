#include <string>
using namespace std;
#include "evalobj.h"

env *newEnv() {
    env *e = new env;
    e->next = NULL;
    return e;
}

env *add(env *e, string s, int c) {
    env *ne = newEnv();
    ne->var = s;
    ne->value = c;
    ne->next = e;
    return ne;
}

int val(env *e, string s) {
    if(e->var == s) return e->value;
    if(e->next) return val(e->next, s);
    return -1;
}

int evalexplist(ExpList *exps, env *env, enum op op) {
    Exp *head = exps->head;
    int val = head->eval(env);
    if(!exps->tail) return val;
    if(op == timesop) return val * evalexplist(exps->tail, env, op);
    if(op == plusop) return val + evalexplist(exps->tail, env, op);
    return -1;
}

int Constant::eval(env *env) { return n; }

int Var::eval(env *env) { return val(env, name); }

int OpApp::eval(env *env) { return evalexplist(args, env, op); }

int Let::eval(env *env) { 
   int res = body->eval((add(env, bvar, bexp->eval(env))));
   return res;
} 
