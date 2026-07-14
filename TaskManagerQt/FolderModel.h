#pragma once
#include <QAbstractListModel>
#include <QVector>
#include "DatabaseManager.h"
#include "Folder.h"

class FolderModel : public QAbstractListModel {
public:
	enum Roles {
		FolderIdRole = Qt::UserRole,
		UserIdRole,
		TitleRole
	};
	FolderModel(int userId, DatabaseManager* dbManager, QObject* parent = nullptr);
	void addFolder(int folderId, int userId, const QString& title);
	QString createFolder(const QString& title);
	QString updateFolder(const Folder& folder);
	QString deleteFolder(int folderId);
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
private:
	QVector<Folder> folders;
	int userId;
	DatabaseManager* dbManager;
};