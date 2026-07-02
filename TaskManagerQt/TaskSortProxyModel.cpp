#include "TaskSortProxyModel.h"

TaskSortProxyModel::TaskSortProxyModel(QObject* parent) : QSortFilterProxyModel(parent) {

}

bool TaskSortProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const {
	QDate leftData = QDate::fromString(source_left.data(TaskModel::Roles::DeadlineRole).toString(), "dd.MM.yyyy");
	QDate rightData = QDate::fromString(source_right.data(TaskModel::Roles::DeadlineRole).toString(), "dd.MM.yyyy");
	return leftData < rightData;
}