#include "CreateFolderWindow.h"

CreateFolderWindow::CreateFolderWindow(DatabaseManager* dbManager, int userId, QWidget* parent) : QDialog(parent) {
	this->dbManager = dbManager;
	this->userId = userId;

	setMinimumSize(400, 250);

	line = new QLineEdit();
	line->setPlaceholderText("enter name here");

	QLabel* lineText = new QLabel("Folder name:");

	QHBoxLayout* lineHbox = new QHBoxLayout();
	lineHbox->addWidget(lineText);
	lineHbox->addWidget(line);

	QPushButton* createBtn = new QPushButton("Create");
	QPushButton* cancelBtn = new QPushButton("Cancel");

	QHBoxLayout* btnHbox = new QHBoxLayout();
	btnHbox->addWidget(createBtn);
	btnHbox->addWidget(cancelBtn);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addLayout(lineHbox);
	vbox->addLayout(btnHbox);

	connect(createBtn, &QPushButton::clicked, this, &CreateFolderWindow::createBtnClicked);
	connect(cancelBtn, &QPushButton::clicked, this, &CreateFolderWindow::cancelBtnClicked);

	setLayout(vbox);
}

void CreateFolderWindow::createBtnClicked() {
	if (line->text().simplified() == "") {
		QMessageBox::warning(this, "error", "line is empty");
		return;
	}
	emit dataReady(line->text().simplified());
	accept();
}

void CreateFolderWindow::cancelBtnClicked() {
	reject();
}