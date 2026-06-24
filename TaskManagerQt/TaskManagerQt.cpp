#include "TaskManagerQt.h"

QString priorityToQString(QString priority) {
    if (priority == "1") return "Low";
    if (priority == "2") return "Medium";
    if (priority == "3") return "High";
    return "None";
}

TaskManagerQt::TaskManagerQt(QWidget *parent) : QMainWindow(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("taskManager.db");
    if (!db.open()) {
        QMessageBox::warning(this, "error", "database opening error");
        return;
    }
    QSqlQuery query;
    QString queryText;
    queryText = "CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT, title TEXT NOT NULL, description TEXT NOT NULL, priority INTEGER NOT NULL, deadline TEXT NOT NULL, completed INTEGER NOT NULL, created_at TEXT NOT NULL);";
    if (!query.exec(queryText)) {
        QMessageBox::warning(this, "error", "creating table error: " + query.lastError().text());
        return;
    }

    list = new QListWidget();
    queryText = "SELECT * FROM tasks";
    if (!query.exec(queryText)) {
        QMessageBox::warning(this, "error", "select error: " + query.lastError().text());
        return;
    }
    while (query.next()) {
        QString mainText = "[" + priorityToQString(query.value(3).toString()) + "] " + query.value(1).toString()
            + "\nDue: " + query.value(4).toString();
        QListWidgetItem* item = new QListWidgetItem(mainText);
        item->setData(Qt::UserRole, query.value(0).toString());
        item->setData(Qt::UserRole + 1, query.value(1).toString());
        item->setData(Qt::UserRole + 2, query.value(2).toString());
        item->setData(Qt::UserRole + 3, query.value(3).toString());
        item->setData(Qt::UserRole + 4, query.value(4).toString());
        item->setData(Qt::UserRole + 5, query.value(5).toString());
        item->setData(Qt::UserRole + 6, query.value(6).toString());
        list->addItem(item);
    }

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    infoWidget = new QLabel();
    infoWidget->setFrameShape(QFrame::StyledPanel);
    infoWidget->setFrameShadow(QFrame::Sunken);
    infoWidget->setLineWidth(1);

    QPushButton* addBtn = new QPushButton("add task");
    QPushButton* deleteBtn = new QPushButton("delete task");
    QPushButton* editBtn = new QPushButton("edit a task");
    QPushButton* markCompleted = new QPushButton("mark as completed");
    addBtn->setMinimumSize(130, 40);
    deleteBtn->setMinimumSize(130, 40);
    editBtn->setMinimumSize(130, 40);
    markCompleted->setMinimumSize(130, 40);

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(addBtn, 0, 0);
    grid->addWidget(deleteBtn, 0, 1);
    grid->addWidget(editBtn, 1, 0);
    grid->addWidget(markCompleted, 1, 1);

    QVBoxLayout* vbox = new QVBoxLayout();
    vbox->addWidget(infoWidget);
    vbox->addItem(grid);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addWidget(list);
    hbox->addLayout(vbox);

    connect(addBtn, &QPushButton::clicked, this, &TaskManagerQt::addTask);
    connect(deleteBtn, &QPushButton::clicked, this, &TaskManagerQt::deleteTask);
    connect(editBtn, &QPushButton::clicked, this, &TaskManagerQt::editTask);
    connect(markCompleted, &QPushButton::clicked, this, &TaskManagerQt::markAsCompleted);
    connect(list, &QListWidget::currentItemChanged, this, &TaskManagerQt::showTask);

    mainWidget->setLayout(hbox);

    statusBar()->showMessage("started", 3000);
}

void TaskManagerQt::addTask() {
    CreateTaskWindow window(this);
    connect(&window, &CreateTaskWindow::saveReady, this, &TaskManagerQt::handCreateData);
    window.exec();
}

void TaskManagerQt::deleteTask() {
    if (!list->currentItem()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QString msg = "Delete selected task?\nThe action cannot be undone";
    int ret = QMessageBox::question(this, "delete task", msg, QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM tasks WHERE id = :id");
        query.bindValue(":id", list->currentItem()->data(Qt::UserRole).toInt());
        if (!query.exec()) {
            QMessageBox::warning(this, "error", "delete error: " + query.lastError().text());
            return;
        }
        auto item = list->takeItem(list->currentRow());
        delete item;
        statusBar()->showMessage("successfully deleted", 3000);
    }
}

void TaskManagerQt::editTask() {
    if (!list->currentItem()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QList<QString> lst = { list->currentItem()->data(Qt::UserRole + 1).toString(),
    list->currentItem()->data(Qt::UserRole + 2).toString(),
    list->currentItem()->data(Qt::UserRole + 3).toString(),
    list->currentItem()->data(Qt::UserRole + 4).toString() };
    EditTaskWindow window(this, lst);
    connect(&window, &EditTaskWindow::saveReady, this, &TaskManagerQt::handEditData);
    window.exec();
}

void TaskManagerQt::markAsCompleted() {
    if (!list->currentItem()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    int newStatus = (list->currentItem()->data(Qt::UserRole + 5).toInt() == 0) ? 1 : 0;
    QSqlQuery query;
    query.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
    query.bindValue(":completed", newStatus);
    query.bindValue(":id", list->currentItem()->data(Qt::UserRole));
    if (!query.exec()) {
        QMessageBox::warning(this, "error", "update error: " + query.lastError().text());
        return;
    }
    list->currentItem()->setData(Qt::UserRole + 5, newStatus);
    statusBar()->showMessage("successfully marked", 3000);
}

void TaskManagerQt::showTask(QListWidgetItem* current, QListWidgetItem* previous) {
    if (previous == nullptr) return;
    QString priorityText = priorityToQString(current->data(Qt::UserRole + 3).toString());
    QString text = "Title: " + current->data(Qt::UserRole + 1).toString() +
        "\nDescription: " + current->data(Qt::UserRole + 2).toString() +
        "\nPriority: " + priorityText +
        "\nDeadline: " + current->data(Qt::UserRole + 4).toString() +
        "\nStatus: " + current->data(Qt::UserRole + 5).toString() +
        "\nCreated at: " + current->data(Qt::UserRole + 6).toString() + 
        "\nId: " + current->data(Qt::UserRole).toString();
    infoWidget->setText(text);
}

void TaskManagerQt::handCreateData(const CreateTaskWindow::TaskData& data) {
    QSqlQuery query;
    query.prepare("INSERT INTO tasks(title, description, priority, deadline, completed, created_at) VALUES (:title, :description, :priority, :deadline, :completed, :created_at)");
    query.bindValue(":title", data.title);
    query.bindValue(":description", data.description);
    query.bindValue(":priority", data.priority);
    query.bindValue(":deadline", data.deadline);
    query.bindValue(":completed", 0);
    query.bindValue(":created_at", data.createdAt);
    if (!query.exec()) {
        QMessageBox::warning(this, "error", "insert error: " + query.lastError().text());
        return;
    }

    QString mainText = "[" + priorityToQString(QString::number(data.priority)) + "] " + data.title + "\nDue: " + data.deadline;
    QListWidgetItem* item = new QListWidgetItem(mainText);
    item->setData(Qt::UserRole, query.lastInsertId().toString());
    item->setData(Qt::UserRole + 1, data.title);
    item->setData(Qt::UserRole + 2, data.description);
    item->setData(Qt::UserRole + 3, data.priority);
    item->setData(Qt::UserRole + 4, data.deadline);
    item->setData(Qt::UserRole + 5, 0);
    item->setData(Qt::UserRole + 6, data.createdAt);
    list->addItem(item);
    list->setCurrentRow(list->count() - 1);
    statusBar()->showMessage("successfully added", 3000);
}

void TaskManagerQt::handEditData(const CreateTaskWindow::TaskData& data) {
    QSqlQuery query;
    query.prepare("UPDATE tasks SET title = :title, description = :description, priority = :priority, deadline = :deadline WHERE id = :id;");
    query.bindValue(":title", data.title);
    query.bindValue(":description", data.description);
    query.bindValue(":priority", data.priority);
    query.bindValue(":deadline", data.deadline);
    query.bindValue(":id", list->currentItem()->data(Qt::UserRole));
    if (!query.exec()) {
        QMessageBox::warning(this, "error", "edit error: " + query.lastError().text());
        return;
    }

    QString mainText = "[" + priorityToQString(QString::number(data.priority)) + "] " + data.title + "\nDue: " + data.deadline;
    list->currentItem()->setText(mainText);
    list->currentItem()->setData(Qt::UserRole + 1, data.title);
    list->currentItem()->setData(Qt::UserRole + 2, data.description);
    list->currentItem()->setData(Qt::UserRole + 3, data.priority);
    list->currentItem()->setData(Qt::UserRole + 4, data.deadline);
    statusBar()->showMessage("successfully edited", 3000);
}

TaskManagerQt::~TaskManagerQt() {
    db.close();
}