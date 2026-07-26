#include "TaskModel.h"

TaskModel::TaskModel(int userId, DatabaseManager* dbManager, QObject* parent) : QAbstractListModel(parent) {
    this->dbManager = dbManager;

    user = dbManager->findUserDataById(userId);

    QVector<Task> tempTasks = dbManager->findTasksById(userId);
    for (Task task : tempTasks) {
        addTask(task.id, task.userId, task.folderId, task.title, task.description, task.priority, task.deadline, task.completed, task.createdAt);
    }
}

QVariant TaskModel::data(const QModelIndex& index, int role) const {
	if (!index.isValid()) return QVariant();
	if (role == Qt::DisplayRole) return tasks.at(index.row()).title;
    if (role == Roles::IdRole) return tasks.at(index.row()).id;
    if (role == Roles::UserIdRole) return tasks.at(index.row()).userId;
    if (role == Roles::FolderIdRole) return tasks.at(index.row()).folderId;
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

Qt::ItemFlags TaskModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags();
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

bool TaskModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid()) return false;
    if (value.toString().simplified() == "") return false;
    if (role == Qt::EditRole) {
        QString errorText = dbManager->updateTaskTitle(value.toString(), index.data(Roles::IdRole).toInt());
        if (errorText != "") return false;

        tasks[index.row()].title = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole | Roles::TitleRole });
        return true;
    }
    return false;
}

void TaskModel::addTask(int id, int user_id, int folder_id, QString title, QString description, int priority, QString deadline, bool completed, QString createdAt) {
    beginInsertRows(QModelIndex(), tasks.size(), tasks.size());
    tasks.append(Task{ id, user_id, folder_id, title, description, priority, deadline, completed, createdAt });
    endInsertRows();
}

QString TaskModel::deleteTask(int id) {
    QString errorText = dbManager->deleteTaskById(id);
    if (errorText != "") return errorText;

    int currentIndex = -1;
    for (int i = 0; i < tasks.size(); i++) if (tasks[i].id == id) currentIndex = i;
    beginRemoveRows(QModelIndex(), currentIndex, currentIndex);
    tasks.removeAt(currentIndex);
    endRemoveRows();
    return "";
}

QString TaskModel::editTask(const CreateTaskWindow::TaskData& data, int id) {
    QString errorText = dbManager->updateTask(data, id);
    if (errorText != "") return errorText;

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
    QString errorText = dbManager->markTaskCompleted(id, newState);
    if (errorText != "") return errorText;

    int currentIndex = -1;
    for (int i = 0; i < tasks.size(); i++) if (tasks[i].id == id) currentIndex = i;

    tasks[currentIndex].completed = newState;
    QModelIndex ind = index(currentIndex, 0);
    emit dataChanged(ind, ind, { Qt::DisplayRole, Roles::CompletedRole });
    return "";
}

QString TaskModel::createTask(const CreateTaskWindow::TaskData& data, int currentFolderId) {
    QString errorText = dbManager->createTask(data, currentFolderId, user.id);
    if (errorText.startsWith("insert error")) return errorText;
    
    beginInsertRows(QModelIndex(), tasks.size(), tasks.size());
    tasks.append(Task{ errorText.toInt(), user.id,  currentFolderId, data.title, data.description, data.priority, data.deadline, 0, data.createdAt});
    endInsertRows();
    return "";
}

QString TaskModel::changeTasksFolder(int taskId, int folderId) {
    QString errorText = dbManager->updateTasksFolder(taskId, folderId);
    if (errorText != "") return errorText;

    int currentIndex = -1;
    for (int i = 0; i < tasks.size(); i++) if (tasks[i].id == taskId) currentIndex = i;

    tasks[currentIndex].folderId = folderId;
    QModelIndex ind = index(currentIndex, 0);
    emit dataChanged(ind, ind, { Qt::DisplayRole, Roles::FolderIdRole });
    return "";
}

User TaskModel::getCurrentUser() {
    return user;
}

QVector<Folder> TaskModel::getFolders() {
    return dbManager->findFoldersByUserId(user.id);
}