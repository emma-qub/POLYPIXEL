#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T19:47:25
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = POLYPIXEL
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cxx \
    GUI/Views/AchievementsView.cxx \
    GUI/Views/CreateLevelView.cxx \
    GUI/Views/GameView.cxx \
    GUI/Views/LevelsView.cxx \
    GUI/Views/LoadingView.cxx \
    GUI/Views/MainMenuView.cxx \
    GUI/Views/MainWindow.cxx \
    GUI/Views/PauseView.cxx \
    GUI/Views/SaveLevelView.cxx \
    GUI/Views/TestLevelView.cxx \
    GUI/Views/WorldsView.cxx \
    GUI/Views/OptionsView.cxx \
    Core/Point.cxx \
    Core/Polygon.cxx \
    Core/Segment.cxx \
    Core/Vector.cxx \
    GUI/Controllers/GameController.cxx \
    GUI/Models/GameModel.cxx \
    GUI/Models/WorldsModel.cxx \
    GUI/Controllers/WorldsController.cxx \
    GUI/Controllers/LevelsController.cxx \
    GUI/Models/LevelsModel.cxx \
    Parser/Serializer.cxx \
    Parser/Parser.cxx \
    Objects/Object.cxx \
    Objects/Deviation.cxx \
    Objects/Obstacle.cxx \
    Objects/Mirror.cxx \
    Objects/Portal.cxx \
    Objects/Polarisator.cxx \
    Objects/Tape.cxx \
    Objects/Diode.cxx \
    GUI/Controllers/CreateLevelController.cxx \
    GUI/Models/AbstractLevelModel.cxx \
    GUI/Views/GameScribblingView.cxx \
    GUI/Views/CreateLevelScribblingView.cxx \
    GUI/Views/AbstractScribblingView.cxx \
    GUI/Models/CreateLevelModel.cxx \
    GUI/Commands/Commands.cxx

HEADERS += \
    GUI/Views/AchievementsView.hxx \
    GUI/Views/CreateLevelView.hxx \
    GUI/Views/GameView.hxx \
    GUI/Views/LevelsView.hxx \
    GUI/Views/LoadingView.hxx \
    GUI/Views/MainMenuView.hxx \
    GUI/Views/MainWindow.hxx \
    GUI/Views/PauseView.hxx \
    GUI/Views/SaveLevelView.hxx \
    GUI/Views/TestLevelView.hxx \
    GUI/Views/WorldsView.hxx \
    GUI/Views/OptionsView.hxx \
    Core/Point.hxx \
    Core/Polygon.hxx \
    Core/Segment.hxx \
    Core/Vector.hxx \
    GUI/Controllers/GameController.hxx \
    GUI/Models/GameModel.hxx \
    GUI/Models/WorldsModel.hxx \
    GUI/Controllers/WorldsController.hxx \
    GUI/Controllers/LevelsController.hxx \
    GUI/Models/LevelsModel.hxx \
    Parser/Serializer.hxx \
    Parser/Parser.hxx \
    Objects/Object.hxx \
    Objects/Deviation.hxx \
    Objects/Obstacle.hxx \
    Objects/Mirror.hxx \
    Objects/Portal.hxx \
    Objects/Polarisator.hxx \
    Objects/Tape.hxx \
    Objects/Diode.hxx \
    GUI/Controllers/CreateLevelController.hxx \
    GUI/Models/AbstractLevelModel.hxx \
    GUI/Views/GameScribblingView.hxx \
    GUI/Views/CreateLevelScribblingView.hxx \
    GUI/Views/AbstractScribblingView.hxx \
    GUI/Models/CreateLevelModel.hxx \
    GUI/Commands/Commands.hxx

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
