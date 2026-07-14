#pragma once
#include <QString>

struct Task {
	int id;
	int userId;
	int folderId;
	QString title;
	QString description;
	int priority;
	QString deadline;
	bool completed;
	QString createdAt;
};