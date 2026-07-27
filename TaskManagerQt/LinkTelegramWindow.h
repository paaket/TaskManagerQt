#pragma once
#include <QDialog>
#include <QLabel>
#include <QFont>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QMessageBox>
#include "DatabaseManager.h"

class LinkTelegramWindow : public QDialog {
public:
	LinkTelegramWindow(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr);
private:
	DatabaseManager* dbManager;
};