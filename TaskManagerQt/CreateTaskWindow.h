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
#include <QDateTimeEdit>
#include <QComboBox>
#include <QCheckBox>

class CreateTaskWindow : public QDialog {
	Q_OBJECT
public:
	struct TaskData {
		QString title;
		QString description;
		int priority;
		QDateTime deadline;
		QDateTime createdAt;
		QVariant notifyAt;
	};
	CreateTaskWindow(QWidget* patent = nullptr);
protected slots:
	virtual void saveClicked();
	void changeNotifMaxTime();
signals:
	void saveReady(const TaskData& task);
protected:
	QLineEdit* title;
	QComboBox* priority;
	QDateTimeEdit* deadline;
	QDateTimeEdit* notification;
	QTextEdit* description;
	QGridLayout* grid;
	QGridLayout* btnGrid;
	QCheckBox* notifCheckBox;
};