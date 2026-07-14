#pragma once
#include <QSortFilterProxyModel>
#include "TaskModel.h"

class TaskSortProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    TaskSortProxyModel(QObject* parent = nullptr);
    void setFolderFilter(int folderId);
    void setSearchText(const QString& text);
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
private:
    int currentFolderId = -1;
    QString searchText = "";
};