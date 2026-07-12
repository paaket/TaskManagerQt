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
#include "User.h"

class RegisterWindow : public QDialog {
	Q_OBJECT
public:
	RegisterWindow(DatabaseManager* dbManager, QWidget* parent = nullptr);
protected slots:
	virtual void registerUser();
	void cancelClicked();
	void changePasswordDisplay();
protected:
	QLineEdit* login;
	QLineEdit* password;
	QLineEdit* repeatPassword;
	QCheckBox* showPassword;
	QLabel* loginText;
	QLabel* passwordText;
	QPushButton* registerBtn;
	DatabaseManager* dbManager;
};