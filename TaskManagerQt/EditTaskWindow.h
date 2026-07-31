#pragma once
#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QLabel>
#include "CreateTaskWindow.h"
#include "Folder.h"
#include "DatabaseManager.h"
#include "TaskModel.h"

class EditTaskWindow : public CreateTaskWindow {
	Q_OBJECT
public:
	EditTaskWindow(const QModelIndex& index, QVector<Folder> folders, DatabaseManager* dbManager, QWidget* parent = nullptr);
private slots:
	void deleteClicked();
	void folderChanged(const QString& text);
	void saveClicked() override;
signals:
	void deleteReady(int iaskId);
	void folderChangeReady(int taskId, int folderId);
private:
	QComboBox* folderBox;
	int taskId;
	int folderId;
	QVector<Folder> folders;
	DatabaseManager* dbManager;
};