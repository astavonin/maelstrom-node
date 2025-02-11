#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace maelstrom_node
{

namespace msg_field
{

struct node_id {
    using type = std::string;

    static constexpr std::string_view type_name() { return "node_id"; }
};

struct node_ids {
    using type = std::vector<std::string>;

    static constexpr std::string_view type_name() { return "node_ids"; }
};

struct topology {
    using type = std::unordered_map<std::string, std::vector<std::string>>;

    static constexpr std::string_view type_name() { return "topology"; }
};

struct msg_id {
    using type = int;

    static constexpr std::string_view type_name() { return "msg_id"; }
};

struct source {
    using type = std::string;

    static constexpr std::string_view type_name() { return "src"; }
};

struct destination {
    using type = std::string;

    static constexpr std::string_view type_name() { return "dest"; }
};

struct id {
    using type = int;

    static constexpr std::string_view type_name() { return "id"; }
};

struct data_type {
    using type = std::string;

    static constexpr std::string_view type_name() { return "type"; }
};

struct in_reply_to {
    using type = int;

    static constexpr std::string_view type_name() { return "in_reply_to"; }
};

template <typename T>
concept is_message_field
    = std::is_same_v<T, msg_field::source> || std::is_same_v<T, msg_field::destination>
      || std::is_same_v<T, msg_field::id>;

} // namespace msg_field

class message;
using message_ptr = std::shared_ptr<message>;

class message
{
public:
    message();
    explicit message( std::string_view data );
    ~message()                            = default;
    message( const message &msg )         = default;
    message( message && )                 = delete;
    message &operator=( const message & ) = default;
    message &operator=( message && )      = delete;

    [[nodiscard]] std::string as_string();

    template <typename T>
    [[nodiscard]] auto get_value() const
    {
        if constexpr( msg_field::is_message_field<T> ) {
            return data_.value( T::type_name(), typename T::type{} );
        } else {
            return data_["body"].value( T::type_name(), typename T::type{} );
        }
    }

    template <typename T>
    void set_value( T::type value )
    {
        if constexpr( msg_field::is_message_field<T> ) {
            data_[T::type_name()] = value;
        } else {
            data_["body"][T::type_name()] = value;
        }
    }

    template <typename T>
    T get_value_raw( std::string_view field_name, bool is_payload = true )
    {
        if( is_payload ) { return data_["body"].value( field_name, T{} ); }

        return data_.value( field_name, T{} );
    }

    template <typename T>
    void set_value_raw( std::string_view field_name, T value, bool is_payload = true )
    {
        if( is_payload ) {
            data_["body"][field_name] = value;
        } else {
            data_[field_name] = value;
        }
    }

    [[nodiscard]] message_ptr make_reply( bool send_ok = true ) const;

private:
    nlohmann::json data_;
};

class sender
{
public:
    sender()                            = default;
    virtual ~sender()                   = default;
    sender( sender && )                 = default;
    sender( const sender & )            = default;
    sender &operator=( sender && )      = default;
    sender &operator=( const sender & ) = default;

    virtual void                      send( message_ptr message ) = 0;
    [[nodiscard]] virtual std::string node_id() const             = 0;
};

} // namespace maelstrom_node
