#include <string>
#include <assert.h>
#include <iostream>

using namespace std;
#include "evalobj.h"


int main(int argc, const char *argv[])
{
    // Tests:
    auto constant = new Constant(355);
    auto plus = new OpApp(plusop, new ExpList(new Constant(10), new ExpList(new Constant(20), nullptr)));
    auto mult = new OpApp(timesop, new ExpList(new Constant(2), new ExpList(new Constant(4), new ExpList(new Constant(6), nullptr))));
    auto let = new Let("x", new Constant(4), new Var("x"));

    // (let x 5 (let y 10 (add x y (let x 6 (mult x 6 y))))) == 375
    auto complex = new Let("x", new Constant(5),
                           new Let("y", new Constant(10),
                           new OpApp(plusop, new ExpList(new Var("x"), new ExpList(new Var("y"), new ExpList(
                                   new Let("x", new Constant(6),
                                   new OpApp(timesop,
                                             new ExpList(new Var("x"), new ExpList(new Constant(6), new ExpList(new Var("y"), nullptr)))
                                   ))
                                   , nullptr
                           )))))
            );

    assert(constant->eval(nullptr) == 355);
    printf("test 1 passed\n");
    assert(plus->eval(nullptr) == 30);
    printf("test 2 passed\n");
    assert(mult->eval(nullptr) == 48);
    printf("test 3 passed\n");
    assert(let->eval(nullptr) == 4);
    printf("test 4 passed\n");
    assert(complex->eval(nullptr) == 375);
    printf("test 5 passed\n");

    return 0;
}
