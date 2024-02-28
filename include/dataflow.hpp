#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <memory>

#pragma once

enum class DATA_TYPE
{
    FLOAT,
    DOUBLE,
    INT,
    STRING,
    UNSET
};
enum class NODE_TYPE
{
    NODE_OF_SUM,
    NODE_OF_DOUBLE_OUT,
    NODE_OF_STRING_OUT,
    NODE_OF_STRING_CONCAT,

};

enum class PORT_TYPE
{
    INPUT,
    OUTPUT,
    UNSET,
    INPUT_OPT
};

class Edge
{

public:
    Edge();
    int edge_id;
    int src_port_id;
    int dst_port_id;
    void print();
    // Port * src;
    // Port * dst;
};
class Node;
class NodeData;
class Property;
class Port
{
public:
    PORT_TYPE port_type;
    DATA_TYPE data_type;
    Port *remote_port;
  //  NodeData *node_data;

    Property * prop;
    int port_id;
    int edge_id;

    Port();
 //Port(PORT_TYPE port_type, NodeData *d);
    Port(PORT_TYPE port_type);
    Port(PORT_TYPE port_type, DATA_TYPE t);
    ~Port();
    void print();
   // void setPortNodeData(NodeData *);
    Port* setPortProperty(Property *);
  //  NodeData *getPortNodeData();
    Property *getPortProperty();
};

class NodeData
{
public:
    void *data;
    DATA_TYPE data_type;

    NodeData();
    NodeData(void *d, DATA_TYPE t);
    ~NodeData();
};
class Property
{
public:
    std::string name;
    NodeData *own_data;
    NodeData *remote_data;

    Property(const std::string &n, NodeData *own_data);
    ~Property();
};
class Node
{

public:
    int node_id;
    bool complete;
    NODE_TYPE node_type;

    std::vector<std::unique_ptr<Port>> ports_out;
    std::vector<std::unique_ptr<Port>> ports_in;
    std::vector<std::unique_ptr<NodeData>> node_data;

    std::vector<std::unique_ptr<Property>> props;

    // std::vector<Port> mod_ports_in;
    // std::vector<NodeData*> conn_data;

    Node();
    ~Node();
    void print();
    virtual void evaluate();
    Port *addOutputPort(DATA_TYPE type);
    Port *addOutputPort();
    NodeData *addNodeData(void *d, DATA_TYPE t);
    Property *addProperty(const std::string &name, NodeData *own);
    Port *addInputPort(DATA_TYPE type);
    Port *addInputPort();

    Port *addModInputPort(DATA_TYPE type);

    Port *getPortOutByPos(int pos);
    Port *getInputPortByPos(int pos);
};
class NodeOfDoubleOut : public Node
{
public:
    NodeOfDoubleOut();
    ~NodeOfDoubleOut();
    void evaluate() override;
};

class NodeOfStringOut : public Node
{
public:
    NodeOfStringOut();
    ~NodeOfStringOut();
    void evaluate() override;
};

class NodeOfSum : public Node
{

public:
    NodeOfSum();
    ~NodeOfSum();

    void evaluate() override;
};

class NodeOfStringConcat : public Node
{

public:
    NodeOfStringConcat();
    ~NodeOfStringConcat();

    void evaluate() override;
};

//-------GRAPH MANAGER
class GraphManager
{
public:
    GraphManager();
    ~GraphManager();
    std::map<int, Edge *> edges;
    std::map<int, Node *> nodes;
    std::map<int, Port *> ports;
    std::map<int, NodeData *> node_datas;

    Node *createNode(NODE_TYPE node_type);
    Edge *connectPorts(Port *src_port_id, Port *dest_port_id);
    void regNodeData(NodeData *nd);
    void disconnectPorts(Edge *edge);

private:
    void portAdd(Port *);
    void nodeAdd(Node *);
    Port *getPortIfExist(int port_id);
    Edge *getEdgeOrNull(int edge_id);
};

inline int genId()
{
    static int cnt = 0;
    cnt++;
    return cnt;
};