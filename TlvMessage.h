#ifndef TLVMESSAGE_H
#define TLVMESSAGE_H

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QList>

//#include "Global.h"
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
    TlvMessage();
    TlvMessage(const TlvMessage&);
    virtual ~TlvMessage();

    // method
    TlvMessage* append(TlvTuple*);
    const TlvTuple* at(int);

    void unmarshall(QDataStream&) const;

    // static
    static int nextMessageEnd(QByteArray&);
    static TlvMessage* marshall(QDataStream&);

    // property
    int size() const;
    int countTuples() const;

    // operator
    friend QDebug operator<<(QDebug dbg, const TlvMessage& info)
    {
        dbg << "TlvMessage [";

        for (int i = 0; i < info.tuples.size(); i++) {
            if (i > 0) dbg << ", ";
            dbg << *(info.tuples.at(i));
        }

        dbg << "]";
        return dbg;
    }

private:
    QList<TlvTuple*> tuples;

};

#endif // TLVMESSAGE_H
