#include "TlvMessage.h"

//TlvMessage::TlvMessage() {}

TlvMessage::TlvMessage(TlvTuple address) : address(address), value(NULL)
{

}

TlvMessage::TlvMessage(const TlvMessage &other) : address(other.address), value(other.value)
{

}

TlvMessage::~TlvMessage()
{
    delete this->value;
    this->value = NULL;
}

bool TlvMessage::check(QDataStream &stream, quint8 expected)
{
    quint8 c;
    stream >> c;
    if (c == expected) return true;
    return false;
}

TlvMessage TlvMessage::marshall(QDataStream &stream)
{
    // chack header
    if (!check(stream, SOH)) throw "Invalid message format!";

    // extract size
    uint size;
    stream >> size;

    if (!check(stream, STX)) throw "Invalid message format!";

    // extract address
    TlvTuple address = TlvTuple::marshall(stream);
    TlvMessage message(address);

    // extract value
    if (address.getType() == TlvTuple::TYPE_ACK) {
    } else if (address.getType() == TlvTuple::TYPE_ADDRESS) {
        TlvTuple value = TlvTuple::marshall(stream);
        message.setValue(new TlvTuple(value));
    } else throw "Invalid message tuple type!";

    // check footer
    if (!check(stream, ETX)) throw "Invalid message format!";
    if (!check(stream, EOT)) throw "Invalid message format!";

    return message;
}

void TlvMessage::setValue(const TlvTuple *value)
{
    this->value = value;
}

bool TlvMessage::hasValue() const {
    return this->value != NULL;
}
