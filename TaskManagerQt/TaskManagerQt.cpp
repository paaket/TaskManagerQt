#include "TaskManagerQt.h"

TaskManagerQt::TaskManagerQt(QWidget *parent) : QMainWindow(parent) {
    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    list = new QListWidget();

    infoWidget = new QLabel();
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
    connect(list, &QListWidget::currentItemChanged, this, &TaskManagerQt::showTask);

    mainWidget->setLayout(hbox);

    statusBar()->showMessage("started", 3000);
}

void TaskManagerQt::addTask() {
    CreateTaskWindow window(this);
    connect(&window, &CreateTaskWindow::saveReady, this, &TaskManagerQt::handAddData);
    window.exec();
}

void TaskManagerQt::showTask(QListWidgetItem* current, QListWidgetItem* previous) {
    QString text = "Title: " + current->data(Qt::UserRole + 1).toString() +
        "\nDescription: " + current->data(Qt::UserRole + 2).toString() +
        "\nPriority: " + current->data(Qt::UserRole + 3).toString() +
        "\nDeadline: " + current->data(Qt::UserRole + 4).toString() +
        "\nStatus: " + current->data(Qt::UserRole + 5).toString() +
        "\nCreated at: " + current->data(Qt::UserRole + 6).toString();
    infoWidget->setText(text);
}

void TaskManagerQt::handAddData(const QVector<QString>& data) {
    QListWidgetItem* item = new QListWidgetItem(data[0]);
    item->setData(Qt::UserRole, 0);
    item->setData(Qt::UserRole + 1, data[0]);
    item->setData(Qt::UserRole + 2, data[1]);
    item->setData(Qt::UserRole + 3, data[2]);
    item->setData(Qt::UserRole + 4, data[3]);
    item->setData(Qt::UserRole + 5, 0);
    item->setData(Qt::UserRole + 6, "000");
    list->addItem(item);
    list->setCurrentRow(list->count() - 1);
    statusBar()->showMessage("successfully added", 3000);
}

TaskManagerQt::~TaskManagerQt() {

}