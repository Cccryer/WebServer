#include "message.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string Message::serialize() const
{
    json j = {
        {"type", type},
        {"message", message},
        {"content", content},
    };
    return j.dump();
}

Message* Message::deserialize(const std::string& jsonStr)
{
    json j(jsonStr);
    
    MessageType type = j["type"];
    std::string message = j["message"];
    std::string content = j["content"];
    return new Message(type, content, message);
}
