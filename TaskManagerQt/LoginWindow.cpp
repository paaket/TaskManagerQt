#include "LoginWindow.h"

LoginWindow::LoginWindow(DatabaseManager* dbManager, QWidget* parent) : QDialog(parent) {
	setMinimumSize(500, 300);

	this->dbManager = dbManager;

	QLabel* loginText = new QLabel("Login:");
	QLabel* passwordText = new QLabel("Password:");
	QLabel* rememberText = new QLabel("Remember me:");
	QLabel* showPasswordText = new QLabel("Show password:");

	login = new QLineEdit();
	login->setPlaceholderText("enter login here");
	login->setMaxLength(20);
	password = new QLineEdit();
	password->setPlaceholderText("enter password here");
	password->setEchoMode(QLineEdit::Password);
	password->setMaxLength(20);
	remember = new QCheckBox();
	showPassword = new QCheckBox();

	QGridLayout* grid = new QGridLayout();
	grid->addWidget(loginText, 0, 0);
	grid->addWidget(login, 0, 1);
	grid->addWidget(passwordText, 1, 0);
	grid->addWidget(password, 1, 1);
	grid->addWidget(showPasswordText, 2, 0);
	grid->addWidget(showPassword, 2, 1);
	grid->addWidget(rememberText, 3, 0);
	grid->addWidget(remember, 3, 1);

	QPushButton* loginBtn = new QPushButton("Login");
	QPushButton* registerBtn = new QPushButton("Register");
	loginBtn->setMaximumSize(130, 40);
	registerBtn->setMaximumSize(130, 40);

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->setSpacing(15);
	hbox->addStretch();
	hbox->addWidget(loginBtn);
	hbox->addWidget(registerBtn);
	hbox->addStretch();

	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->addLayout(grid);
	vbox->addLayout(hbox);

	connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::loginUser);
	connect(registerBtn, &QPushButton::clicked, this, &LoginWindow::openRegisterWindow);
	connect(showPassword, &QCheckBox::checkStateChanged, this, &LoginWindow::changePasswordDisplay);

	setLayout(vbox);
}

void LoginWindow::loginUser() {
	if (login->text().simplified() == "" or password->text().simplified() == "") {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}
	bool rememberStatus = remember->isChecked() ? 1 : 0;
	QString errorText = dbManager->logInToAccount(login->text().simplified(), password->text().simplified(), rememberStatus);
	if (errorText == "false") {
		QMessageBox::warning(this, "error", "user not found");
		return;
	}
	if (errorText != "true") {
		QMessageBox::warning(this, "error", errorText);
		return;
	}
	accept();
}

void LoginWindow::openRegisterWindow() {
	RegisterWindow window(dbManager, this);
	window.exec();
}

void LoginWindow::changePasswordDisplay() {
	if (showPassword->isChecked()) password->setEchoMode(QLineEdit::Normal);
	else password->setEchoMode(QLineEdit::Password);
}