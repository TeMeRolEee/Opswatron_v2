#include <iostream>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QDebug>

#include "core.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationVersion(QStringLiteral("0.1.0"));

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addOption(
			{{QStringLiteral("i"), QStringLiteral("info")}, QStringLiteral("Get the players info")}
	);

	parser.process(app);

	if (parser.isSet(QStringLiteral("i"))) {
		std::cout << R"({"name":"drop_table_users"})" << std::endl;
		return 0;
	}

	auto core = std::make_unique<Core>();

	core->start();

	core->init();


	return QCoreApplication::exec();
}
