#include <QApplication>
#include <QTextCodec>
#include <QDateTime>

#include "MainWindow.h"
#include "Player.h"
#include "Game.h"

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	qsrand(QDateTime::currentDateTime().toTime_t());

	QApplication a(argc, argv);

	Player playerOne(Field::Circle);
	Player playerTwo(Field::Cross);
	Game game(&playerOne, &playerTwo);

	MainWindow w(&game);
	w.show();

	return a.exec();
}
