#include "Canvas.h"

#include "Game.h"

#include <QPainter>
#include <qmath.h>
#include <QDebug>

Canvas::Canvas(QWidget *parent):
	QWidget(parent)
{
	setMouseTracking(true);
}

void Canvas::paintEvent(QPaintEvent *event)
{
	if(m_game->boardSize() == -1) {
		return;
	}

	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);

	QLinearGradient background(QPointF(0, 0), p.window().bottomRight());
	background.setColorAt(0.0, QColor("#152944"));
	background.setColorAt(1.0, QColor("#4fa2de"));

	p.fillRect(p.window(), background);

	int boardHeight = height() - 75;

	qreal size = 0;
	if(width() < boardHeight) {
		size = width() / m_game->boardSize();
	} else {
		size = boardHeight / m_game->boardSize();
	}

	QFont font = p.font();
	font.setPixelSize(boardHeight * 0.05);
	p.setFont(font);
	p.setPen(Qt::white);

	QString text;
	if(m_game->isEnded()) {
		text = "Koniec gry!";
	} else {
		if(m_game->currentPlayer()->field() == Field::Circle) {
			text = "Kółko";
		} else {
			text = "Krzyżyk";
		}
	}

	p.drawText(width() * 0.05, boardHeight * 1.075, text);

	for(int i = 0; i < m_game->boardSize(); ++i) {
		for(int j = 0; j < m_game->boardSize(); ++j) {
			QRectF field(i * size, j * size, size, size);

			p.setPen(QPen(Qt::black, size * 0.03));
			if(field.contains(mapFromGlobal(QCursor::pos()))) {
				p.setBrush(QColor(255, 255, 255, 100));
			} else {
				p.setBrush(QColor(0, 0, 0, 100));
			}

			int margin = size * 0.05;
			p.drawRoundedRect(field.adjusted(margin, margin, -margin, -margin), margin, margin);

			switch(m_game->field(i, j)) {
				case Field::Circle:
				{
					p.setPen(QPen(QColor("#ccff63"), size * 0.06));
					p.setBrush(Qt::transparent);

					p.drawEllipse(field.center(), size * 0.35, size * 0.35);
				}
				break;

				case Field::Cross:
				{
					p.setPen(QPen(QColor("#09e0ff"), size * 0.06, Qt::SolidLine, Qt::RoundCap));
					p.setBrush(Qt::transparent);

					p.drawLine(QPointF(field.left() + margin * 3, field.top() + margin * 3), QPointF(field.right() - margin * 3, field.bottom() - margin * 3));
					p.drawLine(QPointF(field.left() + margin * 3, field.bottom() - margin * 3), QPointF(field.right() - margin * 3, field.top() + margin * 3));
				}
				break;

				default:
				break;
			}
		}
	}

	if(m_game->win().count() >= 5) {
		p.setPen(QPen(QColor("#e51400"), size * 0.08, Qt::SolidLine, Qt::RoundCap));
		p.setBrush(Qt::transparent);

			p.drawLine(QPointF(size * m_game->win().first().first, size * m_game->win().first().second) + QPointF(size * 0.5, size * 0.5),
					   QPointF(size * m_game->win().last().first,  size * m_game->win().last().second)  + QPointF(size * 0.5, size * 0.5));

	}
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
	update();
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
	if(m_game->isEnded()) {
		qDebug() << "ended";
		return;
	}

	if(m_game->boardSize() < 0) {
		qDebug() << "size < 0";
		return;
	}

	if(m_game->currentPlayer()->type() == Player::Computer) {
		qDebug() << "computer";
		return;
	}

	int boardHeight = height() - 75;

	qreal size = 0;
	if(width() < boardHeight) {
		size = width() / m_game->boardSize();
	} else {
		size = boardHeight / m_game->boardSize();
	}

	QPointF cursorPos = mapFromGlobal(QCursor::pos());
	int x = qCeil(cursorPos.x() / qCeil(m_game->boardSize() * size) * m_game->boardSize()) - 1;
	int y = qCeil(cursorPos.y() / qCeil(m_game->boardSize() * size) * m_game->boardSize()) - 1;

	x = qMin(x, m_game->boardSize() - 1);
	y = qMin(y, m_game->boardSize() - 1);

	m_game->markField(x, y);
}
