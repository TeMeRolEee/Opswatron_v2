#include <iostream>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include "clihandler.h"

void CliHandler::run() {
	QString serverResponse;
	std::string input;
	QJsonObject qJsonObject;
	while (!stopCli) {
		std::getline(std::cin, input);
		serverResponse = QString::fromStdString(input);
		qJsonObject = QJsonDocument::fromJson(serverResponse.toUtf8()).object();
		emit newTask_signal(qJsonObject);
	}

	QThread::run();
}

void CliHandler::stopCli_slot() {
	stopCli = true;
}
