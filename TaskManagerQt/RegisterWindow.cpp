#include "RegisterWindow.h"

RegisterWindow::RegisterWindow(DatabaseManager* dbManager, QWidget* parent) : QDialog(parent) {
	setMinimumSize(500, 300);

	this->dbManager = dbManager;

	QLabel* loginText = new QLabel("Login:");
	QLabel* passwordText = new QLabel("Password:");
	QLabel* repeatPasswordText = new QLabel("Repeat password:");
	QLabel* showPasswordText = new QLabel("Show password:");

	login = new QLineEdit();
	login->setPlaceholderText("enter login here");
	login->setMaxLength(20);
	password = new QLineEdit();
	password->setPlaceholderText("enter password here (at least 9 characters)");
	password->setMaxLength(20);
	repeatPassword = new QLineEdit();
	repeatPassword->setEchoMode(QLineEdit::Password);
	repeatPassword->setPlaceholderText("repeat password here");
	repeatPassword->setMaxLength(20);
	showPassword = new QCheckBox();

	QGridLayout* grid = new QGridLayout();
	grid->addWidget(loginText, 0, 0);
	grid->addWidget(login, 0, 1);
	grid->addWidget(passwordText, 1, 0);
	grid->addWidget(password, 1, 1);
	grid->addWidget(repeatPasswordText, 2, 0);
	grid->addWidget(repeatPassword, 2, 1);
	grid->addWidget(showPasswordText, 3, 0);
	grid->addWidget(showPassword, 3, 1);

	QPushButton* registerBtn = new QPushButton("Register");
	QPushButton* cancelBtn = new QPushButton("Cancel");
	registerBtn->setMaximumSize(130, 40);
	cancelBtn->setMaximumSize(130, 40);

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addStretch();
	hbox->addWidget(registerBtn);
	hbox->addWidget(cancelBtn);
	hbox->addStretch();

	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->addLayout(grid);
	vbox->addLayout(hbox);

	connect(registerBtn, &QPushButton::clicked, this, &RegisterWindow::registerUser);
	connect(cancelBtn, &QPushButton::clicked, this, &RegisterWindow::cancelClicked);
	connect(showPassword, &QCheckBox::checkStateChanged, this, &RegisterWindow::changePasswordDisplay);

	setLayout(vbox);
}

void RegisterWindow::registerUser() {
	if (login->text().simplified() == "" or password->text().simplified() == "") {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}
	if (password->text().simplified().size() < 9) {
		QMessageBox::warning(this, "error", "the password must contain at least 9 characters");
		return;
	}
	if (password->text().simplified() != repeatPassword->text().simplified()) {
		QMessageBox::warning(this, "error", "passwords must match");
		return;
	}
	if (!dbManager->checkLogin(login->text().simplified())) {
		QMessageBox::warning(this, "error", "Error\nThis username might already be taken");
		return;
	}
	QString errorText = dbManager->addNewUser(login->text().simplified(), password->text().simplified());
	if (errorText != "") {
		QMessageBox::warning(this, "error", errorText);
		return;
	}
	QMessageBox::information(this, "registration", "you have successfully registered\nlog in to your account in the next window");
	accept();
}

void RegisterWindow::cancelClicked() {
	reject();
}

void RegisterWindow::changePasswordDisplay() {
	if (showPassword->isChecked()) repeatPassword->setEchoMode(QLineEdit::Normal);
	else repeatPassword->setEchoMode(QLineEdit::Password);
}