#include "maelstrom-node/message.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <format>
#include <spdlog/spdlog.h>
#include <stdexcept>

using json = nlohmann::json;

namespace maelstrom_node
{

message::message()
{
    data_["body"] = nullptr;
}

message::message( std::string_view data )
{
    init( data );
}

void message::init( std::string_view data )
{
    try {
        data_ = json::parse( data );

        auto fields = std::vector{ "body", "src", "dest" };

        if( !std::ranges::all_of( fields, [this]( const auto &field ) {
                return data_.contains( field );
            } ) ) {
            throw std::logic_error( std::format(
                "invalid object \"{}\", one of {} is missing", data, fields ) );
        }
    } catch( json::exception &ex ) {
        throw std::logic_error(
            std::format( "unable to parce: \"{}\", error: {}", data, ex.what() ) );
    }
}

[[nodiscard]] std::string message::as_string()
{
    auto msg_string = data_.dump();
    boost::trim_left( msg_string );
    boost::trim_right( msg_string );
    return msg_string;
}

[[nodiscard]] message_ptr message::make_reply() const
{
    auto replay = std::make_shared<message>();

    replay->data_["src"]                 = data_.at( "dest" );
    replay->data_["dest"]                = data_.at( "src" );
    replay->data_["body"]["in_reply_to"] = data_["body"].at( "msg_id" );

    return replay;
}

} // namespace maelstrom_node
