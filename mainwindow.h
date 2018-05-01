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

private slots:
	void on_pbRunSQL_clicked();
	void on_pbFilter_clicked();
public slots:
	void buttonEnabled(uint8_t ind);

signals:
	void executeQuery(QString *query);
	void setCurrentQuery(QString *query, uint8_t pos);
	void refreshTable();
};

#endif // MAINWINDOW_H
