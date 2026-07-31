#include "EditTaskWindow.h"

EditTaskWindow::EditTaskWindow(const QModelIndex& index, QVector<Folder> folders, DatabaseManager* dbManager, QWidget* parent) : CreateTaskWindow(parent) {
	this->dbManager = dbManager;
	this->folders = folders;
	taskId = index.data(TaskModel::Roles::IdRole).toInt();
	folderId = index.data(TaskModel::Roles::FolderIdRole).toInt();

	title->setText(index.data(TaskModel::Roles::TitleRole).toString());
	priority->setCurrentIndex(index.data(TaskModel::Roles::PriorityRole).toInt());
	deadline->setDateTime(index.data(TaskModel::Roles::DeadlineRole).toDateTime());
	description->setText(index.data(TaskModel::Roles::DescriptionRole).toString());

	QString result = dbManager->checkNotification(index.data(TaskModel::Roles::IdRole).toInt());
	if (result == "")
		notifCheckBox->setChecked(true);
	else
		notification->setDateTime(QDateTime::fromString(result, "dd.MM.yyyy HH:mm"));

	QPushButton* deleteBtn = new QPushButton("Delete task", this);
	btnGrid->addWidget(deleteBtn, 0, 0, 1, 2);

	QLabel* folderLbl = new QLabel("Folder:", this);
	folderBox = new QComboBox(this);
	for (auto folder : folders) folderBox->addItem(folder.title);
	for (int i = 0; i < folders.size(); i++) if (folders[i].id == index.data(TaskModel::Roles::FolderIdRole).toInt()) folderBox->setCurrentIndex(i);

	grid->addWidget(folderLbl, 6, 0);
	grid->addWidget(folderBox, 6, 1);

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
	if (title->text().simplified() == "" or description->toPlainText().simplified() == "" or priority->currentIndex() == 0) {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}

	QVariant notifDate;
	if (notifCheckBox->isChecked())
		notifDate = QVariant(QVariant::DateTime);
	else
		notifDate = notification->dateTime();

	TaskData task = { title->text(), description->toPlainText(), priority->currentIndex(), deadline->dateTime(), QDateTime::currentDateTime(), notifDate };
	emit saveReady(task);
	emit folderChangeReady(taskId, folderId);
	accept();
}