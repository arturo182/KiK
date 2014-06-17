#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QtGui/QDialog>

#include "Player.h"

namespace Ui
{
	class NewGameDialog;
}

class NewGameDialog: public QDialog
{
	Q_OBJECT

	public:
		explicit NewGameDialog(QWidget *parent = 0);
		~NewGameDialog();

		Player::Type playerOne() const;
		Player::Type playerTwo() const;
		int boardSize() const;


	private:
		Ui::NewGameDialog *m_ui;
};

#endif // NEWGAMEDIALOG_H
