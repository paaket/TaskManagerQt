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
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QCheckBox>

class RegisterWindow : public QDialog {
	Q_OBJECT
public:
	struct User {
		QString login;
		QString password;
	};
	RegisterWindow(QWidget* parent = nullptr);
	~RegisterWindow();
private slots:
	void registerUser();
	void cancelClicked();
	void changePasswordDisplay();
private:
	QLineEdit* login;
	QLineEdit* password;
	QLineEdit* repeatPassword;
	QCheckBox* showPassword;
	QSqlDatabase db;
};