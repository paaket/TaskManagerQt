#pragma once
#include <QStyledItemDelegate>
#include <QPainter>
#include "TaskModel.h"

class TaskDelegator : public QStyledItemDelegate {
public:
	TaskDelegator(QObject* parent = nullptr);
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};