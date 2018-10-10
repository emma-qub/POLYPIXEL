#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T19:47:25
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = POLYPIXEL
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
    main.cxx \
    Core/Point.cxx \
    Core/Polygon.cxx \
    Core/Segment.cxx \
    Core/Vector.cxx \
    GUI/Commands/CreateLevelCommands.cxx \
    GUI/Commands/TestLevelCommands.cxx \
    GUI/Controllers/CreateLevelController.cxx \
    GUI/Controllers/GameController.cxx \
    GUI/Controllers/PlayingController.cxx \
    GUI/Controllers/TestingController.cxx \
    GUI/Controllers/TestLevelController.cxx \
    GUI/Delegates/PolygonItemDelegate.cxx \
    GUI/GraphicsItem/GameOverItem.cxx \
    GUI/GraphicsItem/GameStartItem.cxx \
    GUI/GraphicsItem/GraphicsGoalItem.cxx \
    GUI/GraphicsItem/GraphicsMessageBoxItem.cxx \
    GUI/GraphicsItem/GraphicsPixelLine.cxx \
    GUI/GraphicsItem/GraphicsPixelRectItem.cxx \
    GUI/GraphicsItem/GraphicsRoundedRectItem.cxx \
    GUI/GraphicsItem/GraphicsStarsItem.cxx \
    GUI/GraphicsItem/PlayerItem.cxx \
    GUI/Models/CreateLevelModel.cxx \
    GUI/Models/LineModel.cxx \
    GUI/Models/PolygonModel.cxx \
    GUI/Views/AbstractScribblingView.cxx \
    GUI/Views/AchievementsView.cxx \
    GUI/Views/CreateLevelScribblingView.cxx \
    GUI/Views/CreateLevelView.cxx \
    GUI/Views/GameView.cxx \
    GUI/Views/LoadingView.cxx \
    GUI/Views/MainMenuView.cxx \
    GUI/Views/MainWindow.cxx \
    GUI/Views/MapView.cxx \
    GUI/Views/OptionsView.cxx \
    GUI/Views/PauseView.cxx \
    GUI/Views/PlayingScribblingView.cxx \
    GUI/Views/PlayingView.cxx \
    GUI/Views/TestingView.cxx \
    GUI/Views/TestLevelView.cxx \
    Objects/Deviations/Deviation.cxx \
    Objects/Deviations/Mirror.cxx \
    Objects/Deviations/Portal.cxx \
    Objects/Obstacles/Tape.cxx \
    Objects/Obstacles/Obstacle.cxx \
    Objects/Obstacles/OneWay.cxx \
    Objects/Object.cxx \
    Parser/Parser.cxx \
    Parser/Serializer.cxx

HEADERS += \
    Core/Point.hxx \
    Core/Polygon.hxx \
    Core/Segment.hxx \
    Core/Vector.hxx \
    GUI/Commands/CreateLevelCommands.hxx \
    GUI/Commands/TestLevelCommands.hxx \
    GUI/Controllers/CreateLevelController.hxx \
    GUI/Controllers/GameController.hxx \
    GUI/Controllers/PlayingController.hxx \
    GUI/Controllers/TestingController.hxx \
    GUI/Controllers/TestLevelController.hxx \
    GUI/Delegates/PolygonItemDelegate.hxx \
    GUI/GraphicsItem/GameOverItem.hxx \
    GUI/GraphicsItem/GameStartItem.hxx \
    GUI/GraphicsItem/GraphicsGoalItem.hxx \
    GUI/GraphicsItem/GraphicsMessageBoxItem.hxx \
    GUI/GraphicsItem/GraphicsPixelLine.hxx \
    GUI/GraphicsItem/GraphicsPixelRectItem.hxx \
    GUI/GraphicsItem/GraphicsRoundedRectItem.hxx \
    GUI/GraphicsItem/GraphicsStarsItem.hxx \
    GUI/GraphicsItem/PlayerItem.hxx \
    GUI/Models/CreateLevelModel.hxx \
    GUI/Models/LineModel.hxx \
    GUI/Models/PolygonModel.hxx \
    GUI/Views/AbstractScribblingView.hxx \
    GUI/Views/AchievementsView.hxx \
    GUI/Views/CreateLevelScribblingView.hxx \
    GUI/Views/CreateLevelView.hxx \
    GUI/Views/GameView.hxx \
    GUI/Views/LoadingView.hxx \
    GUI/Views/MainMenuView.hxx \
    GUI/Views/MainWindow.hxx \
    GUI/Views/MapView.hxx \
    GUI/Views/OptionsView.hxx \
    GUI/Views/PauseView.hxx \
    GUI/Views/PlayingScribblingView.hxx \
    GUI/Views/PlayingView.hxx \
    GUI/Views/TestingView.hxx \
    GUI/Views/TestLevelView.hxx \
    Objects/Deviations/Deviation.hxx \
    Objects/Deviations/Mirror.hxx \
    Objects/Deviations/Portal.hxx \
    Objects/Obstacles/Tape.hxx \
    Objects/Obstacles/Obstacle.hxx \
    Objects/Obstacles/OneWay.hxx \
    Objects/Object.hxx \
    Parser/Parser.hxx \
    Parser/Serializer.hxx

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
