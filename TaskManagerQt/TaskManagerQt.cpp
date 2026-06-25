#include "TaskManagerQt.h"

QString priorityToQString(QString priority) {
    if (priority == "1") return "Low";
    if (priority == "2") return "Medium";
    if (priority == "3") return "High";
    return "None";
}

QString taskTitle(const QString& priority, const QString& title, const QString& deadline) {
    return "[" + priority + "] " + title + "\nDue: " + deadline;
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
        QListWidgetItem* item = new QListWidgetItem(taskTitle(priorityToQString(query.value(3).toString()), query.value(1).toString(), query.value(4).toString()));
        item->setData(Roles::IdRole, query.value(0).toString());
        item->setData(Roles::TitleRole, query.value(1).toString());
        item->setData(Roles::DescriptionRole, query.value(2).toString());
        item->setData(Roles::PriorityRole, query.value(3).toString());
        item->setData(Roles::DeadlineRole, query.value(4).toString());
        item->setData(Roles::CompletedRole, query.value(5).toString());
        item->setData(Roles::CreatedAtRole, query.value(6).toString());
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

    line = new QLineEdit();
    line->setPlaceholderText("search by title: ");

    QComboBox* comboBox = new QComboBox();
    comboBox->addItem("Filters");
    comboBox->addItem("By priority (High to Low)");
    comboBox->addItem("By priority (Low to High)");
    comboBox->addItem("By deadline (Soonest to Lastest)");
    comboBox->addItem("By deadline (Lastest to Soonest)");
    comboBox->addItem("By status (Incomplete to Completed)");
    comboBox->addItem("By status (Completed to Incomplete)");
    comboBox->addItem("By date created (Oldest to Newest)");
    comboBox->addItem("By date created (Newest to Oldest)");
    comboBox->setCurrentIndex(0);

    QHBoxLayout* leftHbox = new QHBoxLayout();
    leftHbox->addWidget(line);
    leftHbox->addWidget(comboBox);

    QVBoxLayout* leftVbox = new QVBoxLayout();
    leftVbox->addLayout(leftHbox);
    leftVbox->addWidget(list);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addLayout(leftVbox);
    hbox->addLayout(vbox);

    connect(addBtn, &QPushButton::clicked, this, &TaskManagerQt::addTask);
    connect(deleteBtn, &QPushButton::clicked, this, &TaskManagerQt::deleteTask);
    connect(editBtn, &QPushButton::clicked, this, &TaskManagerQt::editTask);
    connect(markCompleted, &QPushButton::clicked, this, &TaskManagerQt::markAsCompleted);
    connect(list, &QListWidget::currentItemChanged, this, &TaskManagerQt::showTask);
    connect(line, &QLineEdit::textChanged, this, &TaskManagerQt::searchTask);
    connect(comboBox, &QComboBox::currentIndexChanged, this, &TaskManagerQt::sortTasks);

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
        query.bindValue(":id", list->currentItem()->data(Roles::IdRole).toInt());
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
    QList<QString> lst = { list->currentItem()->data(Roles::TitleRole).toString(),
    list->currentItem()->data(Roles::DescriptionRole).toString(),
    list->currentItem()->data(Roles::PriorityRole).toString(),
    list->currentItem()->data(Roles::DeadlineRole).toString() };
    EditTaskWindow window(this, lst);
    connect(&window, &EditTaskWindow::saveReady, this, &TaskManagerQt::handEditData);
    window.exec();
}

void TaskManagerQt::markAsCompleted() {
    if (!list->currentItem()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    int newStatus = (list->currentItem()->data(Roles::CompletedRole).toInt() == 0) ? 1 : 0;
    QSqlQuery query;
    query.prepare("UPDATE tasks SET completed = :completed WHERE id = :id");
    query.bindValue(":completed", newStatus);
    query.bindValue(":id", list->currentItem()->data(Roles::IdRole));
    if (!query.exec()) {
        QMessageBox::warning(this, "error", "update error: " + query.lastError().text());
        return;
    }
    list->currentItem()->setData(Roles::CompletedRole, newStatus);
    statusBar()->showMessage("successfully marked", 3000);
}

void TaskManagerQt::searchTask(const QString& text) {
    if (text.isEmpty()) {
        for (int i = 0; i < list->count(); i++) {
            list->item(i)->setHidden(false);
        }
        return;
    }
    for (int i = 0; i < list->count(); i++)
        list->item(i)->setHidden(true);

    QList<QListWidgetItem*> items = list->findItems(text, Qt::MatchContains | Qt::MatchFixedString);
    for (QListWidgetItem* item : items)
        item->setHidden(false);
}

void TaskManagerQt::sortTasks(int index) {
    list->blockSignals(true);
    infoWidget->setText("");
    QList<QListWidgetItem*> items;
    while (list->count() > 0)
        items.append(list->takeItem(0));
    switch (index) {
    case 1:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) { 
            return first->data(Roles::PriorityRole).toInt() > second->data(Roles::PriorityRole).toInt(); });
        break;
    case 2:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return first->data(Roles::PriorityRole).toInt() < second->data(Roles::PriorityRole).toInt(); });
        break;
    case 3:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return QDate::fromString(first->data(Roles::DeadlineRole).toString(), "dd.MM.yyyy") < QDate::fromString(second->data(Roles::DeadlineRole).toString(), "dd.MM.yyyy"); });
        break;
    case 4:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return QDate::fromString(first->data(Roles::DeadlineRole).toString(), "dd.MM.yyyy") > QDate::fromString(second->data(Roles::DeadlineRole).toString(), "dd.MM.yyyy"); });
        break;
    case 5:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return first->data(Roles::CompletedRole).toInt() < second->data(Roles::CompletedRole).toInt(); });
        break;
    case 6:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return first->data(Roles::CompletedRole).toInt() > second->data(Roles::CompletedRole).toInt(); });
        break;
    case 7:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return QDate::fromString(first->data(Roles::CreatedAtRole).toString(), "dd.MM.yyyy") < QDate::fromString(second->data(Roles::CreatedAtRole).toString(), "dd.MM.yyyy"); });
        break;
    case 8:
        std::sort(items.begin(), items.end(), [](const QListWidgetItem* first, const QListWidgetItem* second) {
            return QDate::fromString(first->data(Roles::CreatedAtRole).toString(), "dd.MM.yyyy") > QDate::fromString(second->data(Roles::CreatedAtRole).toString(), "dd.MM.yyyy"); });
        break;
    }
    for (QListWidgetItem* item : items)
        list->addItem(item);
    list->blockSignals(false);
}

void TaskManagerQt::showTask(QListWidgetItem* current, QListWidgetItem* previous) {
    QString priorityText = priorityToQString(current->data(Roles::PriorityRole).toString());
    QString text = "Title: " + current->data(Roles::TitleRole).toString() +
        "\nDescription: " + current->data(Roles::DescriptionRole).toString() +
        "\nPriority: " + priorityText +
        "\nDeadline: " + current->data(Roles::DeadlineRole).toString() +
        "\nStatus: " + current->data(Roles::CompletedRole).toString() +
        "\nCreated at: " + current->data(Roles::CreatedAtRole).toString() + 
        "\nId: " + current->data(Roles::IdRole).toString();
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

    QListWidgetItem* item = new QListWidgetItem(taskTitle(priorityToQString(QString::number(data.priority)), data.title, data.deadline));
    item->setData(Roles::IdRole, query.lastInsertId().toString());
    item->setData(Roles::TitleRole, data.title);
    item->setData(Roles::DescriptionRole, data.description);
    item->setData(Roles::PriorityRole, data.priority);
    item->setData(Roles::DeadlineRole, data.deadline);
    item->setData(Roles::CompletedRole, 0);
    item->setData(Roles::CreatedAtRole, data.createdAt);
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
    query.bindValue(":id", list->currentItem()->data(Roles::IdRole));
    if (!query.exec()) {
        QMessageBox::warning(this, "error", "edit error: " + query.lastError().text());
        return;
    }

    list->currentItem()->setText(taskTitle(priorityToQString(QString::number(data.priority)), data.title, data.deadline));
    list->currentItem()->setData(Roles::TitleRole, data.title);
    list->currentItem()->setData(Roles::DescriptionRole, data.description);
    list->currentItem()->setData(Roles::PriorityRole, data.priority);
    list->currentItem()->setData(Roles::DeadlineRole, data.deadline);
    statusBar()->showMessage("successfully edited", 3000);
}

TaskManagerQt::~TaskManagerQt() {
    db.close();
}