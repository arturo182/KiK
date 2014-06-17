#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "NewGameDialog.h"
#include "Game.h"

#include <QMessageBox>
#include <QShowEvent>
#include <QPainter>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(Game *game, QWidget *parent):
	QMainWindow(parent),
	m_ui(new Ui::MainWindow),
	m_game(game)
{
	m_ui->setupUi(this);
	m_ui->canvas->setGame(m_game);

	QActionGroup *group = new QActionGroup(this);
	connect(group, SIGNAL(triggered(QAction*)), SLOT(changeDifficulty(QAction*)));
	group->addAction(m_ui->action_atwo);
	group->addAction(m_ui->action_rednio);
	group->addAction(m_ui->actionTrudno);

	m_ui->action_atwo->setChecked(true);

	connect(m_game, SIGNAL(updated()), m_ui->canvas, SLOT(repaint()));
	connect(m_game, SIGNAL(ended()), SLOT(showGameEnd()));

	QTimer::singleShot(0, this, SLOT(on_actionNowa_triggered()));
}

MainWindow::~MainWindow()
{
	delete m_ui;
}

void MainWindow::on_actionO_triggered()
{
	QMessageBox::about(this, "O...", "Kółko i Krzyżyk\nWykonanie Artur Pacholec");
}

void MainWindow::on_actionWyjd_triggered()
{
	qApp->quit();
}

void MainWindow::on_actionNowa_triggered()
{
	NewGameDialog dlg(this);

	if(dlg.exec()) {
		m_game->startNew(dlg.boardSize(), dlg.playerOne(), dlg.playerTwo());
	}
}

void MainWindow::showGameEnd()
{
	if(m_game->win().empty()) {
		QMessageBox::information(this, "The End", "Remis!");
	} else {
		QMessageBox::information(this, "The End", "Koniec gry!");
	}
}

void MainWindow::changeDifficulty(QAction *action)
{
	if(action == m_ui->action_atwo) {
		m_game->setDifficulty(Game::Easy);
	} else if(action == m_ui->action_rednio) {
		m_game->setDifficulty(Game::Normal);
	} else {
		m_game->setDifficulty(Game::Hard);
	}
}
