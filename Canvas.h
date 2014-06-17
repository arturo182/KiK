#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

class Game;

class Canvas: public QWidget
{
	Q_OBJECT

	public:
		Canvas(QWidget *parent = 0);

		void setGame(Game *game) { m_game = game; }

	protected:
		void paintEvent(QPaintEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		Game *m_game;
};

#endif // CANVAS_H
