#ifndef TLVMESSAGE_H
#define TLVMESSAGE_H

#include <QByteArray>
#include <QDataStream>
#include <QDebug>

#include "TlvTuple.h"

class TlvMessage
{
public:
    static const int ENCAP_SIZE = 4 + sizeof(int);

    static const quint8 SOH = 0x01;
    static const quint8 STX = 0x02;
    static const quint8 ETX = 0x03;
    static const quint8 EOT = 0x04;

    // constructor
    TlvMessage(TlvTuple);
    TlvMessage(const TlvMessage&);
    virtual ~TlvMessage();

    // method
    void unmarshall(QDataStream&) const;

    static bool containsMessage(QByteArray& buffer);
    static TlvMessage marshall(QDataStream& stream);

    // property
    void setValue(const TlvTuple*);
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
