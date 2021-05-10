
#include "Message.hpp"

Message::Message(MsgType type, pizza_t pizza)
    : _type(type), _pizza(pizza)
{
}

Message::Message(MsgType type, bool approval)
    : _type(type), _hasApproval(approval)
{
}

Message::Message(MsgType type)
    : _type(type)
{
}

