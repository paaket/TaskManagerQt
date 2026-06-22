#include "TaskManagerQt.h"

TaskManagerQt::TaskManagerQt(QWidget *parent) : QMainWindow(parent) {
    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    list = new QListWidget();

    QLabel* infoWidget = new QLabel();
    infoWidget->setFrameShape(QFrame::StyledPanel);
    infoWidget->setFrameShadow(QFrame::Sunken);
    infoWidget->setLineWidth(1);

    QPushButton* addBtn = new QPushButton("add task");
    QPushButton* deleteBtn = new QPushButton("delete task");
    QPushButton* editBtn = new QPushButton("edit a task");
    QPushButton* tempBtn = new QPushButton("temp btn");
    addBtn->setMinimumSize(130, 40);
    deleteBtn->setMinimumSize(130, 40);
    editBtn->setMinimumSize(130, 40);
    tempBtn->setMinimumSize(130, 40);

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(addBtn, 0, 0);
    grid->addWidget(deleteBtn, 0, 1);
    grid->addWidget(editBtn, 1, 0);
    grid->addWidget(tempBtn, 1, 1);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(infoWidget);
    vbox->addItem(grid);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(list);
    hbox->addLayout(vbox);

    connect(addBtn, &QPushButton::clicked, this, &TaskManagerQt::addTask);

    mainWidget->setLayout(hbox);

    statusBar()->showMessage("started", 3000);
}

void TaskManagerQt::addTask() {
    CreateTaskWindow window(this);
    connect(&window, &CreateTaskWindow::saveReady, this, &TaskManagerQt::handAddData);
    window.exec();
}

void TaskManagerQt::handAddData(const QVector<QString>& data) {

}

TaskManagerQt::~TaskManagerQt() {

}