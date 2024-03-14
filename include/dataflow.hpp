#pragma once

#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include <list>
#include <stdexcept>

namespace bo
{

    enum class DATA_TYPE
    {
        DATA_FLOAT,
        DATA_DOUBLE,
        DATA_INT,
        DATA_STRING,
        DATA_UNSET
    };
    enum class PORT_TYPE
    {
        PORT_INPUT,
        PORT_OUTPUT,
        PORT_UNSET,
        PORT_INPUT_OPT
    };
    enum class NODE_TYPE
    {
        NODE_OF_SUM,
        NODE_OF_DOUBLE_OUT,
        NODE_OF_STRING_OUT,
        NODE_OF_STRING_CONCAT,
        NODE_OF_UNSET,

    };

    class Node;
    class Property;
    // struct TreeManager;
    class Port;
    class Connection
    {
    public:
        Port *src_port = nullptr;
        Port *dst_port = nullptr;
        Node *src_node = nullptr;
        Node *dst_node = nullptr;
    };

    class Port
    {
    public:
        PORT_TYPE port_type = PORT_TYPE::PORT_UNSET;
        DATA_TYPE data_type = DATA_TYPE::DATA_UNSET;
        Node *node = nullptr;
        std::string name;
        int prop_idx = -1;

        void attachPortProp(int idx);
        void setPortName(const std::string &name);
        
    };

    class Property
    {
    public:
        DATA_TYPE type = DATA_TYPE::DATA_UNSET;
        void *data = nullptr;
    };
    class Node
    {
    public:
        NODE_TYPE type = NODE_TYPE::NODE_OF_UNSET;

        //  Property *main = nullptr;
        std::set<Node *> dest_nodes;
        // std::vector<Node *> child_nodes;
        std::vector<Property *> original_prop;
        std::vector<Property *> actual_props;

        std::vector<Port *> ports;

        void (*evaluate)(Node *) = nullptr;

        void SetOriginalProperty(int idx, Property *prop);
        void SetRemoteProperty(int idx, Property *prop);
        void RestorOriginalProperty(int idx);
        int PropertyIndex(Property *prop);
        bool IsOriginalProp(int idx, Property *prop);
        bool IsDestinationOf(Node *b);
        void SetEvaluateFn(void (*evaluate)(Node *));
        void attachNodePort(Port *port);
        void *portData(const std::string &name);
    };

    //-------GRAPH MANAGER
    struct TreeManager
    {

        std::list<Node *> nodes;
        std::list<Port *> ports;
        std::list<Property *> properties;
        std::list<Connection *> connections;
        std::unordered_map<Port *, Connection *> port_conn;
        //    std::unordered_map<Port *, std::string> port_name;
        //  std::unordered_map<Port *, Node *> port_node;
        // std::unordered_map<Node *, void (*)(Node *)> node_eval;
        //  std::unordered_map<Port *, int> port_prop_idx;
    };

    Node *createNode(NODE_TYPE type);
    Port *createPort(const std::string &name, PORT_TYPE port_type, DATA_TYPE data_type);
    Property *createProperty(void *data, DATA_TYPE);
    Port *findPortByNameAndNode(const std::string &name, Node *);
    Connection *connect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port);
    Connection *connect(Port *src_port, Port *dst_port);

    void disconnect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port);
    void disconnect(Connection *conn);

    void clear();
    //-------   
}