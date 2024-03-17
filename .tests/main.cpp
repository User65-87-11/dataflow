#include <iostream>
#include <memory>
#include "dataflow.h"

void test2()
{
    try
    {
        using namespace bo;
        std::cout << "TEST 02" << std::endl;

        Node *a = createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *b = createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *c = createNode(NODE_TYPE::NODE_OF_SUM);

        connect(findPortByNameAndNode("out", a), findPortByNameAndNode("in_a", c));
        connect(findPortByNameAndNode("out", b), findPortByNameAndNode("in_b", c));

        c->evaluate(c);
        // c->evaluate(c);

        Node *d = createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *e = createNode(NODE_TYPE::NODE_OF_SUM);

        // c->port("out")->connect(e->port("in_a"));
        // d->port("out")->connect(e->port("in_b"));

        connect(findPortByNameAndNode("out", c), findPortByNameAndNode("in_a", e));
        connect(findPortByNameAndNode("out", d), findPortByNameAndNode("in_b", e));

        e->evaluate(e);

        // double *result = static_cast<double *>(e->port("out")->property()->data);
        // double *result = double *<double *>data2(e->port("out")->property()->data);
        double *result = static_cast<double *>(e->portData("out"));

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