#pragma once
#include <QString>

struct Task {
	int id = -1;
	int userId = -1;
	QString title;
	QString description;
	int priority = 1;
	QString deadline;
	bool completed = false;
	QString createdAt;
};