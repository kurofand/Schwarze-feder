#ifndef TABLEEDITOR_H
#define TABLEEDITOR_H
#include <QTableWidget>
#include <QObject>
#include "mysqlClient.hpp"
#include "sqliteclient.hpp"

class TableEditor: public QObject
{
	Q_OBJECT
public:
	TableEditor(QTableWidget &widget,QObject *parent=0);
	void setTable(QTableWidget &widget);
	bool clientConnect();

	virtual ~TableEditor();

private:
	QTableWidget *table;
	SqliteClient *client;
	//MySQLClient *client;
	QString currentQuery="SELECT expenses.name, val, date, descr, categories.name, shops.name "
						 "FROM expenses INNER JOIN categories, shops "
						 "WHERE categoryId=categories.id "
						 "AND shopId=shops.id";
	QList <uint32_t> *ids;
	uint8_t tableIndex;
	void reloadTable(auto *vec);
	uint16_t getSelectedId();
	void convertTo(double *result, auto *vals, uint16_t to);

public slots:
	void executeQuery(QString *query);
	void refreshTable();
	//тут я пытаюсь собрать квери из кусков пришедших с фильтра или селекта. нулевая позиция указывает на селект,
	//первая на вхере
	void setCurrentQuery(QString *part, uint8_t pos);
	void editTable();
	void selectTable();
	void insertTable();
	void deleteTable();
	void setFilter();
	void setCurrency();
	void createMonthReport();
	void createYearReport();

private slots:
	void enableEditDelete();

signals:
	void buttonEnabled(uint8_t ind);
};

#endif // TABLEEDITOR_H
