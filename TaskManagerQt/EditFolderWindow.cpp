#include "EditFolderWindow.h"

EditFolderWindow::EditFolderWindow(DatabaseManager* dbManager, int userId, QWidget* parent, const Folder& folder) : CreateFolderWindow(dbManager, userId, parent) {
	this->folder = folder;
	createBtn->setText("Save");
	line->setText(folder.title);

	QPushButton* deleteBtn = new QPushButton("Delete folder");
	btnGrid->addWidget(deleteBtn, 1, 0, 1, 2);

	connect(deleteBtn, &QPushButton::clicked, this, &EditFolderWindow::deleteBtnClicked);
}

void EditFolderWindow::createBtnClicked() {
	if (line->text().simplified() == "") {
		QMessageBox::warning(this, "error", "line is empty");
		return;
	}
	folder.title = line->text().simplified();
	emit editFolder(folder);
	accept();
}

void EditFolderWindow::cancelBtnClicked() {
	reject();
}

void EditFolderWindow::deleteBtnClicked() {
	auto result = QMessageBox::question(this, "Confirmation", "Do you really want to delete the folder?\nAll tasks attached to it will be deleted along with the folder");
	if (result == QMessageBox::Yes) { 
		emit deleteFolder(folder.id); 
		accept();
	}
	else return;
}