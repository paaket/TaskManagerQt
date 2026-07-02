#include "TaskModel.h"

TaskModel::TaskModel(int userId, QObject* parent) : QAbstractListModel(parent) {
	this->userId = userId;
    db = QSqlDatabase::database();

    QSqlQuery query;
    query.prepare("SELECT * FROM tasks WHERE user_id = :user_id");
    query.bindValue(":user_id", userId);
    if (!query.exec()) return;
    while (query.next()) {
        addTask(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toString(), query.value(3).toString(),
            query.value(4).toInt(), query.value(5).toString(), query.value(6).toBool(), query.value(7).toString());
    }
}

QVariant TaskModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid()) return QVariant();
	if (role == Qt::DisplayRole) return tasks.at(index.row()).title;
    if (role == Roles::IdRole) return tasks.at(index.row()).id;
    if (role == Roles::UserIdRole) return tasks.at(index.row()).userId;
    if (role == Roles::TitleRole) return tasks.at(index.row()).title;
    if (role == Roles::DescriptionRole) return tasks.at(index.row()).description;
    if (role == Roles::PriorityRole) return tasks.at(index.row()).priority;
    if (role == Roles::DeadlineRole) return tasks.at(index.row()).deadline;
    if (role == Roles::CompletedRole) return tasks.at(index.row()).completed;
    if (role == Roles::CreatedAtRole) return tasks.at(index.row()).createdAt;
    return QVariant();
}

int TaskModel::rowCount(const QModelIndex& parent) const {
	return tasks.size();
}

void TaskModel::addTask(int id, int user_id, QString title, QString description, int priority, QString deadline, bool completed, QString createdAt) {
    beginInsertRows(QModelIndex(), tasks.size(), tasks.size());
    tasks.append(Task{ id, user_id, title, description, priority, deadline, completed, createdAt });
    endInsertRows();
}

QString TaskModel::deleteTask(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) return "delete error: " + query.lastError().text();

    int currentIndex = -1;
    for (int i = 0; i < tasks.size(); i++) if (tasks[i].id == id) currentIndex = i;
    beginRemoveRows(QModelIndex(), currentIndex, currentIndex);
    tasks.removeAt(currentIndex);
    endRemoveRows();
    return "";
}

QString TaskModel::editTask(const CreateTaskWindow::TaskData& data, int id) {
    QSqlQuery query;
    query.prepare("UPDATE tasks SET title = :title, description = :description, priority = :priority, deadline = :deadline WHERE id = :id;");
    query.bindValue(":title", data.title);
    query.bindValue(":description", data.description);
    query.bindValue(":priority", data.priority);
    query.bindValue(":deadline", data.deadline);
    query.bindValue(":id", id);
    if (!query.exec()) return "update error: " + query.lastError().text();

    int currentIndex = - 1;
    for (int i = 0; i < tasks.size(); i++) if (tasks[i].id == id) currentIndex = i;

    tasks[currentIndex].title = data.title;
    tasks[currentIndex].description = data.description;
    tasks[currentIndex].priority = data.priority;
    tasks[currentIndex].deadline = data.deadline;
    QModelIndex ind = index(currentIndex, 0);
    emit dataChanged(ind, ind, {Qt::DisplayRole, Roles::TitleRole, Roles::DescriptionRole, Roles::PriorityRole, Roles::DeadlineRole});
    return "";
}

QString TaskModel::markCompleted(int id, int newState) {
    QSqlQuery query;
    query.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
    query.bindValue(":completed", newState);
    query.bindValue(":id", id);
    if (!query.exec()) return "update error: " + query.lastError().text();

    int currentIndex = -1;
    for (int i = 0; i < tasks.size(); i++) if (tasks[i].id == id) currentIndex = i;

    tasks[currentIndex].completed = newState;
    return "";
}

QString TaskModel::createTask(const CreateTaskWindow::TaskData& data) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks(user_id, title, description, priority, deadline, completed, created_at) VALUES (:user_id, :title, :description, :priority, :deadline, :completed, :created_at)");
    query.bindValue(":user_id", userId);
    query.bindValue(":title", data.title);
    query.bindValue(":description", data.description);
    query.bindValue(":priority", data.priority);
    query.bindValue(":deadline", data.deadline);
    query.bindValue(":completed", 0);
    query.bindValue(":created_at", data.createdAt);
    if (!query.exec()) return "insert error: " + query.lastError().text();
    
    beginInsertRows(QModelIndex(), tasks.size(), tasks.size());
    tasks.append(Task{ query.lastInsertId().toInt(), userId,  data.title, data.description, data.priority, data.deadline, 0, data.createdAt});
    endInsertRows();
    return "";
}