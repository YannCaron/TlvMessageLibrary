#ifndef TLVTUPLE_H
#define TLVTUPLE_H

#include <QByteArray>
#include <QDataStream>
#include <QDebug>

class TlvTuple
{
public:
    static const uint HEADER_SIZE = 5;
    static const quint8 TYPE_STRING = 0x14;

    // constructor
    TlvTuple(const quint8, const QByteArray&);
	virtual ~TlvTuple(void);

    // accessor
    char getType() const;
    const char* getValue() const;
    int size();

    // method
    void unmarshall(QDataStream&);
    static TlvTuple marshall(QDataStream&);

    // operator
    friend QDebug operator<<(QDebug dbg, const TlvTuple& info)
    {
        dbg << "TlvTuple [type:" << QString::number(info.type, 16) << ", value:" << info.value << "]";
        return dbg;
    }

private:
    // attribute
    const quint8 type;
    const QByteArray value;
};




#endif // TLVTUPLE_H
