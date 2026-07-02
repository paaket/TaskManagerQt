#pragma once
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QlineEdit>
#include <QPushButton>
#include <QgridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QCheckBox>
#include "DatabaseManager.h"

class RegisterWindow : public QDialog {
	Q_OBJECT
public:
	struct User {
		QString login;
		QString password;
	};
	RegisterWindow(DatabaseManager* dbManager, QWidget* parent = nullptr);
private slots:
	void registerUser();
	void cancelClicked();
	void changePasswordDisplay();
private:
	QLineEdit* login;
	QLineEdit* password;
	QLineEdit* repeatPassword;
	QCheckBox* showPassword;
	DatabaseManager* dbManager;
};