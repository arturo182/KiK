#include "Game.h"

#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <QTime>

Game::Game(Player *playerOne, Player *playerTwo, QObject *parent):
	QObject(parent),
	m_playerOne(playerOne),
	m_playerTwo(playerTwo),
	m_currentPlayer(playerOne),
	m_boardSize(-1)
{
}

void Game::startNew(const int &boardSize, const Player::Type &playerOne, const Player::Type &playerTwo)
{
	m_boardSize = boardSize;
	m_takenFields = 0;

	m_win.clear();

	m_fields.clear();
	m_fields = QVector<QVector<Field::Type> >(m_boardSize);
	for(int i = 0; i < m_boardSize; ++i) {
		m_fields[i] = QVector<Field::Type>(m_boardSize, Field::Empty);
	}

	m_playerOne->setType(playerOne);
	m_playerTwo->setType(playerTwo);

	m_currentPlayer = m_playerOne;

	if(m_currentPlayer->type() == Player::Computer) {
		computeAMove();
	}

	emit updated();
}

void Game::markField(const int &x, const int &y)
{
	if(field(x, y) == Field::Empty) {
		Field::Type field = m_currentPlayer->field();

		m_fields[x][y] = field;

		m_currentPlayer = (m_currentPlayer == m_playerTwo) ? m_playerOne : m_playerTwo;
		m_takenFields++;

		emit updated();

		checkWinner(x, y, field);

		if(isEnded() || m_win.count()) {
			emit ended();
		} else {
			if(m_currentPlayer->type() == Player::Computer) {
				computeAMove();
				emit updated();
			}
		}
	} else {
		qDebug() << "not empty";
	}
}

void Game::checkWinner(const int &x, const int &y, const Field::Type &type)
{
	QList<QPair<int, int> > win = isWinner(x, y, type);
	if(win.length() >= 5) {
		m_win = win;
	}
}

QList<QPair<int, int> > Game::isWinner(const int &x, const int &y, const Field::Type &type)
{
	int xStart = qMax(0, x - 4);
	int xEnd = qMin(m_boardSize - 1, x + 4);

	int yStart = qMax(0, y - 4);
	int yEnd = qMin(m_boardSize - 1, y + 4);

	QList<QPair<int, int> > win;

	//horizontal
	for(int i = xStart; i <= xEnd; ++i) {
		if(field(i, y) == type) {
			win.append(qMakePair<int, int>(i, y));

			if(win.count() >= 5) {
				break;
			}
		} else  {
			win.clear();
		}
	}

	if(win.count() >= 5) {
		return win;
	} else {
		win.clear();
	}

	//vertical
	for(int i = yStart; i <= yEnd; ++i) {
		if(field(x, i) == type) {
			win.append(qMakePair<int, int>(x, i));

			if(win.count() >= 5) {
				break;
			}
		} else  {
			win.clear();
		}
	}

	if(win.count() >= 5) {
		return win;
	} else {
		win.clear();
	}

	//diagonal
	for(int i = -4; i <= 4; ++i) {
		if((qMax(x + i, y + i) < m_boardSize) && (qMin(x + i, y + i) >= 0)) {
			if(field(x + i, y + i) == type) {
				win.append(qMakePair<int, int>(x + i, y + i));

				if(win.count() >= 5) {
					break;
				}
			} else  {
				win.clear();
			}
		}
	}

	if(win.count() >= 5) {
		return win;
	} else {
		win.clear();
	}

	//2nd diagonal
	for(int i = -4; i <= 4; ++i) {
		if((qMax(x - i, y + i) < m_boardSize) && (qMin(x - i, y + i) >= 0)) {
			if(field(x - i, y + i) == type) {
				win.append(qMakePair<int, int>(x - i, y + i));

				if(win.count() >= 5) {
					break;
				}
			} else  {
				win.clear();
			}
		}
	}

	if(win.count() >= 5) {
		return win;
	} else {
		win.clear();
	}

	return win;
}

void Game::computeAMove()
{
	minMax(0, 0, m_currentPlayer->field());

	markField(m_move.first, m_move.second);
}

int Game::evaluate(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer)
{
	int punkty = 0, s, i, j;

	s = 1;
	for(i = 1; i < 5 && x + i < m_boardSize && field(x + i, y) != otherPlayer; i++)
		if(field(x + i, y) == player)
			s++;
	for(j = 1; j < 5 && x - j >= 0 && field(x - j, y) != otherPlayer; j++)
		if(field(x - j, y) == player)
			s++;
	if(i + j > 5)
		punkty += s * s;

	s = 1;
	for(i = 1; i < 5 && y + i < m_boardSize && field(x, y + i) != otherPlayer; i++)
		if(field(x, y + i) == player)
			s++;
	for(j = 1; j < 5 && y - j >= 0 && field(x, y - j) != otherPlayer; j++)
		if(field(x, y - j) == player)
			s++;
	if(i + j > 5)
		punkty += s * s;

	s = 1;
	for(i = 1; i < 5 && x + i < m_boardSize && y + i < m_boardSize && field(x + i, y + i) != otherPlayer; i++)
		if(field(x + i, y + i) == player)
			s++;
	for(j = 1; j < 5 && x - j >= 0 && y - j >= 0 && field(x - j, y - j) != otherPlayer; j++)
		if(field(x - j, y - j) == player)
			s++;
	if(i + j > 5)
		punkty += s * s;

	s = 1;
	for(i = 1; i < 5 && x + i < m_boardSize && y - i >= 0 && field(x + i, y - i) != otherPlayer; i++)
		if(field(x + i, y - i) == player)
			s++;
	for(j = 1; j < 5 && x - j >= 0 && y + j < m_boardSize && field(x - j, y + j) != otherPlayer; j++)
		if(field(x - j, y + j) == player)
			s++;
	if(i + j > 5)
		punkty += s * s;

	return punkty;
}

int Game::evaluateRow(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer)
{
	int part = 1;
	int left = 1;
	int right = 1;

	for(int i = 1; i < 5; ++i) {
		if((x + i >= m_boardSize) || (field(x + i, y) == otherPlayer)) {
			break;
		}

		++right;

		if(field(x + i, y) == player) {
			++part;
		}
	}

	for(int j = 1; j < 5; ++j) {
		if((x - j < 0) || (field(x - j, y) == otherPlayer)) {
			break;
		}

		++left;

		if(field(x - j, y) == player) {
			++part;
		}
	}

	if(left + right > 5) {
		return part * part;
	}

	return 0;
}

int Game::evaluateCol(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer)
{
	int part = 1;
	int left = 1;
	int right = 1;

	for(int i = 1; i < 5; ++i) {
		if((y + i >= m_boardSize) || (field(x, y + i) == otherPlayer)) {
			break;
		}

		++right;

		if(field(x, y + i) == player) {
			++part;
		}
	}

	for(int j = 1; j < 5; ++j) {
		if((y - j < 0) || (field(x, y - j) == otherPlayer)) {
			break;
		}

		++left;

		if(field(x, y - j) == player) {
			++part;
		}
	}

	if(left + right > 5) {
		return part * part;
	}

	return 0;
}

int Game::evaluateDiag(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer)
{
	int part = 1;
	int left = 1;
	int right = 1;

	for(int i = 1; i < 5; ++i) {
		if((x + i >= m_boardSize) ||(y + i >= m_boardSize) || (field(x + i, y + i) == otherPlayer)) {
			break;
		}

		++right;

		if(field(x + i, y + i) == player) {
			++part;
		}
	}

	for(int j = 1; j < 5; ++j) {
		if((x - j < 0) || (y - j < 0) || (field(x - j, y - j) == otherPlayer)) {
			break;
		}

		++left;

		if(field(x - j, y - j) == player) {
			++part;
		}
	}

	if(left + right > 5) {
		return part * part;
	}

	return 0;
}

int Game::evaluateDiag2(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer)
{
	int part = 1;
	int left = 1;
	int right = 1;

	for(int i = 1; i < 5; ++i) {
		if((x + i >= m_boardSize) ||(y - i < 0) || (field(x + i, y - i) == otherPlayer)) {
			break;
		}

		++right;

		if(field(x + i, y - i) == player) {
			++part;
		}
	}

	for(int j = 1; j < 5; ++j) {
		if((x - j < 0) || (y + j >= m_boardSize) || (field(x - j, y + j) == otherPlayer)) {
			break;
		}

		++left;

		if(field(x - j, y + j) == player) {
			++part;
		}
	}

	if(left + right > 5) {
		return part * part;
	}

	return 0;
}

bool Game::deepEnough(const int &depth)
{
	return (depth == 3);
}

QList<QPair<int, int> > Game::moveGen(const int &x, const int &y)
{
	QList<QPair<int, int> > moves;

	for(int i = 0; i < m_boardSize; ++i) {
		for(int j = 0; j < m_boardSize; ++j) {
			if((i == x) && (j == y)) {
				continue;
			}

			if(field(i, j) == Field::Empty) {
				moves << qMakePair<int, int>(i, j);
			}
		}
	}

	return moves;
}

bool Game::piatka(int x, int y, const Field::Type &player){
			int i, j;

			for(i = 1; i < 5 && x + i < m_boardSize && field(x + i, y) == player; i++);
			for(j = 1; j < 5 && x - j >= 0 && field(x - j, y) == player; j++);
			if(i + j > 5)
				return true;

			for(i = 1; i < 5 && y + i < m_boardSize && field(x, y + i) == player; i++);
			for(j = 1; j < 5 && y - j >= 0 && field(x, y - j) == player; j++);
			if(i + j > 5)
				return true;

			for(i = 1; i < 5 && x + i < m_boardSize && y + i < m_boardSize && field(x + i, y + i) == player; i++);
			for(j = 1; j < 5 && x - j >= 0 && y - j >= 0 && field(x - j, y - j) == player; j++);
			if(i + j > 5)
				return true;

			for(i = 1; i < 5 && x + i < m_boardSize && y - i >= 0 && field(x + i, y - i) == player; i++);
			for(j = 1; j < 5 && x - j >= 0 && y + j < m_boardSize && field(x - j, y + j) == player; j++);
			if(i + j > 5)
				return true;

			return false;
		}

int Game::minMax(const int &points, const int &depth, const Field::Type &player)
{
	if(m_takenFields == m_boardSize * m_boardSize || depth == 4) {
		return points;
	}

	int punktyNajlepsze = (player == m_currentPlayer->field()) ? INT_MIN : INT_MAX;
	int punktyTmp;
	Field::Type znakRywala = (player == Field::Circle) ? Field::Cross : Field::Circle;

	for(int i = 0; i < m_boardSize; i++)
		for(int j = 0; j < m_boardSize; j++)
			if(field(i, j) == Field::Empty) {
				if(piatka(i, j, player)){
					if(depth == 0){
						m_move = qMakePair<int, int>(i, j);
						return 0;
					} else {
						return points + (player == m_currentPlayer->field() ? 1000 : -1000);
					}
				}

				m_fields[i][j] = player;
				m_takenFields++;

				if(player == m_currentPlayer->field()) {
					punktyTmp = evaluate(i, j, player, znakRywala);
				} else {
					punktyTmp = -evaluate(i, j, player, znakRywala);
				}

				punktyTmp = minMax(points + punktyTmp, depth + 1, player);

				if(player == m_currentPlayer->field()){
					if(punktyTmp > punktyNajlepsze){
						punktyNajlepsze = punktyTmp;
						if(depth == 0){
							m_move = qMakePair<int, int>(i, j);
						}
					}
				} else if(punktyTmp < punktyNajlepsze) {
					punktyNajlepsze = punktyTmp;
				}

				m_fields[i][j] = Field::Empty;
				m_takenFields--;
			}

	return punktyNajlepsze;
}

Field::Type Game::field(const int &x, const int &y) const
{
	return m_fields[x][y];
}

bool Game::isEnded() const
{
	if(m_win.count() >= 5) {
		return true;
	}

	if(emptyCount() > 0) {
		return false;
	}

	return true;
}

int Game::emptyCount() const
{
	int count = 0;

	for(int i = 0; i < m_boardSize; ++i) {
		for(int j = 0; j < m_boardSize; ++j) {
			if(m_fields[i][j] == Field::Empty) {
				++count;
			}
		}
	}

	return count;
}

