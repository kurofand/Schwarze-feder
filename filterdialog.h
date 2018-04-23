#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QList>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FilterDialog(QWidget *parent = 0, uint8_t tableIndex=0);
	QString returnFilterString();
	~FilterDialog();

private:
	Ui::FilterDialog *ui;
	QList<QVBoxLayout *> filterLayouts;
	QList<QComboBox *> cbCols, cbCombo;
	QList<QPushButton *> pbDeletes;
	QList<QFormLayout *> innerLayouts;
	QList<QLabel *> lFilterCaptions;
	QList<QLineEdit *> leLines;
	QList<QDoubleSpinBox *> dsbVals;
	QList<QDateEdit *> deDates;
	uint8_t tIndex=0;
	void deleteElements(QList<QWidget *> *list);
private slots:
	void on_pbAdd_clicked();
	void deleteFilter();
	void selectCol(int index);
};

#endif // FILTERDIALOG_H
