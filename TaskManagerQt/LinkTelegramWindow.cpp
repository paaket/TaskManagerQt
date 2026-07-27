#include "LinkTelegramWindow.h"

LinkTelegramWindow::LinkTelegramWindow(DatabaseManager* dbManager, int userId, QWidget* parent) : QDialog(parent) {
	this->dbManager = dbManager;

	setMinimumSize(400, 200);
	setWindowTitle("Link Telegram");

	QFont fontCode;
	fontCode.setPointSize(30);
	QFont text;
	text.setPointSize(12);

	QString errorText = dbManager->deleteExpiredTelegramCodes();
	if (errorText != "") QMessageBox::warning(this, "error", errorText);
	errorText = dbManager->deleteActiveUserCodes(userId);
	if (errorText != "") QMessageBox::warning(this, "error", errorText);

	int randomNum;
	while (true) {
		randomNum = QRandomGenerator::global()->bounded(100000, 1000000);
		if (!dbManager->checkTelegramCodeStatus(randomNum)) break;
	}

	QLabel* first = new QLabel("To link your Telegram account, go to the @taskManagerQtBot\nand enter your registration code:");
	first->setFont(text);
	first->setTextInteractionFlags(Qt::TextSelectableByMouse);
	QLabel* second = new QLabel(QString::number(randomNum));
	second->setFont(fontCode);
	second->setAlignment(Qt::AlignCenter);
	second->setTextInteractionFlags(Qt::TextSelectableByMouse);
	QLabel* third = new QLabel("The code is temporary and valid for 10 minutes from this moment");
	third->setFont(text);

	errorText = dbManager->addNewTelegramCode(userId, randomNum);
	if (errorText != "") QMessageBox::warning(this, "error", errorText);

	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->addWidget(first);
	vbox->addWidget(second, Qt::AlignHCenter);
	vbox->addWidget(third);
}