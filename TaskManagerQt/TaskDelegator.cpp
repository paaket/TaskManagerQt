#include "TaskDelegator.h"

QString taskTitle(bool status, const QString& priority, const QString& title, const QString& deadline) {
    QString smile = (status == 1) ? "\u2705" : "\U0001F534";
    QString priorityText;
    if (priority == "1") priorityText = "Low";
    if (priority == "2") priorityText = "Medium";
    if (priority == "3") priorityText = "High";
    return smile + " [" + priorityText + "] " + title + "\nDue: " + deadline;
}

TaskDelegator::TaskDelegator(QObject* parent) : QStyledItemDelegate(parent) {

}

void TaskDelegator::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();
    if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, QColor("#f2f6fa"));
    else painter->fillRect(option.rect, option.palette.base());

    QFont taskFont = painter->font();
    taskFont.setPointSize(10);
    painter->setFont(taskFont);

    painter->drawText(option.rect, taskTitle(index.data(TaskModel::Roles::CompletedRole).toBool(),
        index.data(TaskModel::Roles::PriorityRole).toString(), index.data(TaskModel::Roles::TitleRole).toString(),
        index.data(TaskModel::Roles::DeadlineRole).toString()));
    painter->restore();
}

QSize TaskDelegator::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    return QSize(option.rect.width(), 40);
}