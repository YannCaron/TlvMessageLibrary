#ifndef TLVTUPLE_H
#define TLVTUPLE_H

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>

class TlvTuple
{
public:
    static const uint HEADER_SIZE = 5;

    static const quint8 TYPE_STATUS = 0x05;
    static const quint8 TYPE_ACK = 0x06;
    static const quint8 TYPE_ADDRESS = 0x10;

    static const quint8 TYPE_BOOL = 0x11;
    static const quint8 TYPE_INT = 0x12;
    static const quint8 TYPE_FLOAT = 0x13;
    static const quint8 TYPE_STRING = 0x14;

    // constructor
    TlvTuple(const TlvTuple&); // copy

    TlvTuple(const quint8, const QByteArray&);
    TlvTuple(const quint8, bool);
    TlvTuple(bool);
    TlvTuple(const quint8, const QString&);
    TlvTuple(const QString&);

    virtual ~TlvTuple(void);

    // accessor
    char getType() const;
    const char* data() const;
    int size() const;
    QString toString() const;
    bool getBool() const;
    int getInt() const;
    QString getString() const;

    // method
    static void checkSize(QDataStream& stream, qint64 min);
    static void check(QDataStream& stream, quint8 expected);

    void unmarshall(QDataStream&) const;
    static TlvTuple marshall(QDataStream&);

    // operator
    friend QDebug operator<<(QDebug dbg, const TlvTuple& info)
    {
        dbg << "TlvTuple [type:" << QString::number(info.type, 16) << ", value:";

        if (info.type == TlvTuple::TYPE_BOOL) {
            dbg << info.getBool();
        } else if (info.type == TlvTuple::TYPE_INT) {
            dbg << info.getInt();
        } else if (info.type == TlvTuple::TYPE_ADDRESS || info.type == TlvTuple::TYPE_STRING) {
            dbg << info.value;
        }

        dbg << "]";
        return dbg;
    }

private:
    // attribute
    const quint8 type;
    const QByteArray value;
};




#endif // TLVTUPLE_H
