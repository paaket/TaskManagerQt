#include "DeleteUserWindow.h"

DeleteUserWindow::DeleteUserWindow(const User& user, DatabaseManager* dbManager, QWidget* parent) : QDialog(parent) {
	this->user = user;
	this->dbManager = dbManager;

	setMinimumSize(500, 300);

	QLabel* infoText = new QLabel("To confirm account deletion, enter your login below");
	QLabel* loginText = new QLabel("Login:");

	line = new QLineEdit();
	line->setPlaceholderText("enter login here");

	QHBoxLayout* lineHbox = new QHBoxLayout();
	lineHbox->addWidget(loginText);
	lineHbox->addWidget(line);

	QPushButton* confirmBtn = new QPushButton("Confirm");
	QPushButton* cancelBtn = new QPushButton("Cancel");

	QHBoxLayout* btnHbox = new QHBoxLayout();
	btnHbox->addWidget(confirmBtn);
	btnHbox->addWidget(cancelBtn);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(infoText);
	vbox->addLayout(lineHbox);
	vbox->addLayout(btnHbox);

	connect(confirmBtn, &QPushButton::clicked, this, &DeleteUserWindow::confirmClicked);
	connect(cancelBtn, &QPushButton::clicked, this, &DeleteUserWindow::cancelClicked);

	setLayout(vbox);
}

void DeleteUserWindow::confirmClicked() {
	if (line->text() != user.login) {
		QMessageBox::warning(this, "error", "invalid login");
		return;
	}
	auto reply = QMessageBox::question(this, "confirmation", "Do you really want to delete your account?\nThis action cannot be undone.\nDeleting the account will result in the deletion of all data linked to it");
	if (reply == QMessageBox::No) return;
	QString errorText = dbManager->deleteAccountById(user.id);
	if (errorText != "") {
		QMessageBox::warning(this, "error", errorText);
		return;
	}
	accept();
}
void DeleteUserWindow::cancelClicked() {
	reject();
}