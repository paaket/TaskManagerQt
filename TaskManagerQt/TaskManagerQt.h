#pragma once
#include "CreateTaskWindow.h"
#include "EditTaskWindow.h"
#include "LoginWindow.h"
#include "TaskModel.h"
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
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QDebug>

class TaskManagerQt : public QMainWindow {
    Q_OBJECT
public:
    TaskManagerQt(QWidget* parent = nullptr);
    ~TaskManagerQt();
private slots:
    void addTask();
    void deleteTask();
    void editTask();
    void markAsCompleted();
    void sortTasks(int index);
    void handCreateData(const CreateTaskWindow::TaskData& data);
    void handEditData(const CreateTaskWindow::TaskData& data);
    void exitAccount();
private:
    QListView* list;
    QLabel* infoWidget;
    QLineEdit* line;
    TaskModel* model;
    QSortFilterProxyModel* proxy;
};