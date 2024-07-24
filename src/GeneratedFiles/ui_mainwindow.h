/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "DockSelectElement.h"
#include "DockTextEdit.h"
#include "oglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave_Visualization;
    QAction *actionSave_Nary_Data;
    QAction *actionOpen_Nary_Data;
    QAction *actionSelectEntities;
    QAction *actionFontSettings;
    QAction *actionRemoveBinaryRelationshipsOfNary;
    QAction *actionRemoveUnvisibleElements;
    QAction *actionSelectEntity;
    QAction *actionSelectRelationship;
    QAction *actionaddEntity;
    QAction *actionaddRandomEntities;
    QAction *actionaddRelationship;
    QAction *actionMoveSelect;
    QAction *actionEditElement;
    QAction *actionMergeElements;
    QAction *actionDivideRelationship;
    QAction *actionDeleteSelectedElements;
    QAction *actionRemoveElementsFromRelationship;
    QAction *actionRemove_Dangling_Entities;
    QAction *actionRemove_Dangling_Relationships;
    QAction *actionAnimate_Optimization;
    QAction *actionAuto_Adjust_Viewpoint;
    QAction *actionAssign_Relationships_Color;
    QAction *actionAssign_Entities_Color;
    QAction *actionClear_all_data;
    QAction *actionDraw_Edges_as_Cylinders;
    QAction *actionCreate_Dual_VisWidget;
    QAction *actionLoad_Nary_Locations;
    QAction *actionSave_Console;
    QAction *actionClear_Console;
    QAction *actionCreate_Relationships_From_Cliques;
    QAction *actionConvert_to_Linear;
    QAction *actionRemove_PolygonsOfCard;
    QAction *actionBringReToFront;
    QAction *actionBringReToBack;
    QAction *actionSet_Polygon_Boundary_Width;
    QAction *actionSet_Nodes_Boundary_Width;
    QAction *actionDraw_Nodes_as_Spheres;
    QAction *actionLights_Settings;
    QAction *actionCreate_Cliques_from_Relationships;
    QAction *actionCreate_Relationships_From_KCliques;
    QAction *actionCreate_Relationships_From_KClans;
    QAction *actionConver_to_Simple;
    QAction *actionOnly_Do_Individual_Optimization;
    QAction *actionOptimzie_With_Fixed_Verts;
    QAction *actionCreateRelationship;
    QAction *actionSet_Selection_Color;
    QAction *actionConvert_to_Bipartite;
    QAction *actionPolygon_View;
    QAction *actionKonig_View;
    QAction *actionDraw_Curved_Scaffolds;
    QAction *actionSave_Decomposition;
    QAction *actionLoad_Decomposition;
    QAction *actionDraw_Axes;
    QAction *actionDraw_Grid;
    QAction *actionDraw_Cursor;
    QAction *actionOpen_CSV_Data;
    QAction *actionSave_CSV_Data;
    QAction *actionLoad_Hyperedge_Colors;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOption;
    QMenu *menuData_Process;
    QMenu *menuWindows;
    QMenu *menuDebug;
    QMenu *menuView;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QDockWidget *dockWidget_Information;
    DockSelectElement *dockWidgetContents_Information;
    QDockWidget *dockWidget_Console;
    DockTextEdit *dockWidgetContents_Console;
    QDockWidget *dockWidget_DualVis;
    OGLWidget *dualWdgt;
    QDockWidget *dockWidget_Control;
    QWidget *dockWidgetContents_Edit;
    QDockWidget *dockWidget_PrimeVis;
    OGLWidget *visWdgt;
    QDockWidget *dockWidget_BipartVis;
    OGLWidget *bipartWdgt;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->setWindowModality(Qt::ApplicationModal);
        MainWindow->resize(932, 1035);
        MainWindow->setMinimumSize(QSize(0, 0));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/picon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setAnimated(true);
        actionSave_Visualization = new QAction(MainWindow);
        actionSave_Visualization->setObjectName(QString::fromUtf8("actionSave_Visualization"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/noun_save_image_98043.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Visualization->setIcon(icon1);
        actionSave_Nary_Data = new QAction(MainWindow);
        actionSave_Nary_Data->setObjectName(QString::fromUtf8("actionSave_Nary_Data"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/noun_603596_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Nary_Data->setIcon(icon2);
        actionOpen_Nary_Data = new QAction(MainWindow);
        actionOpen_Nary_Data->setObjectName(QString::fromUtf8("actionOpen_Nary_Data"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/noun_1509580_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Nary_Data->setIcon(icon3);
        actionSelectEntities = new QAction(MainWindow);
        actionSelectEntities->setObjectName(QString::fromUtf8("actionSelectEntities"));
        actionSelectEntities->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/noun_177528_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectEntities->setIcon(icon4);
        actionFontSettings = new QAction(MainWindow);
        actionFontSettings->setObjectName(QString::fromUtf8("actionFontSettings"));
        actionRemoveBinaryRelationshipsOfNary = new QAction(MainWindow);
        actionRemoveBinaryRelationshipsOfNary->setObjectName(QString::fromUtf8("actionRemoveBinaryRelationshipsOfNary"));
        actionRemoveBinaryRelationshipsOfNary->setIconVisibleInMenu(true);
        actionRemoveUnvisibleElements = new QAction(MainWindow);
        actionRemoveUnvisibleElements->setObjectName(QString::fromUtf8("actionRemoveUnvisibleElements"));
        actionSelectEntity = new QAction(MainWindow);
        actionSelectEntity->setObjectName(QString::fromUtf8("actionSelectEntity"));
        actionSelectEntity->setCheckable(true);
        actionSelectEntity->setChecked(false);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icons/design.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectEntity->setIcon(icon5);
        actionSelectRelationship = new QAction(MainWindow);
        actionSelectRelationship->setObjectName(QString::fromUtf8("actionSelectRelationship"));
        actionSelectRelationship->setCheckable(true);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/icons/noun_Pentagon_554225.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectRelationship->setIcon(icon6);
        actionaddEntity = new QAction(MainWindow);
        actionaddEntity->setObjectName(QString::fromUtf8("actionaddEntity"));
        actionaddEntity->setCheckable(true);
        actionaddEntity->setChecked(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/icons/noun_612655_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionaddEntity->setIcon(icon7);
        actionaddRandomEntities = new QAction(MainWindow);
        actionaddRandomEntities->setObjectName(QString::fromUtf8("actionaddRandomEntities"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/icons/noun_1291736_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionaddRandomEntities->setIcon(icon8);
        actionaddRelationship = new QAction(MainWindow);
        actionaddRelationship->setObjectName(QString::fromUtf8("actionaddRelationship"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/icons/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionaddRelationship->setIcon(icon9);
        actionMoveSelect = new QAction(MainWindow);
        actionMoveSelect->setObjectName(QString::fromUtf8("actionMoveSelect"));
        actionMoveSelect->setCheckable(true);
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/icons/noun_45503_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMoveSelect->setIcon(icon10);
        actionEditElement = new QAction(MainWindow);
        actionEditElement->setObjectName(QString::fromUtf8("actionEditElement"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/icons/noun_189670_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionEditElement->setIcon(icon11);
        actionMergeElements = new QAction(MainWindow);
        actionMergeElements->setObjectName(QString::fromUtf8("actionMergeElements"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/icons/merge2-512.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMergeElements->setIcon(icon12);
        actionDivideRelationship = new QAction(MainWindow);
        actionDivideRelationship->setObjectName(QString::fromUtf8("actionDivideRelationship"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/icons/noun_979068_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDivideRelationship->setIcon(icon13);
        actionDeleteSelectedElements = new QAction(MainWindow);
        actionDeleteSelectedElements->setObjectName(QString::fromUtf8("actionDeleteSelectedElements"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/icons/icons/noun_1045985_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDeleteSelectedElements->setIcon(icon14);
        actionRemoveElementsFromRelationship = new QAction(MainWindow);
        actionRemoveElementsFromRelationship->setObjectName(QString::fromUtf8("actionRemoveElementsFromRelationship"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/icons/icons/noun_1112746_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRemoveElementsFromRelationship->setIcon(icon15);
        actionRemove_Dangling_Entities = new QAction(MainWindow);
        actionRemove_Dangling_Entities->setObjectName(QString::fromUtf8("actionRemove_Dangling_Entities"));
        actionRemove_Dangling_Relationships = new QAction(MainWindow);
        actionRemove_Dangling_Relationships->setObjectName(QString::fromUtf8("actionRemove_Dangling_Relationships"));
        actionAnimate_Optimization = new QAction(MainWindow);
        actionAnimate_Optimization->setObjectName(QString::fromUtf8("actionAnimate_Optimization"));
        actionAnimate_Optimization->setCheckable(true);
        actionAnimate_Optimization->setChecked(true);
        actionAuto_Adjust_Viewpoint = new QAction(MainWindow);
        actionAuto_Adjust_Viewpoint->setObjectName(QString::fromUtf8("actionAuto_Adjust_Viewpoint"));
        actionAuto_Adjust_Viewpoint->setCheckable(true);
        actionAssign_Relationships_Color = new QAction(MainWindow);
        actionAssign_Relationships_Color->setObjectName(QString::fromUtf8("actionAssign_Relationships_Color"));
        actionAssign_Entities_Color = new QAction(MainWindow);
        actionAssign_Entities_Color->setObjectName(QString::fromUtf8("actionAssign_Entities_Color"));
        actionClear_all_data = new QAction(MainWindow);
        actionClear_all_data->setObjectName(QString::fromUtf8("actionClear_all_data"));
        actionDraw_Edges_as_Cylinders = new QAction(MainWindow);
        actionDraw_Edges_as_Cylinders->setObjectName(QString::fromUtf8("actionDraw_Edges_as_Cylinders"));
        actionDraw_Edges_as_Cylinders->setCheckable(true);
        actionDraw_Edges_as_Cylinders->setChecked(false);
        actionCreate_Dual_VisWidget = new QAction(MainWindow);
        actionCreate_Dual_VisWidget->setObjectName(QString::fromUtf8("actionCreate_Dual_VisWidget"));
        actionCreate_Dual_VisWidget->setCheckable(false);
        actionCreate_Dual_VisWidget->setChecked(false);
        actionLoad_Nary_Locations = new QAction(MainWindow);
        actionLoad_Nary_Locations->setObjectName(QString::fromUtf8("actionLoad_Nary_Locations"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/icons/icons/noun_778853_cc.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionLoad_Nary_Locations->setIcon(icon16);
        actionSave_Console = new QAction(MainWindow);
        actionSave_Console->setObjectName(QString::fromUtf8("actionSave_Console"));
        actionClear_Console = new QAction(MainWindow);
        actionClear_Console->setObjectName(QString::fromUtf8("actionClear_Console"));
        actionCreate_Relationships_From_Cliques = new QAction(MainWindow);
        actionCreate_Relationships_From_Cliques->setObjectName(QString::fromUtf8("actionCreate_Relationships_From_Cliques"));
        actionConvert_to_Linear = new QAction(MainWindow);
        actionConvert_to_Linear->setObjectName(QString::fromUtf8("actionConvert_to_Linear"));
        actionRemove_PolygonsOfCard = new QAction(MainWindow);
        actionRemove_PolygonsOfCard->setObjectName(QString::fromUtf8("actionRemove_PolygonsOfCard"));
        actionBringReToFront = new QAction(MainWindow);
        actionBringReToFront->setObjectName(QString::fromUtf8("actionBringReToFront"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/icons/icons/noun_Bring to Front_1606147.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBringReToFront->setIcon(icon17);
        actionBringReToBack = new QAction(MainWindow);
        actionBringReToBack->setObjectName(QString::fromUtf8("actionBringReToBack"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/icons/icons/noun_Bring to back.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBringReToBack->setIcon(icon18);
        actionSet_Polygon_Boundary_Width = new QAction(MainWindow);
        actionSet_Polygon_Boundary_Width->setObjectName(QString::fromUtf8("actionSet_Polygon_Boundary_Width"));
        actionSet_Nodes_Boundary_Width = new QAction(MainWindow);
        actionSet_Nodes_Boundary_Width->setObjectName(QString::fromUtf8("actionSet_Nodes_Boundary_Width"));
        actionDraw_Nodes_as_Spheres = new QAction(MainWindow);
        actionDraw_Nodes_as_Spheres->setObjectName(QString::fromUtf8("actionDraw_Nodes_as_Spheres"));
        actionDraw_Nodes_as_Spheres->setCheckable(true);
        actionLights_Settings = new QAction(MainWindow);
        actionLights_Settings->setObjectName(QString::fromUtf8("actionLights_Settings"));
        actionCreate_Cliques_from_Relationships = new QAction(MainWindow);
        actionCreate_Cliques_from_Relationships->setObjectName(QString::fromUtf8("actionCreate_Cliques_from_Relationships"));
        actionCreate_Relationships_From_KCliques = new QAction(MainWindow);
        actionCreate_Relationships_From_KCliques->setObjectName(QString::fromUtf8("actionCreate_Relationships_From_KCliques"));
        actionCreate_Relationships_From_KClans = new QAction(MainWindow);
        actionCreate_Relationships_From_KClans->setObjectName(QString::fromUtf8("actionCreate_Relationships_From_KClans"));
        actionConver_to_Simple = new QAction(MainWindow);
        actionConver_to_Simple->setObjectName(QString::fromUtf8("actionConver_to_Simple"));
        actionOnly_Do_Individual_Optimization = new QAction(MainWindow);
        actionOnly_Do_Individual_Optimization->setObjectName(QString::fromUtf8("actionOnly_Do_Individual_Optimization"));
        actionOnly_Do_Individual_Optimization->setCheckable(true);
        actionOptimzie_With_Fixed_Verts = new QAction(MainWindow);
        actionOptimzie_With_Fixed_Verts->setObjectName(QString::fromUtf8("actionOptimzie_With_Fixed_Verts"));
        actionOptimzie_With_Fixed_Verts->setCheckable(true);
        actionCreateRelationship = new QAction(MainWindow);
        actionCreateRelationship->setObjectName(QString::fromUtf8("actionCreateRelationship"));
        actionCreateRelationship->setCheckable(false);
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/icons/icons/noun_189082.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCreateRelationship->setIcon(icon19);
        actionSet_Selection_Color = new QAction(MainWindow);
        actionSet_Selection_Color->setObjectName(QString::fromUtf8("actionSet_Selection_Color"));
        actionConvert_to_Bipartite = new QAction(MainWindow);
        actionConvert_to_Bipartite->setObjectName(QString::fromUtf8("actionConvert_to_Bipartite"));
        actionPolygon_View = new QAction(MainWindow);
        actionPolygon_View->setObjectName(QString::fromUtf8("actionPolygon_View"));
        actionPolygon_View->setCheckable(true);
        actionPolygon_View->setChecked(true);
        actionKonig_View = new QAction(MainWindow);
        actionKonig_View->setObjectName(QString::fromUtf8("actionKonig_View"));
        actionKonig_View->setCheckable(true);
        actionDraw_Curved_Scaffolds = new QAction(MainWindow);
        actionDraw_Curved_Scaffolds->setObjectName(QString::fromUtf8("actionDraw_Curved_Scaffolds"));
        actionDraw_Curved_Scaffolds->setCheckable(true);
        actionSave_Decomposition = new QAction(MainWindow);
        actionSave_Decomposition->setObjectName(QString::fromUtf8("actionSave_Decomposition"));
        actionLoad_Decomposition = new QAction(MainWindow);
        actionLoad_Decomposition->setObjectName(QString::fromUtf8("actionLoad_Decomposition"));
        actionDraw_Axes = new QAction(MainWindow);
        actionDraw_Axes->setObjectName(QString::fromUtf8("actionDraw_Axes"));
        actionDraw_Axes->setCheckable(true);
        actionDraw_Grid = new QAction(MainWindow);
        actionDraw_Grid->setObjectName(QString::fromUtf8("actionDraw_Grid"));
        actionDraw_Grid->setCheckable(true);
        actionDraw_Cursor = new QAction(MainWindow);
        actionDraw_Cursor->setObjectName(QString::fromUtf8("actionDraw_Cursor"));
        actionDraw_Cursor->setCheckable(true);
        actionOpen_CSV_Data = new QAction(MainWindow);
        actionOpen_CSV_Data->setObjectName(QString::fromUtf8("actionOpen_CSV_Data"));
        actionSave_CSV_Data = new QAction(MainWindow);
        actionSave_CSV_Data->setObjectName(QString::fromUtf8("actionSave_CSV_Data"));
        actionLoad_Hyperedge_Colors = new QAction(MainWindow);
        actionLoad_Hyperedge_Colors->setObjectName(QString::fromUtf8("actionLoad_Hyperedge_Colors"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        centralWidget->setMinimumSize(QSize(0, 0));
        centralWidget->setCursor(QCursor(Qt::ArrowCursor));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 932, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuFile->setToolTipsVisible(true);
        menuOption = new QMenu(menuBar);
        menuOption->setObjectName(QString::fromUtf8("menuOption"));
        menuData_Process = new QMenu(menuBar);
        menuData_Process->setObjectName(QString::fromUtf8("menuData_Process"));
        menuWindows = new QMenu(menuBar);
        menuWindows->setObjectName(QString::fromUtf8("menuWindows"));
        menuDebug = new QMenu(menuBar);
        menuDebug->setObjectName(QString::fromUtf8("menuDebug"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setEnabled(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidget_Information = new QDockWidget(MainWindow);
        dockWidget_Information->setObjectName(QString::fromUtf8("dockWidget_Information"));
        dockWidget_Information->setMinimumSize(QSize(100, 40));
        dockWidget_Information->setMaximumSize(QSize(524287, 524287));
        dockWidgetContents_Information = new DockSelectElement();
        dockWidgetContents_Information->setObjectName(QString::fromUtf8("dockWidgetContents_Information"));
        dockWidget_Information->setWidget(dockWidgetContents_Information);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_Information);
        dockWidget_Console = new QDockWidget(MainWindow);
        dockWidget_Console->setObjectName(QString::fromUtf8("dockWidget_Console"));
        dockWidget_Console->setMinimumSize(QSize(100, 80));
        dockWidget_Console->setMaximumSize(QSize(524287, 524287));
        dockWidgetContents_Console = new DockTextEdit();
        dockWidgetContents_Console->setObjectName(QString::fromUtf8("dockWidgetContents_Console"));
        dockWidget_Console->setWidget(dockWidgetContents_Console);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget_Console);
        dockWidget_DualVis = new QDockWidget(MainWindow);
        dockWidget_DualVis->setObjectName(QString::fromUtf8("dockWidget_DualVis"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget_DualVis->sizePolicy().hasHeightForWidth());
        dockWidget_DualVis->setSizePolicy(sizePolicy);
        dockWidget_DualVis->setMinimumSize(QSize(256, 275));
        dockWidget_DualVis->setAcceptDrops(false);
        dualWdgt = new OGLWidget();
        dualWdgt->setObjectName(QString::fromUtf8("dualWdgt"));
        sizePolicy.setHeightForWidth(dualWdgt->sizePolicy().hasHeightForWidth());
        dualWdgt->setSizePolicy(sizePolicy);
        dualWdgt->setMinimumSize(QSize(256, 256));
        dualWdgt->setFocusPolicy(Qt::ClickFocus);
        dualWdgt->setAcceptDrops(true);
        dockWidget_DualVis->setWidget(dualWdgt);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_DualVis);
        dockWidget_Control = new QDockWidget(MainWindow);
        dockWidget_Control->setObjectName(QString::fromUtf8("dockWidget_Control"));
        dockWidget_Control->setMinimumSize(QSize(256, 40));
        dockWidget_Control->setMaximumSize(QSize(524287, 524287));
        dockWidgetContents_Edit = new QWidget();
        dockWidgetContents_Edit->setObjectName(QString::fromUtf8("dockWidgetContents_Edit"));
        dockWidget_Control->setWidget(dockWidgetContents_Edit);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_Control);
        dockWidget_PrimeVis = new QDockWidget(MainWindow);
        dockWidget_PrimeVis->setObjectName(QString::fromUtf8("dockWidget_PrimeVis"));
        sizePolicy.setHeightForWidth(dockWidget_PrimeVis->sizePolicy().hasHeightForWidth());
        dockWidget_PrimeVis->setSizePolicy(sizePolicy);
        dockWidget_PrimeVis->setMinimumSize(QSize(256, 280));
        dockWidget_PrimeVis->setAcceptDrops(false);
        visWdgt = new OGLWidget();
        visWdgt->setObjectName(QString::fromUtf8("visWdgt"));
        sizePolicy.setHeightForWidth(visWdgt->sizePolicy().hasHeightForWidth());
        visWdgt->setSizePolicy(sizePolicy);
        visWdgt->setMinimumSize(QSize(256, 256));
        visWdgt->setFocusPolicy(Qt::ClickFocus);
        visWdgt->setAcceptDrops(true);
        dockWidget_PrimeVis->setWidget(visWdgt);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_PrimeVis);
        dockWidget_BipartVis = new QDockWidget(MainWindow);
        dockWidget_BipartVis->setObjectName(QString::fromUtf8("dockWidget_BipartVis"));
        dockWidget_BipartVis->setMinimumSize(QSize(256, 275));
        bipartWdgt = new OGLWidget();
        bipartWdgt->setObjectName(QString::fromUtf8("bipartWdgt"));
        sizePolicy.setHeightForWidth(bipartWdgt->sizePolicy().hasHeightForWidth());
        bipartWdgt->setSizePolicy(sizePolicy);
        bipartWdgt->setMinimumSize(QSize(256, 256));
        bipartWdgt->setFocusPolicy(Qt::ClickFocus);
        bipartWdgt->setAcceptDrops(true);
        dockWidget_BipartVis->setWidget(bipartWdgt);
        MainWindow->addDockWidget(Qt::RightDockWidgetArea, dockWidget_BipartVis);
        dockWidget_PrimeVis->raise();

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuData_Process->menuAction());
        menuBar->addAction(menuOption->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuWindows->menuAction());
        menuBar->addAction(menuDebug->menuAction());
        menuFile->addAction(actionOpen_Nary_Data);
        menuFile->addAction(actionSave_Nary_Data);
        menuFile->addAction(actionLoad_Nary_Locations);
        menuFile->addAction(actionLoad_Hyperedge_Colors);
        menuFile->addSeparator();
        menuFile->addAction(actionOpen_CSV_Data);
        menuFile->addAction(actionSave_CSV_Data);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Visualization);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Console);
        menuFile->addAction(actionClear_Console);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_Decomposition);
        menuFile->addAction(actionLoad_Decomposition);
        menuOption->addAction(actionFontSettings);
        menuOption->addAction(actionLights_Settings);
        menuOption->addAction(actionSet_Polygon_Boundary_Width);
        menuOption->addAction(actionSet_Nodes_Boundary_Width);
        menuOption->addAction(actionAssign_Relationships_Color);
        menuOption->addAction(actionAssign_Entities_Color);
        menuOption->addAction(actionSet_Selection_Color);
        menuOption->addSeparator();
        menuOption->addAction(actionAnimate_Optimization);
        menuOption->addAction(actionOnly_Do_Individual_Optimization);
        menuOption->addAction(actionOptimzie_With_Fixed_Verts);
        menuOption->addAction(actionAuto_Adjust_Viewpoint);
        menuOption->addSeparator();
        menuOption->addAction(actionDraw_Curved_Scaffolds);
        menuOption->addAction(actionDraw_Nodes_as_Spheres);
        menuOption->addAction(actionDraw_Edges_as_Cylinders);
        menuOption->addAction(actionDraw_Axes);
        menuOption->addAction(actionDraw_Grid);
        menuOption->addAction(actionDraw_Cursor);
        menuData_Process->addAction(actionCreate_Dual_VisWidget);
        menuData_Process->addSeparator();
        menuData_Process->addAction(actionConvert_to_Bipartite);
        menuData_Process->addAction(actionCreate_Cliques_from_Relationships);
        menuData_Process->addAction(actionConver_to_Simple);
        menuData_Process->addAction(actionConvert_to_Linear);
        menuData_Process->addSeparator();
        menuData_Process->addAction(actionRemoveBinaryRelationshipsOfNary);
        menuData_Process->addAction(actionRemoveUnvisibleElements);
        menuData_Process->addAction(actionRemove_PolygonsOfCard);
        menuData_Process->addAction(actionRemove_Dangling_Entities);
        menuData_Process->addAction(actionRemove_Dangling_Relationships);
        menuData_Process->addSeparator();
        menuData_Process->addAction(actionCreate_Relationships_From_Cliques);
        menuData_Process->addAction(actionCreate_Relationships_From_KCliques);
        menuData_Process->addAction(actionCreate_Relationships_From_KClans);
        menuData_Process->addSeparator();
        menuData_Process->addAction(actionClear_all_data);
        menuWindows->addSeparator();
        menuView->addAction(actionPolygon_View);
        menuView->addAction(actionKonig_View);
        toolBar->addAction(actionSelectEntity);
        toolBar->addAction(actionSelectRelationship);
        toolBar->addAction(actionSelectEntities);
        toolBar->addAction(actionMoveSelect);
        toolBar->addSeparator();
        toolBar->addAction(actionaddEntity);
        toolBar->addAction(actionaddRandomEntities);
        toolBar->addAction(actionaddRelationship);
        toolBar->addAction(actionCreateRelationship);
        toolBar->addSeparator();
        toolBar->addAction(actionMergeElements);
        toolBar->addAction(actionRemoveElementsFromRelationship);
        toolBar->addAction(actionDivideRelationship);
        toolBar->addAction(actionDeleteSelectedElements);
        toolBar->addSeparator();
        toolBar->addAction(actionBringReToFront);
        toolBar->addAction(actionBringReToBack);
        toolBar->addAction(actionEditElement);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Nary Visualization", nullptr));
        actionSave_Visualization->setText(QCoreApplication::translate("MainWindow", "Save Visualization", nullptr));
#if QT_CONFIG(tooltip)
        actionSave_Visualization->setToolTip(QCoreApplication::translate("MainWindow", "Save Primary Visualization", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSave_Visualization->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_Nary_Data->setText(QCoreApplication::translate("MainWindow", "Save Nary Data", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_Nary_Data->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen_Nary_Data->setText(QCoreApplication::translate("MainWindow", "Open Nary Data", nullptr));
#if QT_CONFIG(tooltip)
        actionOpen_Nary_Data->setToolTip(QCoreApplication::translate("MainWindow", "Open Nary Data", nullptr));
#endif // QT_CONFIG(tooltip)
        actionSelectEntities->setText(QCoreApplication::translate("MainWindow", "SelectEntities", nullptr));
#if QT_CONFIG(tooltip)
        actionSelectEntities->setToolTip(QCoreApplication::translate("MainWindow", "Select entities by drawing a polygon.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSelectEntities->setShortcut(QCoreApplication::translate("MainWindow", "Shift+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFontSettings->setText(QCoreApplication::translate("MainWindow", "Fonts Settings...", nullptr));
        actionRemoveBinaryRelationshipsOfNary->setText(QCoreApplication::translate("MainWindow", "Remove Binary Re. of  Nary Re.", nullptr));
#if QT_CONFIG(tooltip)
        actionRemoveBinaryRelationshipsOfNary->setToolTip(QCoreApplication::translate("MainWindow", "Remove the edges inside each polygon", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRemoveUnvisibleElements->setText(QCoreApplication::translate("MainWindow", "Remove Unvisible En. and Re.", nullptr));
        actionSelectEntity->setText(QCoreApplication::translate("MainWindow", "Select Entity", nullptr));
#if QT_CONFIG(tooltip)
        actionSelectEntity->setToolTip(QCoreApplication::translate("MainWindow", "Select entities, double click to remove selections.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSelectEntity->setShortcut(QCoreApplication::translate("MainWindow", "Shift+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelectRelationship->setText(QCoreApplication::translate("MainWindow", "Select Relationship", nullptr));
#if QT_CONFIG(tooltip)
        actionSelectRelationship->setToolTip(QCoreApplication::translate("MainWindow", "Select relationships, double click to remove selections.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionSelectRelationship->setShortcut(QCoreApplication::translate("MainWindow", "Shift+R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionaddEntity->setText(QCoreApplication::translate("MainWindow", "addEntity", nullptr));
#if QT_CONFIG(tooltip)
        actionaddEntity->setToolTip(QCoreApplication::translate("MainWindow", "Add one enitity by left click.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionaddEntity->setShortcut(QCoreApplication::translate("MainWindow", "E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionaddRandomEntities->setText(QCoreApplication::translate("MainWindow", "addRandomEntities", nullptr));
#if QT_CONFIG(tooltip)
        actionaddRandomEntities->setToolTip(QCoreApplication::translate("MainWindow", "Add a number of entities which are distributed randomly.", nullptr));
#endif // QT_CONFIG(tooltip)
        actionaddRelationship->setText(QCoreApplication::translate("MainWindow", "addRelationship", nullptr));
#if QT_CONFIG(tooltip)
        actionaddRelationship->setToolTip(QCoreApplication::translate("MainWindow", "Add relationships by identifying the cardinality.", nullptr));
#endif // QT_CONFIG(tooltip)
        actionMoveSelect->setText(QCoreApplication::translate("MainWindow", "MoveSelect", nullptr));
#if QT_CONFIG(tooltip)
        actionMoveSelect->setToolTip(QCoreApplication::translate("MainWindow", "Move selected elements.", nullptr));
#endif // QT_CONFIG(tooltip)
        actionEditElement->setText(QCoreApplication::translate("MainWindow", "renameElement", nullptr));
#if QT_CONFIG(tooltip)
        actionEditElement->setToolTip(QCoreApplication::translate("MainWindow", "Rename selected elements.", nullptr));
#endif // QT_CONFIG(tooltip)
        actionMergeElements->setText(QCoreApplication::translate("MainWindow", "Merge Elements", nullptr));
#if QT_CONFIG(tooltip)
        actionMergeElements->setToolTip(QCoreApplication::translate("MainWindow", "Merge selected entities or relationships.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionMergeElements->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+M", nullptr));
#endif // QT_CONFIG(shortcut)
        actionDivideRelationship->setText(QCoreApplication::translate("MainWindow", "Divide Relationship", nullptr));
#if QT_CONFIG(tooltip)
        actionDivideRelationship->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Exclude a relationship from the selected relationship.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        actionDeleteSelectedElements->setText(QCoreApplication::translate("MainWindow", "Delete Selected Elements", nullptr));
#if QT_CONFIG(tooltip)
        actionDeleteSelectedElements->setToolTip(QCoreApplication::translate("MainWindow", "Delete selected entities and relationships.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionDeleteSelectedElements->setShortcut(QCoreApplication::translate("MainWindow", "X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionRemoveElementsFromRelationship->setText(QCoreApplication::translate("MainWindow", "Remove Entities From Relationship", nullptr));
#if QT_CONFIG(tooltip)
        actionRemoveElementsFromRelationship->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Exclude entities from the selected relationship.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        actionRemove_Dangling_Entities->setText(QCoreApplication::translate("MainWindow", "Remove Dangling En.", nullptr));
        actionRemove_Dangling_Relationships->setText(QCoreApplication::translate("MainWindow", "Remove Dangling Re.", nullptr));
        actionAnimate_Optimization->setText(QCoreApplication::translate("MainWindow", "Animate Optimization Procedure", nullptr));
        actionAuto_Adjust_Viewpoint->setText(QCoreApplication::translate("MainWindow", "Auto Adjust Viewpoint", nullptr));
        actionAssign_Relationships_Color->setText(QCoreApplication::translate("MainWindow", "Assign Relationships Color...", nullptr));
        actionAssign_Entities_Color->setText(QCoreApplication::translate("MainWindow", "Assign Entities Color...", nullptr));
        actionClear_all_data->setText(QCoreApplication::translate("MainWindow", "Clear all data", nullptr));
        actionDraw_Edges_as_Cylinders->setText(QCoreApplication::translate("MainWindow", "Draw Edge as Cylinder", nullptr));
        actionCreate_Dual_VisWidget->setText(QCoreApplication::translate("MainWindow", "Create Dual Vis", nullptr));
        actionLoad_Nary_Locations->setText(QCoreApplication::translate("MainWindow", "Load Vertex Locations", nullptr));
        actionSave_Console->setText(QCoreApplication::translate("MainWindow", "Save Console", nullptr));
        actionClear_Console->setText(QCoreApplication::translate("MainWindow", "Clear Console", nullptr));
        actionCreate_Relationships_From_Cliques->setText(QCoreApplication::translate("MainWindow", "Create Re. From Cliques", nullptr));
        actionConvert_to_Linear->setText(QCoreApplication::translate("MainWindow", "Convert to Linear", nullptr));
        actionRemove_PolygonsOfCard->setText(QCoreApplication::translate("MainWindow", "Remove Re. By Card", nullptr));
        actionBringReToFront->setText(QCoreApplication::translate("MainWindow", "BringReToFront", nullptr));
#if QT_CONFIG(tooltip)
        actionBringReToFront->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Change the order of the selected relationship so that it would be rendered later.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionBringReToFront->setShortcut(QCoreApplication::translate("MainWindow", "Up", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBringReToBack->setText(QCoreApplication::translate("MainWindow", "BringReToBack", nullptr));
#if QT_CONFIG(tooltip)
        actionBringReToBack->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Change the order of the selected relationship so that it would be rendered first.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionBringReToBack->setShortcut(QCoreApplication::translate("MainWindow", "Down", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSet_Polygon_Boundary_Width->setText(QCoreApplication::translate("MainWindow", "Set Polygon Boundary Width...", nullptr));
        actionSet_Nodes_Boundary_Width->setText(QCoreApplication::translate("MainWindow", "Set Nodes Boundary Width...", nullptr));
        actionDraw_Nodes_as_Spheres->setText(QCoreApplication::translate("MainWindow", "Draw Nodes as Shpere", nullptr));
        actionLights_Settings->setText(QCoreApplication::translate("MainWindow", "Lights Settings...", nullptr));
        actionCreate_Cliques_from_Relationships->setText(QCoreApplication::translate("MainWindow", "Convert to Cliques", nullptr));
#if QT_CONFIG(tooltip)
        actionCreate_Cliques_from_Relationships->setToolTip(QCoreApplication::translate("MainWindow", "Create graph from N-ary data by converting relationships into cliques.", nullptr));
#endif // QT_CONFIG(tooltip)
        actionCreate_Relationships_From_KCliques->setText(QCoreApplication::translate("MainWindow", "Create Re. From K-cliques...", nullptr));
        actionCreate_Relationships_From_KClans->setText(QCoreApplication::translate("MainWindow", "Create Re. From K-clans...", nullptr));
        actionConver_to_Simple->setText(QCoreApplication::translate("MainWindow", "Convert to Simple", nullptr));
        actionOnly_Do_Individual_Optimization->setText(QCoreApplication::translate("MainWindow", "Only Do Individual Optimization", nullptr));
        actionOptimzie_With_Fixed_Verts->setText(QCoreApplication::translate("MainWindow", "Optimzie With Fixed Verts", nullptr));
        actionCreateRelationship->setText(QCoreApplication::translate("MainWindow", "Create Relationship", nullptr));
#if QT_CONFIG(tooltip)
        actionCreateRelationship->setToolTip(QCoreApplication::translate("MainWindow", "Create new relationship from selected entities.", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionCreateRelationship->setShortcut(QCoreApplication::translate("MainWindow", "R", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSet_Selection_Color->setText(QCoreApplication::translate("MainWindow", "Set Selection Color...", nullptr));
        actionConvert_to_Bipartite->setText(QCoreApplication::translate("MainWindow", "Convert to Bipartite", nullptr));
        actionPolygon_View->setText(QCoreApplication::translate("MainWindow", "Polygon View", nullptr));
#if QT_CONFIG(tooltip)
        actionPolygon_View->setToolTip(QCoreApplication::translate("MainWindow", "Polygon View", nullptr));
#endif // QT_CONFIG(tooltip)
        actionKonig_View->setText(QCoreApplication::translate("MainWindow", "Konig View", nullptr));
#if QT_CONFIG(tooltip)
        actionKonig_View->setToolTip(QCoreApplication::translate("MainWindow", "Konig View", nullptr));
#endif // QT_CONFIG(tooltip)
        actionDraw_Curved_Scaffolds->setText(QCoreApplication::translate("MainWindow", "Draw Curved Boundaries", nullptr));
        actionSave_Decomposition->setText(QCoreApplication::translate("MainWindow", "Save Decomposition", nullptr));
        actionLoad_Decomposition->setText(QCoreApplication::translate("MainWindow", "Load Decomposition", nullptr));
        actionDraw_Axes->setText(QCoreApplication::translate("MainWindow", "Draw Axes", nullptr));
        actionDraw_Grid->setText(QCoreApplication::translate("MainWindow", "Draw Grid", nullptr));
        actionDraw_Cursor->setText(QCoreApplication::translate("MainWindow", "Draw Cursor", nullptr));
        actionOpen_CSV_Data->setText(QCoreApplication::translate("MainWindow", "Open CSV Data", nullptr));
        actionSave_CSV_Data->setText(QCoreApplication::translate("MainWindow", "Save CSV Data", nullptr));
        actionLoad_Hyperedge_Colors->setText(QCoreApplication::translate("MainWindow", "Load Hyperedge Colors", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuOption->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        menuData_Process->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuWindows->setTitle(QCoreApplication::translate("MainWindow", "Windows", nullptr));
        menuDebug->setTitle(QCoreApplication::translate("MainWindow", "Debug", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
        dockWidget_Information->setWindowTitle(QCoreApplication::translate("MainWindow", "Selection", nullptr));
        dockWidget_Console->setWindowTitle(QCoreApplication::translate("MainWindow", "Console", nullptr));
        dockWidget_DualVis->setWindowTitle(QCoreApplication::translate("MainWindow", "Dual Vis", nullptr));
        dockWidget_Control->setWindowTitle(QCoreApplication::translate("MainWindow", "Control", nullptr));
        dockWidget_PrimeVis->setWindowTitle(QCoreApplication::translate("MainWindow", "Primal Vis", nullptr));
        dockWidget_BipartVis->setWindowTitle(QCoreApplication::translate("MainWindow", "Bipartite Vis", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
