#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
	class MainWindow;
}

class Game;

class MainWindow: public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(Game *game, QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_actionO_triggered();
		void on_actionWyjd_triggered();
		void on_actionNowa_triggered();
		void showGameEnd();
		void changeDifficulty(QAction *action);

	private:
		Ui::MainWindow *m_ui;
		Game *m_game;
};

#endif // MAINWINDOW_H
