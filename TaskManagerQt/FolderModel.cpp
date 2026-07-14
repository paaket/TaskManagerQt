#include "FolderModel.h"

FolderModel::FolderModel(int userId, DatabaseManager* dbManager, QObject* parent) : QAbstractListModel(parent) {
	this->dbManager = dbManager;
	this->userId = userId;

	QVector<Folder> userFolders = dbManager->findFoldersByUserId(userId);
	for (auto folder : userFolders) {
		addFolder(folder.id, folder.userId, folder.title);
	}
}

void FolderModel::addFolder(int folderId, int userId, const QString& title) {
	beginInsertRows(QModelIndex(), folders.size(), folders.size());
	folders.append(Folder{ folderId, userId, title });
	endInsertRows();
}

QString FolderModel::createFolder(const QString& title) {
	QString errorText = dbManager->createFolder(title, userId);
	if (errorText.startsWith("insert error")) return errorText;

	beginInsertRows(QModelIndex(), folders.size(), folders.size());
	folders.append(Folder{ errorText.toInt(), userId, title });
	endInsertRows();
	return "";
}

QString FolderModel::updateFolder(const Folder& folder) {
	QString errorText = dbManager->updadeFolder(folder);
	if (errorText != "") return errorText;

	int currentIndex = -1;
	for (int i = 0; i < folders.size(); i++) if (folders[i].id == folder.id) currentIndex = i;

	folders[currentIndex].title = folder.title;
	folders[currentIndex].id = folder.id;
	folders[currentIndex].userId = folder.userId;
	QModelIndex ind = index(currentIndex, 0);
	emit dataChanged(ind, ind, { Qt::DisplayRole, Roles::TitleRole });
	return "";
}

QString FolderModel::deleteFolder(int folderId) {
	QString errorText = dbManager->deleteFolderById(folderId);
	if (errorText != "") return errorText;

	int currentIndex = -1;
	for (int i = 0; i < folders.size(); i++) if (folders[i].id == folderId) currentIndex = i;
	beginRemoveRows(QModelIndex(), currentIndex, currentIndex);
	folders.removeAt(currentIndex);
	endRemoveRows();
	return "";
}

QVariant FolderModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid()) return QVariant();
	if (role == Qt::DisplayRole) return folders.at(index.row()).title;
	if (role == Roles::FolderIdRole) return folders.at(index.row()).id;
	if (role == Roles::UserIdRole) return folders.at(index.row()).userId;
	if (role == Roles::TitleRole) return folders.at(index.row()).title;
	return QVariant();
}

int FolderModel::rowCount(const QModelIndex& parent) const {
	return folders.size();
}