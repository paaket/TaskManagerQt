#pragma once
#include <QWidget>
#include <QList>
#include <QComboBox>
#include <QLabel>
#include "CreateTaskWindow.h"
#include "Folder.h"
#include "DatabaseManager.h"

class EditTaskWindow : public CreateTaskWindow {
	Q_OBJECT
public:
	EditTaskWindow(QWidget* parent = nullptr, const QList<QString>& list = {}, const QVector<Folder>& folders = {});
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
};