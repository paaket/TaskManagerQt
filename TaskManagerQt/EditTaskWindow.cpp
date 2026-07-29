#include "EditTaskWindow.h"

EditTaskWindow::EditTaskWindow(QWidget* parent, const QList<QString>& list, const QVector<Folder>& folders) : CreateTaskWindow(parent) {
	this->folders = folders;
	taskId = list[4].toInt();
	folderId = list[5].toInt();

	title->setText(list[0]);
	priority->setCurrentIndex(list[2].toInt());
	deadline->setDateTime(QDateTime::fromString(list[3], "dd.MM.yyyy HH:mm"));
	description->setText(list[1]);

	QPushButton* deleteBtn = new QPushButton("Delete task", this);
	btnGrid->addWidget(deleteBtn, 0, 0, 1, 2);

	QLabel* folderLbl = new QLabel("Folder:", this);
	folderBox = new QComboBox(this);
	for (auto folder : folders) folderBox->addItem(folder.title);
	for (int i = 0; i < folders.size(); i++) if (folders[i].id == list[5].toInt()) folderBox->setCurrentIndex(i);

	grid->addWidget(folderLbl, 4, 0);
	grid->addWidget(folderBox, 4, 1);

	connect(deleteBtn, &QPushButton::clicked, this, &EditTaskWindow::deleteClicked);
	connect(folderBox, &QComboBox::currentTextChanged, this, &EditTaskWindow::folderChanged);
}

void EditTaskWindow::deleteClicked() {
	QString msg = "Delete selected task?\nThe action cannot be undone";
	int ret = QMessageBox::question(this, "delete task", msg, QMessageBox::Yes | QMessageBox::No);
	if (ret == QMessageBox::Yes) {
		emit deleteReady(taskId);
		accept();
	}
}

void EditTaskWindow::folderChanged(const QString& text) {
	for (auto folder : folders)
		if (folder.title == text)
			folderId = folder.id;
}

void EditTaskWindow::saveClicked() {
	if (title->text().simplified() == "" or description->toPlainText().simplified() == ""
		or priority->currentIndex() == 0 or deadline->text().simplified() == "") {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}
	TaskData task = { title->text(), description->toPlainText(), priority->currentIndex(), deadline->dateTime(), QDateTime::currentDateTime() };
	emit saveReady(task);
	emit folderChangeReady(taskId, folderId);
	accept();
}