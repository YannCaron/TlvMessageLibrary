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

void TlvMessage::checkSize(QDataStream &stream, qint64 min)
{
    qint64 size = stream.device()->bytesAvailable();
    if (size < min) throw QString("Invalid message size [%1] but expected [%2]")
            .arg(size).arg(min);
}

void TlvMessage::check(QDataStream &stream, quint8 expected)
{
    quint8 c;
    stream >> c;
    if (c != expected) throw QString("Invalid message format, char [%1] but expected [%2]!")
            .arg(QString::number(c, 16))
            .arg(QString::number(expected, 16));
}

bool TlvMessage::containsMessage(QByteArray &buffer)
{
    QDataStream stream(buffer);
    if (buffer.size() < ENCAP_SIZE) return false;
    check(stream, SOH);

    int size;
    stream >> size;

    return buffer.size() >= size;
}

TlvMessage TlvMessage::marshall(QDataStream &stream) {

    checkSize(stream, ENCAP_SIZE);

    // check header
    check(stream, SOH);

    // extract size
    uint size;
    stream >> size;

    check(stream, STX);

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
    check(stream, ETX);
    check(stream, EOT);

    return message;
}


void TlvMessage::setValue(const TlvTuple *value)
{
    this->value = value;
}

bool TlvMessage::hasValue() const {
    return this->value != NULL;
}
