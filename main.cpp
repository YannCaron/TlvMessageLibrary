#include <QDebug>
#include <QCoreApplication>

#include "TlvMessage.h"
#include "TlvTuple.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	{
		qDebug() << "Test unmarshall -----------------------------------------------------------";
		TlvTuple tuple = TlvTuple(QString("Hi by my tlv friend !"));

		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);

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
		qDebug() << "Test message unmarshall ----------------------------------------------------";
		TlvTuple* address = new TlvTuple(TlvTuple::TYPE_ADDRESS, QString("abcd"));
		TlvTuple* value = new TlvTuple(true);

		TlvMessage message;
		message.append(address)->append(value);

		QByteArray buffer;
		QDataStream stream(&buffer, QIODevice::WriteOnly);

		message.unmarshall(stream);
		qDebug() << "result:  " << buffer.toHex();
		qDebug() << "expected: \"0100000017021000000004616263641100000001010304\"";
		qDebug() << message;

	}

	{
		qDebug() << "Test message marshall ------------------------------------------------------";
		char data[] = {0x01, 0x00, 0x00, 0x00, 0x17, 0x02, 0x10, 0x00, 0x00, 0x00, 0x04, 0x61, 0x62, 0x63, 0x64, 0x11, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x04};
		QByteArray buffer;
		buffer.append(data, sizeof(data) / sizeof(char));
		QDataStream stream(&buffer, QIODevice::ReadOnly);

		TlvMessage message = TlvMessage::marshall(stream);

		qDebug() << buffer.toHex();
		qDebug() << message;
		qDebug() << "Contains message" << TlvMessage::nextMessageEnd(buffer);

	}

	return a.exec();
}
