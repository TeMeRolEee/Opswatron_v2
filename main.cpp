#include <iostream>
#include <memory>

#include <QDebug>

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
    std::getline(std::cin, temp);
    //temp += R"({"interval":150,"map":[10,5],"other_players":[{"alive":true,"coords":[1,1],"dir":"RIGHT","id":0,"name":"DUMMY"}],"player":{"alive":true,"coords":[2,1],"dir":"LEFT","id":1,"name":"drop_table_users"},"walls":[{"coords":[0,1],"player_id":0},{"coords":[4,3],"player_id":1}]})";
    serverResponse = QString::fromStdString(temp);
    core->initCore(serverResponse);
    QJsonObject qJsonObject = QJsonDocument::fromJson(serverResponse.toUtf8()).object();
    core->processData(qJsonObject);

    while (go) {
        //core->think();
        std::getline(std::cin, temp);
        //auto point1 = std::chrono::high_resolution_clock::now();
        serverResponse = QString::fromStdString(temp);
        qJsonObject = QJsonDocument::fromJson(serverResponse.toUtf8()).object();
        core->processData(qJsonObject);
        serverResponse.clear();
        //auto point2 = std::chrono::high_resolution_clock::now();
        //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point2 - point1).count();
        //qDebug() << "Time took to calculate everything" << duration << "microseconds";
    }
}
