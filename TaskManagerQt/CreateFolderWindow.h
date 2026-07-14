#pragma once
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include "DatabaseManager.h"

class CreateFolderWindow : public QDialog {
	Q_OBJECT
public:
	CreateFolderWindow(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr);
protected slots:
	virtual void createBtnClicked();
	virtual void cancelBtnClicked();
signals:
	void dataReady(const QString& title);
protected:
	QLineEdit* line;
	QPushButton* createBtn;
	QGridLayout* btnGrid;
	int userId;
	DatabaseManager* dbManager;
};