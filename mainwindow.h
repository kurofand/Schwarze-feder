#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mysqlClient.hpp"
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
	MySQLClient *client;
	void reloadTable(std::vector<std::string>* vec);

private slots:
	void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
