#pragma once
#include "RegisterWindow.h"
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <QSettings>

class LoginWindow : public QDialog {
	Q_OBJECT
public:
	LoginWindow(QWidget* parent = nullptr);
private slots:
	void openRegisterWindow();
	void loginUser();
	void changePasswordDisplay();
private:
	QLineEdit* login;
	QLineEdit* password;
	QCheckBox* remember;
	QCheckBox* showPassword;
	QSqlDatabase db;
};