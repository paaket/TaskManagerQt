#pragma once
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include "DatabaseManager.h"

class CreateFolderWindow : public QDialog {
	Q_OBJECT
public:
	CreateFolderWindow(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr);
private slots:
	void createBtnClicked();
	void cancelBtnClicked();
signals:
	void dataReady(const QString& title);
private:
	QLineEdit* line;
	int userId;
	DatabaseManager* dbManager;
};