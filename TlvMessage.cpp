#include "TlvMessage.h"

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

void TlvMessage::unmarshall(QDataStream &stream) const
{
    stream << SOH;
    stream << 4;
    stream << STX;
    address.unmarshall(stream);
    if (hasValue()) value->unmarshall(stream);
    stream << ETX;
    stream << EOT;
}

bool TlvMessage::containsMessage(QByteArray &buffer)
{
    QDataStream stream(buffer);
    if (buffer.size() < ENCAP_SIZE) return false;
    TlvTuple::check(stream, SOH);

    int size;
    stream >> size;

    return buffer.size() >= size;
}

TlvMessage TlvMessage::marshall(QDataStream &stream) {

    TlvTuple::checkSize(stream, ENCAP_SIZE);

    // check header
    TlvTuple::check(stream, SOH);

    // extract size
    uint size;
    stream >> size;

    TlvTuple::check(stream, STX);

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
    TlvTuple::check(stream, ETX);
    TlvTuple::check(stream, EOT);

    return message;
}


void TlvMessage::setValue(const TlvTuple *value)
{
    this->value = value;
}

bool TlvMessage::hasValue() const {
    return this->value != NULL;
}
