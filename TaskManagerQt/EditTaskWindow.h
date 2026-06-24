#pragma once
#include "CreateTaskWindow.h"
#include <QWidget>
#include <QList>

class EditTaskWindow : public CreateTaskWindow {
	Q_OBJECT
public:
	EditTaskWindow(QWidget* parent = nullptr, const QList<QString>& list = {});
};