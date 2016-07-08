#include "TlvTuple.h"

TlvTuple::TlvTuple(const quint8 type, const QByteArray& value) : type(type), value(value)
{
}

TlvTuple::TlvTuple(const TlvTuple &other) : type(other.type), value(other.value) {
}

TlvTuple::~TlvTuple(void)
{
}

char TlvTuple::getType() const { return this->type; }

const char* TlvTuple::data() const { return this->value; }

int TlvTuple::size() const { return HEADER_SIZE + value.size(); }

QString TlvTuple::toString() const
{
    return QString(this->value);
}

bool TlvTuple::getBool() const
{
    bool b;
    QDataStream(value) >> b;
    return b;
}

int TlvTuple::getInt() const
{
    int i;
    QDataStream(value) >> i;
    return i;
}

void TlvTuple::unmarshall(QDataStream& stream) const
{
    stream << this->type;
    //stream << value.size();
    stream << this->value;
}

TlvTuple TlvTuple::marshall(QDataStream& stream) {
    // extract type
    quint8 type;
    stream >> type;

    // extract length
    uint size;
    stream >> size;

    // extract data
    QByteArray buffer;
    char *temp = new char[size];
    stream.readRawData(temp, size);
    buffer.append(temp, size);
    delete [] temp;

    return TlvTuple(type, buffer);
}
