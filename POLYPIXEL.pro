#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T19:47:25
#
#-------------------------------------------------

QT       += core gui widgets xml

TARGET = POLYPIXEL
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
    GUI/Models/CreateLevelObjectsDetailModel.cxx \
    GUI/Models/CreateLevelVertexListModel.cxx \
    main.cxx \
    Core/Geometry/Point.cxx \
    Core/Geometry/Polygon.cxx \
    Core/Geometry/Segment.cxx \
    Core/Geometry/Vector.cxx \
    Core/Objects/Deviations/Deviation.cxx \
    Core/Objects/Deviations/Mirror.cxx \
    Core/Objects/Deviations/Portal.cxx \
    Core/Objects/Mutables/Countdown.cxx \
    Core/Objects/Mutables/Mutable.cxx \
    Core/Objects/Mutables/Disposable.cxx \
    Core/Objects/Mutables/Switch.cxx \
    Core/Objects/Mutables/Transfer.cxx \
    Core/Objects/Obstacles/Tape.cxx \
    Core/Objects/Obstacles/Obstacle.cxx \
    Core/Objects/Obstacles/OneWay.cxx \
    Core/Objects/Object.cxx \
    Core/Slicer.cxx \
    GUI/Commands/CreateLevelCommands.cxx \
    GUI/Commands/TestLevelCommands.cxx \
    GUI/Components/lib/qtmaterialoverlaywidget.cpp \
    GUI/Components/lib/qtmaterialrippleoverlay.cpp \
    GUI/Components/lib/qtmaterialripple.cpp \
    GUI/Components/lib/qtmaterialstatetransition.cpp \
    GUI/Components/lib/qtmaterialstyle.cpp \
    GUI/Components/lib/qtmaterialtheme.cpp \
    GUI/Components/qtmaterialbutton.cpp \
    GUI/Components/qtmaterialbutton_internal.cpp \
    GUI/Components/qtmaterialcontainedbutton.cpp \
    GUI/Components/qtmaterialoutlinedbutton.cpp \
    GUI/Components/qtmaterialtextbutton.cpp \
    GUI/Controllers/CreateLevelController.cxx \
    GUI/Controllers/GameController.cxx \
    GUI/Controllers/LevelsController.cxx \
    GUI/Controllers/PlayingController.cxx \
    GUI/Controllers/TestingController.cxx \
    GUI/Controllers/TestLevelController.cxx \
    GUI/Controllers/WorldsController.cxx \
    GUI/Delegates/PolygonItemDelegate.cxx \
    GUI/GraphicsItem/GameOverItem.cxx \
    GUI/GraphicsItem/GameStartItem.cxx \
    GUI/GraphicsItem/GraphicsGoalItem.cxx \
    GUI/GraphicsItem/GraphicsMessageBoxItem.cxx \
    GUI/GraphicsItem/GraphicsObjectItem.cxx \
    GUI/GraphicsItem/GraphicsPixelLine.cxx \
    GUI/GraphicsItem/GraphicsPixelRectItem.cxx \
    GUI/GraphicsItem/GraphicsRoundedRectItem.cxx \
    GUI/GraphicsItem/GraphicsStarsItem.cxx \
    GUI/GraphicsItem/PlayerItem.cxx \
    GUI/Models/CreateLevelObjectsListModel.cxx \
    GUI/Models/LevelsModel.cxx \
    GUI/Models/LineModel.cxx \
    GUI/Models/ObjectModel.cxx \
    GUI/Models/PolygonModel.cxx \
    GUI/Models/WorldsModel.cxx \
    GUI/Views/AbstractScribblingView.cxx \
    GUI/Views/AchievementsWidget.cxx \
    GUI/Views/CreateLevelScribblingView.cxx \
    GUI/Views/CreateLevelWidget.cxx \
    GUI/Views/GameWidget.cxx \
    GUI/Views/LevelsView.cxx \
    GUI/Views/LoadingWidget.cxx \
    GUI/Views/MainMenuWidget.cxx \
    GUI/Views/MainWindow.cxx \
    GUI/Views/MapView.cxx \
    GUI/Views/OptionsWidget.cxx \
    GUI/Views/PauseWidget.cxx \
    GUI/Views/PlayingScribblingView.cxx \
    GUI/Views/PlayingWidget.cxx \
    GUI/Views/TestingWidget.cxx \
    GUI/Views/TestLevelWidget.cxx \
    GUI/Views/WorldsView.cxx \
    Parser/Parser.cxx \
    Parser/Serializer.cxx

HEADERS += \
    Core/Geometry/Point.hxx \
    Core/Geometry/Polygon.hxx \
    Core/Geometry/Segment.hxx \
    Core/Geometry/Vector.hxx \
    Core/Objects/Deviations/Deviation.hxx \
    Core/Objects/Deviations/Mirror.hxx \
    Core/Objects/Deviations/Portal.hxx \
    Core/Objects/Mutables/Countdown.hxx \
    Core/Objects/Mutables/Mutable.hxx \
    Core/Objects/Mutables/Disposable.hxx \
    Core/Objects/Mutables/Switch.hxx \
    Core/Objects/Mutables/Transfer.hxx \
    Core/Objects/Obstacles/Tape.hxx \
    Core/Objects/Obstacles/Obstacle.hxx \
    Core/Objects/Obstacles/OneWay.hxx \
    Core/Objects/Object.hxx \
    Core/Slicer.hxx \
    GUI/Commands/CreateLevelCommands.hxx \
    GUI/Commands/TestLevelCommands.hxx \
    GUI/Components/lib/qtmaterialoverlaywidget.h \
    GUI/Components/lib/qtmaterialripple.h \
    GUI/Components/lib/qtmaterialrippleoverlay.h \
    GUI/Components/lib/qtmaterialstatetransition.h \
    GUI/Components/lib/qtmaterialstatetransitionevent.h \
    GUI/Components/lib/qtmaterialstyle.h \
    GUI/Components/lib/qtmaterialstyle_p.h \
    GUI/Components/lib/qtmaterialtheme.h \
    GUI/Components/lib/qtmaterialtheme_p.h \
    GUI/Components/qtmaterialbutton.h \
    GUI/Components/qtmaterialbutton_internal.h \
    GUI/Components/qtmaterialbutton_p.h \
    GUI/Components/qtmaterialcontainedbutton.h \
    GUI/Components/qtmaterialcontainedbutton_p.h \
    GUI/Components/qtmaterialoutlinedbutton.h \
    GUI/Components/qtmaterialoutlinedbutton_p.h \
    GUI/Components/qtmaterialtextbutton.h \
    GUI/Components/qtmaterialtextbutton_p.h \
    GUI/Controllers/CreateLevelController.hxx \
    GUI/Controllers/GameController.hxx \
    GUI/Controllers/LevelsController.hxx \
    GUI/Controllers/PlayingController.hxx \
    GUI/Controllers/TestingController.hxx \
    GUI/Controllers/TestLevelController.hxx \
    GUI/Controllers/WorldsController.hxx \
    GUI/Delegates/PolygonItemDelegate.hxx \
    GUI/GraphicsItem/GameOverItem.hxx \
    GUI/GraphicsItem/GameStartItem.hxx \
    GUI/GraphicsItem/GraphicsGoalItem.hxx \
    GUI/GraphicsItem/GraphicsMessageBoxItem.hxx \
    GUI/GraphicsItem/GraphicsObjectItem.hxx \
    GUI/GraphicsItem/GraphicsPixelLine.hxx \
    GUI/GraphicsItem/GraphicsPixelRectItem.hxx \
    GUI/GraphicsItem/GraphicsRoundedRectItem.hxx \
    GUI/GraphicsItem/GraphicsStarsItem.hxx \
    GUI/GraphicsItem/PlayerItem.hxx \
    GUI/Models/CreateLevelObjectsDetailModel.hxx \
    GUI/Models/CreateLevelObjectsListModel.hxx \
    GUI/Models/CreateLevelVertexListModel.hxx \
    GUI/Models/LevelsModel.hxx \
    GUI/Models/LineModel.hxx \
    GUI/Models/ObjectModel.hxx \
    GUI/Models/PolygonModel.hxx \
    GUI/Models/WorldsModel.hxx \
    GUI/Views/AbstractScribblingView.hxx \
    GUI/Views/AchievementsWidget.hxx \
    GUI/Views/CreateLevelScribblingView.hxx \
    GUI/Views/CreateLevelWidget.hxx \
    GUI/Views/GameWidget.hxx \
    GUI/Views/LevelsView.hxx \
    GUI/Views/LoadingWidget.hxx \
    GUI/Views/MainMenuWidget.hxx \
    GUI/Views/MainWindow.hxx \
    GUI/Views/MapView.hxx \
    GUI/Views/OptionsWidget.hxx \
    GUI/Views/PauseWidget.hxx \
    GUI/Views/PlayingScribblingView.hxx \
    GUI/Views/PlayingWidget.hxx \
    GUI/Views/TestingWidget.hxx \
    GUI/Views/TestLevelWidget.hxx \
    GUI/Views/WorldsView.hxx \
    Parser/Parser.hxx \
    Parser/Serializer.hxx

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
