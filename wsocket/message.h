#include "../gamebox/jsoner.h"
#include <string>


enum class MessageType{
    GAME_STATE,
    MOVE,
    GAME_END,  
    UNKNOWN,
};


class Message
{
    
    public:
        MessageType type;
        // uint32_t id;
        std::string message;
        std::string content; //包含游戏具体指令
    
    public:
        Message(MessageType _type, const std::string& _content, const std::string& _msg = ""):
            type(_type), content(_content), message(_msg){}
        virtual ~Message() = default;

    public:
        virtual std::string serialize() const;
        static Message* deserialize(const std::string& jsonStr);
};