#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "freegraph.h"

struct L {
    struct N *data;
    struct L *next;
};

// Returns True iff L contains N, otherwise returns False.
// Nullchecks by doing l && ____ because the rest will only
// ever be evaluated iff l is not null.
bool contains(struct L *l, struct N *n) {
    return l && ((n == l -> data) || contains(l -> next, n)); 
}

// Adds the struct, N, to the list, L.
void add(struct L *l, struct N *new) {
    if(l -> next) add(l -> next, new);
    else {
        // Create the new list element
        struct L *n = malloc(sizeof(struct L));
        n -> data = NULL; 
        n -> next = NULL;
        // Append the new element to the given list.
        l -> data = new;
        l -> next = n;
    }
}

// Recursively add to the list.
void gtla(struct N *n, struct L *l) { 
    if(contains(l, n)) return; // If we already have this node, stop traversing it.
    // Otherwise add n to the list and keep going.
    add(l, n);
    if(n -> x) gtla(n -> x, l);
    if(n -> y) gtla(n -> y, l);
    if(n -> z) gtla(n -> z, l);
}

// Converts a given graph to a linked list format (without repeats).
struct L *gtl(struct N *n) {
    // Create the list to add to.
    struct L *l = malloc(sizeof(struct L));
    l -> data = NULL;
    l -> next = NULL;
    // Add to the list.
    gtla(n, l); 
    return l; 
}

void freeall(struct L *l) {
    if(l -> next) freeall(l -> next);
    free(l -> data);
    free(l);
}

void freelist(struct L *l) {
    if(l -> next) freelist(l -> next);
    free(l);
}

// Sums the data in a list and returns.
long suml(struct L *l) {
    if(!l || !(l -> data)) return 0;
    return l -> data -> data + suml(l -> next);
}

long sum(struct N *ap) {
    struct L *l = gtl(ap); // Turn the graph into a list.
    long n = suml(l);
    freelist(l);
    return n;
}

void deallocate(struct N *ap) {
    struct L *l = gtl(ap); // Turn the graph into a list.
    if(l) freeall(l);
}

// Tree-only implementation of sum.
/*
long sum(struct N *ap) {
    if(!ap) return 0l;
    long n = (ap -> data);
    return n + sum(ap -> x) + sum(ap -> y) + sum(ap -> z);
}

// Tree-only implementation of deallocate.
void deallocate(struct N *ap) {
    if(!ap) return;
    deallocate(ap -> x);
    deallocate(ap -> y);
    deallocate(ap -> z);
    free(ap);
}
*/


