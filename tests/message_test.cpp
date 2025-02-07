#include "maelstrom-node/message.hpp"

#include "gtest/gtest.h"
#include <string_view>

using maelstrom_message = maelstrom_node::message;
namespace mm            = maelstrom_node::msg_field;

TEST( Message, CommonFields )
{
    constexpr std::string_view init_msg
        = R"({"body":{"msg_id":1,"node_id":"n4","node_ids":["n1","n2","n3","n4","n5"],
        "type":"init"},"dest":"n4","id":3,"src":"c3"})";
    maelstrom_message msg( init_msg );

    EXPECT_EQ( msg.payload().get_value<mm::msg_id>(), 1 );
    EXPECT_EQ( msg.payload().get_value<mm::node_id>(), "n4" );
}

TEST( Message, InitMessage )
{
    constexpr std::string_view init_msg
        = R"({"body":{"msg_id":1,"node_id":"n4","node_ids":["n1","n2","n3","n4","n5"],
        "type":"init"},"dest":"n4","id":3,"src":"c3"})";
    maelstrom_message  msg( init_msg );
    mm::node_ids::type exp = { "n1", "n2", "n3", "n4", "n5" };

    auto ids = msg.payload().get_value<mm::node_ids>();

    EXPECT_EQ( ids, exp );
}

TEST( Message, TopologyMessage )
{
    constexpr std::string_view topo_msg
        = R"({"body":{"msg_id":1,"topology":{"n1":["n4","n2"],"n2":["n5","n3","n1"]},
        "type":"topology"},"dest":"n2","id":10,"src":"c6"})";
    maelstrom_message  msg( topo_msg );
    mm::topology::type exp = {
        {"n1",       { "n4", "n2" }},
        {"n2", { "n5", "n3", "n1" }},
    };
    auto topo = msg.payload().get_value<mm::topology>();

    EXPECT_EQ( topo, exp );
}
