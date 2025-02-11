#include "maelstrom-node/message.hpp"

#include "gtest/gtest.h"
#include <memory>
#include <string_view>

using maelstrom_message = maelstrom_node::message;
namespace mm            = maelstrom_node::msg_field;

class MessageTest : public testing::Test
{
public:
    MessageTest()
    {
        init_msg_ = std::make_shared<maelstrom_node::message>( init_msg_str_ );
    }

protected:
    static constexpr std::string_view init_msg_str_
        = R"({"body":{"msg_id":1,"node_id":"n4","node_ids":["n1","n2","n3","n4","n5"],
        "type":"init"},"dest":"n4","id":3,"src":"c3"})";
    maelstrom_node::message_ptr init_msg_;
};

TEST_F( MessageTest, WriteReadRaw )
{
    EXPECT_EQ( init_msg_->get_value_raw<std::string>( "node_id" ), "n4" );

    constexpr std::string_view new_val = "42";
    init_msg_->set_value_raw( "node_id", new_val );
    EXPECT_EQ( init_msg_->get_value_raw<std::string>( "node_id" ), new_val );
}

TEST_F( MessageTest, WriteTest )
{
    EXPECT_EQ( init_msg_->get_value<mm::data_type>(), "init" );
    init_msg_->set_value<mm::data_type>( "init_ok" );
    EXPECT_EQ( init_msg_->get_value<mm::data_type>(), "init_ok" );

    EXPECT_EQ( init_msg_->get_value<mm::source>(), "c3" );
    init_msg_->set_value<mm::source>( "c42" );
    EXPECT_EQ( init_msg_->get_value<mm::source>(), "c42" );
}

TEST_F( MessageTest, MakeReplay )
{
    constexpr std::string_view init_msg
        = R"({"body":{"msg_id":1,"node_id":"n4","node_ids":["n1","n2","n3","n4","n5"],
        "type":"init"},"dest":"n4","id":3,"src":"c3"})";
    maelstrom_message msg( init_msg );
    auto              repl = msg.make_reply();

    EXPECT_EQ( repl->get_value<mm::source>(), msg.get_value<mm::destination>() );
    EXPECT_EQ( repl->get_value<mm::destination>(), msg.get_value<mm::source>() );
    EXPECT_EQ( repl->get_value<mm::destination>(), msg.get_value<mm::source>() );
    EXPECT_EQ( repl->get_value<mm::in_reply_to>(), msg.get_value<mm::msg_id>() );
    EXPECT_EQ( repl->get_value<mm::data_type>(), "init_ok" );

    auto repl_nok = init_msg_->make_reply( false );
    EXPECT_EQ( repl_nok->get_value<mm::data_type>(), "" );
}

TEST_F( MessageTest, CommonFields )
{
    EXPECT_EQ( init_msg_->get_value<mm::msg_id>(), 1 );
    EXPECT_EQ( init_msg_->get_value<mm::node_id>(), "n4" );
}

TEST_F( MessageTest, InitMessage )
{
    mm::node_ids::type exp = { "n1", "n2", "n3", "n4", "n5" };

    auto ids = init_msg_->get_value<mm::node_ids>();

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
    auto topo = msg.get_value<mm::topology>();

    EXPECT_EQ( topo, exp );
}
