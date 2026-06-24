#include "CreateTaskWindow.h"

CreateTaskWindow::CreateTaskWindow(QWidget* parent) : QDialog(parent) {
	setMinimumSize(500, 300);

	QLabel* titleLbl = new QLabel("Title:");
	QLabel* priorityLbl = new QLabel("Priority:");
	QLabel* deadlineLbl = new QLabel("Deadline:");
	QLabel* descriptionLbl = new QLabel("Description:");

	QList<QString> list = { "None", "Low", "Medium", "High" };

	title = new QLineEdit();
	title->setPlaceholderText("enter title here");
	priority = new QComboBox();
	priority->addItems(list);
	priority->setCurrentIndex(0);
	deadline = new QDateEdit();
	deadline->setDisplayFormat("dd.MM.yyyy");

	description = new QTextEdit();
	description->setPlaceholderText("enter description here");

	QGridLayout* grid = new QGridLayout();
	grid->addWidget(titleLbl, 0, 0);
	grid->addWidget(title, 0, 1);
	grid->addWidget(descriptionLbl, 1, 0, Qt::AlignTop);
	grid->addWidget(description, 1, 1);
	grid->addWidget(priorityLbl, 2, 0);
	grid->addWidget(priority, 2, 1);
	grid->addWidget(deadlineLbl, 3, 0);
	grid->addWidget(deadline, 3, 1);
	

	QPushButton* save = new QPushButton("Save");
	
	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->addLayout(grid);
	vbox->addWidget(save, 0, Qt::AlignRight);

	connect(save, &QPushButton::clicked, this, &CreateTaskWindow::saveClicked);

	setLayout(vbox);
}

void CreateTaskWindow::saveClicked() {
	if (title->text().simplified() == "" or description->toPlainText().simplified() == ""
		or priority->currentIndex() == 0 or deadline->text().simplified() == "") {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}
	TaskData task = { title->text(), description->toPlainText(), priority->currentIndex(), deadline->text(), QDate::currentDate().toString("dd.MM.yyyy") };
	emit saveReady(task);
	accept();
}