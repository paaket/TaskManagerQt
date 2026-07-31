#include "CreateTaskWindow.h"

CreateTaskWindow::CreateTaskWindow(QWidget* parent) : QDialog(parent) {
	setMinimumSize(500, 300);

	QLabel* titleLbl = new QLabel("Title:");
	QLabel* priorityLbl = new QLabel("Priority:");
	QLabel* deadlineLbl = new QLabel("Deadline:");
	QLabel* descriptionLbl = new QLabel("Description:");
	QLabel* notificationLbl = new QLabel("Notification:");
	QLabel* notifCheckBoxLbl = new QLabel("Don't notify me:");

	QList<QString> list = { "None", "Low", "Medium", "High" };

	title = new QLineEdit();
	title->setPlaceholderText("enter title here");
	priority = new QComboBox();
	priority->addItems(list);
	priority->setCurrentIndex(0);
	deadline = new QDateTimeEdit();
	deadline->setDisplayFormat("dd.MM.yyyy HH:mm");
	QDateTime current = QDateTime::currentDateTime();
	QTime time = current.time();
	time.setHMS(time.hour(), time.minute(), 0, 0);
	current.setTime(time);
	deadline->setDateTime(current);
	deadline->setCalendarPopup(true);
	deadline->setMinimumDateTime(QDateTime::currentDateTime());

	notification = new QDateTimeEdit();
	notification->setDisplayFormat("dd.MM.yyyy HH:mm");
	notification->setDateTime(current);
	notification->setCalendarPopup(true);
	//notification->setMinimumDateTime(QDateTime::currentDateTime());
	notification->setMaximumDateTime(current);

	notifCheckBox = new QCheckBox();

	description = new QTextEdit();
	description->setPlaceholderText("enter description here");

	grid = new QGridLayout();
	grid->addWidget(titleLbl, 0, 0);
	grid->addWidget(title, 0, 1);
	grid->addWidget(descriptionLbl, 1, 0, Qt::AlignTop);
	grid->addWidget(description, 1, 1);
	grid->addWidget(priorityLbl, 2, 0);
	grid->addWidget(priority, 2, 1);
	grid->addWidget(deadlineLbl, 3, 0);
	grid->addWidget(deadline, 3, 1);
	grid->addWidget(notificationLbl, 4, 0);
	grid->addWidget(notification, 4, 1);
	grid->addWidget(notifCheckBoxLbl, 5, 0);
	grid->addWidget(notifCheckBox, 5, 1);
	
	QPushButton* saveBtn = new QPushButton("Save");

	btnGrid = new QGridLayout();
	btnGrid->addWidget(saveBtn, 1, 0, 1, 2);
	
	QVBoxLayout* vbox = new QVBoxLayout(this);
	vbox->addLayout(grid);
	vbox->addLayout(btnGrid);

	connect(saveBtn, &QPushButton::clicked, this, &CreateTaskWindow::saveClicked);
	connect(deadline, &QDateTimeEdit::dateTimeChanged, this, &CreateTaskWindow::changeNotifMaxTime);
	connect(notifCheckBox, &QCheckBox::toggled, notification, [this](bool checked) {
		notification->setEnabled(!checked);
		});

	setLayout(vbox);
}

void CreateTaskWindow::changeNotifMaxTime() {
	notification->setMaximumDateTime(deadline->dateTime());
}

void CreateTaskWindow::saveClicked() {
	if (title->text().simplified() == "" or description->toPlainText().simplified() == "" or priority->currentIndex() == 0) {
		QMessageBox::warning(this, "error", "fill in all fields");
		return;
	}

	QVariant notifDate;
	if (notifCheckBox->isChecked())
		notifDate = QVariant(QVariant::DateTime);
	else
		notifDate = notification->dateTime();

	TaskData task = { title->text(), description->toPlainText(), priority->currentIndex(), deadline->dateTime(), QDateTime::currentDateTime(), notifDate };
	emit saveReady(task);
	accept();
}