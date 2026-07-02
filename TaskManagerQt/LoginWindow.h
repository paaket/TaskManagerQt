#pragma once
#include "RegisterWindow.h"
#include "DatabaseManager.h"
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QString>
#include <QApplication>
#include <QSettings>

class LoginWindow : public QDialog {
	Q_OBJECT
public:
	LoginWindow(DatabaseManager* dbManager, QWidget* parent = nullptr);
private slots:
	void openRegisterWindow();
	void loginUser();
	void changePasswordDisplay();
private:
	QLineEdit* login;
	QLineEdit* password;
	QCheckBox* remember;
	QCheckBox* showPassword;
	DatabaseManager* dbManager;
};