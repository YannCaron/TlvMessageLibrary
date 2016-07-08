#include <QDebug>
#include <QCoreApplication>

#include "TlvMessage.h"
#include "TlvTuple.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    {
        qDebug() << "Test unmarshall -----------------------------------------------------------";
        TlvTuple tuple = TlvTuple((qint8)0x14, QString("Hi by my tlv friend !").toLatin1().data());

        QByteArray buffer;
        QDataStream stream(&buffer, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::ByteOrder(QSysInfo::ByteOrder));

        tuple.unmarshall(stream);

        qDebug() << buffer.toHex();
        qDebug() << tuple;
    }

    {
        qDebug() << "Test marshall -------------------------------------------------------------";
        char data[] = {0x14, 0x00, 0x00, 0x00, 0x04, 0x41, 0x42, 0x43, 0x44};
        QByteArray buffer;
        buffer.append(data, 9);
        QDataStream stream(&buffer, QIODevice::ReadOnly);

        TlvTuple tuple = TlvTuple::marshall(stream);

        qDebug() << buffer.toHex();
        qDebug() << tuple;
    }

    {
        qDebug() << "Test to bool --------------------------------------------------------------";
        char data[] = {0x11, 0x00, 0x00, 0x00, 0x01, 0x01};
        QByteArray buffer;
        buffer.append(data, 6);
        QDataStream stream(&buffer, QIODevice::ReadOnly);

        TlvTuple tuple = TlvTuple::marshall(stream);

        qDebug() << buffer.toHex();
        qDebug() << tuple;

    }

    {
        qDebug() << "Test to int ---------------------------------------------------------------";
        char data[] = {0x12, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08};
        QByteArray buffer;
        buffer.append(data, 9);
        QDataStream stream(&buffer, QIODevice::ReadOnly);

        TlvTuple tuple = TlvTuple::marshall(stream);

        qDebug() << buffer.toHex();
        qDebug() << tuple;

    }


    {
        qDebug() << "Test message --------------------------------------------------------------";
        char data[] = {0x01, 0x00, 0x00, 0x00, 0x11, 0x02, 0x10, 0x00, 0x00, 0x00, 0x04, 0x41, 0x42, 0x43, 0x44, 0x14, 0x00, 0x00, 0x00, 0x04, 0x74, 0x65, 0x73, 0x74, 0x03, 0x04};
        QByteArray buffer;
        buffer.append(data, 26);
        QDataStream stream(&buffer, QIODevice::ReadOnly);

        TlvMessage message = TlvMessage::marshall(stream);

        qDebug() << buffer.toHex();
        qDebug() << message;

    }

    return a.exec();
}
