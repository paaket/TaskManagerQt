#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include "User.h"
#include "DatabaseManager.h"

class DeleteUserWindow : public QDialog {
public:
	DeleteUserWindow(const User& user, DatabaseManager* dbManager, QWidget* parent = nullptr);
private slots:
	void confirmClicked();
	void cancelClicked();
private:
	QLineEdit* line;
	User user;
	DatabaseManager* dbManager;
};