#include "TlvMessage.h"

TlvMessage::TlvMessage()
{
}

TlvMessage::TlvMessage(const TlvMessage &other) : tuples()
{
	// deep copy
	for (int i = 0; i<other.tuples.size(); i++) {
		tuples.append(new TlvTuple(other.tuples.at(i)));

		// TODO : deep copy does not works properly
		qDebug() << "COPY " << *tuples.at(i);
	}
}

TlvMessage::~TlvMessage()
{
	qDeleteAll(tuples);
	tuples.clear();
}

void TlvMessage::unmarshall(QDataStream &stream) const
{
	stream << SOH;
	stream << this->size();
	stream << STX;

	for (int i = 0; i < tuples.size(); i++) {
		tuples.at(i)->unmarshall(stream);
	}

	stream << ETX;
	stream << EOT;
}

int TlvMessage::nextMessageEnd(QByteArray& buffer)
{
	QDataStream stream(buffer);
	if (buffer.size() < ENCAP_SIZE) return -1;
	TlvTuple::check(stream, SOH);

	int size;
	stream >> size;

	if (buffer.size() < size) return -1;

	return size;
}

TlvMessage TlvMessage::marshall(QDataStream &stream) {

	TlvMessage message;

	TlvTuple::checkSize(stream, ENCAP_SIZE);

	// check header
	TlvTuple::check(stream, SOH);

	// extract size
	uint size;
	stream >> size;

	TlvTuple::check(stream, STX);

	// extract values
	while (stream.device()->bytesAvailable() > 2) {
		message.append(new TlvTuple(TlvTuple::marshall(stream)));
	}

	// check footer
	TlvTuple::check(stream, ETX);
	TlvTuple::check(stream, EOT);

	return message;
}

int TlvMessage::size() const
{
	int s = 0;
	for (int i = 0; i < tuples.size(); i++) {
		s += tuples.at(i)->size();
	}

	return ENCAP_SIZE + s;
}
