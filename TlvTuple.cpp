#include "TlvTuple.h"

TlvTuple::TlvTuple(const quint8 type, const QByteArray& value) : type(type), value(value)
{
}

TlvTuple::~TlvTuple(void)
{
}

char TlvTuple::getType() const { return this->type; }

const char* TlvTuple::getValue() const { return this->value; }

int TlvTuple::size() { return HEADER_SIZE + value.size(); }

void TlvTuple::unmarshall(QDataStream& stream)
{
    stream << this->type;
	//stream << sizeof(value);
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
    if (type == TYPE_STRING) {
        char *temp = new char[size];
        stream.readRawData(temp, size);
        buffer.append(temp, size);
        delete [] temp;
    } else {
        // TODO manage exception
        qDebug() << "Unknown type: " << type;
    }

    return TlvTuple(type, buffer.data());
}
