#pragma once
#include "CreateTaskWindow.h"
#include "EditTaskWindow.h"
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QFrame>
#include <QLabel>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QList>
#include <QSqlError>
#include <QLineEdit>

class TaskManagerQt : public QMainWindow {
    Q_OBJECT
public:
    TaskManagerQt(QWidget *parent = nullptr);
    ~TaskManagerQt();
private slots:
    void addTask();
    void deleteTask();
    void editTask();
    void markAsCompleted();
    void searchTask(const QString& text);
    void showTask(QListWidgetItem* current, QListWidgetItem* previous);
    void handCreateData(const CreateTaskWindow::TaskData& data);
    void handEditData(const CreateTaskWindow::TaskData& data);
private:
    enum Roles {
        IdRole = Qt::UserRole,
        TitleRole,
        DescriptionRole,
        PriorityRole,
        DeadlineRole,
        CompletedRole,
        CreatedAtRole
    };
    QListWidget* list;
    QLabel* infoWidget;
    QLineEdit* line;
    QSqlDatabase db;
};