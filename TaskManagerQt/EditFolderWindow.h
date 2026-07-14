#pragma once
#include <QMessageBox>
#include "CreateFolderWindow.h"
#include "Folder.h"

class EditFolderWindow : public CreateFolderWindow {
	Q_OBJECT
public:
	EditFolderWindow(DatabaseManager* dbManager, int userId, QWidget* parent = nullptr, const Folder& folder = {});
signals:
	void editFolder(const Folder& folder);
	void deleteFolder(int folderId);
private slots:
	void createBtnClicked();
	void cancelBtnClicked();
	void deleteBtnClicked();
private:
	Folder folder;
};