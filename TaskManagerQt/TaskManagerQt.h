#pragma once
#include "CreateTaskWindow.h"
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
#include <QVector>

class TaskManagerQt : public QMainWindow {
    Q_OBJECT
public:
    TaskManagerQt(QWidget *parent = nullptr);
    ~TaskManagerQt();
private slots:
    void addTask();
    void handAddData(const QVector<QString>& data);
private:
    QListWidget* list;
};