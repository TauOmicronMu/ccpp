#include <string>
using namespace std;

#include "templatesast.h"

template<typename V> env<V> *newEnv() {
    env<V> *e = new env<V>;
    e->next = nullptr;
    return e;
}

template<typename V> env<V> *adde(env<V> *e, string s, V c) {
    env<V> *ne = newEnv<V>();
    ne->var = s;
    ne->value = c;
    ne->next = e;
    return ne;
}

template<typename V> V val(env<V> *e, string s) {
    if(e->var == s) return e->value;
    if(e->next) return val(e->next, s);
    return -1;
}

template<typename V> V evalexplist(ExpList<V> *exps, env<V> *env, operators<V> ops) {
    if(!exps) return ops.unit;
    Exp<V> *head = exps->head;
    V val = head->eval(env);
    if(!exps->tail) return val;
    return ops.binop(val, evalexplist(exps->tail, env, ops));
}

template<typename V> V Constant<V>::eval(env<V> *env) { return c; }

template<typename V> V Var<V>::eval(env<V> *env) { return val(env, name); }

template<typename V> V OpApp<V>::eval(env<V> *env) { return evalexplist(args, env, ops); }

template<typename V> V Let<V>::eval(env<V> *env) { return body->eval((adde(env, bvar, bexp->eval(env)))); } 
