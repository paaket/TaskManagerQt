#pragma once
#include <QString>
#include "RegisterWindow.h"
#include "DatabaseManager.h"
#include "User.h"

class EditUserWindow : public RegisterWindow {
	Q_OBJECT
public:
	EditUserWindow(DatabaseManager* dbManager, QWidget* parent = nullptr, const User& user = User());
private slots:
	void registerUser() override;
private:
	User user;
};