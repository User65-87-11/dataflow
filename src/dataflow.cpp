#include "dataflow.hpp"
#include <iostream>

/*
    output ports have own value allocated

    input ports receive poitner but dont deallocate values;

*/

std::string nodeTypeToString(NODE_TYPE type)
{
    switch (type)
    {
    case NODE_TYPE::NODE_OF_DOUBLE_OUT:
        return "NODE_OF_DOUBLE_OUT";

    case NODE_TYPE::NODE_OF_STRING_OUT:
        return "NODE_OF_STRING_OUT";

    case NODE_TYPE::NODE_OF_STRING_CONCAT:
        return "NODE_OF_STRING_CONCAT";
    case NODE_TYPE::NODE_OF_SUM:
        return "NODE_OF_SUM";
    default:
        return "NODE_OF_?";
    }
}
std::string dataTypeToString(DATA_TYPE type)
{
    switch (type)
    {
    case DATA_TYPE::DOUBLE:
        return "DATA_TYPE DOUBLE";

    case DATA_TYPE::STRING:
        return "DATA_TYPE STRING";

    case DATA_TYPE::INT:
        return "DATA_TYPE INT";

    case DATA_TYPE::FLOAT:
        return "DATA_TYPE FLOAT";

    default:
        return "DATA_TYPE?";
    }
}

inline void print1(int i)
{
    std::cout << "dbg: " << i << std::endl;
}

//---------Edge

Connection::Connection()
{
    id = genId();
    src_node_id = 0;
    src_port_id = 0;
    dst_node_id = 0;
    dst_port_id = 0;
};
void Connection::print()
{
    std::cout << "Connection id: " << id << std::endl;
    std::cout << " src_node_id: " << src_node_id << std::endl;
    std::cout << " src_port_id: " << src_port_id << std::endl;
    std::cout << " dst_node_id: " << dst_node_id << std::endl;
    std::cout << " dst_port_id: " << dst_port_id << std::endl;
}
//--------PORT
Port::Port()
{
    id = genId();
    prop_id = 0;
    port_type = PORT_TYPE::UNSET;
    data_type = DATA_TYPE::UNSET;
}

Port::Port(PORT_TYPE port_type, DATA_TYPE data_type) : Port()
{
    port_type = port_type;
    data_type = data_type;
}
// void Port::setPortNodeData(NodeData *d)
// {
//     node_data = d;
//     data_type = d->data_type;
// }
// NodeData *Port::getPortNodeData()
// {
//     return node_data;
// }

void Port::setPortPropId(int prop_id)
{
    this->prop_id = prop_id;
    this->dst_port_id = 0;
    this->prop_id = 0;
}
void Port::print()
{
    std::cout << "Port id: " << id << std::endl;
    std::cout << "  prop_id: " << this->prop_id << std::endl;
}
Port::~Port() {}

//------------PROPERTY-------
Property::Property(void *data, DATA_TYPE type)
{
    this->id = genId();
    this->data = data;
    this->type = type;
}
Property::~Property()
{
}

//---------NODE START--------------
Node::Node()
{
    id = genId();
    this->type = NODE_TYPE::NODE_OF_UNSET;
    //  port_out = nullptr;
}
void Node::setMainProperty(Property *prop)
{
    this->main = prop;
}
int Node::propertyIndex(Property *prop)
{
    int idx = -1;
    for (size_t i = 0; i < this->original_prop.size(); ++i)
    {
        // Check if the current element is equal to the target value
        if (this->original_prop[i] == prop)
        {
            idx = i; // Store the index of the target value
            break;   // Exit the loop once the target value is found
        }
    }

    return idx;
}
void Node::print()
{
    std::cout << "Node Id: " << id << std::endl;
    std::cout << "  type: " << nodeTypeToString(type) << std::endl;
}
Node::~Node(){};
void Node::evaluate() {}
bool Node::is_complete() { return completeness == 0; }

void Node::setOriginalProperty(int idx, Property *prop)
{
    this->original_prop[idx] = prop;
    this->actual_props[idx] = prop;
}
void Node::setRemoteProperty(int idx, Property *prop)
{
    this->actual_props[idx] = prop;

    completeness &= ~(1 << idx);
}
void Node::restorOriginalProperty(int idx)
{

    this->actual_props[idx] = this->original_prop[idx];
    completeness |= (1 << idx);
}

//-----------NODE_OF_DOUBLE_OUT-------
NodeOfDoubleOut::NodeOfDoubleOut()
{
    this->type = NODE_TYPE::NODE_OF_DOUBLE_OUT;
    this->actual_props.assign(1, nullptr);
    this->original_prop.assign(1, nullptr);
    completeness = 1;
}

NodeOfDoubleOut::~NodeOfDoubleOut() {}
void NodeOfDoubleOut::evaluate()
{
}

//-----------NODE_OF_STRING_OUT-------
NodeOfStringOut::NodeOfStringOut()
{
    this->type = NODE_TYPE::NODE_OF_STRING_OUT;
    this->actual_props.assign(1, nullptr);
    this->original_prop.assign(1, nullptr);
    completeness = 1;
}
NodeOfStringOut::~NodeOfStringOut() {}
void NodeOfStringOut::evaluate() {}

//-----------NodeOfSum---------
NodeOfSum::NodeOfSum()
{
    this->type = NODE_TYPE::NODE_OF_SUM;
    this->actual_props.assign(3, nullptr);
    this->original_prop.assign(3, nullptr);
    completeness = 7;
}

NodeOfSum::~NodeOfSum() {}

void NodeOfSum::evaluate()
{
    Property *in_a = this->actual_props[1];
    Property *in_b = this->actual_props[2];

    if (in_a != nullptr && in_b != nullptr)
    {
        Property *out = this->actual_props[0];

        double *d = static_cast<double *>(out->data);

        double *op1 = static_cast<double *>(in_a->data);

        double *op2 = static_cast<double *>(in_b->data);

        *d = *op1 + *op2;
    }
    else
    {
        std::cout << "operands null" << std::endl;
    }
}

//-----------NodeOfStringConcat---------
NodeOfStringConcat::NodeOfStringConcat()
{
    this->type = NODE_TYPE::NODE_OF_STRING_CONCAT;
    this->actual_props.assign(3, nullptr);
    this->original_prop.assign(3, nullptr);
    completeness = 7; // 3 bits set to complete
}
NodeOfStringConcat::~NodeOfStringConcat() {}
void NodeOfStringConcat::evaluate()
{

    Property *in_a = this->actual_props[1];
    Property *in_b = this->actual_props[2];

    if (in_a != nullptr && in_b != nullptr)
    {
        Property *out = this->actual_props[0];

        std::string *d = static_cast<std::string *>(out->data);

        std::string *op1 = static_cast<std::string *>(in_a->data);

        std::string *op2 = static_cast<std::string *>(in_b->data);

        *d = *op1 + *op2;
    }
    else
    {
        std::cout << "operands null" << std::endl;
    }
}

//---------------GRAPH MANAGER
GraphManager::GraphManager() {}
GraphManager::~GraphManager()
{
    // std::cout << "~GraphManager " << std::endl;

    // std::cout << " Nodes:" << nodes.size() << std::endl;

    // std::cout << " Edge:" << edges.size() << std::endl;

    // std::cout << " Ports:" << ports.size() << std::endl;

    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        delete it->second;
    }

    nodes.clear();

    for (auto it = connections.begin(); it != connections.end(); ++it)
    {
        delete it->second;
    }

    connections.clear();

    for (auto it = ports.begin(); it != ports.end(); ++it)
    {
        delete it->second;
    }

    ports.clear();

    for (auto it = properties.begin(); it != properties.end(); ++it)
    {
        Property *nd = it->second;
        switch (nd->type)
        {
        case DATA_TYPE::STRING:
        {
            std::string *d = static_cast<std::string *>(nd->data);
            delete d;
        }
        break;
        case DATA_TYPE::DOUBLE:
        {
            double *d = static_cast<double *>(nd->data);
            delete d;
        }
        break;
        case DATA_TYPE::INT:
        {
            int *d = static_cast<int *>(nd->data);
            delete d;
        }
        break;
        case DATA_TYPE::UNSET:
        {
        }
        break;
        }
        delete nd;
    }
    properties.clear();

    port_name_to_id.clear();
}
void GraphManager::attachPortProp(Port *port, Property *prop)
{
    port->prop_id = prop->id;
}
Port *GraphManager::createPort(const std::string &name, PORT_TYPE port_type, DATA_TYPE data_type)
{
    Port *p = new Port(port_type, data_type);
    ports[p->id] = p;

    return p;
}
Property *GraphManager::createProperty(void *data, DATA_TYPE type)
{
    Property *p = new Property(data, type);

    properties[p->id] = p;
    return p;
}

Node *GraphManager::createNode(NODE_TYPE node_type)
{
    Node *ret = nullptr;
    switch (node_type)
    {
    case NODE_TYPE::NODE_OF_STRING_OUT:
    {
        NodeOfStringOut *n = new NodeOfStringOut();
        nodes[n->id] = n;

        Port *out = createPort("out", PORT_TYPE::OUTPUT, DATA_TYPE::STRING);
        Property *main = createProperty(new std::string("Hello"), DATA_TYPE::STRING);
        n->setMainProperty(main);
        attachPortProp(out, main);

        ret = n;
    }
    break;

    case NODE_TYPE::NODE_OF_DOUBLE_OUT:
    {
        NodeOfDoubleOut *n = new NodeOfDoubleOut();
        nodes[n->id] = n;

        Port *out = createPort("out", PORT_TYPE::OUTPUT, DATA_TYPE::DOUBLE);
        Property *main = createProperty(new double(10), DATA_TYPE::DOUBLE);
        n->setMainProperty(main);
        attachPortProp(out, main);

        // n->setOriginalProperty(0, main);

        ret = n;
    }
    break;
    case NODE_TYPE::NODE_OF_STRING_CONCAT:
    {
        NodeOfStringConcat *n = new NodeOfStringConcat();
        nodes[n->id] = n;

        Port *out = createPort("out", PORT_TYPE::OUTPUT, DATA_TYPE::STRING);
        Port *in_a = createPort("in_a", PORT_TYPE::INPUT, DATA_TYPE::STRING);
        Port *in_b = createPort("in_b", PORT_TYPE::INPUT, DATA_TYPE::STRING);

        Property *main = createProperty(new std::string("default"), DATA_TYPE::STRING);
        Property *sum = createProperty(new std::string(""), DATA_TYPE::STRING);
        Property *prop_a = createProperty(nullptr, DATA_TYPE::STRING);
        Property *prop_b = createProperty(nullptr, DATA_TYPE::STRING);

        n->setMainProperty(main);

        attachPortProp(out, sum);
        attachPortProp(in_a, prop_a);
        attachPortProp(in_b, prop_b);

        n->setOriginalProperty(0, sum);
        n->setOriginalProperty(1, prop_a);
        n->setOriginalProperty(2, prop_b);

        ret = n;
    }
    break;

    case NODE_TYPE::NODE_OF_SUM:
    {
        NodeOfSum *n = new NodeOfSum();
        nodes[n->id] = n;

        Port *out = createPort("out", PORT_TYPE::OUTPUT, DATA_TYPE::DOUBLE);
        Port *in_a = createPort("in_a", PORT_TYPE::INPUT, DATA_TYPE::DOUBLE);
        Port *in_b = createPort("in_b", PORT_TYPE::INPUT, DATA_TYPE::DOUBLE);

        Property *main = createProperty(new double(123), DATA_TYPE::DOUBLE);
        Property *sum = createProperty(new double(0), DATA_TYPE::DOUBLE);
        Property *prop_a = createProperty(nullptr, DATA_TYPE::DOUBLE);
        Property *prop_b = createProperty(nullptr, DATA_TYPE::DOUBLE);

        n->setMainProperty(main);

        attachPortProp(out, sum);
        attachPortProp(in_a, prop_a);
        attachPortProp(in_b, prop_b);

        n->setOriginalProperty(0, sum);
        n->setOriginalProperty(1, prop_a);
        n->setOriginalProperty(2, prop_b);

        ret = n;
    }
    break;

    default:

        std::cout << "createNode " << nodeTypeToString(node_type) << std::endl;
        break;
    }

    return ret;
}

Connection *GraphManager::connect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port)
{
    Connection *ret = new Connection();
    connections[ret->id] = ret;
    ret->src_node_id = src_node->id;
    ret->dst_node_id = dst_node->id;

    ret->src_port_id = src_port->id;
    ret->dst_port_id = dst_port->id;

    src_node->dest_nodes.insert(dst_node);

    // change actual property

    Property *out = properties.at(src_port->prop_id);
    Property *in = properties.at(dst_port->prop_id);

    int idx = dst_node->propertyIndex(in);

    if (idx == -1)
    {
        throw std::runtime_error("Index -1 error occurred!");
    }

    dst_node->setRemoteProperty(idx, out);

    return ret;
}

void GraphManager::disconnect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port)
{
}

void GraphManager::disconnect(Connection *connection)
{
}