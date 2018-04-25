#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include "mysqlClient.hpp"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditDialog(QWidget *parent = 0, uint8_t tableIndex=0, uint32_t id=0);
	void returnParams();
	~EditDialog();

private:
	Ui::EditDialog *ui;
	MySQLClient *client;
};

#endif // EDITDIALOG_H
