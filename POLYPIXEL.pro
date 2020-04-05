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
  GUI/TestLevel/Views/TestLevelGraphicsView.cxx \
    main.cxx \
#CORE
# GEOMETRY
    Core/Geometry/Point.cxx \
    Core/Geometry/Polygon.cxx \
    Core/Geometry/Segment.cxx \
    Core/Geometry/Vector.cxx \
# OBJECTS
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
# SLICER
    Core/Slicer.cxx \
#GUI
    GUI/MainWindow.cxx \
# COMPONENTS
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
# PLAY
#  MODEL
#  VIEW
    GUI/PlayLevel/Views/PauseWidget.cxx \
    GUI/PlayLevel/Views/PlayLevelWidget.cxx \
#  CONTROLLER
    GUI/PlayLevel/Controllers/PlayLevelController.cxx \
# CREATE
#  MODEL
    GUI/CreateLevel/Models/CreateLevelObjectsDetailModel.cxx \
    GUI/CreateLevel/Models/CreateLevelObjectsListModel.cxx \
    GUI/CreateLevel/Models/CreateLevelVertexListModel.cxx \
    GUI/CreateLevel/Models/GraphicsObjectItem.cxx \
#  VIEW
    GUI/CreateLevel/Views/CreateLevelGraphicsView.cxx \
    GUI/CreateLevel/Views/CreateLevelWidget.cxx \
#  CONTROLLER
    GUI/CreateLevel/Controllers/CreateLevelController.cxx \
# TEST
#  MODEL
#  VIEW
    GUI/TestLevel/Views/TestLevelWidget.cxx \
#  CONTROLLER
    GUI/TestLevel/Controllers/TestLevelController.cxx \
# ACHIEVEMENTS
    GUI/Achievements/AchievementsWidget.cxx \
# MAIN MENU
    GUI/MainMenu/MainMenuWidget.cxx \
# CHOOSE LEVELS
#  MODEL
#  VIEW
    GUI/ChooseLevel/Views/ChooseLevelWidget.cxx \
#  CONTROLLER
    GUI/ChooseLevel/Controllers/ChooseLevelController.cxx \
# OPTIONS
    GUI/Options/OptionsWidget.cxx \
#PARSER
    Parser/Parser.cxx \
    Parser/Serializer.cxx

HEADERS += \
#CORE
# GEOMETRY
    Core/Geometry/Point.hxx \
    Core/Geometry/Polygon.hxx \
    Core/Geometry/Segment.hxx \
    Core/Geometry/Vector.hxx \
# OBJECTS
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
# SLICER
    Core/Slicer.hxx \
#GUI
    GUI/MainWindow.hxx \
# COMPONENTS
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
# PLAY
#  MODEL
#  VIEW
    GUI/PlayLevel/Views/PauseWidget.hxx \
    GUI/PlayLevel/Views/PlayLevelWidget.hxx \
#  CONTROLLER
    GUI/PlayLevel/Controllers/PlayLevelController.hxx \
# CREATE
#  MODEL
    GUI/CreateLevel/Models/CreateLevelObjectsDetailModel.hxx \
    GUI/CreateLevel/Models/CreateLevelObjectsListModel.hxx \
    GUI/CreateLevel/Models/CreateLevelVertexListModel.hxx \
    GUI/CreateLevel/Models/GraphicsObjectItem.hxx \
#  VIEW
    GUI/CreateLevel/Views/CreateLevelGraphicsView.hxx \
    GUI/CreateLevel/Views/CreateLevelWidget.hxx \
#  CONTROLLER
    GUI/CreateLevel/Controllers/CreateLevelController.hxx \
# TEST
#  MODEL
#  VIEW
  GUI/TestLevel/Views/TestLevelGraphicsView.hxx \
    GUI/TestLevel/Views/TestLevelWidget.hxx \
#  CONTROLLER
    GUI/TestLevel/Controllers/TestLevelController.hxx \
# ACHIEVEMENTS
    GUI/Achievements/AchievementsWidget.hxx \
# MAIN MENU
    GUI/MainMenu/MainMenuWidget.hxx \
# CHOOSE LEVELS
#  MODEL
#  VIEW
    GUI/ChooseLevel/Views/ChooseLevelWidget.hxx \
#  CONTROLLER
    GUI/ChooseLevel/Controllers/ChooseLevelController.hxx \
# OPTIONS
    GUI/Options/OptionsWidget.hxx \
#PARSER
    Parser/Parser.hxx \
    Parser/Serializer.hxx

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
