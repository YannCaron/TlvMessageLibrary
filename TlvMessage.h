#ifndef TLVMESSAGE_H
#define TLVMESSAGE_H

#include <QByteArray>
#include <QDataStream>
#include <QDebug>

#include "TlvTuple.h"

class TlvMessage
{
public:
    static const uint ENCAP_SIZE = 5;

    static const quint8 SOH = 0x01;
    static const quint8 STX = 0x02;
    static const quint8 ETX = 0x03;
    static const quint8 EOT = 0x04;

    // constructor
    TlvMessage(TlvTuple address);
    TlvMessage(const TlvMessage& other);
    virtual ~TlvMessage();

    // method
    void unmarshall(QDataStream& stream) const
    {
        stream << SOH;
        stream << 4;
        stream << STX;
        address.unmarshall(stream);
        if (hasValue()) value->unmarshall(stream);
        stream << ETX;
        stream << EOT;
    }

    static bool check(QDataStream& stream, quint8 expected);
    static TlvMessage marshall(QDataStream& stream);

    // property
    void setValue(const TlvTuple* value);
    bool hasValue() const;

    // operator
    friend QDebug operator<<(QDebug dbg, const TlvMessage& info)
    {
        dbg << "TlvMessage [";

        dbg << info.address;
        if (info.hasValue()) dbg << ", " << *info.value;

        dbg << "]";
        return dbg;
    }

private:
    TlvTuple address;
    const TlvTuple* value;

};

#endif // TLVMESSAGE_H
