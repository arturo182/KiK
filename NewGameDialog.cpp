#include "NewGameDialog.h"
#include "ui_NewGameDialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::NewGameDialog)
{
	m_ui->setupUi(this);
}

NewGameDialog::~NewGameDialog()
{
	delete m_ui;
}

Player::Type NewGameDialog::playerOne() const
{
	return m_ui->playerOneComputerRadioButton->isChecked() ? Player::Computer : Player::Human;
}

Player::Type NewGameDialog::playerTwo() const
{
	return m_ui->playerTwoComputerRadioButton->isChecked() ? Player::Computer : Player::Human;
}

int NewGameDialog::boardSize() const
{
	return m_ui->boardSizeSpinBox->value();
}
