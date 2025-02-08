#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <unordered_map>

namespace maelstrom_node
{

class message;
using message_ptr = std::shared_ptr<message>;

namespace msg_field
{

struct node_id {
    using type = std::string;

    static constexpr const char *type_name() { return "node_id"; }
};

struct node_ids {
    using type = std::vector<std::string>;

    static constexpr const char *type_name() { return "node_ids"; }
};

struct topology {
    using type = std::unordered_map<std::string, std::vector<std::string>>;

    static constexpr const char *type_name() { return "topology"; }
};

struct msg_id {
    using type = int;

    static constexpr const char *type_name() { return "msg_id"; }
};

} // namespace msg_field

class payload
{
public:
    payload() = default;

    template <typename T>
    [[nodiscard]] auto get_value( const std::string_view field ) const
    {
        return data_.value( field, T{} );
    }

    template <typename T>
    [[nodiscard]] auto get_value() const
    {
        return data_.value( T::type_name(), typename T::type{} );
    }

    template <typename T>
    void set_value( const std::string_view field, T value )
    {
        data_[field] = value;
    }

    [[nodiscard]] std::string type() const;
    void                      type( const std::string &type );
    [[nodiscard]] std::string node_id() const;

private:
    friend class message;

    void data( nlohmann::json &data ) { data_ = data; }

    nlohmann::json &data() { return data_; }

    nlohmann::json data_;
};

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

    [[nodiscard]] std::string source() const { return data_.value( "src", "" ); }

    void source( const std::string_view &src ) { data_["src"] = src; }

    [[nodiscard]] std::string destination() const { return data_.value( "dest", "" ); }

    void destination( const std::string_view &dst ) { data_["dest"] = dst; }

    payload &payload() { return payload_; }

    [[nodiscard]] message_ptr make_replay() const;

protected:
    void init( std::string_view data );

private:
    nlohmann::json data_;

    class payload payload_;
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

    virtual void send( message_ptr message ) = 0;
};

} // namespace maelstrom_node
