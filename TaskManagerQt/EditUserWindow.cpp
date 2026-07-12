#include "EditUserWindow.h"

EditUserWindow::EditUserWindow(DatabaseManager* dbManager, QWidget* parent, const User& user) : RegisterWindow(dbManager, parent) {
	loginText->setText("New login:");
	passwordText->setText("New password:");
	login->setText(user.login);
	registerBtn->setText("Save");
	this->user = user;
}

void EditUserWindow::registerUser() {
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
	user.login = login->text().simplified();
	user.password = password->text().simplified();
	QString errorText = dbManager->updateUser(user);
	if (errorText != "") {
		QMessageBox::warning(this, "error", errorText);
		return;
	}
	QMessageBox::information(this, "info", "data successfully updated");
	accept();
}