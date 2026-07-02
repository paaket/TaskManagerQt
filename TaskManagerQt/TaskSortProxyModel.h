#pragma once
#include <QSortFilterProxyModel>
#include <QDate>
#include "TaskModel.h"

class TaskSortProxyModel : public QSortFilterProxyModel {
	Q_OBJECT
public:
	TaskSortProxyModel(QObject* parent = nullptr);
	virtual bool lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const;
};