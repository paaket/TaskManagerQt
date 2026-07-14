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
    
    taskModel = new TaskModel(settings.value("currentUserId").toInt(), dbManager);
    taskProxy = new TaskSortProxyModel(this);
    taskProxy->setSourceModel(taskModel);

    taskDelegator = new TaskDelegator(this);

    taskList = new QListView();
    taskList->setModel(taskProxy);
    taskList->setItemDelegate(taskDelegator);

    folderModel = new FolderModel(settings.value("currentUserId").toInt(), dbManager);

    QFont folderListFont;
    folderListFont.setPointSize(11);

    folderList = new QListView();
    folderList->setModel(folderModel);
    folderList->setViewMode(QListView::IconMode);
    folderList->setWrapping(false);
    folderList->setFixedHeight(30);
    folderList->setFont(folderListFont);

    QMenu* menu = menuBar()->addMenu("Account");
    QAction* edit = menu->addAction("Edit account");
    QAction* deleteAccount = menu->addAction("Delete account");
    QAction* exit = menu->addAction("Log out");

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    infoWidget = new QLabel();
    infoWidget->setFrameShape(QFrame::StyledPanel);
    infoWidget->setFrameShadow(QFrame::Sunken);
    infoWidget->setLineWidth(1);
    QFont infoWidgetFont;
    infoWidgetFont.setPointSize(11);
    infoWidget->setFont(infoWidgetFont);
    infoWidget->setAlignment(Qt::AlignTop);

    QPushButton* addBtn = new QPushButton("Add a task");
    QPushButton* deleteBtn = new QPushButton("Delete task");
    QPushButton* editTaskBtn = new QPushButton("Edit task");
    QPushButton* markCompleted = new QPushButton("Mark as completed");
    QPushButton* addFolder = new QPushButton("Add folder");
    QPushButton* editFolderBtn = new QPushButton("Edit folder");
    addBtn->setMinimumSize(130, 40);
    deleteBtn->setMinimumSize(130, 40);
    editTaskBtn->setMinimumSize(130, 40);
    markCompleted->setMinimumSize(130, 40);
    addFolder->setMinimumSize(130, 40);
    editFolderBtn->setMinimumSize(130, 40);

    QGridLayout* grid = new QGridLayout();
    grid->addWidget(addBtn, 0, 0);
    grid->addWidget(deleteBtn, 0, 1);
    grid->addWidget(editTaskBtn, 1, 0);
    grid->addWidget(markCompleted, 1, 1);
    grid->addWidget(addFolder, 2, 0);
    grid->addWidget(editFolderBtn, 2, 1);

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
    leftVbox->addWidget(folderList);
    leftVbox->addLayout(leftHbox);
    leftVbox->addWidget(taskList);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->addLayout(leftVbox);
    hbox->addLayout(vbox);

    connect(addBtn, &QPushButton::clicked, this, &TaskManagerQt::addTask);
    connect(deleteBtn, &QPushButton::clicked, this, &TaskManagerQt::deleteTask);
    connect(editTaskBtn, &QPushButton::clicked, this, &TaskManagerQt::editTask);
    connect(editFolderBtn, &QPushButton::clicked, this, &TaskManagerQt::editFolder);
    connect(markCompleted, &QPushButton::clicked, this, &TaskManagerQt::markAsCompleted);
    connect(addFolder, &QPushButton::clicked, this, &TaskManagerQt::addFolder);
    connect(comboBox, &QComboBox::currentIndexChanged, this, &TaskManagerQt::sortTasks);
    connect(exit, &QAction::triggered, this, &TaskManagerQt::exitAccount);
    connect(edit, &QAction::triggered, this, &TaskManagerQt::editAccount);
    connect(deleteAccount, &QAction::triggered, this, &TaskManagerQt::deleteAccount);
    connect(line, &QLineEdit::textChanged, this, [this](const QString& text) { taskProxy->setSearchText(text); });
    connect(taskList, &QListView::clicked, this, &TaskManagerQt::showTask);
    connect(folderList, &QListView::clicked, this, &TaskManagerQt::folderChanged);

    mainWidget->setLayout(hbox);

    statusBar()->showMessage("started", 3000);
}

void TaskManagerQt::addTask() {
    if (!folderList->currentIndex().isValid()) {
        QMessageBox::warning(this, "error", "shoose the folder");
        return;
    }
    CreateTaskWindow window(this);
    connect(&window, &CreateTaskWindow::saveReady, this, &TaskManagerQt::handTaskCreateData);
    window.exec();
}

void TaskManagerQt::deleteTask() {
    if (!taskProxy->mapToSource(taskList->currentIndex()).isValid()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QString result;
    QString msg = "Delete selected task?\nThe action cannot be undone";
    int ret = QMessageBox::question(this, "delete task", msg, QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) result = taskModel->deleteTask(taskProxy->mapToSource(taskList->currentIndex()).data(Qt::UserRole).toInt());
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully deleted", 3000);
}

void TaskManagerQt::editTask() {
    if (!taskProxy->mapToSource(taskList->currentIndex()).isValid()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QList<QString> lst = { taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::TitleRole).toString(),
    taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::DescriptionRole).toString(),
    taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::PriorityRole).toString(),
    taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::DeadlineRole).toString() };
    EditTaskWindow window(this, lst);
    connect(&window, &EditTaskWindow::saveReady, this, &TaskManagerQt::handTaskEditData);
    window.exec();
}

void TaskManagerQt::editFolder() {
    if (!folderList->currentIndex().isValid()) {
        QMessageBox::warning(this, "error", "shoose the folder");
        return;
    }
    Folder folder{ folderList->currentIndex().data(FolderModel::Roles::FolderIdRole).toInt(),
        folderList->currentIndex().data(FolderModel::Roles::UserIdRole).toInt(),
        folderList->currentIndex().data(FolderModel::Roles::TitleRole).toString() };
    EditFolderWindow window(dbManager, taskModel->getCurrentUser().id, this, folder);
    connect(&window, &EditFolderWindow::editFolder, this, &TaskManagerQt::handFolderEditData);
    connect(&window, &EditFolderWindow::deleteFolder, this, &TaskManagerQt::handFolderDeleteData);
    window.exec();
}

void TaskManagerQt::markAsCompleted() {
    if (!taskProxy->mapToSource(taskList->currentIndex()).isValid()) {
        QMessageBox::warning(this, "error", "shoose the task");
        return;
    }
    QString result;
    int newStatus = (taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::CompletedRole).toInt() == 0) ? 1 : 0;
    result = taskModel->markCompleted(taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::IdRole).toInt(), newStatus);
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully marked", 3000);
}

void TaskManagerQt::addFolder() {
    CreateFolderWindow window(dbManager, taskModel->getCurrentUser().id, this);
    connect(&window, &CreateFolderWindow::dataReady, this, &TaskManagerQt::handFolderCreateData);
    window.exec();
}

void TaskManagerQt::sortTasks(int index) {
    taskList->blockSignals(true);
    infoWidget->setText("");
    switch (index) {
    case 1:
        taskProxy->setSortRole(TaskModel::Roles::PriorityRole);
        taskProxy->sort(0, Qt::DescendingOrder);
        break;
    case 2:
        taskProxy->setSortRole(TaskModel::Roles::PriorityRole);
        taskProxy->sort(0);
        break;
    case 3:
        taskProxy->setSortRole(TaskModel::Roles::DeadlineRole);
        taskProxy->sort(0, Qt::DescendingOrder);
        break;
    case 4:
        taskProxy->setSortRole(TaskModel::Roles::DeadlineRole);
        taskProxy->sort(0);
        break;
    case 5:
        taskProxy->setSortRole(TaskModel::Roles::CompletedRole);
        taskProxy->sort(0);
        break;
    case 6:
        taskProxy->setSortRole(TaskModel::Roles::CompletedRole);
        taskProxy->sort(0, Qt::DescendingOrder);
        break;
    case 7:
        taskProxy->setSortRole(TaskModel::Roles::CreatedAtRole);
        taskProxy->sort(0);
        break;
    case 8:
        taskProxy->setSortRole(TaskModel::Roles::CreatedAtRole);
        taskProxy->sort(0, Qt::DescendingOrder);
        break;
    }
    taskList->blockSignals(false);
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

void TaskManagerQt::handTaskCreateData(const CreateTaskWindow::TaskData& data) {
    QString result;
    qDebug() << folderList->currentIndex().data(FolderModel::Roles::FolderIdRole).toInt();
    result = taskModel->createTask(data, folderList->currentIndex().data(FolderModel::Roles::FolderIdRole).toInt());
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully added", 3000);
}

void TaskManagerQt::handFolderCreateData(const QString& title) {
    QString errorText = folderModel->createFolder(title);
    if (errorText != "") {
        QMessageBox::warning(this, "error", errorText);
        return;
    }
    statusBar()->showMessage("successfully added", 3000);
}

void TaskManagerQt::handTaskEditData(const CreateTaskWindow::TaskData& data) {
    QString result;
    result = taskModel->editTask(data, taskProxy->mapToSource(taskList->currentIndex()).data(TaskModel::Roles::IdRole).toInt());
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    statusBar()->showMessage("successfully edited", 3000);
}

void TaskManagerQt::handFolderEditData(const Folder& folder) {
    QString result = folderModel->updateFolder(folder);
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    folderList->setViewMode(QListView::ListMode);
    folderList->setViewMode(QListView::IconMode);
    statusBar()->showMessage("successfully edited", 3000);
}

void TaskManagerQt::handFolderDeleteData(int folderId) {
    QString result = folderModel->deleteFolder(folderId);
    if (result != "") {
        QMessageBox::warning(this, "error", result);
        return;
    }
    taskList->setViewMode(QListView::ListMode);
    taskList->setViewMode(QListView::IconMode);
    statusBar()->showMessage("successfully deleted", 3000);
}

void TaskManagerQt::exitAccount() {
    QSettings settings("Paket", "TaskManagerQt");
    settings.remove("currentUserId");
    QApplication::quit();
}

void TaskManagerQt::editAccount() {
    EditUserWindow window(dbManager, this, taskModel->getCurrentUser());
    window.exec();
}

void TaskManagerQt::deleteAccount() {
    DeleteUserWindow window(taskModel->getCurrentUser(), dbManager, this);
    window.exec();
    QSettings settings("Paket", "TaskManagerQt");
    settings.remove("currentUserId");
    QApplication::quit();
}

void TaskManagerQt::folderChanged(const QModelIndex& index) {
    if (!index.isValid()) return;
    int selectedFolderId = index.data(FolderModel::Roles::FolderIdRole).toInt();
    taskProxy->setFolderFilter(selectedFolderId);
}

TaskManagerQt::~TaskManagerQt() {
    QSettings settings("Paket", "TaskManagerQt");
    settings.setValue("geometry", saveGeometry());
    if (settings.value("remember") == 0) settings.remove("currentUserId");
}