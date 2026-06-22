#pragma once
#include <QMainWindow>

class TaskManagerQt : public QMainWindow {
    Q_OBJECT
public:
    TaskManagerQt(QWidget *parent = nullptr);
    ~TaskManagerQt();
};
