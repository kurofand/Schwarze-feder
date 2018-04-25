#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(QWidget *parent, uint8_t tableIndex, const char *record) :
	QDialog(parent),
	ui(new Ui::EditDialog)
{
	ui->setupUi(this);

}

EditDialog::~EditDialog()
{
	delete ui;
}
