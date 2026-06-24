#include "EditTaskWindow.h"

EditTaskWindow::EditTaskWindow(QWidget* parent, const QList<QString>& list) : CreateTaskWindow(parent) {
	title->setText(list[0]);
	priority->setCurrentIndex(list[2].toInt());
	deadline->setDate(QDate::fromString(list[3], "dd.MM.yyyy"));
	description->setText(list[1]);
}