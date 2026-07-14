#pragma once
#include "CreateTaskWindow.h"
#include "EditTaskWindow.h"
#include "LoginWindow.h"
#include "TaskDelegator.h"
#include "TaskModel.h"
#include "TaskSortProxyModel.h"
#include "DatabaseManager.h"
#include "EditUserWindow.h"
#include "DeleteUserWindow.h"
#include "FolderModel.h"
#include "CreateFolderWindow.h"
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QGridLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QFrame>
#include <QLabel>
#include <QString>
#include <QMessageBox>
#include <QList>
#include <QLineEdit>
#include <QComboBox>
#include <QDate>
#include <QSettings>
#include <QCoreApplication>
#include <QApplication>
#include <QAction>
#include <QMenuBar>
#include <QModelIndex>
#include <QDebug>

class TaskManagerQt : public QMainWindow {
    Q_OBJECT
public:
    TaskManagerQt(DatabaseManager* dbManager, QWidget* parent = nullptr);
    ~TaskManagerQt();
private slots:
    void addTask();
    void deleteTask();
    void editTask();
    void markAsCompleted();
    void addFolder();
    void sortTasks(int index);
    void showTask(const QModelIndex& index);
    void handTaskCreateData(const CreateTaskWindow::TaskData& data);
    void handFolderCreateData(const QString& title);
    void handEditData(const CreateTaskWindow::TaskData& data);
    void exitAccount();
    void editAccount();
    void deleteAccount();
    void folderChanged(const QModelIndex& index);
private:
    QListView* taskList;
    QListView* folderList;
    QLabel* infoWidget;
    QLineEdit* line;
    TaskModel* taskModel;
    FolderModel* folderModel;
    TaskSortProxyModel* taskProxy;
    TaskDelegator* taskDelegator;
    DatabaseManager* dbManager;
};