#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

namespace Field
{
	enum Type
	{
		Empty = 0,
		Cross,
		Circle
	};
}

class Player: public QObject
{
	Q_OBJECT

	public:
		enum Type
		{
			Human = 0,
			Computer
		};

	public:
		Player(const Field::Type &field, QObject *parent = 0);

		void setField(const Field::Type &field) { m_field = field; }
		Field::Type field() const { return m_field; }

		void setType(const Type &type) { m_type = type; }
		Type type() const { return m_type; }

	private:
		Field::Type m_field;
		Type m_type;
};

#endif // PLAYER_H
