#include <iostream>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>

#include "Core.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    app.setApplicationVersion(QStringLiteral("0.1.0"));

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

    QString serverResponse;
    bool go = true;
    std::string temp;

    //Getting the info of the game
    std::getline(std::cin, temp);
    serverResponse = QString::fromStdString(temp);
    core->processData(serverResponse, true);
    while (go) {
        std::getline(std::cin, temp);
        serverResponse = QString::fromStdString(temp);
        core->processData(serverResponse, false);
        serverResponse.clear();
    }
}