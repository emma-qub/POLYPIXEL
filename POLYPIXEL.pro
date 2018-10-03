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
    GUI/Views/LoadingView.cxx \
    GUI/Views/MainMenuView.cxx \
    GUI/Views/MainWindow.cxx \
    GUI/Views/PauseView.cxx \
    GUI/Views/TestLevelView.cxx \
    GUI/Views/OptionsView.cxx \
    Core/Point.cxx \
    Core/Polygon.cxx \
    Core/Segment.cxx \
    Core/Vector.cxx \
    GUI/Controllers/GameController.cxx \
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
    GUI/Models/PolygonModel.cxx \
    GUI/Views/CreateLevelScribblingView.cxx \
    GUI/Views/AbstractScribblingView.cxx \
    GUI/Models/CreateLevelModel.cxx \
    GUI/Delegates/PolygonItemDelegate.cxx \
    GUI/Controllers/TestLevelController.cxx \
    GUI/Views/PlayingView.cxx \
    GUI/Controllers/PlayingController.cxx \
    GUI/Controllers/TestingController.cxx \
    GUI/Views/TestingView.cxx \
    GUI/Views/PlayingScribblingView.cxx \
    GUI/Commands/CreateLevelCommands.cxx \
    GUI/Commands/TestLevelCommands.cxx \
    GUI/Models/LineModel.cxx \
    GUI/Views/MapView.cxx \
    GUI/GraphicsItem/GraphicsMessageBoxItem.cxx \
    GUI/GraphicsItem/PlayerItem.cxx \
    GUI/Views/AbstractScribblingView2.cxx

HEADERS += \
    GUI/Views/AchievementsView.hxx \
    GUI/Views/CreateLevelView.hxx \
    GUI/Views/GameView.hxx \
    GUI/Views/LoadingView.hxx \
    GUI/Views/MainMenuView.hxx \
    GUI/Views/MainWindow.hxx \
    GUI/Views/PauseView.hxx \
    GUI/Views/TestLevelView.hxx \
    GUI/Views/OptionsView.hxx \
    Core/Point.hxx \
    Core/Polygon.hxx \
    Core/Segment.hxx \
    Core/Vector.hxx \
    GUI/Controllers/GameController.hxx \
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
    GUI/Models/PolygonModel.hxx \
    GUI/Views/CreateLevelScribblingView.hxx \
    GUI/Views/AbstractScribblingView.hxx \
    GUI/Models/CreateLevelModel.hxx \
    GUI/Delegates/PolygonItemDelegate.hxx \
    GUI/Controllers/TestLevelController.hxx \
    GUI/Views/PlayingView.hxx \
    GUI/Controllers/PlayingController.hxx \
    GUI/Controllers/TestingController.hxx \
    GUI/Views/TestingView.hxx \
    GUI/Views/PlayingScribblingView.hxx \
    GUI/Commands/CreateLevelCommands.hxx \
    GUI/Commands/TestLevelCommands.hxx \
    GUI/Models/LineModel.hxx \
    GUI/Views/MapView.hxx \
    GUI/GraphicsItem/GraphicsMessageBoxItem.hxx \
    GUI/GraphicsItem/PlayerItem.hxx \
    GUI/Views/AbstractScribblingView2.hxx

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
