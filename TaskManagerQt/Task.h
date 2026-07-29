#pragma once
#include <QString>
#include <QDateTime>

struct Task {
	int id;
	int userId;
	int folderId;
	QString title;
	QString description;
	int priority;
	QDateTime deadline;
	bool completed;
	QDateTime createdAt;
};