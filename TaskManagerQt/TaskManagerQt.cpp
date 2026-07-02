#include "TaskManagerQt.h"

QString priorityToQString(const QString& priority) {
    if (priority == "1") return "Low";
    if (priority == "2") return "Medium";
    if (priority == "3") return "High";
    return "None";
}

TaskManagerQt::TaskManagerQt(DatabaseManager* dbManager, QWidget *parent) : QMainWindow(parent) {
    this->dbManager = dbManager;

    QSettings settings("Paket", "TaskManagerQt");
    restoreGeometry(settings.value("geometry").toByteArray());
    
    model = new TaskModel(settings.value("currentUserId").toInt(), dbManager);
    proxy = new TaskSortProxyModel(this);
    proxy->setSourceModel(model);

    delegator = new TaskDelegator(this);

    list = new QListView();
    list->setModel(proxy);
    list->setItemDelegate(delegator);

    QMenu* menu = menuBar()->addMenu("Account");
    QAction* exit = menu->addAction("Exit");

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    infoWidget = new QLabel();
    infoWidget->setFrameShape(QFrame::StyledPanel);
    infoWidget->setFrameShadow(QFrame::Sunken);
    infoWidget->setLineWidth(1);
    QFont font;
    font.setPointSize(11);
    infoWidget->setFont(font);
    infoWidget->setAlignment(Qt::AlignTop);

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
    vbox->addLayout(grid);

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
    connect(comboBox, &QComboBox::currentIndexChanged, this, &TaskManagerQt::sortTasks);
    connect(exit, &QAction::triggered, this, &TaskManagerQt::exitAccount);
    connect(line, &QLineEdit::textChanged, proxy, &QSortFilterProxyModel::setFilterFixedString);
    connect(list, &QListView::clicked, this, &TaskManagerQt::showTask);

    mainWidget->setLayout(hbox);

    statusBar()->showMessage("started", 3000);
}

void TaskManagerQt::addTask() {
    CreateTaskWindow window(this);
    connect(&window, &CreateTaskWindow::saveReady, this, &TaskManagerQt::handCreateData);
    window.exec();
}

void TaskManagerQt::deleteTask() {
    if (!proxy->mapToSource(list->currentIndex()).isValid()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QString result;
    QString msg = "Delete selected task?\nThe action cannot be undone";
    int ret = QMessageBox::question(this, "delete task", msg, QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) result = model->deleteTask(proxy->mapToSource(list->currentIndex()).data(Qt::UserRole).toInt());
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully deleted", 3000);
}

void TaskManagerQt::editTask() {
    if (!proxy->mapToSource(list->currentIndex()).isValid()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QList<QString> lst = { proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::TitleRole).toString(),
    proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::DescriptionRole).toString(),
    proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::PriorityRole).toString(),
    proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::DeadlineRole).toString() };
    EditTaskWindow window(this, lst);
    connect(&window, &EditTaskWindow::saveReady, this, &TaskManagerQt::handEditData);
    window.exec();
}

void TaskManagerQt::markAsCompleted() {
    if (!proxy->mapToSource(list->currentIndex()).isValid()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QString result;
    int newStatus = (proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::CompletedRole).toInt() == 0) ? 1 : 0;
    result = model->markCompleted(proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::IdRole).toInt(), newStatus);
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully marked", 3000);
}

void TaskManagerQt::sortTasks(int index) {
    list->blockSignals(true);
    infoWidget->setText("");
    switch (index) {
    case 1:
        proxy->setSortRole(TaskModel::Roles::PriorityRole);
        proxy->sort(0, Qt::DescendingOrder);
        break;
    case 2:
        proxy->setSortRole(TaskModel::Roles::PriorityRole);
        proxy->sort(0);
        break;
    case 3:
        proxy->setSortRole(TaskModel::Roles::DeadlineRole);
        proxy->sort(0, Qt::DescendingOrder);
        break;
    case 4:
        proxy->setSortRole(TaskModel::Roles::DeadlineRole);
        proxy->sort(0);
        break;
    case 5:
        proxy->setSortRole(TaskModel::Roles::CompletedRole);
        proxy->sort(0);
        break;
    case 6:
        proxy->setSortRole(TaskModel::Roles::CompletedRole);
        proxy->sort(0, Qt::DescendingOrder);
        break;
    case 7:
        proxy->setSortRole(TaskModel::Roles::CreatedAtRole);
        proxy->sort(0);
        break;
    case 8:
        proxy->setSortRole(TaskModel::Roles::CreatedAtRole);
        proxy->sort(0, Qt::DescendingOrder);
        break;
    }
    list->blockSignals(false);
}

void TaskManagerQt::showTask(const QModelIndex& index) {
    QString priorityText = priorityToQString(index.data(TaskModel::Roles::PriorityRole).toString());
    QString statusText = index.data(TaskModel::Roles::CompletedRole).toBool() == 1 ? "Completed" : "Not Completed";
    QString text = "Title: " + index.data(TaskModel::Roles::TitleRole).toString() +
        "\nDescription: " + index.data(TaskModel::Roles::DescriptionRole).toString() +
        "\nPriority: " + priorityText +
        "\nDeadline: " + index.data(TaskModel::Roles::DeadlineRole).toString() +
        "\nStatus: " + statusText +
        "\nCreated at: " + index.data(TaskModel::Roles::CreatedAtRole).toString() +
        "\nId: " + index.data(TaskModel::Roles::IdRole).toString();
    infoWidget->setText(text);
}

void TaskManagerQt::handCreateData(const CreateTaskWindow::TaskData& data) {
    QString result;
    result = model->createTask(data);
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully added", 3000);
}

void TaskManagerQt::handEditData(const CreateTaskWindow::TaskData& data) {
    QString result;
    result = model->editTask(data, proxy->mapToSource(list->currentIndex()).data(TaskModel::Roles::IdRole).toInt());
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully edited", 3000);
}

void TaskManagerQt::exitAccount() {
    QSettings settings("Paket", "TaskManagerQt");
    settings.remove("currentUserId");
    QApplication::quit();
}

TaskManagerQt::~TaskManagerQt() {
    QSettings settings("Paket", "TaskManagerQt");
    settings.setValue("geometry", saveGeometry());
    if (settings.value("remember") == 0) settings.remove("currentUserId");
}