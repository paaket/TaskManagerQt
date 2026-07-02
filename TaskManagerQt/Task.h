#pragma once
#include <QString>

struct Task {
	int id;
	int userId;
	QString title;
	QString description;
	int priority;
	QString deadline;
	bool completed;
	QString createdAt;
};