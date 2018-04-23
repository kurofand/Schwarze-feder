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
	//MySQLClient *client;
	TableEditor *tableEditor;
	//void reloadTable(std::vector<std::string>* vec);

private slots:
	void on_pbRunSQL_clicked();
	void on_pbAdd_clicked();
	void on_pbSelect_clicked();
	void on_pbFilter_clicked();
public slots:
	void buttonEnabled(uint8_t ind);

signals:
	void executeQuery(QString *query);
	void refreshTable();
};

#endif // MAINWINDOW_H
