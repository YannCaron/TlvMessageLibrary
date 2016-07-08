#include <QDebug>
#include <QCoreApplication>

#include "TlvTuple.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

    TlvTuple tuple = TlvTuple((qint8)0x14, QString("Hi by my tlv friend !").toLatin1().data());

	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));

	tuple.unmarshall(stream);
	qDebug() << buffer.toHex();
    qDebug() << tuple;

    char data[] = {0x14, 0x00, 0x00, 0x00, 0x04, 0x41, 0x42, 0x43, 0x44};
    QByteArray buffer2;
    buffer2.append(data, 9);
    QDataStream stream2(&buffer2, QIODevice::ReadOnly);

    qDebug() << buffer2.toHex();

    TlvTuple tuple2 = TlvTuple::marshall(stream2);
    qDebug() << tuple2;

    return a.exec();
}
