#ifndef GAME_H
#define GAME_H

#include "Player.h"

#include <QVector>
#include <QPair>

class Game: public QObject
{
	Q_OBJECT

	public:
		enum Difficulty
		{
			Easy = 0,
			Normal,
			Hard
		};

	public:
		Game(Player *playerOne, Player *playerTwo, QObject *parent = 0);

		void startNew(const int &boardSize, const Player::Type &playerOne, const Player::Type &playerTwo);

		Field::Type field(const int &x, const int &y) const;
		bool isEnded() const;

		void setDifficulty(const Difficulty &difficulty) { m_difficulty = difficulty; }
		Difficulty difficulty() const { return m_difficulty; }

		void setWin(const QList<QPair<int, int> > &win) { m_win = win; }
		QList<QPair<int, int> > win() const { return m_win; }

		Player *currentPlayer() { return m_currentPlayer; }
		int boardSize() const { return m_boardSize; }

		int emptyCount() const;

		void markField(const int &x, const int &y);
		void checkWinner(const int &x, const int &y, const Field::Type &type);
		QList<QPair<int, int> > isWinner(const int &x, const int &y, const Field::Type &type);
		void computeAMove();

		//minmax
		int evaluate(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer);
		int evaluateRow(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer);
		int evaluateCol(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer);
		int evaluateDiag(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer);
		int evaluateDiag2(const int &x, const int &y, const Field::Type &player, const Field::Type &otherPlayer);
		bool deepEnough(const int &depth);
		QList<QPair<int, int> > moveGen(const int &x, const int &y);
		int minMax(const int &points, const int &depth, const Field::Type &player);

		bool piatka(int x, int y, const Field::Type &player);
	signals:
		void updated();
		void ended();

	private:
		Player *m_playerOne;
		Player *m_playerTwo;
		Player *m_currentPlayer;
		int m_takenFields;
		QVector<QVector<Field::Type> > m_fields;
		QList<QPair<int, int> > m_win;
		QPair<int, int> m_move;
		Difficulty m_difficulty;
		int m_boardSize;
};

#endif // GAME_H
