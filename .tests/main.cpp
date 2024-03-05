#include <iostream>
#include <memory>
#include "dataflow.hpp"

void test2()
{
    try
    {
        using namespace bo;
        std::cout << "TEST 03" << std::endl;

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
        //double *result = double *<double *>data2(e->port("out")->property()->data);
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
    return 0;
}