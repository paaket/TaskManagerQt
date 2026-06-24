#pragma once
#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include <QDateEdit>
#include <QComboBox>

class CreateTaskWindow : public QDialog {
	Q_OBJECT
public:
	CreateTaskWindow(QWidget* patent = nullptr);
private slots:
	void saveClicked();
signals:
	void saveReady(const QVector<QString>& data);
protected:
	QLineEdit* title;
	QComboBox* priority;
	QDateEdit* deadline;
	QTextEdit* description;
};