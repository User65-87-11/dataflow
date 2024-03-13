#include <iostream>
#include <memory>
#include "dataflow.hpp"
#include "namespace.h"
 
void test4(){
    std::cout << "TEST 04" << std::endl;

    ns::print();

}
void test3()
{
       std::cout << "TEST 03" << std::endl;
    class Test
    {
        char array[5];
        Test(){
            
        }
        void print()
        {
            for (char &c : array)
            {
                std::cout << "symbol:" << c << std::endl;
            }
        }
    };
}

void test2()
{
    try
    {
        using namespace bo;
        std::cout << "TEST 02" << std::endl;

        GraphManager gm;

        Node *a = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *b = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *c = gm.createNode(NODE_TYPE::NODE_OF_SUM);

        a->port("out")->connect(c->port("in_a"));

        b->port("out")->connect(c->port("in_b"));

        c->evaluate();

        Node *d = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *e = gm.createNode(NODE_TYPE::NODE_OF_SUM);

        c->port("out")->connect(e->port("in_a"));
        d->port("out")->connect(e->port("in_b"));

        e->evaluate();

        // double *result = static_cast<double *>(e->port("out")->property()->data);
        // double *result = double *<double *>data2(e->port("out")->property()->data);
        double *result = DataHandler<double *>::data(e->port("out")->property()->data);

        std::cout << " res:" << *result << std::endl;
        if (*result == 50)
        {
            std::cout << "SUCCESS" << std::endl;
        }
        else
        {
            std::cout << "FAIL with: " << result << std::endl;
        }

        std::cout << "END" << std::endl;
    }
    catch (const std::exception &e)
    {
        // Handle the exception
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
int main(void)
{
    test2();
    test3();
    test4();
    return 0;
}