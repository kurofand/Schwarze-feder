#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tableeditor.h"
class TableEditor;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	TableEditor *tableEditor;
	qint8 currentTable=-1;
	static const uint8_t disableAll=0;
	static const uint8_t enableSelect=1;
	static const uint8_t enableFilter=2;
	static const uint8_t enableAdd=3;
	static const uint8_t enableEditDelete=4;
	static const uint8_t disableEditDelete=5;
	static const uint8_t enableCurrency=6;

private slots:
	void on_pbRunSQL_clicked();
public slots:
	void buttonEnabled(uint8_t ind);
	void close();

signals:
	void executeQuery(QString *query);
	void setCurrentQuery(QString *query, uint8_t pos);
	void refreshTable();
};

#endif // MAINWINDOW_H
