#include "Player.h"

Player::Player(const Field::Type &field, QObject *parent) :
	QObject(parent),
	m_field(field),
	m_type(Computer)
{
}
