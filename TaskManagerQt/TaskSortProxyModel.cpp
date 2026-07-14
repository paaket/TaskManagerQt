#include "TaskSortProxyModel.h"

TaskSortProxyModel::TaskSortProxyModel(QObject* parent) : QSortFilterProxyModel(parent) {}

void TaskSortProxyModel::setFolderFilter(int folderId) {
    currentFolderId = folderId;
    invalidateFilter();
}

void TaskSortProxyModel::setSearchText(const QString& text) {
    searchText = text.trimmed();
    invalidateFilter();
}

bool TaskSortProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    int taskFolderId = index.data(TaskModel::Roles::FolderIdRole).toInt();
    if (taskFolderId != currentFolderId) {
        return false;
    }

    if (!searchText.isEmpty()) {
        QString taskTitle = index.data(TaskModel::Roles::TitleRole).toString();
        if (!taskTitle.contains(searchText, Qt::CaseInsensitive)) {
            return false;
        }
    }
    return true;
}