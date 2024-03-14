#include "dataflow.hpp"
#include <functional>
#include <iostream>

/*
    output ports have own value allocated

    input ports receive poitner but dont deallocate values;

*/

namespace bo
{

    struct TreeManager TreeManager
    {
    };

    inline void print1(int i)
    {
        std::cout << "dbg: " << i << std::endl;
    }

    void Node::SetOriginalProperty(int idx, Property *prop)
    {
        Node &node = *this;
        if (idx >= node.original_prop.size())
        {
            node.original_prop.resize(idx + 1);
            node.actual_props.resize(idx + 1);
        }
        node.original_prop[idx] = prop;
        node.actual_props[idx] = prop;
    }
    void Node::SetRemoteProperty(int idx, Property *prop)
    {
        this->actual_props[idx] = prop;
    }
    void Node::RestorOriginalProperty(int idx)
    {
        Node &node = *this;
        node.actual_props[idx] = node.original_prop[idx];
    }
    int Node::PropertyIndex(Property *prop)
    {
        Node &node = *this;
        for (int i = 0; i < node.actual_props.size(); i++)
        {
            if (node.actual_props[i] == prop)
                return i;
        }
        return -1;
    }
    bool Node::IsOriginalProp(int idx, Property *prop)
    {
        Node &node = *this;
        if (node.original_prop[idx] == prop)
            return true;

        return false;
    }
    bool Node::IsDestinationOf(Node *b)
    {

        for (Node *dest : b->dest_nodes)
        {
            if (dest == this)
            {
                return true;
            }
            if (IsDestinationOf(dest))
            {
                return true;
            }
        }
        return false;
    }
    void NodeOfStringConcat_evaluate(Node *node)
    {
        std::cout << "NodeOfStringConcat::evaluate" << std::endl;
        Node &n = *node;
        void *in_a_data = n.actual_props[1]->data;
        void *in_b_data = n.actual_props[2]->data;

        if (in_a_data != nullptr && in_b_data != nullptr)
        {
            void *out = n.actual_props[0]->data;

            std::string *d = static_cast<std::string *>(out);

            std::string *op1 = static_cast<std::string *>(in_a_data);

            std::string *op2 = static_cast<std::string *>(in_b_data);

            *d = *op1 + *op2;
        }
        else
        {
            std::cout << "operands null" << std::endl;
        }
    }
    void NodeOfSum_evaluate(Node *node)
    {
        std::cout << "NodeOfSum::evaluate" << std::endl;

        Node &n = *node;

        void *in_a_data = n.actual_props[1]->data;
        void *in_b_data = n.actual_props[2]->data;

        if (in_a_data != nullptr && in_b_data != nullptr)
        {

            void *out = n.actual_props[0]->data;
            double *d = static_cast<double *>(out);

            double *op1 = static_cast<double *>(in_a_data);

            double *op2 = static_cast<double *>(in_b_data);

            *d = *op1 + *op2;
        }
        else
        {
            std::cout << "operands null" << std::endl;
        }
    }
    Node *createNode(NODE_TYPE type)
    {
        std::cout << "FIX HERE" << std::endl;
        Node *node = new Node();
        node->type = type;

        switch (type)
        {
        case NODE_TYPE::NODE_OF_STRING_OUT:
        {

            Port *out = createPort("out", PORT_TYPE::PORT_OUTPUT, DATA_TYPE::DATA_STRING);
            Property *main = createProperty(new std::string("Hello"), DATA_TYPE::DATA_STRING);
            node->SetOriginalProperty(0, main);
            out->attachPortProp(0);
            node->attachNodePort(out);
        }
        break;

        case NODE_TYPE::NODE_OF_DOUBLE_OUT:
        {
            std::cout << "FIX 1" << std::endl;
            Port *out = createPort("out", PORT_TYPE::PORT_OUTPUT, DATA_TYPE::DATA_DOUBLE);
            std::cout << "FIX 1" << std::endl;
            Property *main = createProperty(new double(10), DATA_TYPE::DATA_DOUBLE);
            std::cout << "FIX 1" << std::endl;
            node->SetOriginalProperty(0, main);
            std::cout << "FIX 1" << std::endl;
            out->attachPortProp(0);
            std::cout << "FIX 1" << std::endl;
            node->attachNodePort(out);
            std::cout << "FIX 1" << std::endl;
        }
        break;
        case NODE_TYPE::NODE_OF_STRING_CONCAT:
        {

            Port *out = createPort("out", PORT_TYPE::PORT_OUTPUT, DATA_TYPE::DATA_STRING);
            Port *in_a = createPort("in_a", PORT_TYPE::PORT_INPUT, DATA_TYPE::DATA_STRING);
            Port *in_b = createPort("in_b", PORT_TYPE::PORT_INPUT, DATA_TYPE::DATA_STRING);

            Property *sum = createProperty(new std::string(""), DATA_TYPE::DATA_STRING);
            Property *prop_a = createProperty(nullptr, DATA_TYPE::DATA_STRING);
            Property *prop_b = createProperty(nullptr, DATA_TYPE::DATA_STRING);

            out->attachPortProp(0);
            in_a->attachPortProp(1);
            in_b->attachPortProp(2);

            node->attachNodePort(out);
            node->attachNodePort(in_a);
            node->attachNodePort(in_b);

            node->SetOriginalProperty(0, sum);
            node->SetOriginalProperty(1, prop_a);
            node->SetOriginalProperty(2, prop_b);
            node->evaluate = NodeOfStringConcat_evaluate;
        }
        break;

        case NODE_TYPE::NODE_OF_SUM:
        {

            Port *out = createPort("out", PORT_TYPE::PORT_OUTPUT, DATA_TYPE::DATA_DOUBLE);
            Port *in_a = createPort("in_a", PORT_TYPE::PORT_INPUT, DATA_TYPE::DATA_DOUBLE);
            Port *in_b = createPort("in_b", PORT_TYPE::PORT_INPUT, DATA_TYPE::DATA_DOUBLE);

            Property *sum = createProperty(new double(0), DATA_TYPE::DATA_DOUBLE);
            Property *prop_a = createProperty(nullptr, DATA_TYPE::DATA_DOUBLE);
            Property *prop_b = createProperty(nullptr, DATA_TYPE::DATA_DOUBLE);

            out->attachPortProp(0);
            in_a->attachPortProp(1);
            in_b->attachPortProp(2);

            node->attachNodePort(out);
            node->attachNodePort(in_a);
            node->attachNodePort(in_b);

            node->SetOriginalProperty(0, sum);
            node->SetOriginalProperty(1, prop_a);
            node->SetOriginalProperty(2, prop_b);

            node->evaluate = NodeOfSum_evaluate;
        }
        break;

        default:

            break;
        }

        TreeManager.nodes.push_back(node);
        return node;
    }
    void Node::SetEvaluateFn(void (*ev)(Node *))
    {
        // TreeManager.node_eval[n] = ev;
        this->evaluate = ev;
    }
    Port *createPort(const std::string &name, PORT_TYPE port_type, DATA_TYPE data_type)
    {
        Port *port = new Port();
        port->data_type = data_type;
        port->port_type = port_type;
        port->name = name;

        TreeManager.ports.push_back(port);

        return port;
    }
    Property *createProperty(void *data, DATA_TYPE type)
    {
        Property *prop = new Property();
        prop->data = data;
        prop->type = type;
        TreeManager.properties.push_back(prop);

        return prop;
    }
    void Port::attachPortProp(int idx)
    {
        this->prop_idx = idx;
        //  TreeManager.port_prop_idx[port] = idx;
    }
    void Node::attachNodePort(Port *port)
    {
        port->node = this;
        this->ports.push_back(port);
        // TreeManager.port_node[port] = node;
    }

    void Port::setPortName(const std::string &name)
    {
        this->name = name;
        // TreeManager.port_name[port] = name;
    }
    void *Node::portData(const std::string &name)
    {
        Port *p = findPortByNameAndNode(name, this);
        Property *pr = this->actual_props[p->prop_idx];
        return pr->data;
    }
    Port *findPortByNameAndNode(const std::string &name, Node *node)
    {

        for (auto it = node->ports.begin(); it != node->ports.end(); ++it)
        {
            if ((*it)->name == name)
            {
                //  std::cout << "add: " <<it->first   << std::endl;
                return *it;
            }
        }

        return nullptr;
    }
    Connection *connect(Port *src_port, Port *dst_port)
    {
        Node *src_node = src_port->node;
        Node *dst_node = dst_port->node;
        return connect(src_node, src_port, dst_node, dst_port);
    }
    Connection *connect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port)
    {

        if (TreeManager.port_conn.find(src_port) != TreeManager.port_conn.end() && TreeManager.port_conn.find(dst_port) != TreeManager.port_conn.end())
        {
            std::cout << "connecton exist! " << std::endl;
            std::cout << "  src: " << src_port << std::endl;
            std::cout << "  dst: " << dst_port << std::endl;

            return nullptr;
        }
        Connection *ret = new Connection();
        TreeManager.connections.push_back(ret);
        TreeManager.port_conn[src_port] = ret;
        TreeManager.port_conn[dst_port] = ret;

        ret->src_node = src_node;
        ret->dst_node = dst_node;

        ret->src_port = src_port;
        ret->dst_port = dst_port;

        src_node->dest_nodes.insert(dst_node);

        // change actual property

        Property *out = src_node->actual_props[src_port->prop_idx];
        Property *in = dst_node->actual_props[dst_port->prop_idx];

        dst_node->actual_props[dst_port->prop_idx] = out;

        dst_node->evaluate(dst_node);

        return ret;
    }

    void disconnect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port)
    {

        Connection *con1 = TreeManager.port_conn[src_port];
        Connection *con2 = TreeManager.port_conn[dst_port];
        if (con1 == con2 && con1 != nullptr)
        {
            src_node->dest_nodes.erase(dst_node);

            dst_node->RestorOriginalProperty(dst_port->prop_idx);
            delete con1;
            TreeManager.connections.remove(con1);
            TreeManager.port_conn[src_port] = nullptr;
            TreeManager.port_conn[dst_port] = nullptr;

            dst_node->evaluate(dst_node);
        }
        else
        {
            std::cout << "not connected" << std::endl;
        }
    }
    void disconnect(Connection *conn)
    {
        Node *src_node = conn->src_node;
        Port *src_port = conn->src_port;

        Node *dst_node = conn->dst_node;
        Port *dst_port = conn->dst_port;

        disconnect(src_node, src_port, dst_node, dst_port);
    }

    //---------------GRAPH MANAGER
    void clear()
    {

        for (auto it = TreeManager.nodes.begin(); it != TreeManager.nodes.end(); ++it)
        {
            delete *it;
        }

        TreeManager.nodes.clear();

        for (auto it = TreeManager.connections.begin(); it != TreeManager.connections.end(); ++it)
        {
            delete *it;
        }

        TreeManager.connections.clear();

        for (auto it = TreeManager.ports.begin(); it != TreeManager.ports.end(); ++it)
        {
            delete *it;
        }

        TreeManager.ports.clear();

        for (auto it = TreeManager.properties.begin(); it != TreeManager.properties.end(); ++it)
        {
            Property *nd = *it;
            switch (nd->type)
            {
            case DATA_TYPE::DATA_STRING:
            {
                std::string *d = static_cast<std::string *>(nd->data);
                delete d;
            }
            break;
            case DATA_TYPE::DATA_DOUBLE:
            {
                double *d = static_cast<double *>(nd->data);
                delete d;
            }
            break;
            case DATA_TYPE::DATA_INT:
            {
                int *d = static_cast<int *>(nd->data);
                delete d;
            }
            break;
            case DATA_TYPE::DATA_UNSET:
            {
            }
            break;
            }
            delete nd;
        }
        TreeManager.properties.clear();
    }

}