#include "CreateTaskWindow.h"

CreateTaskWindow::CreateTaskWindow(QWidget* parent) : QDialog(parent) {
	setMinimumSize(500, 300);

	QLabel* titleLbl = new QLabel("Title:");
	QLabel* priorityLbl = new QLabel("Priority:");
	QLabel* deadlineLbl = new QLabel("Deadline:");
	QLabel* descriptionLbl = new QLabel("Description:");

	title = new QLineEdit();
	title->setPlaceholderText("enter title here");
	priority = new QLineEdit();
	priority->setPlaceholderText("enter priority here");
	deadline = new QLineEdit();
	deadline->setPlaceholderText("enter deadline here");

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
		or priority->text().simplified() == "" or deadline->text().simplified() == "") {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}
	QVector<QString> data = { title->text(), description->toPlainText(), priority->text(), deadline->text() };
	emit saveReady(data);
	accept();
}