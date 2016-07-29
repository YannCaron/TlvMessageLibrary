#include "TlvTuple.h"

QByteArray toByteArray(bool value) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);

    stream << value;

    return byteArray;
}

TlvTuple::TlvTuple(const TlvTuple& other) : type(other.type), value(other.value) {}

TlvTuple::TlvTuple(const quint8 type, const QByteArray& value) : type(type), value(value) {}

TlvTuple::TlvTuple(const quint8 type, bool value) : type(type), value(toByteArray(value == true ? 0x01 : 0x00)) {}
TlvTuple::TlvTuple(bool value) : type(TlvTuple::TYPE_BOOL), value(toByteArray(value == true ? 0x01 : 0x00)) {}

TlvTuple::TlvTuple(const quint8 type, const QString& value) : type(type), value(value.toLatin1().data()) {}
TlvTuple::TlvTuple(const QString& value) : type(TlvTuple::TYPE_STRING), value(value.toLatin1().data()) {}

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

QString TlvTuple::getString() const
{
    return QString(value);
}

void TlvTuple::checkSize(QDataStream &stream, qint64 min)
{
    qint64 size = stream.device()->bytesAvailable();
    if (size < min) throw QString("Invalid message size [%1] but expected [%2]")
            .arg(size).arg(min);
}

void TlvTuple::check(QDataStream &stream, quint8 expected)
{
    quint8 c;
    stream >> c;
    if (c != expected) throw QString("Invalid message format, char [%1] but expected [%2]!")
            .arg(QString::number(c, 16))
            .arg(QString::number(expected, 16));
}

void TlvTuple::unmarshall(QDataStream& stream) const
{
    stream << this->type;
    //stream << value.size();
    stream << this->value;
}

TlvTuple* TlvTuple::marshall(QDataStream& stream) {

    checkSize(stream, HEADER_SIZE);

    // extract type
    quint8 type;
    stream >> type;

    // extract length
    uint size;
    stream >> size;

    checkSize(stream, size);

    // extract data
    QByteArray buffer;
    char *temp = new char[size];
    stream.readRawData(temp, size);
    buffer.append(temp, size);
    delete [] temp;

    return new TlvTuple(type, buffer);
}
