#ifndef TLVMESSAGE_H
#define TLVMESSAGE_H

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

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
	TlvMessage(TlvTuple);
	TlvMessage(const TlvMessage&);
	virtual ~TlvMessage();

	// method
	void unmarshall(QDataStream&) const;

	static int nextMessageEnd(QByteArray& buffer);
	static TlvMessage marshall(QDataStream& stream);

	// property
	void setValue(const TlvTuple*);
	bool hasValue() const;
	int getAddressSize() const { return address.size(); };
	int getType() const { return address.getType(); };
	const char* getAddress() const { return address.data(); }
	int getStatus() const { return address.getInt(); }
	int getValueSize() const
	{
		if (value != NULL) return value->size();
		return 0;
	}
	const char* getValue() const
	{
		if (value != NULL) return value->data();
		return NULL;
	}
	int size() const
	{
		return ENCAP_SIZE + getAddressSize() + getValueSize();
	}

	// operator
	friend QDebug operator<<(QDebug dbg, const TlvMessage& info)
	{
		dbg << "TlvMessage [";

		dbg << info.address;
		if (info.hasValue()) dbg << ", " << *info.value;

		dbg << "]";
		return dbg;
	}

private:
	TlvTuple address;
	const TlvTuple* value;

};

#endif // TLVMESSAGE_H
