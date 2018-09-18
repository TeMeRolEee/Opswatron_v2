#include <iostream>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>


#include "Core.h"

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

    QString serverResponse;
    bool go = true;
    std::string temp;

    //Getting the info of the game
    //std::getline(std::cin, temp);
    temp += R"({"interval":150,"map":[10,5],"other_players":[{"alive":true,"coords":[1,1],"dir":"RIGHT","id":0,"name":"DUMMY"}],"player":{"alive":true,"coords":[2,1],"dir":"LEFT","id":1,"name":"drop_table_users"},"walls":[{"coords":[0,1],"player_id":0},{"coords":[4,3],"player_id":1}]})";
    serverResponse = QString::fromStdString(temp);
    core->initCore(serverResponse);

    while (go) {
        //std::getline(std::cin, temp);
        serverResponse = QString::fromStdString(temp);
        QJsonObject qJsonObject = QJsonDocument::fromJson(serverResponse.toUtf8()).object();
        core->processData(qJsonObject);
        serverResponse.clear();
        go = false;
    }
}
