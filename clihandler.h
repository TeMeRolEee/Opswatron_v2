#pragma once


#include <QtCore/QThread>

class CliHandler : public QThread {
Q_OBJECT

protected:
	void run() override;

private:
	bool stopCli = false;

public slots:

	void stopCli_slot();

signals:

	void newTask_signal(const QJsonObject &inputState);

	void stopCli_signal();
};


