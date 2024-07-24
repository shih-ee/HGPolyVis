/********************************************************************************
** Form generated from reading UI file 'DockControl.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCKCONTROL_H
#define UI_DOCKCONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DockControl
{
public:
    QVBoxLayout *verticalLayout_17;
    QTabWidget *tabWidget;
    QWidget *VisualizationTab;
    QVBoxLayout *verticalLayout_31;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_10;
    QGroupBox *groupBox_22;
    QHBoxLayout *horizontalLayout_6;
    QRadioButton *radioButton_PrimeVisWidget;
    QRadioButton *radioButton_DualVisWidget;
    QRadioButton *radioButton_BipartVisWidget;
    QGridLayout *gridLayout_32;
    QPushButton *pushButton_RecenterView;
    QPushButton *pushButton_SyncViewZooming;
    QDoubleSpinBox *doubleSpinBox_Zoom;
    QPushButton *pushButton_SetZoom;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_21;
    QLabel *label_30;
    QComboBox *comboBox_entityLabelContent;
    QLabel *label_31;
    QComboBox *comboBox_relationshipLabelContent;
    QCheckBox *checkBox_ShowEntities;
    QCheckBox *checkBox_ShowRelationships;
    QGroupBox *groupBox_filters;
    QGridLayout *gridLayout;
    QSpinBox *spinBox_VertexMinDegree;
    QCheckBox *checkBox_EntityDegreeFilter;
    QSpinBox *spinBox_PolyMinDegree;
    QLabel *label_9;
    QCheckBox *checkBox_RelationshipDegreeFilter;
    QSpinBox *spinBox_PolyMaxDegree;
    QLabel *label_8;
    QSpinBox *spinBox_VertexMaxDegree;
    QSpinBox *spinBox_EntityIndexBottom;
    QCheckBox *checkBox_EntityIndexFilter;
    QSpinBox *spinBox_EntityIndexTop;
    QSpinBox *spinBox_RelationshipIndexBottom;
    QCheckBox *checkBox_RelationshipIndexFilter;
    QSpinBox *spinBox_EntityCardinalityTop;
    QCheckBox *checkBox_EntityCardinalityFilter;
    QSpinBox *spinBox_RelationshipIndexTop;
    QSpinBox *spinBox_EntityCardinalityBottom;
    QCheckBox *checkBox_polygonCardinalityFilter;
    QSpinBox *spinBox_RelationshipCardinalityBottom;
    QSpinBox *spinBox_RelationshipCardinalityTop;
    QGroupBox *groupBox_ColorElements;
    QVBoxLayout *verticalLayout_26;
    QPushButton *pushButton_CalcDegreeAndCardinality;
    QGridLayout *gridLayout_6;
    QLabel *label_59;
    QLabel *label_33;
    QComboBox *comboBox_EntityColor;
    QComboBox *comboBox_RelationshipColor;
    QLabel *label_32;
    QPushButton *pushButton_ApplyEnColorScheme;
    QComboBox *comboBox_ColorScheme;
    QPushButton *pushButton_ApplyReColorScheme;
    QGridLayout *gridLayout_2;
    QPushButton *pushButton_SyncDualElements;
    QGroupBox *groupBox_5;
    QVBoxLayout *verticalLayout_16;
    QGridLayout *gridLayout_29;
    QLabel *label_lensAngle;
    QDoubleSpinBox *doubleSpinBox_VertexSize;
    QDoubleSpinBox *doubleSpinBox_MonogonSize;
    QDoubleSpinBox *doubleSpinBox_MonogonAspectRatio;
    QDoubleSpinBox *doubleSpinBox_LensAngle;
    QLabel *label_4;
    QLabel *label_25;
    QLabel *label_24;
    QSpacerItem *verticalSpacer_7;
    QWidget *LayoutTab;
    QVBoxLayout *verticalLayout_8;
    QVBoxLayout *verticalLayout_5;
    QComboBox *comboBox_LayoutSelector;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_Apply;
    QPushButton *pushButton_Stop;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_ClearFixedVert;
    QPushButton *pushButton_FixSelectedVerts;
    QStackedWidget *stackedWidget_LayoutPages;
    QWidget *default_page;
    QVBoxLayout *verticalLayout_14;
    QTreeWidget *treeWidget_DefaultProperties;
    QWidget *TransformProperties;
    QVBoxLayout *verticalLayout_18;
    QTreeWidget *treeWidget_TransformProperties;
    QWidget *RandomProperties;
    QVBoxLayout *verticalLayout_9;
    QTreeWidget *treeWidget_RandomProperties;
    QWidget *GridProperties;
    QVBoxLayout *verticalLayout_12;
    QTreeWidget *treeWidget_GridProperties;
    QWidget *CircleProperties;
    QVBoxLayout *verticalLayout_11;
    QTreeWidget *treeWidget_CircleProperties;
    QWidget *MonogonProperties;
    QVBoxLayout *verticalLayout_13;
    QTreeWidget *treeWidget_MonogonProperties;
    QWidget *ForceDirectedProperties;
    QVBoxLayout *verticalLayout_7;
    QTreeWidget *treeWidget_FDProperties;
    QWidget *OptimizationProperties;
    QVBoxLayout *verticalLayout_6;
    QTreeWidget *treeWidget_OptimizationProperties;
    QWidget *EnergyProperties;
    QVBoxLayout *verticalLayout_19;
    QTreeWidget *treeWidget_EnergyProperties;
    QWidget *KonigProperties;
    QVBoxLayout *verticalLayout_15;
    QTreeWidget *treeWidget_KonigProperties;
    QWidget *MultiscaleTab;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_14;
    QLabel *label_18;
    QLabel *label_20;
    QLabel *label_19;
    QDoubleSpinBox *doubleSpinBox_Alpha;
    QDoubleSpinBox *doubleSpinBox_Beta;
    QDoubleSpinBox *doubleSpinBox_Gamma;
    QGroupBox *Options;
    QGridLayout *gridLayout_3;
    QCheckBox *checkBox_OperationNodeMerge;
    QCheckBox *checkBox_OperationNodeRemove;
    QCheckBox *checkBox_OperationEdgeRemove;
    QCheckBox *checkBox_OperationEdgeMerge;
    QPushButton *pushButton_GenerateAtomicOperations;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_12;
    QCheckBox *checkBox_PreserveConnections;
    QCheckBox *checkBox_PreserveBranches;
    QCheckBox *checkBox_PruneOnly;
    QSpinBox *spinBox_MaxEdgeMergeSize;
    QLabel *label_28;
    QGroupBox *groupBox_13;
    QGridLayout *gridLayout_15;
    QLabel *label_23;
    QSpinBox *spinBox_MinNodes;
    QSpinBox *spinBox_MaxOps;
    QSpinBox *spinBox_MinEdges;
    QLabel *label_22;
    QLabel *label_21;
    QDoubleSpinBox *doubleSpinBox_PriorityThreshold;
    QLabel *label_34;
    QCheckBox *checkBox_optimizeInversions;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_InvertOperation;
    QPushButton *pushButton_ApplyOperation;
    QPushButton *pushButton_ApplyAllOperations;
    QPushButton *pushButton_InvertAllOperations;
    QPushButton *pushButton_OptimizeFootprint;
    QSpacerItem *verticalSpacer;
    QWidget *DecompositionTab;
    QVBoxLayout *verticalLayout_20;
    QGroupBox *groupBox_CycleDecomp;
    QGridLayout *gridLayout_5;
    QPushButton *pushButton_HighlightAllTrees;
    QPushButton *pushButton_ResetHighlights;
    QPushButton *pushButton_HighlightAllBiComps;
    QPushButton *pushButton_HighlightNextTree;
    QPushButton *pushButton_HighlightAllCycles;
    QPushButton *pushButton_HighlightAllClusters;
    QPushButton *pushButton_CycleDecompose;
    QPushButton *pushButton_HighlightNextCluster;
    QPushButton *pushButton_HighlightNextBiComp;
    QPushButton *pushButton_HighlightNextCycle;
    QGroupBox *groupBox_15;
    QGridLayout *gridLayout_9;
    QPushButton *pushButton_SimpAllTrees;
    QPushButton *pushButton_RecordBipartOps;
    QPushButton *pushButton_SimpAllBicomps;
    QPushButton *pushButton_ApplyNextClusterOp;
    QPushButton *pushButton_ApplyNextTreeOp;
    QPushButton *pushButton_ApplyNextBicompOp;
    QPushButton *pushButton_SimpAllClusters;
    QPushButton *pushButton_SaveBipartOps;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_4;
    QPushButton *pushButton_ApplyNextBipartOp;
    QPushButton *pushButton_InvertLastBipartOp;
    QPushButton *pushButton_LoadBipartOps;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *DockControl)
    {
        if (DockControl->objectName().isEmpty())
            DockControl->setObjectName(QString::fromUtf8("DockControl"));
        DockControl->resize(400, 1000);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DockControl->sizePolicy().hasHeightForWidth());
        DockControl->setSizePolicy(sizePolicy);
        DockControl->setMinimumSize(QSize(400, 100));
        DockControl->setMaximumSize(QSize(16777215, 16777215));
        DockControl->setToolTipDuration(1000);
        verticalLayout_17 = new QVBoxLayout(DockControl);
        verticalLayout_17->setSpacing(6);
        verticalLayout_17->setContentsMargins(11, 11, 11, 11);
        verticalLayout_17->setObjectName(QString::fromUtf8("verticalLayout_17"));
        verticalLayout_17->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(DockControl);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy1);
        tabWidget->setMinimumSize(QSize(0, 0));
        tabWidget->setMaximumSize(QSize(16777215, 16777215));
        tabWidget->setAutoFillBackground(true);
        tabWidget->setTabPosition(QTabWidget::East);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabWidget->setElideMode(Qt::ElideRight);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(true);
        tabWidget->setTabBarAutoHide(false);
        VisualizationTab = new QWidget();
        VisualizationTab->setObjectName(QString::fromUtf8("VisualizationTab"));
        VisualizationTab->setMinimumSize(QSize(0, 0));
        VisualizationTab->setMaximumSize(QSize(16777215, 16777215));
        VisualizationTab->setAutoFillBackground(true);
        verticalLayout_31 = new QVBoxLayout(VisualizationTab);
        verticalLayout_31->setSpacing(4);
        verticalLayout_31->setContentsMargins(11, 11, 11, 11);
        verticalLayout_31->setObjectName(QString::fromUtf8("verticalLayout_31"));
        verticalLayout_31->setContentsMargins(2, 2, 6, 2);
        groupBox_4 = new QGroupBox(VisualizationTab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy2);
        groupBox_4->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_10 = new QVBoxLayout(groupBox_4);
        verticalLayout_10->setSpacing(1);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QString::fromUtf8("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(1, 1, 1, 1);
        groupBox_22 = new QGroupBox(groupBox_4);
        groupBox_22->setObjectName(QString::fromUtf8("groupBox_22"));
        horizontalLayout_6 = new QHBoxLayout(groupBox_22);
        horizontalLayout_6->setSpacing(1);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(1, 1, 1, 1);
        radioButton_PrimeVisWidget = new QRadioButton(groupBox_22);
        radioButton_PrimeVisWidget->setObjectName(QString::fromUtf8("radioButton_PrimeVisWidget"));
        sizePolicy1.setHeightForWidth(radioButton_PrimeVisWidget->sizePolicy().hasHeightForWidth());
        radioButton_PrimeVisWidget->setSizePolicy(sizePolicy1);
        radioButton_PrimeVisWidget->setChecked(true);

        horizontalLayout_6->addWidget(radioButton_PrimeVisWidget);

        radioButton_DualVisWidget = new QRadioButton(groupBox_22);
        radioButton_DualVisWidget->setObjectName(QString::fromUtf8("radioButton_DualVisWidget"));
        sizePolicy1.setHeightForWidth(radioButton_DualVisWidget->sizePolicy().hasHeightForWidth());
        radioButton_DualVisWidget->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(radioButton_DualVisWidget);

        radioButton_BipartVisWidget = new QRadioButton(groupBox_22);
        radioButton_BipartVisWidget->setObjectName(QString::fromUtf8("radioButton_BipartVisWidget"));

        horizontalLayout_6->addWidget(radioButton_BipartVisWidget);


        verticalLayout_10->addWidget(groupBox_22);

        gridLayout_32 = new QGridLayout();
        gridLayout_32->setSpacing(2);
        gridLayout_32->setObjectName(QString::fromUtf8("gridLayout_32"));
        gridLayout_32->setContentsMargins(-1, -1, -1, 0);
        pushButton_RecenterView = new QPushButton(groupBox_4);
        pushButton_RecenterView->setObjectName(QString::fromUtf8("pushButton_RecenterView"));
        sizePolicy2.setHeightForWidth(pushButton_RecenterView->sizePolicy().hasHeightForWidth());
        pushButton_RecenterView->setSizePolicy(sizePolicy2);

        gridLayout_32->addWidget(pushButton_RecenterView, 0, 1, 1, 1);

        pushButton_SyncViewZooming = new QPushButton(groupBox_4);
        pushButton_SyncViewZooming->setObjectName(QString::fromUtf8("pushButton_SyncViewZooming"));

        gridLayout_32->addWidget(pushButton_SyncViewZooming, 0, 0, 1, 1);

        doubleSpinBox_Zoom = new QDoubleSpinBox(groupBox_4);
        doubleSpinBox_Zoom->setObjectName(QString::fromUtf8("doubleSpinBox_Zoom"));
        doubleSpinBox_Zoom->setMaximum(1000.000000000000000);
        doubleSpinBox_Zoom->setSingleStep(0.100000000000000);
        doubleSpinBox_Zoom->setValue(1.000000000000000);

        gridLayout_32->addWidget(doubleSpinBox_Zoom, 1, 0, 1, 1);

        pushButton_SetZoom = new QPushButton(groupBox_4);
        pushButton_SetZoom->setObjectName(QString::fromUtf8("pushButton_SetZoom"));

        gridLayout_32->addWidget(pushButton_SetZoom, 1, 1, 1, 1);


        verticalLayout_10->addLayout(gridLayout_32);


        verticalLayout_31->addWidget(groupBox_4);

        groupBox_2 = new QGroupBox(VisualizationTab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        sizePolicy1.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy1);
        groupBox_2->setMinimumSize(QSize(0, 0));
        groupBox_2->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_21 = new QGridLayout(groupBox_2);
        gridLayout_21->setSpacing(1);
        gridLayout_21->setContentsMargins(11, 11, 11, 11);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        gridLayout_21->setContentsMargins(1, 1, 1, 1);
        label_30 = new QLabel(groupBox_2);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_21->addWidget(label_30, 1, 0, 1, 1);

        comboBox_entityLabelContent = new QComboBox(groupBox_2);
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->addItem(QString());
        comboBox_entityLabelContent->setObjectName(QString::fromUtf8("comboBox_entityLabelContent"));

        gridLayout_21->addWidget(comboBox_entityLabelContent, 2, 0, 1, 1);

        label_31 = new QLabel(groupBox_2);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_21->addWidget(label_31, 1, 1, 1, 1);

        comboBox_relationshipLabelContent = new QComboBox(groupBox_2);
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->addItem(QString());
        comboBox_relationshipLabelContent->setObjectName(QString::fromUtf8("comboBox_relationshipLabelContent"));

        gridLayout_21->addWidget(comboBox_relationshipLabelContent, 2, 1, 1, 1);

        checkBox_ShowEntities = new QCheckBox(groupBox_2);
        checkBox_ShowEntities->setObjectName(QString::fromUtf8("checkBox_ShowEntities"));
        sizePolicy1.setHeightForWidth(checkBox_ShowEntities->sizePolicy().hasHeightForWidth());
        checkBox_ShowEntities->setSizePolicy(sizePolicy1);
        checkBox_ShowEntities->setMaximumSize(QSize(16777215, 16777215));
        checkBox_ShowEntities->setChecked(true);

        gridLayout_21->addWidget(checkBox_ShowEntities, 0, 0, 1, 1);

        checkBox_ShowRelationships = new QCheckBox(groupBox_2);
        checkBox_ShowRelationships->setObjectName(QString::fromUtf8("checkBox_ShowRelationships"));
        sizePolicy1.setHeightForWidth(checkBox_ShowRelationships->sizePolicy().hasHeightForWidth());
        checkBox_ShowRelationships->setSizePolicy(sizePolicy1);
        checkBox_ShowRelationships->setChecked(true);

        gridLayout_21->addWidget(checkBox_ShowRelationships, 0, 1, 1, 1);


        verticalLayout_31->addWidget(groupBox_2);

        groupBox_filters = new QGroupBox(VisualizationTab);
        groupBox_filters->setObjectName(QString::fromUtf8("groupBox_filters"));
        sizePolicy1.setHeightForWidth(groupBox_filters->sizePolicy().hasHeightForWidth());
        groupBox_filters->setSizePolicy(sizePolicy1);
        groupBox_filters->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(groupBox_filters);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(1, 1, 1, 1);
        spinBox_VertexMinDegree = new QSpinBox(groupBox_filters);
        spinBox_VertexMinDegree->setObjectName(QString::fromUtf8("spinBox_VertexMinDegree"));
        sizePolicy1.setHeightForWidth(spinBox_VertexMinDegree->sizePolicy().hasHeightForWidth());
        spinBox_VertexMinDegree->setSizePolicy(sizePolicy1);
        spinBox_VertexMinDegree->setMinimumSize(QSize(0, 0));
        spinBox_VertexMinDegree->setMaximumSize(QSize(16777215, 16777215));
        spinBox_VertexMinDegree->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_VertexMinDegree, 5, 1, 1, 1);

        checkBox_EntityDegreeFilter = new QCheckBox(groupBox_filters);
        checkBox_EntityDegreeFilter->setObjectName(QString::fromUtf8("checkBox_EntityDegreeFilter"));
        sizePolicy1.setHeightForWidth(checkBox_EntityDegreeFilter->sizePolicy().hasHeightForWidth());
        checkBox_EntityDegreeFilter->setSizePolicy(sizePolicy1);
        checkBox_EntityDegreeFilter->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(checkBox_EntityDegreeFilter, 5, 0, 1, 1);

        spinBox_PolyMinDegree = new QSpinBox(groupBox_filters);
        spinBox_PolyMinDegree->setObjectName(QString::fromUtf8("spinBox_PolyMinDegree"));
        sizePolicy1.setHeightForWidth(spinBox_PolyMinDegree->sizePolicy().hasHeightForWidth());
        spinBox_PolyMinDegree->setSizePolicy(sizePolicy1);
        spinBox_PolyMinDegree->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_PolyMinDegree, 6, 1, 1, 1);

        label_9 = new QLabel(groupBox_filters);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_9, 0, 2, 1, 1);

        checkBox_RelationshipDegreeFilter = new QCheckBox(groupBox_filters);
        checkBox_RelationshipDegreeFilter->setObjectName(QString::fromUtf8("checkBox_RelationshipDegreeFilter"));
        sizePolicy1.setHeightForWidth(checkBox_RelationshipDegreeFilter->sizePolicy().hasHeightForWidth());
        checkBox_RelationshipDegreeFilter->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(checkBox_RelationshipDegreeFilter, 6, 0, 1, 1);

        spinBox_PolyMaxDegree = new QSpinBox(groupBox_filters);
        spinBox_PolyMaxDegree->setObjectName(QString::fromUtf8("spinBox_PolyMaxDegree"));
        sizePolicy1.setHeightForWidth(spinBox_PolyMaxDegree->sizePolicy().hasHeightForWidth());
        spinBox_PolyMaxDegree->setSizePolicy(sizePolicy1);
        spinBox_PolyMaxDegree->setFocusPolicy(Qt::ClickFocus);
        spinBox_PolyMaxDegree->setValue(1);

        gridLayout->addWidget(spinBox_PolyMaxDegree, 6, 2, 1, 1);

        label_8 = new QLabel(groupBox_filters);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_8, 0, 1, 1, 1);

        spinBox_VertexMaxDegree = new QSpinBox(groupBox_filters);
        spinBox_VertexMaxDegree->setObjectName(QString::fromUtf8("spinBox_VertexMaxDegree"));
        sizePolicy1.setHeightForWidth(spinBox_VertexMaxDegree->sizePolicy().hasHeightForWidth());
        spinBox_VertexMaxDegree->setSizePolicy(sizePolicy1);
        spinBox_VertexMaxDegree->setMaximumSize(QSize(16777215, 16777215));
        spinBox_VertexMaxDegree->setFocusPolicy(Qt::ClickFocus);
        spinBox_VertexMaxDegree->setValue(1);

        gridLayout->addWidget(spinBox_VertexMaxDegree, 5, 2, 1, 1);

        spinBox_EntityIndexBottom = new QSpinBox(groupBox_filters);
        spinBox_EntityIndexBottom->setObjectName(QString::fromUtf8("spinBox_EntityIndexBottom"));
        sizePolicy1.setHeightForWidth(spinBox_EntityIndexBottom->sizePolicy().hasHeightForWidth());
        spinBox_EntityIndexBottom->setSizePolicy(sizePolicy1);
        spinBox_EntityIndexBottom->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_EntityIndexBottom, 1, 1, 1, 1);

        checkBox_EntityIndexFilter = new QCheckBox(groupBox_filters);
        checkBox_EntityIndexFilter->setObjectName(QString::fromUtf8("checkBox_EntityIndexFilter"));
        sizePolicy1.setHeightForWidth(checkBox_EntityIndexFilter->sizePolicy().hasHeightForWidth());
        checkBox_EntityIndexFilter->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(checkBox_EntityIndexFilter, 1, 0, 1, 1);

        spinBox_EntityIndexTop = new QSpinBox(groupBox_filters);
        spinBox_EntityIndexTop->setObjectName(QString::fromUtf8("spinBox_EntityIndexTop"));
        sizePolicy1.setHeightForWidth(spinBox_EntityIndexTop->sizePolicy().hasHeightForWidth());
        spinBox_EntityIndexTop->setSizePolicy(sizePolicy1);
        spinBox_EntityIndexTop->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_EntityIndexTop, 1, 2, 1, 1);

        spinBox_RelationshipIndexBottom = new QSpinBox(groupBox_filters);
        spinBox_RelationshipIndexBottom->setObjectName(QString::fromUtf8("spinBox_RelationshipIndexBottom"));
        sizePolicy1.setHeightForWidth(spinBox_RelationshipIndexBottom->sizePolicy().hasHeightForWidth());
        spinBox_RelationshipIndexBottom->setSizePolicy(sizePolicy1);
        spinBox_RelationshipIndexBottom->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_RelationshipIndexBottom, 2, 1, 1, 1);

        checkBox_RelationshipIndexFilter = new QCheckBox(groupBox_filters);
        checkBox_RelationshipIndexFilter->setObjectName(QString::fromUtf8("checkBox_RelationshipIndexFilter"));
        sizePolicy1.setHeightForWidth(checkBox_RelationshipIndexFilter->sizePolicy().hasHeightForWidth());
        checkBox_RelationshipIndexFilter->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(checkBox_RelationshipIndexFilter, 2, 0, 1, 1);

        spinBox_EntityCardinalityTop = new QSpinBox(groupBox_filters);
        spinBox_EntityCardinalityTop->setObjectName(QString::fromUtf8("spinBox_EntityCardinalityTop"));
        sizePolicy1.setHeightForWidth(spinBox_EntityCardinalityTop->sizePolicy().hasHeightForWidth());
        spinBox_EntityCardinalityTop->setSizePolicy(sizePolicy1);
        spinBox_EntityCardinalityTop->setFocusPolicy(Qt::ClickFocus);
        spinBox_EntityCardinalityTop->setValue(1);

        gridLayout->addWidget(spinBox_EntityCardinalityTop, 3, 2, 1, 1);

        checkBox_EntityCardinalityFilter = new QCheckBox(groupBox_filters);
        checkBox_EntityCardinalityFilter->setObjectName(QString::fromUtf8("checkBox_EntityCardinalityFilter"));
        sizePolicy1.setHeightForWidth(checkBox_EntityCardinalityFilter->sizePolicy().hasHeightForWidth());
        checkBox_EntityCardinalityFilter->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(checkBox_EntityCardinalityFilter, 3, 0, 1, 1);

        spinBox_RelationshipIndexTop = new QSpinBox(groupBox_filters);
        spinBox_RelationshipIndexTop->setObjectName(QString::fromUtf8("spinBox_RelationshipIndexTop"));
        sizePolicy1.setHeightForWidth(spinBox_RelationshipIndexTop->sizePolicy().hasHeightForWidth());
        spinBox_RelationshipIndexTop->setSizePolicy(sizePolicy1);
        spinBox_RelationshipIndexTop->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_RelationshipIndexTop, 2, 2, 1, 1);

        spinBox_EntityCardinalityBottom = new QSpinBox(groupBox_filters);
        spinBox_EntityCardinalityBottom->setObjectName(QString::fromUtf8("spinBox_EntityCardinalityBottom"));
        sizePolicy1.setHeightForWidth(spinBox_EntityCardinalityBottom->sizePolicy().hasHeightForWidth());
        spinBox_EntityCardinalityBottom->setSizePolicy(sizePolicy1);
        spinBox_EntityCardinalityBottom->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(spinBox_EntityCardinalityBottom, 3, 1, 1, 1);

        checkBox_polygonCardinalityFilter = new QCheckBox(groupBox_filters);
        checkBox_polygonCardinalityFilter->setObjectName(QString::fromUtf8("checkBox_polygonCardinalityFilter"));
        sizePolicy1.setHeightForWidth(checkBox_polygonCardinalityFilter->sizePolicy().hasHeightForWidth());
        checkBox_polygonCardinalityFilter->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(checkBox_polygonCardinalityFilter, 4, 0, 1, 1);

        spinBox_RelationshipCardinalityBottom = new QSpinBox(groupBox_filters);
        spinBox_RelationshipCardinalityBottom->setObjectName(QString::fromUtf8("spinBox_RelationshipCardinalityBottom"));
        sizePolicy1.setHeightForWidth(spinBox_RelationshipCardinalityBottom->sizePolicy().hasHeightForWidth());
        spinBox_RelationshipCardinalityBottom->setSizePolicy(sizePolicy1);
        spinBox_RelationshipCardinalityBottom->setFocusPolicy(Qt::ClickFocus);
        spinBox_RelationshipCardinalityBottom->setMinimum(2);
        spinBox_RelationshipCardinalityBottom->setMaximum(10);
        spinBox_RelationshipCardinalityBottom->setValue(2);

        gridLayout->addWidget(spinBox_RelationshipCardinalityBottom, 4, 1, 1, 1);

        spinBox_RelationshipCardinalityTop = new QSpinBox(groupBox_filters);
        spinBox_RelationshipCardinalityTop->setObjectName(QString::fromUtf8("spinBox_RelationshipCardinalityTop"));
        sizePolicy1.setHeightForWidth(spinBox_RelationshipCardinalityTop->sizePolicy().hasHeightForWidth());
        spinBox_RelationshipCardinalityTop->setSizePolicy(sizePolicy1);
        spinBox_RelationshipCardinalityTop->setFocusPolicy(Qt::ClickFocus);
        spinBox_RelationshipCardinalityTop->setMinimum(3);
        spinBox_RelationshipCardinalityTop->setMaximum(10);
        spinBox_RelationshipCardinalityTop->setValue(10);

        gridLayout->addWidget(spinBox_RelationshipCardinalityTop, 4, 2, 1, 1);


        verticalLayout_31->addWidget(groupBox_filters);

        groupBox_ColorElements = new QGroupBox(VisualizationTab);
        groupBox_ColorElements->setObjectName(QString::fromUtf8("groupBox_ColorElements"));
        verticalLayout_26 = new QVBoxLayout(groupBox_ColorElements);
        verticalLayout_26->setSpacing(2);
        verticalLayout_26->setContentsMargins(11, 11, 11, 11);
        verticalLayout_26->setObjectName(QString::fromUtf8("verticalLayout_26"));
        verticalLayout_26->setContentsMargins(2, 2, 2, 2);
        pushButton_CalcDegreeAndCardinality = new QPushButton(groupBox_ColorElements);
        pushButton_CalcDegreeAndCardinality->setObjectName(QString::fromUtf8("pushButton_CalcDegreeAndCardinality"));

        verticalLayout_26->addWidget(pushButton_CalcDegreeAndCardinality);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        label_59 = new QLabel(groupBox_ColorElements);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setMaximumSize(QSize(80, 16777215));

        gridLayout_6->addWidget(label_59, 0, 0, 1, 1);

        label_33 = new QLabel(groupBox_ColorElements);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_6->addWidget(label_33, 2, 0, 1, 1);

        comboBox_EntityColor = new QComboBox(groupBox_ColorElements);
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->addItem(QString());
        comboBox_EntityColor->setObjectName(QString::fromUtf8("comboBox_EntityColor"));

        gridLayout_6->addWidget(comboBox_EntityColor, 1, 1, 1, 1);

        comboBox_RelationshipColor = new QComboBox(groupBox_ColorElements);
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->addItem(QString());
        comboBox_RelationshipColor->setObjectName(QString::fromUtf8("comboBox_RelationshipColor"));

        gridLayout_6->addWidget(comboBox_RelationshipColor, 2, 1, 1, 1);

        label_32 = new QLabel(groupBox_ColorElements);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_6->addWidget(label_32, 1, 0, 1, 1);

        pushButton_ApplyEnColorScheme = new QPushButton(groupBox_ColorElements);
        pushButton_ApplyEnColorScheme->setObjectName(QString::fromUtf8("pushButton_ApplyEnColorScheme"));

        gridLayout_6->addWidget(pushButton_ApplyEnColorScheme, 1, 2, 1, 1);

        comboBox_ColorScheme = new QComboBox(groupBox_ColorElements);
        comboBox_ColorScheme->setObjectName(QString::fromUtf8("comboBox_ColorScheme"));
        comboBox_ColorScheme->setFocusPolicy(Qt::ClickFocus);
        comboBox_ColorScheme->setAutoFillBackground(true);

        gridLayout_6->addWidget(comboBox_ColorScheme, 0, 1, 1, 2);

        pushButton_ApplyReColorScheme = new QPushButton(groupBox_ColorElements);
        pushButton_ApplyReColorScheme->setObjectName(QString::fromUtf8("pushButton_ApplyReColorScheme"));

        gridLayout_6->addWidget(pushButton_ApplyReColorScheme, 2, 2, 1, 1);


        verticalLayout_26->addLayout(gridLayout_6);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(1);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, -1, -1, 0);
        pushButton_SyncDualElements = new QPushButton(groupBox_ColorElements);
        pushButton_SyncDualElements->setObjectName(QString::fromUtf8("pushButton_SyncDualElements"));
        pushButton_SyncDualElements->setToolTipDuration(2000);

        gridLayout_2->addWidget(pushButton_SyncDualElements, 0, 1, 1, 1);


        verticalLayout_26->addLayout(gridLayout_2);


        verticalLayout_31->addWidget(groupBox_ColorElements);

        groupBox_5 = new QGroupBox(VisualizationTab);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout_16 = new QVBoxLayout(groupBox_5);
        verticalLayout_16->setSpacing(1);
        verticalLayout_16->setContentsMargins(11, 11, 11, 11);
        verticalLayout_16->setObjectName(QString::fromUtf8("verticalLayout_16"));
        verticalLayout_16->setContentsMargins(1, 1, 1, 1);
        gridLayout_29 = new QGridLayout();
        gridLayout_29->setSpacing(1);
        gridLayout_29->setObjectName(QString::fromUtf8("gridLayout_29"));
        label_lensAngle = new QLabel(groupBox_5);
        label_lensAngle->setObjectName(QString::fromUtf8("label_lensAngle"));
        label_lensAngle->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_29->addWidget(label_lensAngle, 2, 0, 1, 1);

        doubleSpinBox_VertexSize = new QDoubleSpinBox(groupBox_5);
        doubleSpinBox_VertexSize->setObjectName(QString::fromUtf8("doubleSpinBox_VertexSize"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_VertexSize->sizePolicy().hasHeightForWidth());
        doubleSpinBox_VertexSize->setSizePolicy(sizePolicy1);
        doubleSpinBox_VertexSize->setFocusPolicy(Qt::ClickFocus);
        doubleSpinBox_VertexSize->setDecimals(4);
        doubleSpinBox_VertexSize->setMaximum(99999999.000000000000000);
        doubleSpinBox_VertexSize->setSingleStep(0.010000000000000);
        doubleSpinBox_VertexSize->setValue(0.010000000000000);

        gridLayout_29->addWidget(doubleSpinBox_VertexSize, 0, 1, 1, 1);

        doubleSpinBox_MonogonSize = new QDoubleSpinBox(groupBox_5);
        doubleSpinBox_MonogonSize->setObjectName(QString::fromUtf8("doubleSpinBox_MonogonSize"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_MonogonSize->sizePolicy().hasHeightForWidth());
        doubleSpinBox_MonogonSize->setSizePolicy(sizePolicy1);
        doubleSpinBox_MonogonSize->setFocusPolicy(Qt::ClickFocus);
        doubleSpinBox_MonogonSize->setDecimals(4);
        doubleSpinBox_MonogonSize->setMinimum(0.000000000000000);
        doubleSpinBox_MonogonSize->setMaximum(99999999.000000000000000);
        doubleSpinBox_MonogonSize->setSingleStep(0.010000000000000);
        doubleSpinBox_MonogonSize->setValue(0.050000000000000);

        gridLayout_29->addWidget(doubleSpinBox_MonogonSize, 4, 1, 1, 1);

        doubleSpinBox_MonogonAspectRatio = new QDoubleSpinBox(groupBox_5);
        doubleSpinBox_MonogonAspectRatio->setObjectName(QString::fromUtf8("doubleSpinBox_MonogonAspectRatio"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_MonogonAspectRatio->sizePolicy().hasHeightForWidth());
        doubleSpinBox_MonogonAspectRatio->setSizePolicy(sizePolicy1);
        doubleSpinBox_MonogonAspectRatio->setFocusPolicy(Qt::ClickFocus);
        doubleSpinBox_MonogonAspectRatio->setDecimals(4);
        doubleSpinBox_MonogonAspectRatio->setMinimum(0.000000000000000);
        doubleSpinBox_MonogonAspectRatio->setMaximum(99999999.000000000000000);
        doubleSpinBox_MonogonAspectRatio->setSingleStep(0.010000000000000);
        doubleSpinBox_MonogonAspectRatio->setValue(5.400000000000000);

        gridLayout_29->addWidget(doubleSpinBox_MonogonAspectRatio, 3, 1, 1, 1);

        doubleSpinBox_LensAngle = new QDoubleSpinBox(groupBox_5);
        doubleSpinBox_LensAngle->setObjectName(QString::fromUtf8("doubleSpinBox_LensAngle"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_LensAngle->sizePolicy().hasHeightForWidth());
        doubleSpinBox_LensAngle->setSizePolicy(sizePolicy1);
        doubleSpinBox_LensAngle->setFocusPolicy(Qt::ClickFocus);
        doubleSpinBox_LensAngle->setDecimals(4);
        doubleSpinBox_LensAngle->setMinimum(0.010000000000000);
        doubleSpinBox_LensAngle->setMaximum(179.990000000000009);
        doubleSpinBox_LensAngle->setValue(15.000000000000000);

        gridLayout_29->addWidget(doubleSpinBox_LensAngle, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox_5);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_29->addWidget(label_4, 0, 0, 1, 1);

        label_25 = new QLabel(groupBox_5);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_29->addWidget(label_25, 4, 0, 1, 1);

        label_24 = new QLabel(groupBox_5);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_29->addWidget(label_24, 3, 0, 1, 1);


        verticalLayout_16->addLayout(gridLayout_29);


        verticalLayout_31->addWidget(groupBox_5);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_31->addItem(verticalSpacer_7);

        tabWidget->addTab(VisualizationTab, QString());
        LayoutTab = new QWidget();
        LayoutTab->setObjectName(QString::fromUtf8("LayoutTab"));
        LayoutTab->setAutoFillBackground(true);
        verticalLayout_8 = new QVBoxLayout(LayoutTab);
        verticalLayout_8->setSpacing(4);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        verticalLayout_8->setContentsMargins(2, 2, 6, 2);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(4);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        comboBox_LayoutSelector = new QComboBox(LayoutTab);
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->addItem(QString());
        comboBox_LayoutSelector->setObjectName(QString::fromUtf8("comboBox_LayoutSelector"));
        comboBox_LayoutSelector->setMinimumSize(QSize(0, 32));
        comboBox_LayoutSelector->setFrame(true);

        verticalLayout_5->addWidget(comboBox_LayoutSelector);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButton_Apply = new QPushButton(LayoutTab);
        pushButton_Apply->setObjectName(QString::fromUtf8("pushButton_Apply"));
        pushButton_Apply->setLayoutDirection(Qt::LeftToRight);
        pushButton_Apply->setAutoFillBackground(false);
        pushButton_Apply->setStyleSheet(QString::fromUtf8(""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/video-play-64.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Apply->setIcon(icon);
        pushButton_Apply->setIconSize(QSize(24, 24));
        pushButton_Apply->setFlat(true);

        horizontalLayout_4->addWidget(pushButton_Apply);

        pushButton_Stop = new QPushButton(LayoutTab);
        pushButton_Stop->setObjectName(QString::fromUtf8("pushButton_Stop"));
        pushButton_Stop->setAutoFillBackground(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/stop-flat.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Stop->setIcon(icon1);
        pushButton_Stop->setIconSize(QSize(24, 24));
        pushButton_Stop->setFlat(true);

        horizontalLayout_4->addWidget(pushButton_Stop);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_5->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_ClearFixedVert = new QPushButton(LayoutTab);
        pushButton_ClearFixedVert->setObjectName(QString::fromUtf8("pushButton_ClearFixedVert"));

        horizontalLayout->addWidget(pushButton_ClearFixedVert);

        pushButton_FixSelectedVerts = new QPushButton(LayoutTab);
        pushButton_FixSelectedVerts->setObjectName(QString::fromUtf8("pushButton_FixSelectedVerts"));

        horizontalLayout->addWidget(pushButton_FixSelectedVerts);


        verticalLayout_5->addLayout(horizontalLayout);


        verticalLayout_8->addLayout(verticalLayout_5);

        stackedWidget_LayoutPages = new QStackedWidget(LayoutTab);
        stackedWidget_LayoutPages->setObjectName(QString::fromUtf8("stackedWidget_LayoutPages"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(stackedWidget_LayoutPages->sizePolicy().hasHeightForWidth());
        stackedWidget_LayoutPages->setSizePolicy(sizePolicy3);
        stackedWidget_LayoutPages->setFrameShape(QFrame::StyledPanel);
        default_page = new QWidget();
        default_page->setObjectName(QString::fromUtf8("default_page"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(default_page->sizePolicy().hasHeightForWidth());
        default_page->setSizePolicy(sizePolicy4);
        default_page->setMinimumSize(QSize(360, 0));
        verticalLayout_14 = new QVBoxLayout(default_page);
        verticalLayout_14->setSpacing(0);
        verticalLayout_14->setContentsMargins(11, 11, 11, 11);
        verticalLayout_14->setObjectName(QString::fromUtf8("verticalLayout_14"));
        verticalLayout_14->setContentsMargins(0, 0, 0, 0);
        treeWidget_DefaultProperties = new QTreeWidget(default_page);
        treeWidget_DefaultProperties->setObjectName(QString::fromUtf8("treeWidget_DefaultProperties"));
        treeWidget_DefaultProperties->setEnabled(true);
        sizePolicy4.setHeightForWidth(treeWidget_DefaultProperties->sizePolicy().hasHeightForWidth());
        treeWidget_DefaultProperties->setSizePolicy(sizePolicy4);
        treeWidget_DefaultProperties->setMinimumSize(QSize(360, 0));
        treeWidget_DefaultProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_DefaultProperties->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeWidget_DefaultProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_DefaultProperties->setProperty("showDropIndicator", QVariant(true));
        treeWidget_DefaultProperties->setAlternatingRowColors(false);
        treeWidget_DefaultProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_DefaultProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_DefaultProperties->setAutoExpandDelay(0);
        treeWidget_DefaultProperties->setIndentation(0);
        treeWidget_DefaultProperties->setRootIsDecorated(false);
        treeWidget_DefaultProperties->setAnimated(false);
        treeWidget_DefaultProperties->setAllColumnsShowFocus(false);
        treeWidget_DefaultProperties->setWordWrap(false);
        treeWidget_DefaultProperties->setHeaderHidden(false);
        treeWidget_DefaultProperties->header()->setVisible(true);
        treeWidget_DefaultProperties->header()->setCascadingSectionResizes(false);
        treeWidget_DefaultProperties->header()->setMinimumSectionSize(180);
        treeWidget_DefaultProperties->header()->setDefaultSectionSize(180);
        treeWidget_DefaultProperties->header()->setHighlightSections(false);

        verticalLayout_14->addWidget(treeWidget_DefaultProperties);

        stackedWidget_LayoutPages->addWidget(default_page);
        TransformProperties = new QWidget();
        TransformProperties->setObjectName(QString::fromUtf8("TransformProperties"));
        sizePolicy4.setHeightForWidth(TransformProperties->sizePolicy().hasHeightForWidth());
        TransformProperties->setSizePolicy(sizePolicy4);
        TransformProperties->setMinimumSize(QSize(360, 0));
        TransformProperties->setLayoutDirection(Qt::LeftToRight);
        verticalLayout_18 = new QVBoxLayout(TransformProperties);
        verticalLayout_18->setSpacing(0);
        verticalLayout_18->setContentsMargins(11, 11, 11, 11);
        verticalLayout_18->setObjectName(QString::fromUtf8("verticalLayout_18"));
        verticalLayout_18->setContentsMargins(0, 0, 0, 0);
        treeWidget_TransformProperties = new QTreeWidget(TransformProperties);
        QBrush brush(QColor(200, 200, 200, 255));
        brush.setStyle(Qt::SolidPattern);
        QFont font;
        font.setBold(true);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget_TransformProperties);
        __qtreewidgetitem->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem->setFont(1, font);
        __qtreewidgetitem->setBackground(1, brush);
        __qtreewidgetitem->setFont(0, font);
        __qtreewidgetitem->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(__qtreewidgetitem);
        __qtreewidgetitem1->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem1->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(__qtreewidgetitem);
        __qtreewidgetitem2->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem(__qtreewidgetitem);
        __qtreewidgetitem3->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem4 = new QTreeWidgetItem(treeWidget_TransformProperties);
        __qtreewidgetitem4->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem4->setFont(1, font);
        __qtreewidgetitem4->setBackground(1, brush);
        __qtreewidgetitem4->setFont(0, font);
        __qtreewidgetitem4->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem5 = new QTreeWidgetItem(__qtreewidgetitem4);
        __qtreewidgetitem5->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem5->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem6 = new QTreeWidgetItem(__qtreewidgetitem4);
        __qtreewidgetitem6->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem7 = new QTreeWidgetItem(treeWidget_TransformProperties);
        __qtreewidgetitem7->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem7->setFont(1, font);
        __qtreewidgetitem7->setBackground(1, brush);
        __qtreewidgetitem7->setFont(0, font);
        __qtreewidgetitem7->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem8 = new QTreeWidgetItem(__qtreewidgetitem7);
        __qtreewidgetitem8->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem8->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem9 = new QTreeWidgetItem(__qtreewidgetitem7);
        __qtreewidgetitem9->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem10 = new QTreeWidgetItem(__qtreewidgetitem7);
        __qtreewidgetitem10->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem11 = new QTreeWidgetItem(treeWidget_TransformProperties);
        __qtreewidgetitem11->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem11->setFont(1, font);
        __qtreewidgetitem11->setBackground(1, brush);
        __qtreewidgetitem11->setFont(0, font);
        __qtreewidgetitem11->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem12 = new QTreeWidgetItem(__qtreewidgetitem11);
        __qtreewidgetitem12->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem12->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem13 = new QTreeWidgetItem(__qtreewidgetitem11);
        __qtreewidgetitem13->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_TransformProperties->setObjectName(QString::fromUtf8("treeWidget_TransformProperties"));
        sizePolicy4.setHeightForWidth(treeWidget_TransformProperties->sizePolicy().hasHeightForWidth());
        treeWidget_TransformProperties->setSizePolicy(sizePolicy4);
        treeWidget_TransformProperties->setMinimumSize(QSize(360, 0));
        treeWidget_TransformProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_TransformProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_TransformProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_TransformProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_TransformProperties->setAutoExpandDelay(0);
        treeWidget_TransformProperties->setIndentation(0);
        treeWidget_TransformProperties->setRootIsDecorated(false);
        treeWidget_TransformProperties->header()->setMinimumSectionSize(180);
        treeWidget_TransformProperties->header()->setDefaultSectionSize(180);

        verticalLayout_18->addWidget(treeWidget_TransformProperties);

        stackedWidget_LayoutPages->addWidget(TransformProperties);
        RandomProperties = new QWidget();
        RandomProperties->setObjectName(QString::fromUtf8("RandomProperties"));
        sizePolicy4.setHeightForWidth(RandomProperties->sizePolicy().hasHeightForWidth());
        RandomProperties->setSizePolicy(sizePolicy4);
        RandomProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_9 = new QVBoxLayout(RandomProperties);
        verticalLayout_9->setSpacing(0);
        verticalLayout_9->setContentsMargins(11, 11, 11, 11);
        verticalLayout_9->setObjectName(QString::fromUtf8("verticalLayout_9"));
        verticalLayout_9->setContentsMargins(0, 0, 0, 0);
        treeWidget_RandomProperties = new QTreeWidget(RandomProperties);
        QTreeWidgetItem *__qtreewidgetitem14 = new QTreeWidgetItem(treeWidget_RandomProperties);
        __qtreewidgetitem14->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem14->setBackground(1, brush);
        __qtreewidgetitem14->setFont(0, font);
        __qtreewidgetitem14->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem15 = new QTreeWidgetItem(__qtreewidgetitem14);
        __qtreewidgetitem15->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem15->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem16 = new QTreeWidgetItem(treeWidget_RandomProperties);
        __qtreewidgetitem16->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem16->setBackground(1, brush);
        __qtreewidgetitem16->setFont(0, font);
        __qtreewidgetitem16->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem17 = new QTreeWidgetItem(__qtreewidgetitem16);
        __qtreewidgetitem17->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem18 = new QTreeWidgetItem(__qtreewidgetitem16);
        __qtreewidgetitem18->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_RandomProperties->setObjectName(QString::fromUtf8("treeWidget_RandomProperties"));
        treeWidget_RandomProperties->setEnabled(true);
        treeWidget_RandomProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_RandomProperties->setAutoScrollMargin(16);
        treeWidget_RandomProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_RandomProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_RandomProperties->setAlternatingRowColors(false);
        treeWidget_RandomProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_RandomProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_RandomProperties->setAutoExpandDelay(0);
        treeWidget_RandomProperties->setIndentation(0);
        treeWidget_RandomProperties->setRootIsDecorated(false);
        treeWidget_RandomProperties->setAnimated(false);
        treeWidget_RandomProperties->setAllColumnsShowFocus(false);
        treeWidget_RandomProperties->setWordWrap(false);
        treeWidget_RandomProperties->setHeaderHidden(false);
        treeWidget_RandomProperties->header()->setVisible(true);
        treeWidget_RandomProperties->header()->setCascadingSectionResizes(false);
        treeWidget_RandomProperties->header()->setMinimumSectionSize(180);
        treeWidget_RandomProperties->header()->setDefaultSectionSize(180);
        treeWidget_RandomProperties->header()->setHighlightSections(false);
        treeWidget_RandomProperties->header()->setProperty("showSortIndicator", QVariant(false));

        verticalLayout_9->addWidget(treeWidget_RandomProperties);

        stackedWidget_LayoutPages->addWidget(RandomProperties);
        GridProperties = new QWidget();
        GridProperties->setObjectName(QString::fromUtf8("GridProperties"));
        sizePolicy4.setHeightForWidth(GridProperties->sizePolicy().hasHeightForWidth());
        GridProperties->setSizePolicy(sizePolicy4);
        GridProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_12 = new QVBoxLayout(GridProperties);
        verticalLayout_12->setSpacing(0);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QString::fromUtf8("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(0, 0, 0, 0);
        treeWidget_GridProperties = new QTreeWidget(GridProperties);
        QTreeWidgetItem *__qtreewidgetitem19 = new QTreeWidgetItem(treeWidget_GridProperties);
        __qtreewidgetitem19->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem19->setFont(1, font);
        __qtreewidgetitem19->setBackground(1, brush);
        __qtreewidgetitem19->setFont(0, font);
        __qtreewidgetitem19->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem20 = new QTreeWidgetItem(__qtreewidgetitem19);
        __qtreewidgetitem20->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem21 = new QTreeWidgetItem(__qtreewidgetitem19);
        __qtreewidgetitem21->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem21->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem22 = new QTreeWidgetItem(__qtreewidgetitem19);
        __qtreewidgetitem22->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem23 = new QTreeWidgetItem(__qtreewidgetitem19);
        __qtreewidgetitem23->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_GridProperties->setObjectName(QString::fromUtf8("treeWidget_GridProperties"));
        treeWidget_GridProperties->setEnabled(true);
        sizePolicy4.setHeightForWidth(treeWidget_GridProperties->sizePolicy().hasHeightForWidth());
        treeWidget_GridProperties->setSizePolicy(sizePolicy4);
        treeWidget_GridProperties->setMinimumSize(QSize(360, 0));
        treeWidget_GridProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_GridProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_GridProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_GridProperties->setAlternatingRowColors(false);
        treeWidget_GridProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_GridProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_GridProperties->setAutoExpandDelay(0);
        treeWidget_GridProperties->setIndentation(0);
        treeWidget_GridProperties->setRootIsDecorated(false);
        treeWidget_GridProperties->setAnimated(false);
        treeWidget_GridProperties->setAllColumnsShowFocus(false);
        treeWidget_GridProperties->setWordWrap(false);
        treeWidget_GridProperties->setHeaderHidden(false);
        treeWidget_GridProperties->header()->setVisible(true);
        treeWidget_GridProperties->header()->setCascadingSectionResizes(false);
        treeWidget_GridProperties->header()->setMinimumSectionSize(180);
        treeWidget_GridProperties->header()->setDefaultSectionSize(180);
        treeWidget_GridProperties->header()->setHighlightSections(false);

        verticalLayout_12->addWidget(treeWidget_GridProperties);

        stackedWidget_LayoutPages->addWidget(GridProperties);
        CircleProperties = new QWidget();
        CircleProperties->setObjectName(QString::fromUtf8("CircleProperties"));
        sizePolicy4.setHeightForWidth(CircleProperties->sizePolicy().hasHeightForWidth());
        CircleProperties->setSizePolicy(sizePolicy4);
        CircleProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_11 = new QVBoxLayout(CircleProperties);
        verticalLayout_11->setSpacing(0);
        verticalLayout_11->setContentsMargins(11, 11, 11, 11);
        verticalLayout_11->setObjectName(QString::fromUtf8("verticalLayout_11"));
        verticalLayout_11->setContentsMargins(0, 0, 0, 0);
        treeWidget_CircleProperties = new QTreeWidget(CircleProperties);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::NoBrush);
        QBrush brush2(QColor(0, 0, 0, 255));
        brush2.setStyle(Qt::NoBrush);
        QBrush brush3(QColor(0, 0, 0, 255));
        brush3.setStyle(Qt::NoBrush);
        QBrush brush4(QColor(0, 0, 0, 255));
        brush4.setStyle(Qt::NoBrush);
        QIcon icon2;
        QString iconThemeName = QString::fromUtf8("battery");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        QTreeWidgetItem *__qtreewidgetitem24 = new QTreeWidgetItem(treeWidget_CircleProperties);
        __qtreewidgetitem24->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem24->setBackground(1, brush);
        __qtreewidgetitem24->setFont(0, font);
        __qtreewidgetitem24->setBackground(0, brush);
        __qtreewidgetitem24->setForeground(0, brush1);
        QTreeWidgetItem *__qtreewidgetitem25 = new QTreeWidgetItem(__qtreewidgetitem24);
        __qtreewidgetitem25->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem25->setBackground(1, brush4);
        __qtreewidgetitem25->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem25->setBackground(0, brush3);
        __qtreewidgetitem25->setForeground(0, brush2);
        QTreeWidgetItem *__qtreewidgetitem26 = new QTreeWidgetItem(__qtreewidgetitem24);
        __qtreewidgetitem26->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem26->setIcon(1, icon2);
        QTreeWidgetItem *__qtreewidgetitem27 = new QTreeWidgetItem(__qtreewidgetitem24);
        __qtreewidgetitem27->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem27->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem28 = new QTreeWidgetItem(__qtreewidgetitem24);
        __qtreewidgetitem28->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_CircleProperties->setObjectName(QString::fromUtf8("treeWidget_CircleProperties"));
        treeWidget_CircleProperties->setEnabled(true);
        sizePolicy4.setHeightForWidth(treeWidget_CircleProperties->sizePolicy().hasHeightForWidth());
        treeWidget_CircleProperties->setSizePolicy(sizePolicy4);
        treeWidget_CircleProperties->setMinimumSize(QSize(360, 0));
        treeWidget_CircleProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_CircleProperties->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        treeWidget_CircleProperties->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        treeWidget_CircleProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_CircleProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_CircleProperties->setAlternatingRowColors(false);
        treeWidget_CircleProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_CircleProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_CircleProperties->setAutoExpandDelay(0);
        treeWidget_CircleProperties->setIndentation(0);
        treeWidget_CircleProperties->setRootIsDecorated(false);
        treeWidget_CircleProperties->setAnimated(false);
        treeWidget_CircleProperties->setAllColumnsShowFocus(false);
        treeWidget_CircleProperties->setWordWrap(false);
        treeWidget_CircleProperties->setHeaderHidden(false);
        treeWidget_CircleProperties->header()->setVisible(true);
        treeWidget_CircleProperties->header()->setCascadingSectionResizes(false);
        treeWidget_CircleProperties->header()->setMinimumSectionSize(180);
        treeWidget_CircleProperties->header()->setDefaultSectionSize(180);
        treeWidget_CircleProperties->header()->setHighlightSections(false);

        verticalLayout_11->addWidget(treeWidget_CircleProperties);

        stackedWidget_LayoutPages->addWidget(CircleProperties);
        MonogonProperties = new QWidget();
        MonogonProperties->setObjectName(QString::fromUtf8("MonogonProperties"));
        sizePolicy4.setHeightForWidth(MonogonProperties->sizePolicy().hasHeightForWidth());
        MonogonProperties->setSizePolicy(sizePolicy4);
        MonogonProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_13 = new QVBoxLayout(MonogonProperties);
        verticalLayout_13->setSpacing(0);
        verticalLayout_13->setContentsMargins(11, 11, 11, 11);
        verticalLayout_13->setObjectName(QString::fromUtf8("verticalLayout_13"));
        verticalLayout_13->setContentsMargins(0, 0, 0, 0);
        treeWidget_MonogonProperties = new QTreeWidget(MonogonProperties);
        QBrush brush5(QColor(0, 0, 0, 255));
        brush5.setStyle(Qt::NoBrush);
        QBrush brush6(QColor(240, 240, 240, 255));
        brush6.setStyle(Qt::NoBrush);
        QBrush brush7(QColor(240, 240, 240, 255));
        brush7.setStyle(Qt::NoBrush);
        QTreeWidgetItem *__qtreewidgetitem29 = new QTreeWidgetItem(treeWidget_MonogonProperties);
        __qtreewidgetitem29->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem29->setBackground(1, brush);
        __qtreewidgetitem29->setFont(0, font);
        __qtreewidgetitem29->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem30 = new QTreeWidgetItem(__qtreewidgetitem29);
        __qtreewidgetitem30->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem30->setCheckState(1, Qt::Unchecked);
        __qtreewidgetitem30->setBackground(1, brush7);
        __qtreewidgetitem30->setBackground(0, brush6);
        __qtreewidgetitem30->setForeground(0, brush5);
        treeWidget_MonogonProperties->setObjectName(QString::fromUtf8("treeWidget_MonogonProperties"));
        treeWidget_MonogonProperties->setEnabled(true);
        sizePolicy4.setHeightForWidth(treeWidget_MonogonProperties->sizePolicy().hasHeightForWidth());
        treeWidget_MonogonProperties->setSizePolicy(sizePolicy4);
        treeWidget_MonogonProperties->setMinimumSize(QSize(360, 0));
        treeWidget_MonogonProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_MonogonProperties->setAutoScrollMargin(16);
        treeWidget_MonogonProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_MonogonProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_MonogonProperties->setAlternatingRowColors(false);
        treeWidget_MonogonProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_MonogonProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_MonogonProperties->setAutoExpandDelay(0);
        treeWidget_MonogonProperties->setIndentation(0);
        treeWidget_MonogonProperties->setRootIsDecorated(false);
        treeWidget_MonogonProperties->setAnimated(false);
        treeWidget_MonogonProperties->setAllColumnsShowFocus(false);
        treeWidget_MonogonProperties->setWordWrap(false);
        treeWidget_MonogonProperties->setHeaderHidden(false);
        treeWidget_MonogonProperties->header()->setVisible(true);
        treeWidget_MonogonProperties->header()->setCascadingSectionResizes(false);
        treeWidget_MonogonProperties->header()->setMinimumSectionSize(180);
        treeWidget_MonogonProperties->header()->setDefaultSectionSize(180);
        treeWidget_MonogonProperties->header()->setHighlightSections(false);

        verticalLayout_13->addWidget(treeWidget_MonogonProperties);

        stackedWidget_LayoutPages->addWidget(MonogonProperties);
        ForceDirectedProperties = new QWidget();
        ForceDirectedProperties->setObjectName(QString::fromUtf8("ForceDirectedProperties"));
        sizePolicy4.setHeightForWidth(ForceDirectedProperties->sizePolicy().hasHeightForWidth());
        ForceDirectedProperties->setSizePolicy(sizePolicy4);
        ForceDirectedProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_7 = new QVBoxLayout(ForceDirectedProperties);
        verticalLayout_7->setSpacing(0);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        verticalLayout_7->setContentsMargins(0, 0, 0, 0);
        treeWidget_FDProperties = new QTreeWidget(ForceDirectedProperties);
        QBrush brush8(QColor(200, 200, 200, 255));
        brush8.setStyle(Qt::NoBrush);
        QBrush brush9(QColor(0, 0, 0, 255));
        brush9.setStyle(Qt::NoBrush);
        QBrush brush10(QColor(0, 0, 0, 255));
        brush10.setStyle(Qt::NoBrush);
        QBrush brush11(QColor(240, 240, 240, 255));
        brush11.setStyle(Qt::NoBrush);
        QBrush brush12(QColor(240, 240, 240, 255));
        brush12.setStyle(Qt::NoBrush);
        QBrush brush13(QColor(240, 240, 240, 255));
        brush13.setStyle(Qt::NoBrush);
        QFont font1;
        font1.setUnderline(false);
        QTreeWidgetItem *__qtreewidgetitem31 = new QTreeWidgetItem(treeWidget_FDProperties);
        __qtreewidgetitem31->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem31->setBackground(1, brush);
        __qtreewidgetitem31->setForeground(1, brush8);
        __qtreewidgetitem31->setFont(0, font);
        __qtreewidgetitem31->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem32 = new QTreeWidgetItem(__qtreewidgetitem31);
        __qtreewidgetitem32->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem32->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        QTreeWidgetItem *__qtreewidgetitem33 = new QTreeWidgetItem(__qtreewidgetitem31);
        __qtreewidgetitem33->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem33->setBackground(1, brush11);
        __qtreewidgetitem33->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem33->setBackground(0, brush10);
        __qtreewidgetitem33->setForeground(0, brush9);
        QTreeWidgetItem *__qtreewidgetitem34 = new QTreeWidgetItem(__qtreewidgetitem31);
        __qtreewidgetitem34->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem34->setBackground(1, brush13);
        __qtreewidgetitem34->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem34->setBackground(0, brush12);
        QTreeWidgetItem *__qtreewidgetitem35 = new QTreeWidgetItem(__qtreewidgetitem31);
        __qtreewidgetitem35->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem35->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem35->setFont(0, font1);
        QTreeWidgetItem *__qtreewidgetitem36 = new QTreeWidgetItem(__qtreewidgetitem31);
        __qtreewidgetitem36->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem37 = new QTreeWidgetItem(__qtreewidgetitem31);
        __qtreewidgetitem37->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_FDProperties->setObjectName(QString::fromUtf8("treeWidget_FDProperties"));
        treeWidget_FDProperties->setEnabled(true);
        sizePolicy4.setHeightForWidth(treeWidget_FDProperties->sizePolicy().hasHeightForWidth());
        treeWidget_FDProperties->setSizePolicy(sizePolicy4);
        treeWidget_FDProperties->setMinimumSize(QSize(360, 0));
        treeWidget_FDProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_FDProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_FDProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_FDProperties->setAlternatingRowColors(false);
        treeWidget_FDProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_FDProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_FDProperties->setAutoExpandDelay(0);
        treeWidget_FDProperties->setIndentation(0);
        treeWidget_FDProperties->setRootIsDecorated(false);
        treeWidget_FDProperties->setAnimated(false);
        treeWidget_FDProperties->setAllColumnsShowFocus(false);
        treeWidget_FDProperties->setWordWrap(false);
        treeWidget_FDProperties->setHeaderHidden(false);
        treeWidget_FDProperties->header()->setVisible(true);
        treeWidget_FDProperties->header()->setCascadingSectionResizes(false);
        treeWidget_FDProperties->header()->setMinimumSectionSize(180);
        treeWidget_FDProperties->header()->setDefaultSectionSize(180);
        treeWidget_FDProperties->header()->setHighlightSections(false);

        verticalLayout_7->addWidget(treeWidget_FDProperties);

        stackedWidget_LayoutPages->addWidget(ForceDirectedProperties);
        OptimizationProperties = new QWidget();
        OptimizationProperties->setObjectName(QString::fromUtf8("OptimizationProperties"));
        sizePolicy4.setHeightForWidth(OptimizationProperties->sizePolicy().hasHeightForWidth());
        OptimizationProperties->setSizePolicy(sizePolicy4);
        OptimizationProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_6 = new QVBoxLayout(OptimizationProperties);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        treeWidget_OptimizationProperties = new QTreeWidget(OptimizationProperties);
        QBrush brush14(QColor(0, 0, 0, 255));
        brush14.setStyle(Qt::NoBrush);
        QBrush brush15(QColor(0, 0, 0, 255));
        brush15.setStyle(Qt::NoBrush);
        QBrush brush16(QColor(0, 0, 0, 255));
        brush16.setStyle(Qt::NoBrush);
        QBrush brush17(QColor(240, 240, 240, 255));
        brush17.setStyle(Qt::NoBrush);
        QBrush brush18(QColor(240, 240, 240, 255));
        brush18.setStyle(Qt::NoBrush);
        QBrush brush19(QColor(240, 240, 240, 255));
        brush19.setStyle(Qt::NoBrush);
        QBrush brush20(QColor(240, 240, 240, 255));
        brush20.setStyle(Qt::NoBrush);
        QBrush brush21(QColor(240, 240, 240, 255));
        brush21.setStyle(Qt::NoBrush);
        QBrush brush22(QColor(240, 240, 240, 255));
        brush22.setStyle(Qt::NoBrush);
        QBrush brush23(QColor(240, 240, 240, 255));
        brush23.setStyle(Qt::NoBrush);
        QBrush brush24(QColor(0, 0, 0, 255));
        brush24.setStyle(Qt::NoBrush);
        QBrush brush25(QColor(240, 240, 240, 255));
        brush25.setStyle(Qt::NoBrush);
        QTreeWidgetItem *__qtreewidgetitem38 = new QTreeWidgetItem(treeWidget_OptimizationProperties);
        __qtreewidgetitem38->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem38->setBackground(1, brush);
        __qtreewidgetitem38->setFont(0, font);
        __qtreewidgetitem38->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem39 = new QTreeWidgetItem(__qtreewidgetitem38);
        __qtreewidgetitem39->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem39->setCheckState(1, Qt::Checked);
        __qtreewidgetitem39->setBackground(0, brush14);
        QTreeWidgetItem *__qtreewidgetitem40 = new QTreeWidgetItem(__qtreewidgetitem38);
        __qtreewidgetitem40->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem40->setCheckState(1, Qt::Checked);
        __qtreewidgetitem40->setBackground(0, brush15);
        QTreeWidgetItem *__qtreewidgetitem41 = new QTreeWidgetItem(treeWidget_OptimizationProperties);
        __qtreewidgetitem41->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem41->setBackground(1, brush);
        __qtreewidgetitem41->setFont(0, font);
        __qtreewidgetitem41->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem42 = new QTreeWidgetItem(__qtreewidgetitem41);
        __qtreewidgetitem42->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem43 = new QTreeWidgetItem(__qtreewidgetitem41);
        __qtreewidgetitem43->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem43->setBackground(1, brush17);
        __qtreewidgetitem43->setBackground(0, brush16);
        QTreeWidgetItem *__qtreewidgetitem44 = new QTreeWidgetItem(__qtreewidgetitem41);
        __qtreewidgetitem44->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem45 = new QTreeWidgetItem(__qtreewidgetitem41);
        __qtreewidgetitem45->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem45->setBackground(1, brush19);
        __qtreewidgetitem45->setBackground(0, brush18);
        QTreeWidgetItem *__qtreewidgetitem46 = new QTreeWidgetItem(__qtreewidgetitem41);
        __qtreewidgetitem46->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem47 = new QTreeWidgetItem(__qtreewidgetitem41);
        __qtreewidgetitem47->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem47->setBackground(1, brush21);
        __qtreewidgetitem47->setBackground(0, brush20);
        QTreeWidgetItem *__qtreewidgetitem48 = new QTreeWidgetItem(treeWidget_OptimizationProperties);
        __qtreewidgetitem48->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem48->setBackground(1, brush);
        __qtreewidgetitem48->setFont(0, font);
        __qtreewidgetitem48->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem49 = new QTreeWidgetItem(__qtreewidgetitem48);
        __qtreewidgetitem49->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem49->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        QTreeWidgetItem *__qtreewidgetitem50 = new QTreeWidgetItem(__qtreewidgetitem48);
        __qtreewidgetitem50->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem50->setBackground(1, brush23);
        __qtreewidgetitem50->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem50->setBackground(0, brush22);
        QTreeWidgetItem *__qtreewidgetitem51 = new QTreeWidgetItem(__qtreewidgetitem48);
        __qtreewidgetitem51->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem51->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem51->setFont(0, font1);
        QTreeWidgetItem *__qtreewidgetitem52 = new QTreeWidgetItem(__qtreewidgetitem48);
        __qtreewidgetitem52->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        __qtreewidgetitem52->setBackground(1, brush25);
        __qtreewidgetitem52->setTextAlignment(0, Qt::AlignLeading|Qt::AlignVCenter);
        __qtreewidgetitem52->setBackground(0, brush24);
        QTreeWidgetItem *__qtreewidgetitem53 = new QTreeWidgetItem(treeWidget_OptimizationProperties);
        __qtreewidgetitem53->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem53->setBackground(1, brush);
        __qtreewidgetitem53->setFont(0, font);
        __qtreewidgetitem53->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem54 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem54->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem55 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem55->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem56 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem56->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem57 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem57->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem58 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem58->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem59 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem59->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem60 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem60->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem61 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem61->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem62 = new QTreeWidgetItem(__qtreewidgetitem53);
        __qtreewidgetitem62->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_OptimizationProperties->setObjectName(QString::fromUtf8("treeWidget_OptimizationProperties"));
        treeWidget_OptimizationProperties->setEnabled(true);
        sizePolicy4.setHeightForWidth(treeWidget_OptimizationProperties->sizePolicy().hasHeightForWidth());
        treeWidget_OptimizationProperties->setSizePolicy(sizePolicy4);
        treeWidget_OptimizationProperties->setMinimumSize(QSize(360, 0));
        treeWidget_OptimizationProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_OptimizationProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_OptimizationProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_OptimizationProperties->setAlternatingRowColors(false);
        treeWidget_OptimizationProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_OptimizationProperties->setSelectionBehavior(QAbstractItemView::SelectRows);
        treeWidget_OptimizationProperties->setAutoExpandDelay(0);
        treeWidget_OptimizationProperties->setIndentation(0);
        treeWidget_OptimizationProperties->setRootIsDecorated(false);
        treeWidget_OptimizationProperties->setAnimated(false);
        treeWidget_OptimizationProperties->setAllColumnsShowFocus(false);
        treeWidget_OptimizationProperties->setWordWrap(false);
        treeWidget_OptimizationProperties->setHeaderHidden(false);
        treeWidget_OptimizationProperties->header()->setVisible(true);
        treeWidget_OptimizationProperties->header()->setCascadingSectionResizes(false);
        treeWidget_OptimizationProperties->header()->setMinimumSectionSize(180);
        treeWidget_OptimizationProperties->header()->setDefaultSectionSize(180);
        treeWidget_OptimizationProperties->header()->setHighlightSections(false);

        verticalLayout_6->addWidget(treeWidget_OptimizationProperties);

        stackedWidget_LayoutPages->addWidget(OptimizationProperties);
        EnergyProperties = new QWidget();
        EnergyProperties->setObjectName(QString::fromUtf8("EnergyProperties"));
        sizePolicy4.setHeightForWidth(EnergyProperties->sizePolicy().hasHeightForWidth());
        EnergyProperties->setSizePolicy(sizePolicy4);
        EnergyProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_19 = new QVBoxLayout(EnergyProperties);
        verticalLayout_19->setSpacing(0);
        verticalLayout_19->setContentsMargins(11, 11, 11, 11);
        verticalLayout_19->setObjectName(QString::fromUtf8("verticalLayout_19"));
        verticalLayout_19->setContentsMargins(0, 0, 0, 0);
        treeWidget_EnergyProperties = new QTreeWidget(EnergyProperties);
        QTreeWidgetItem *__qtreewidgetitem63 = new QTreeWidgetItem(treeWidget_EnergyProperties);
        __qtreewidgetitem63->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem63->setFont(1, font);
        __qtreewidgetitem63->setBackground(1, brush);
        __qtreewidgetitem63->setFont(0, font);
        __qtreewidgetitem63->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem64 = new QTreeWidgetItem(__qtreewidgetitem63);
        __qtreewidgetitem64->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem64->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem65 = new QTreeWidgetItem(__qtreewidgetitem63);
        __qtreewidgetitem65->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem65->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem66 = new QTreeWidgetItem(__qtreewidgetitem63);
        __qtreewidgetitem66->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem66->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem67 = new QTreeWidgetItem(__qtreewidgetitem63);
        __qtreewidgetitem67->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem67->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem68 = new QTreeWidgetItem(__qtreewidgetitem63);
        __qtreewidgetitem68->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem68->setCheckState(1, Qt::Checked);
        QTreeWidgetItem *__qtreewidgetitem69 = new QTreeWidgetItem(__qtreewidgetitem63);
        __qtreewidgetitem69->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem69->setCheckState(1, Qt::Checked);
        treeWidget_EnergyProperties->setObjectName(QString::fromUtf8("treeWidget_EnergyProperties"));
        sizePolicy4.setHeightForWidth(treeWidget_EnergyProperties->sizePolicy().hasHeightForWidth());
        treeWidget_EnergyProperties->setSizePolicy(sizePolicy4);
        treeWidget_EnergyProperties->setMinimumSize(QSize(360, 0));
        treeWidget_EnergyProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_EnergyProperties->setFrameShadow(QFrame::Plain);
        treeWidget_EnergyProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_EnergyProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_EnergyProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_EnergyProperties->setAutoExpandDelay(0);
        treeWidget_EnergyProperties->setIndentation(0);
        treeWidget_EnergyProperties->setRootIsDecorated(false);
        treeWidget_EnergyProperties->setColumnCount(2);
        treeWidget_EnergyProperties->header()->setMinimumSectionSize(180);
        treeWidget_EnergyProperties->header()->setDefaultSectionSize(180);

        verticalLayout_19->addWidget(treeWidget_EnergyProperties);

        stackedWidget_LayoutPages->addWidget(EnergyProperties);
        KonigProperties = new QWidget();
        KonigProperties->setObjectName(QString::fromUtf8("KonigProperties"));
        sizePolicy4.setHeightForWidth(KonigProperties->sizePolicy().hasHeightForWidth());
        KonigProperties->setSizePolicy(sizePolicy4);
        KonigProperties->setMinimumSize(QSize(360, 0));
        verticalLayout_15 = new QVBoxLayout(KonigProperties);
        verticalLayout_15->setSpacing(0);
        verticalLayout_15->setContentsMargins(11, 11, 11, 11);
        verticalLayout_15->setObjectName(QString::fromUtf8("verticalLayout_15"));
        verticalLayout_15->setContentsMargins(0, 0, 0, 0);
        treeWidget_KonigProperties = new QTreeWidget(KonigProperties);
        QTreeWidgetItem *__qtreewidgetitem70 = new QTreeWidgetItem(treeWidget_KonigProperties);
        __qtreewidgetitem70->setFlags(Qt::ItemIsEnabled);
        __qtreewidgetitem70->setBackground(1, brush);
        __qtreewidgetitem70->setFont(0, font);
        __qtreewidgetitem70->setBackground(0, brush);
        QTreeWidgetItem *__qtreewidgetitem71 = new QTreeWidgetItem(__qtreewidgetitem70);
        __qtreewidgetitem71->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        QTreeWidgetItem *__qtreewidgetitem72 = new QTreeWidgetItem(__qtreewidgetitem70);
        __qtreewidgetitem72->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem72->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem73 = new QTreeWidgetItem(__qtreewidgetitem70);
        __qtreewidgetitem73->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        __qtreewidgetitem73->setCheckState(1, Qt::Unchecked);
        QTreeWidgetItem *__qtreewidgetitem74 = new QTreeWidgetItem(__qtreewidgetitem70);
        __qtreewidgetitem74->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
        treeWidget_KonigProperties->setObjectName(QString::fromUtf8("treeWidget_KonigProperties"));
        sizePolicy4.setHeightForWidth(treeWidget_KonigProperties->sizePolicy().hasHeightForWidth());
        treeWidget_KonigProperties->setSizePolicy(sizePolicy4);
        treeWidget_KonigProperties->setMinimumSize(QSize(360, 0));
        treeWidget_KonigProperties->setFrameShape(QFrame::NoFrame);
        treeWidget_KonigProperties->setEditTriggers(QAbstractItemView::DoubleClicked);
        treeWidget_KonigProperties->setProperty("showDropIndicator", QVariant(false));
        treeWidget_KonigProperties->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget_KonigProperties->setAutoExpandDelay(0);
        treeWidget_KonigProperties->setIndentation(0);
        treeWidget_KonigProperties->setRootIsDecorated(false);
        treeWidget_KonigProperties->setColumnCount(2);
        treeWidget_KonigProperties->header()->setMinimumSectionSize(180);
        treeWidget_KonigProperties->header()->setDefaultSectionSize(180);

        verticalLayout_15->addWidget(treeWidget_KonigProperties);

        stackedWidget_LayoutPages->addWidget(KonigProperties);

        verticalLayout_8->addWidget(stackedWidget_LayoutPages);

        tabWidget->addTab(LayoutTab, QString());
        MultiscaleTab = new QWidget();
        MultiscaleTab->setObjectName(QString::fromUtf8("MultiscaleTab"));
        MultiscaleTab->setAutoFillBackground(true);
        verticalLayout = new QVBoxLayout(MultiscaleTab);
        verticalLayout->setSpacing(4);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 2, 6, 2);
        groupBox_12 = new QGroupBox(MultiscaleTab);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        gridLayout_14 = new QGridLayout(groupBox_12);
        gridLayout_14->setSpacing(6);
        gridLayout_14->setContentsMargins(11, 11, 11, 11);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        gridLayout_14->setVerticalSpacing(1);
        gridLayout_14->setContentsMargins(1, 1, 1, 1);
        label_18 = new QLabel(groupBox_12);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_14->addWidget(label_18, 0, 0, 1, 1);

        label_20 = new QLabel(groupBox_12);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_14->addWidget(label_20, 2, 0, 1, 1);

        label_19 = new QLabel(groupBox_12);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_14->addWidget(label_19, 1, 0, 1, 1);

        doubleSpinBox_Alpha = new QDoubleSpinBox(groupBox_12);
        doubleSpinBox_Alpha->setObjectName(QString::fromUtf8("doubleSpinBox_Alpha"));
        doubleSpinBox_Alpha->setDecimals(4);
        doubleSpinBox_Alpha->setMinimum(0.000100000000000);
        doubleSpinBox_Alpha->setMaximum(2.000000000000000);
        doubleSpinBox_Alpha->setSingleStep(0.100000000000000);
        doubleSpinBox_Alpha->setValue(0.500000000000000);

        gridLayout_14->addWidget(doubleSpinBox_Alpha, 0, 1, 1, 1);

        doubleSpinBox_Beta = new QDoubleSpinBox(groupBox_12);
        doubleSpinBox_Beta->setObjectName(QString::fromUtf8("doubleSpinBox_Beta"));
        doubleSpinBox_Beta->setDecimals(4);
        doubleSpinBox_Beta->setMinimum(0.000100000000000);
        doubleSpinBox_Beta->setMaximum(2.000000000000000);
        doubleSpinBox_Beta->setSingleStep(0.100000000000000);
        doubleSpinBox_Beta->setValue(2.000000000000000);

        gridLayout_14->addWidget(doubleSpinBox_Beta, 1, 1, 1, 1);

        doubleSpinBox_Gamma = new QDoubleSpinBox(groupBox_12);
        doubleSpinBox_Gamma->setObjectName(QString::fromUtf8("doubleSpinBox_Gamma"));
        doubleSpinBox_Gamma->setDecimals(4);
        doubleSpinBox_Gamma->setMinimum(0.000100000000000);
        doubleSpinBox_Gamma->setMaximum(2.000000000000000);
        doubleSpinBox_Gamma->setSingleStep(0.100000000000000);
        doubleSpinBox_Gamma->setValue(1.500000000000000);

        gridLayout_14->addWidget(doubleSpinBox_Gamma, 2, 1, 1, 1);


        verticalLayout->addWidget(groupBox_12);

        Options = new QGroupBox(MultiscaleTab);
        Options->setObjectName(QString::fromUtf8("Options"));
        gridLayout_3 = new QGridLayout(Options);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(1, 1, 1, 1);
        checkBox_OperationNodeMerge = new QCheckBox(Options);
        checkBox_OperationNodeMerge->setObjectName(QString::fromUtf8("checkBox_OperationNodeMerge"));
        checkBox_OperationNodeMerge->setChecked(true);
        checkBox_OperationNodeMerge->setAutoExclusive(false);

        gridLayout_3->addWidget(checkBox_OperationNodeMerge, 2, 0, 1, 1);

        checkBox_OperationNodeRemove = new QCheckBox(Options);
        checkBox_OperationNodeRemove->setObjectName(QString::fromUtf8("checkBox_OperationNodeRemove"));
        checkBox_OperationNodeRemove->setChecked(true);
        checkBox_OperationNodeRemove->setAutoExclusive(false);

        gridLayout_3->addWidget(checkBox_OperationNodeRemove, 1, 0, 1, 1);

        checkBox_OperationEdgeRemove = new QCheckBox(Options);
        checkBox_OperationEdgeRemove->setObjectName(QString::fromUtf8("checkBox_OperationEdgeRemove"));
        checkBox_OperationEdgeRemove->setChecked(true);
        checkBox_OperationEdgeRemove->setAutoExclusive(false);

        gridLayout_3->addWidget(checkBox_OperationEdgeRemove, 1, 1, 1, 1);

        checkBox_OperationEdgeMerge = new QCheckBox(Options);
        checkBox_OperationEdgeMerge->setObjectName(QString::fromUtf8("checkBox_OperationEdgeMerge"));
        checkBox_OperationEdgeMerge->setChecked(true);
        checkBox_OperationEdgeMerge->setAutoExclusive(false);

        gridLayout_3->addWidget(checkBox_OperationEdgeMerge, 2, 1, 1, 1);

        pushButton_GenerateAtomicOperations = new QPushButton(Options);
        pushButton_GenerateAtomicOperations->setObjectName(QString::fromUtf8("pushButton_GenerateAtomicOperations"));

        gridLayout_3->addWidget(pushButton_GenerateAtomicOperations, 6, 0, 1, 2);


        verticalLayout->addWidget(Options);

        groupBox = new QGroupBox(MultiscaleTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(1);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(1, 1, 1, 1);
        groupBox_9 = new QGroupBox(groupBox);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_12 = new QGridLayout(groupBox_9);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setContentsMargins(1, 1, 1, 1);
        checkBox_PreserveConnections = new QCheckBox(groupBox_9);
        checkBox_PreserveConnections->setObjectName(QString::fromUtf8("checkBox_PreserveConnections"));
        checkBox_PreserveConnections->setChecked(true);

        gridLayout_12->addWidget(checkBox_PreserveConnections, 0, 0, 1, 1);

        checkBox_PreserveBranches = new QCheckBox(groupBox_9);
        checkBox_PreserveBranches->setObjectName(QString::fromUtf8("checkBox_PreserveBranches"));
        checkBox_PreserveBranches->setChecked(true);

        gridLayout_12->addWidget(checkBox_PreserveBranches, 0, 1, 1, 1);

        checkBox_PruneOnly = new QCheckBox(groupBox_9);
        checkBox_PruneOnly->setObjectName(QString::fromUtf8("checkBox_PruneOnly"));
        checkBox_PruneOnly->setChecked(true);

        gridLayout_12->addWidget(checkBox_PruneOnly, 0, 2, 1, 1);

        spinBox_MaxEdgeMergeSize = new QSpinBox(groupBox_9);
        spinBox_MaxEdgeMergeSize->setObjectName(QString::fromUtf8("spinBox_MaxEdgeMergeSize"));
        spinBox_MaxEdgeMergeSize->setMinimum(1);
        spinBox_MaxEdgeMergeSize->setMaximum(1000);
        spinBox_MaxEdgeMergeSize->setValue(10);

        gridLayout_12->addWidget(spinBox_MaxEdgeMergeSize, 1, 2, 1, 1);

        label_28 = new QLabel(groupBox_9);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_12->addWidget(label_28, 1, 0, 1, 2);


        verticalLayout_3->addWidget(groupBox_9);

        groupBox_13 = new QGroupBox(groupBox);
        groupBox_13->setObjectName(QString::fromUtf8("groupBox_13"));
        gridLayout_15 = new QGridLayout(groupBox_13);
        gridLayout_15->setSpacing(6);
        gridLayout_15->setContentsMargins(11, 11, 11, 11);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        gridLayout_15->setVerticalSpacing(1);
        gridLayout_15->setContentsMargins(1, 1, 1, 1);
        label_23 = new QLabel(groupBox_13);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_15->addWidget(label_23, 3, 0, 1, 1);

        spinBox_MinNodes = new QSpinBox(groupBox_13);
        spinBox_MinNodes->setObjectName(QString::fromUtf8("spinBox_MinNodes"));
        spinBox_MinNodes->setMinimum(1);
        spinBox_MinNodes->setMaximum(1999);
        spinBox_MinNodes->setValue(1);

        gridLayout_15->addWidget(spinBox_MinNodes, 3, 1, 1, 1);

        spinBox_MaxOps = new QSpinBox(groupBox_13);
        spinBox_MaxOps->setObjectName(QString::fromUtf8("spinBox_MaxOps"));
        spinBox_MaxOps->setMinimum(0);
        spinBox_MaxOps->setMaximum(2147483647);
        spinBox_MaxOps->setValue(0);

        gridLayout_15->addWidget(spinBox_MaxOps, 0, 1, 1, 1);

        spinBox_MinEdges = new QSpinBox(groupBox_13);
        spinBox_MinEdges->setObjectName(QString::fromUtf8("spinBox_MinEdges"));
        spinBox_MinEdges->setMinimum(1);
        spinBox_MinEdges->setMaximum(1999);
        spinBox_MinEdges->setValue(1);

        gridLayout_15->addWidget(spinBox_MinEdges, 2, 1, 1, 1);

        label_22 = new QLabel(groupBox_13);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_15->addWidget(label_22, 2, 0, 1, 1);

        label_21 = new QLabel(groupBox_13);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_15->addWidget(label_21, 0, 0, 1, 1);

        doubleSpinBox_PriorityThreshold = new QDoubleSpinBox(groupBox_13);
        doubleSpinBox_PriorityThreshold->setObjectName(QString::fromUtf8("doubleSpinBox_PriorityThreshold"));
        doubleSpinBox_PriorityThreshold->setMinimum(-1000.000000000000000);
        doubleSpinBox_PriorityThreshold->setMaximum(1000.000000000000000);
        doubleSpinBox_PriorityThreshold->setSingleStep(0.010000000000000);

        gridLayout_15->addWidget(doubleSpinBox_PriorityThreshold, 1, 1, 1, 1);

        label_34 = new QLabel(groupBox_13);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_15->addWidget(label_34, 1, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_13);

        checkBox_optimizeInversions = new QCheckBox(groupBox);
        checkBox_optimizeInversions->setObjectName(QString::fromUtf8("checkBox_optimizeInversions"));
        checkBox_optimizeInversions->setChecked(true);

        verticalLayout_3->addWidget(checkBox_optimizeInversions);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(1);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        pushButton_InvertOperation = new QPushButton(groupBox);
        pushButton_InvertOperation->setObjectName(QString::fromUtf8("pushButton_InvertOperation"));

        horizontalLayout_3->addWidget(pushButton_InvertOperation);

        pushButton_ApplyOperation = new QPushButton(groupBox);
        pushButton_ApplyOperation->setObjectName(QString::fromUtf8("pushButton_ApplyOperation"));

        horizontalLayout_3->addWidget(pushButton_ApplyOperation);


        verticalLayout_3->addLayout(horizontalLayout_3);

        pushButton_ApplyAllOperations = new QPushButton(groupBox);
        pushButton_ApplyAllOperations->setObjectName(QString::fromUtf8("pushButton_ApplyAllOperations"));

        verticalLayout_3->addWidget(pushButton_ApplyAllOperations);

        pushButton_InvertAllOperations = new QPushButton(groupBox);
        pushButton_InvertAllOperations->setObjectName(QString::fromUtf8("pushButton_InvertAllOperations"));

        verticalLayout_3->addWidget(pushButton_InvertAllOperations);

        pushButton_OptimizeFootprint = new QPushButton(groupBox);
        pushButton_OptimizeFootprint->setObjectName(QString::fromUtf8("pushButton_OptimizeFootprint"));

        verticalLayout_3->addWidget(pushButton_OptimizeFootprint);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(MultiscaleTab, QString());
        DecompositionTab = new QWidget();
        DecompositionTab->setObjectName(QString::fromUtf8("DecompositionTab"));
        verticalLayout_20 = new QVBoxLayout(DecompositionTab);
        verticalLayout_20->setSpacing(6);
        verticalLayout_20->setContentsMargins(11, 11, 11, 11);
        verticalLayout_20->setObjectName(QString::fromUtf8("verticalLayout_20"));
        groupBox_CycleDecomp = new QGroupBox(DecompositionTab);
        groupBox_CycleDecomp->setObjectName(QString::fromUtf8("groupBox_CycleDecomp"));
        gridLayout_5 = new QGridLayout(groupBox_CycleDecomp);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(1, 1, 1, 1);
        pushButton_HighlightAllTrees = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightAllTrees->setObjectName(QString::fromUtf8("pushButton_HighlightAllTrees"));

        gridLayout_5->addWidget(pushButton_HighlightAllTrees, 2, 2, 1, 1);

        pushButton_ResetHighlights = new QPushButton(groupBox_CycleDecomp);
        pushButton_ResetHighlights->setObjectName(QString::fromUtf8("pushButton_ResetHighlights"));

        gridLayout_5->addWidget(pushButton_ResetHighlights, 12, 1, 1, 2);

        pushButton_HighlightAllBiComps = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightAllBiComps->setObjectName(QString::fromUtf8("pushButton_HighlightAllBiComps"));

        gridLayout_5->addWidget(pushButton_HighlightAllBiComps, 1, 2, 1, 1);

        pushButton_HighlightNextTree = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightNextTree->setObjectName(QString::fromUtf8("pushButton_HighlightNextTree"));

        gridLayout_5->addWidget(pushButton_HighlightNextTree, 2, 1, 1, 1);

        pushButton_HighlightAllCycles = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightAllCycles->setObjectName(QString::fromUtf8("pushButton_HighlightAllCycles"));

        gridLayout_5->addWidget(pushButton_HighlightAllCycles, 3, 2, 1, 1);

        pushButton_HighlightAllClusters = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightAllClusters->setObjectName(QString::fromUtf8("pushButton_HighlightAllClusters"));

        gridLayout_5->addWidget(pushButton_HighlightAllClusters, 4, 2, 1, 1);

        pushButton_CycleDecompose = new QPushButton(groupBox_CycleDecomp);
        pushButton_CycleDecompose->setObjectName(QString::fromUtf8("pushButton_CycleDecompose"));

        gridLayout_5->addWidget(pushButton_CycleDecompose, 0, 1, 1, 2);

        pushButton_HighlightNextCluster = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightNextCluster->setObjectName(QString::fromUtf8("pushButton_HighlightNextCluster"));

        gridLayout_5->addWidget(pushButton_HighlightNextCluster, 4, 1, 1, 1);

        pushButton_HighlightNextBiComp = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightNextBiComp->setObjectName(QString::fromUtf8("pushButton_HighlightNextBiComp"));

        gridLayout_5->addWidget(pushButton_HighlightNextBiComp, 1, 1, 1, 1);

        pushButton_HighlightNextCycle = new QPushButton(groupBox_CycleDecomp);
        pushButton_HighlightNextCycle->setObjectName(QString::fromUtf8("pushButton_HighlightNextCycle"));

        gridLayout_5->addWidget(pushButton_HighlightNextCycle, 3, 1, 1, 1);


        verticalLayout_20->addWidget(groupBox_CycleDecomp);

        groupBox_15 = new QGroupBox(DecompositionTab);
        groupBox_15->setObjectName(QString::fromUtf8("groupBox_15"));
        gridLayout_9 = new QGridLayout(groupBox_15);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(1, 1, 1, 1);
        pushButton_SimpAllTrees = new QPushButton(groupBox_15);
        pushButton_SimpAllTrees->setObjectName(QString::fromUtf8("pushButton_SimpAllTrees"));

        gridLayout_9->addWidget(pushButton_SimpAllTrees, 3, 1, 1, 1);

        pushButton_RecordBipartOps = new QPushButton(groupBox_15);
        pushButton_RecordBipartOps->setObjectName(QString::fromUtf8("pushButton_RecordBipartOps"));

        gridLayout_9->addWidget(pushButton_RecordBipartOps, 0, 0, 1, 2);

        pushButton_SimpAllBicomps = new QPushButton(groupBox_15);
        pushButton_SimpAllBicomps->setObjectName(QString::fromUtf8("pushButton_SimpAllBicomps"));

        gridLayout_9->addWidget(pushButton_SimpAllBicomps, 2, 1, 1, 1);

        pushButton_ApplyNextClusterOp = new QPushButton(groupBox_15);
        pushButton_ApplyNextClusterOp->setObjectName(QString::fromUtf8("pushButton_ApplyNextClusterOp"));

        gridLayout_9->addWidget(pushButton_ApplyNextClusterOp, 1, 0, 1, 1);

        pushButton_ApplyNextTreeOp = new QPushButton(groupBox_15);
        pushButton_ApplyNextTreeOp->setObjectName(QString::fromUtf8("pushButton_ApplyNextTreeOp"));

        gridLayout_9->addWidget(pushButton_ApplyNextTreeOp, 3, 0, 1, 1);

        pushButton_ApplyNextBicompOp = new QPushButton(groupBox_15);
        pushButton_ApplyNextBicompOp->setObjectName(QString::fromUtf8("pushButton_ApplyNextBicompOp"));

        gridLayout_9->addWidget(pushButton_ApplyNextBicompOp, 2, 0, 1, 1);

        pushButton_SimpAllClusters = new QPushButton(groupBox_15);
        pushButton_SimpAllClusters->setObjectName(QString::fromUtf8("pushButton_SimpAllClusters"));

        gridLayout_9->addWidget(pushButton_SimpAllClusters, 1, 1, 1, 1);

        pushButton_SaveBipartOps = new QPushButton(groupBox_15);
        pushButton_SaveBipartOps->setObjectName(QString::fromUtf8("pushButton_SaveBipartOps"));

        gridLayout_9->addWidget(pushButton_SaveBipartOps, 4, 0, 1, 2);


        verticalLayout_20->addWidget(groupBox_15);

        groupBox_7 = new QGroupBox(DecompositionTab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout_4 = new QGridLayout(groupBox_7);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(1, 1, 1, 1);
        pushButton_ApplyNextBipartOp = new QPushButton(groupBox_7);
        pushButton_ApplyNextBipartOp->setObjectName(QString::fromUtf8("pushButton_ApplyNextBipartOp"));

        gridLayout_4->addWidget(pushButton_ApplyNextBipartOp, 2, 1, 1, 1);

        pushButton_InvertLastBipartOp = new QPushButton(groupBox_7);
        pushButton_InvertLastBipartOp->setObjectName(QString::fromUtf8("pushButton_InvertLastBipartOp"));

        gridLayout_4->addWidget(pushButton_InvertLastBipartOp, 2, 0, 1, 1);

        pushButton_LoadBipartOps = new QPushButton(groupBox_7);
        pushButton_LoadBipartOps->setObjectName(QString::fromUtf8("pushButton_LoadBipartOps"));

        gridLayout_4->addWidget(pushButton_LoadBipartOps, 1, 0, 1, 2);


        verticalLayout_20->addWidget(groupBox_7);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_20->addItem(verticalSpacer_2);

        tabWidget->addTab(DecompositionTab, QString());

        verticalLayout_17->addWidget(tabWidget);


        retranslateUi(DockControl);

        tabWidget->setCurrentIndex(2);
        comboBox_entityLabelContent->setCurrentIndex(0);
        comboBox_EntityColor->setCurrentIndex(0);
        comboBox_RelationshipColor->setCurrentIndex(0);
        pushButton_Apply->setDefault(false);
        stackedWidget_LayoutPages->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(DockControl);
    } // setupUi

    void retranslateUi(QWidget *DockControl)
    {
        DockControl->setWindowTitle(QCoreApplication::translate("DockControl", "DockControl", nullptr));
#if QT_CONFIG(tooltip)
        DockControl->setToolTip(QCoreApplication::translate("DockControl", "Relative strength of spring", nullptr));
#endif // QT_CONFIG(tooltip)
        groupBox_4->setTitle(QString());
        groupBox_22->setTitle(QCoreApplication::translate("DockControl", "Focused Window", nullptr));
        radioButton_PrimeVisWidget->setText(QCoreApplication::translate("DockControl", "Prime", nullptr));
        radioButton_DualVisWidget->setText(QCoreApplication::translate("DockControl", "Dual", nullptr));
        radioButton_BipartVisWidget->setText(QCoreApplication::translate("DockControl", "Bipartite", nullptr));
        pushButton_RecenterView->setText(QCoreApplication::translate("DockControl", "Recenter Viewpoint", nullptr));
        pushButton_SyncViewZooming->setText(QCoreApplication::translate("DockControl", "Sync Zooming", nullptr));
        pushButton_SetZoom->setText(QCoreApplication::translate("DockControl", "Set Zoom", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("DockControl", "Show", nullptr));
        label_30->setText(QCoreApplication::translate("DockControl", "Entity Labels:", nullptr));
        comboBox_entityLabelContent->setItemText(0, QCoreApplication::translate("DockControl", "No Label", nullptr));
        comboBox_entityLabelContent->setItemText(1, QCoreApplication::translate("DockControl", "Name", nullptr));
        comboBox_entityLabelContent->setItemText(2, QCoreApplication::translate("DockControl", "Index", nullptr));
        comboBox_entityLabelContent->setItemText(3, QCoreApplication::translate("DockControl", "Multiscale Index", nullptr));
        comboBox_entityLabelContent->setItemText(4, QCoreApplication::translate("DockControl", "Degree", nullptr));
        comboBox_entityLabelContent->setItemText(5, QCoreApplication::translate("DockControl", "Adjacency Count", nullptr));
        comboBox_entityLabelContent->setItemText(6, QCoreApplication::translate("DockControl", "Adjacency Factor", nullptr));
        comboBox_entityLabelContent->setItemText(7, QCoreApplication::translate("DockControl", "Betweenness Centrality", nullptr));
        comboBox_entityLabelContent->setItemText(8, QCoreApplication::translate("DockControl", "Eccentricity", nullptr));
        comboBox_entityLabelContent->setItemText(9, QCoreApplication::translate("DockControl", "Closeness Centrality", nullptr));
        comboBox_entityLabelContent->setItemText(10, QCoreApplication::translate("DockControl", "Effective Resistance", nullptr));
        comboBox_entityLabelContent->setItemText(11, QCoreApplication::translate("DockControl", "Removal Cost", nullptr));
        comboBox_entityLabelContent->setItemText(12, QCoreApplication::translate("DockControl", "Removal Importance", nullptr));
        comboBox_entityLabelContent->setItemText(13, QCoreApplication::translate("DockControl", "Removal Priority", nullptr));

        label_31->setText(QCoreApplication::translate("DockControl", "Relationship Label:", nullptr));
        comboBox_relationshipLabelContent->setItemText(0, QCoreApplication::translate("DockControl", "No Label", nullptr));
        comboBox_relationshipLabelContent->setItemText(1, QCoreApplication::translate("DockControl", "Name", nullptr));
        comboBox_relationshipLabelContent->setItemText(2, QCoreApplication::translate("DockControl", "Index", nullptr));
        comboBox_relationshipLabelContent->setItemText(3, QCoreApplication::translate("DockControl", "Multiscale Index", nullptr));
        comboBox_relationshipLabelContent->setItemText(4, QCoreApplication::translate("DockControl", "Degree", nullptr));
        comboBox_relationshipLabelContent->setItemText(5, QCoreApplication::translate("DockControl", "Adjacency Count", nullptr));
        comboBox_relationshipLabelContent->setItemText(6, QCoreApplication::translate("DockControl", "Adjacency Factor", nullptr));
        comboBox_relationshipLabelContent->setItemText(7, QCoreApplication::translate("DockControl", "Betweenness Centrality", nullptr));
        comboBox_relationshipLabelContent->setItemText(8, QCoreApplication::translate("DockControl", "Eccentricity", nullptr));
        comboBox_relationshipLabelContent->setItemText(9, QCoreApplication::translate("DockControl", "Closeness Centrality", nullptr));
        comboBox_relationshipLabelContent->setItemText(10, QCoreApplication::translate("DockControl", "Effective Resistance", nullptr));
        comboBox_relationshipLabelContent->setItemText(11, QCoreApplication::translate("DockControl", "Removal Cost", nullptr));
        comboBox_relationshipLabelContent->setItemText(12, QCoreApplication::translate("DockControl", "Removal Importance", nullptr));
        comboBox_relationshipLabelContent->setItemText(13, QCoreApplication::translate("DockControl", "Removal Priority", nullptr));

        checkBox_ShowEntities->setText(QCoreApplication::translate("DockControl", "Entities", nullptr));
        checkBox_ShowRelationships->setText(QCoreApplication::translate("DockControl", "Relations", nullptr));
        groupBox_filters->setTitle(QCoreApplication::translate("DockControl", "Filters", nullptr));
        checkBox_EntityDegreeFilter->setText(QCoreApplication::translate("DockControl", "En. Degree Filter", nullptr));
        label_9->setText(QCoreApplication::translate("DockControl", "top", nullptr));
        checkBox_RelationshipDegreeFilter->setText(QCoreApplication::translate("DockControl", "Re. Degree Filter", nullptr));
        label_8->setText(QCoreApplication::translate("DockControl", "bottom", nullptr));
        checkBox_EntityIndexFilter->setText(QCoreApplication::translate("DockControl", "En. Index Filter", nullptr));
        checkBox_RelationshipIndexFilter->setText(QCoreApplication::translate("DockControl", "Re. Index Filter", nullptr));
        checkBox_EntityCardinalityFilter->setText(QCoreApplication::translate("DockControl", "En. Card. Filter", nullptr));
        checkBox_polygonCardinalityFilter->setText(QCoreApplication::translate("DockControl", "Re. Card. Filter", nullptr));
        groupBox_ColorElements->setTitle(QCoreApplication::translate("DockControl", "Element Colors", nullptr));
        pushButton_CalcDegreeAndCardinality->setText(QCoreApplication::translate("DockControl", "Calculate Element Statistics", nullptr));
        label_59->setText(QCoreApplication::translate("DockControl", "Color Palette", nullptr));
        label_33->setText(QCoreApplication::translate("DockControl", "Re. Color Scheme", nullptr));
        comboBox_EntityColor->setItemText(0, QCoreApplication::translate("DockControl", "Index", nullptr));
        comboBox_EntityColor->setItemText(1, QCoreApplication::translate("DockControl", "Degree", nullptr));
        comboBox_EntityColor->setItemText(2, QCoreApplication::translate("DockControl", "Adjacency Count", nullptr));
        comboBox_EntityColor->setItemText(3, QCoreApplication::translate("DockControl", "Adjacency Factor", nullptr));
        comboBox_EntityColor->setItemText(4, QCoreApplication::translate("DockControl", "Betweenness Centrality", nullptr));
        comboBox_EntityColor->setItemText(5, QCoreApplication::translate("DockControl", "Eccentricity", nullptr));
        comboBox_EntityColor->setItemText(6, QCoreApplication::translate("DockControl", "Closeness Centrality", nullptr));

        comboBox_RelationshipColor->setItemText(0, QCoreApplication::translate("DockControl", "Index", nullptr));
        comboBox_RelationshipColor->setItemText(1, QCoreApplication::translate("DockControl", "Degree", nullptr));
        comboBox_RelationshipColor->setItemText(2, QCoreApplication::translate("DockControl", "Adjacency Count", nullptr));
        comboBox_RelationshipColor->setItemText(3, QCoreApplication::translate("DockControl", "Adjacency Factor", nullptr));
        comboBox_RelationshipColor->setItemText(4, QCoreApplication::translate("DockControl", "Betweenness Centrality", nullptr));
        comboBox_RelationshipColor->setItemText(5, QCoreApplication::translate("DockControl", "Eccentricity", nullptr));
        comboBox_RelationshipColor->setItemText(6, QCoreApplication::translate("DockControl", "Closeness Centrality", nullptr));

        label_32->setText(QCoreApplication::translate("DockControl", "En. Color Scheme", nullptr));
        pushButton_ApplyEnColorScheme->setText(QCoreApplication::translate("DockControl", "Apply", nullptr));
        pushButton_ApplyReColorScheme->setText(QCoreApplication::translate("DockControl", "Apply", nullptr));
#if QT_CONFIG(tooltip)
        pushButton_SyncDualElements->setToolTip(QCoreApplication::translate("DockControl", "Sync the dual of focused vis with focused vis color", nullptr));
#endif // QT_CONFIG(tooltip)
        pushButton_SyncDualElements->setText(QCoreApplication::translate("DockControl", "Sync Dual's Colors", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("DockControl", "Element Vis Properties", nullptr));
        label_lensAngle->setText(QCoreApplication::translate("DockControl", "Digon Angle", nullptr));
        label_4->setText(QCoreApplication::translate("DockControl", "Vertex Radius", nullptr));
        label_25->setText(QCoreApplication::translate("DockControl", "Monogon Radius", nullptr));
        label_24->setText(QCoreApplication::translate("DockControl", "Monogon Length", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(VisualizationTab), QCoreApplication::translate("DockControl", "Visualization", nullptr));
        comboBox_LayoutSelector->setItemText(0, QCoreApplication::translate("DockControl", "-- Select Layout --", nullptr));
        comboBox_LayoutSelector->setItemText(1, QCoreApplication::translate("DockControl", "Transform", nullptr));
        comboBox_LayoutSelector->setItemText(2, QCoreApplication::translate("DockControl", "Random Layout", nullptr));
        comboBox_LayoutSelector->setItemText(3, QCoreApplication::translate("DockControl", "Grid Layout", nullptr));
        comboBox_LayoutSelector->setItemText(4, QCoreApplication::translate("DockControl", "Circle Layout", nullptr));
        comboBox_LayoutSelector->setItemText(5, QCoreApplication::translate("DockControl", "Adjust Monogons", nullptr));
        comboBox_LayoutSelector->setItemText(6, QCoreApplication::translate("DockControl", "Force-Directed", nullptr));
        comboBox_LayoutSelector->setItemText(7, QCoreApplication::translate("DockControl", "Optimize Layout", nullptr));
        comboBox_LayoutSelector->setItemText(8, QCoreApplication::translate("DockControl", "Evaluate Energy", nullptr));
        comboBox_LayoutSelector->setItemText(9, QCoreApplication::translate("DockControl", "Konig Layout", nullptr));

        comboBox_LayoutSelector->setCurrentText(QCoreApplication::translate("DockControl", "-- Select Layout --", nullptr));
        comboBox_LayoutSelector->setPlaceholderText(QString());
        pushButton_Apply->setText(QCoreApplication::translate("DockControl", "  Apply", nullptr));
        pushButton_Stop->setText(QCoreApplication::translate("DockControl", "  Stop", nullptr));
        pushButton_ClearFixedVert->setText(QCoreApplication::translate("DockControl", "Clear Fixed", nullptr));
        pushButton_FixSelectedVerts->setText(QCoreApplication::translate("DockControl", "Fix Selected", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_DefaultProperties->headerItem();
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_TransformProperties->headerItem();
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled = treeWidget_TransformProperties->isSortingEnabled();
        treeWidget_TransformProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget_TransformProperties->topLevelItem(0);
        ___qtreewidgetitem2->setText(0, QCoreApplication::translate("DockControl", "Translate", nullptr));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem2->child(0);
        ___qtreewidgetitem3->setText(0, QCoreApplication::translate("DockControl", "Use Translation", nullptr));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem2->child(1);
        ___qtreewidgetitem4->setText(1, QCoreApplication::translate("DockControl", "0.0", nullptr));
        ___qtreewidgetitem4->setText(0, QCoreApplication::translate("DockControl", "Translation X", nullptr));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem2->child(2);
        ___qtreewidgetitem5->setText(1, QCoreApplication::translate("DockControl", "0.0", nullptr));
        ___qtreewidgetitem5->setText(0, QCoreApplication::translate("DockControl", "Translation Y", nullptr));
        QTreeWidgetItem *___qtreewidgetitem6 = treeWidget_TransformProperties->topLevelItem(1);
        ___qtreewidgetitem6->setText(0, QCoreApplication::translate("DockControl", "Rotate", nullptr));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem6->child(0);
        ___qtreewidgetitem7->setText(0, QCoreApplication::translate("DockControl", "Use Rotation", nullptr));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem6->child(1);
        ___qtreewidgetitem8->setText(1, QCoreApplication::translate("DockControl", "0.0", nullptr));
        ___qtreewidgetitem8->setText(0, QCoreApplication::translate("DockControl", "Angle", nullptr));
        QTreeWidgetItem *___qtreewidgetitem9 = treeWidget_TransformProperties->topLevelItem(2);
        ___qtreewidgetitem9->setText(0, QCoreApplication::translate("DockControl", "Scale", nullptr));
        QTreeWidgetItem *___qtreewidgetitem10 = ___qtreewidgetitem9->child(0);
        ___qtreewidgetitem10->setText(0, QCoreApplication::translate("DockControl", "Use Scaling", nullptr));
        QTreeWidgetItem *___qtreewidgetitem11 = ___qtreewidgetitem9->child(1);
        ___qtreewidgetitem11->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem11->setText(0, QCoreApplication::translate("DockControl", "Scale X", nullptr));
        QTreeWidgetItem *___qtreewidgetitem12 = ___qtreewidgetitem9->child(2);
        ___qtreewidgetitem12->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem12->setText(0, QCoreApplication::translate("DockControl", "Scale Y", nullptr));
        QTreeWidgetItem *___qtreewidgetitem13 = treeWidget_TransformProperties->topLevelItem(3);
        ___qtreewidgetitem13->setText(0, QCoreApplication::translate("DockControl", "Reflect", nullptr));
        QTreeWidgetItem *___qtreewidgetitem14 = ___qtreewidgetitem13->child(0);
        ___qtreewidgetitem14->setText(0, QCoreApplication::translate("DockControl", "Use Reflection", nullptr));
        QTreeWidgetItem *___qtreewidgetitem15 = ___qtreewidgetitem13->child(1);
        ___qtreewidgetitem15->setText(1, QCoreApplication::translate("DockControl", "0.0", nullptr));
        ___qtreewidgetitem15->setText(0, QCoreApplication::translate("DockControl", "Axis Angle", nullptr));
        treeWidget_TransformProperties->setSortingEnabled(__sortingEnabled);

        QTreeWidgetItem *___qtreewidgetitem16 = treeWidget_RandomProperties->headerItem();
        ___qtreewidgetitem16->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem16->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled1 = treeWidget_RandomProperties->isSortingEnabled();
        treeWidget_RandomProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem17 = treeWidget_RandomProperties->topLevelItem(0);
        ___qtreewidgetitem17->setText(0, QCoreApplication::translate("DockControl", "Options", nullptr));
        QTreeWidgetItem *___qtreewidgetitem18 = ___qtreewidgetitem17->child(0);
        ___qtreewidgetitem18->setText(0, QCoreApplication::translate("DockControl", "(Rectangular Domain)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem19 = treeWidget_RandomProperties->topLevelItem(1);
        ___qtreewidgetitem19->setText(0, QCoreApplication::translate("DockControl", "Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem20 = ___qtreewidgetitem19->child(0);
        ___qtreewidgetitem20->setText(1, QCoreApplication::translate("DockControl", "0.0", nullptr));
        ___qtreewidgetitem20->setText(0, QCoreApplication::translate("DockControl", "Radius Min (Width)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem21 = ___qtreewidgetitem19->child(1);
        ___qtreewidgetitem21->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem21->setText(0, QCoreApplication::translate("DockControl", "Radius Max (Height)", nullptr));
        treeWidget_RandomProperties->setSortingEnabled(__sortingEnabled1);

        QTreeWidgetItem *___qtreewidgetitem22 = treeWidget_GridProperties->headerItem();
        ___qtreewidgetitem22->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem22->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled2 = treeWidget_GridProperties->isSortingEnabled();
        treeWidget_GridProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem23 = treeWidget_GridProperties->topLevelItem(0);
        ___qtreewidgetitem23->setText(0, QCoreApplication::translate("DockControl", "Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem24 = ___qtreewidgetitem23->child(0);
        ___qtreewidgetitem24->setText(1, QCoreApplication::translate("DockControl", "10", nullptr));
        ___qtreewidgetitem24->setText(0, QCoreApplication::translate("DockControl", "Min Dimension", nullptr));
        QTreeWidgetItem *___qtreewidgetitem25 = ___qtreewidgetitem23->child(1);
        ___qtreewidgetitem25->setText(0, QCoreApplication::translate("DockControl", "Column-Wise", nullptr));
        QTreeWidgetItem *___qtreewidgetitem26 = ___qtreewidgetitem23->child(2);
        ___qtreewidgetitem26->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem26->setText(0, QCoreApplication::translate("DockControl", "Grid Spacing X", nullptr));
        QTreeWidgetItem *___qtreewidgetitem27 = ___qtreewidgetitem23->child(3);
        ___qtreewidgetitem27->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem27->setText(0, QCoreApplication::translate("DockControl", "Grid Spacing Y", nullptr));
        treeWidget_GridProperties->setSortingEnabled(__sortingEnabled2);

        QTreeWidgetItem *___qtreewidgetitem28 = treeWidget_CircleProperties->headerItem();
        ___qtreewidgetitem28->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem28->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled3 = treeWidget_CircleProperties->isSortingEnabled();
        treeWidget_CircleProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem29 = treeWidget_CircleProperties->topLevelItem(0);
        ___qtreewidgetitem29->setText(0, QCoreApplication::translate("DockControl", "Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem30 = ___qtreewidgetitem29->child(0);
        ___qtreewidgetitem30->setText(1, QCoreApplication::translate("DockControl", "5.0", nullptr));
        ___qtreewidgetitem30->setText(0, QCoreApplication::translate("DockControl", "Radius", nullptr));
        QTreeWidgetItem *___qtreewidgetitem31 = ___qtreewidgetitem29->child(1);
        ___qtreewidgetitem31->setText(0, QCoreApplication::translate("DockControl", "Sort By", nullptr));
        QTreeWidgetItem *___qtreewidgetitem32 = ___qtreewidgetitem29->child(2);
        ___qtreewidgetitem32->setText(0, QCoreApplication::translate("DockControl", "Sort Ascending", nullptr));
        QTreeWidgetItem *___qtreewidgetitem33 = ___qtreewidgetitem29->child(3);
        ___qtreewidgetitem33->setText(1, QCoreApplication::translate("DockControl", "0.0", nullptr));
        ___qtreewidgetitem33->setText(0, QCoreApplication::translate("DockControl", "Start Angle", nullptr));
        treeWidget_CircleProperties->setSortingEnabled(__sortingEnabled3);

        QTreeWidgetItem *___qtreewidgetitem34 = treeWidget_MonogonProperties->headerItem();
        ___qtreewidgetitem34->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem34->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled4 = treeWidget_MonogonProperties->isSortingEnabled();
        treeWidget_MonogonProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem35 = treeWidget_MonogonProperties->topLevelItem(0);
        ___qtreewidgetitem35->setText(0, QCoreApplication::translate("DockControl", "Options", nullptr));
        QTreeWidgetItem *___qtreewidgetitem36 = ___qtreewidgetitem35->child(0);
        ___qtreewidgetitem36->setText(0, QCoreApplication::translate("DockControl", "Distribute Evenly", nullptr));
        treeWidget_MonogonProperties->setSortingEnabled(__sortingEnabled4);

        QTreeWidgetItem *___qtreewidgetitem37 = treeWidget_FDProperties->headerItem();
        ___qtreewidgetitem37->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem37->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled5 = treeWidget_FDProperties->isSortingEnabled();
        treeWidget_FDProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem38 = treeWidget_FDProperties->topLevelItem(0);
        ___qtreewidgetitem38->setText(0, QCoreApplication::translate("DockControl", "Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem39 = ___qtreewidgetitem38->child(0);
        ___qtreewidgetitem39->setText(1, QCoreApplication::translate("DockControl", "0.5", nullptr));
        ___qtreewidgetitem39->setText(0, QCoreApplication::translate("DockControl", "Initial Step Size", nullptr));
        QTreeWidgetItem *___qtreewidgetitem40 = ___qtreewidgetitem38->child(1);
        ___qtreewidgetitem40->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem40->setText(0, QCoreApplication::translate("DockControl", "Target Edge Length", nullptr));
        QTreeWidgetItem *___qtreewidgetitem41 = ___qtreewidgetitem38->child(2);
        ___qtreewidgetitem41->setText(1, QCoreApplication::translate("DockControl", "0.2", nullptr));
        ___qtreewidgetitem41->setText(0, QCoreApplication::translate("DockControl", "Repulsion Coefficient", nullptr));
        QTreeWidgetItem *___qtreewidgetitem42 = ___qtreewidgetitem38->child(3);
        ___qtreewidgetitem42->setText(1, QCoreApplication::translate("DockControl", "2.8", nullptr));
        ___qtreewidgetitem42->setText(0, QCoreApplication::translate("DockControl", "Repulsion Normalization Power", nullptr));
        QTreeWidgetItem *___qtreewidgetitem43 = ___qtreewidgetitem38->child(4);
        ___qtreewidgetitem43->setText(1, QCoreApplication::translate("DockControl", "500", nullptr));
        ___qtreewidgetitem43->setText(0, QCoreApplication::translate("DockControl", "Number Iterations", nullptr));
        QTreeWidgetItem *___qtreewidgetitem44 = ___qtreewidgetitem38->child(5);
        ___qtreewidgetitem44->setText(1, QCoreApplication::translate("DockControl", "0.00001", nullptr));
        ___qtreewidgetitem44->setText(0, QCoreApplication::translate("DockControl", "Convergence Tolerance", nullptr));
        treeWidget_FDProperties->setSortingEnabled(__sortingEnabled5);

        QTreeWidgetItem *___qtreewidgetitem45 = treeWidget_OptimizationProperties->headerItem();
        ___qtreewidgetitem45->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem45->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled6 = treeWidget_OptimizationProperties->isSortingEnabled();
        treeWidget_OptimizationProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem46 = treeWidget_OptimizationProperties->topLevelItem(0);
        ___qtreewidgetitem46->setText(0, QCoreApplication::translate("DockControl", "Options", nullptr));
        QTreeWidgetItem *___qtreewidgetitem47 = ___qtreewidgetitem46->child(0);
        ___qtreewidgetitem47->setText(0, QCoreApplication::translate("DockControl", "Use Monogons", nullptr));
        QTreeWidgetItem *___qtreewidgetitem48 = ___qtreewidgetitem46->child(1);
        ___qtreewidgetitem48->setText(0, QCoreApplication::translate("DockControl", "Joint Optimize", nullptr));
        QTreeWidgetItem *___qtreewidgetitem49 = treeWidget_OptimizationProperties->topLevelItem(1);
        ___qtreewidgetitem49->setText(0, QCoreApplication::translate("DockControl", "Optimization Energies", nullptr));
        QTreeWidgetItem *___qtreewidgetitem50 = ___qtreewidgetitem49->child(0);
        ___qtreewidgetitem50->setText(1, QCoreApplication::translate("DockControl", "0.3", nullptr));
        ___qtreewidgetitem50->setText(0, QCoreApplication::translate("DockControl", "Polygon Regularity (PR)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem51 = ___qtreewidgetitem49->child(1);
        ___qtreewidgetitem51->setText(1, QCoreApplication::translate("DockControl", "0.16", nullptr));
        ___qtreewidgetitem51->setText(0, QCoreApplication::translate("DockControl", "Polygon Area (PA)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem52 = ___qtreewidgetitem49->child(2);
        ___qtreewidgetitem52->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem52->setText(0, QCoreApplication::translate("DockControl", "Digon Length (DL)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem53 = ___qtreewidgetitem49->child(3);
        ___qtreewidgetitem53->setText(1, QCoreApplication::translate("DockControl", "0.36", nullptr));
        ___qtreewidgetitem53->setText(0, QCoreApplication::translate("DockControl", "Polygon Separation (PS)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem54 = ___qtreewidgetitem49->child(4);
        ___qtreewidgetitem54->setText(1, QCoreApplication::translate("DockControl", "0.18", nullptr));
        ___qtreewidgetitem54->setText(0, QCoreApplication::translate("DockControl", "Polygon Interation (PI)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem55 = ___qtreewidgetitem49->child(5);
        ___qtreewidgetitem55->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem55->setText(0, QCoreApplication::translate("DockControl", "Dual Distance (DD)", nullptr));
        QTreeWidgetItem *___qtreewidgetitem56 = treeWidget_OptimizationProperties->topLevelItem(2);
        ___qtreewidgetitem56->setText(0, QCoreApplication::translate("DockControl", "Energy Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem57 = ___qtreewidgetitem56->child(0);
        ___qtreewidgetitem57->setText(1, QCoreApplication::translate("DockControl", "30.0", nullptr));
        ___qtreewidgetitem57->setText(0, QCoreApplication::translate("DockControl", "Separation Angle Buffer", nullptr));
        QTreeWidgetItem *___qtreewidgetitem58 = ___qtreewidgetitem56->child(1);
        ___qtreewidgetitem58->setText(1, QCoreApplication::translate("DockControl", "0.1", nullptr));
        ___qtreewidgetitem58->setText(0, QCoreApplication::translate("DockControl", "Separation Distance Buffer", nullptr));
        QTreeWidgetItem *___qtreewidgetitem59 = ___qtreewidgetitem56->child(2);
        ___qtreewidgetitem59->setText(1, QCoreApplication::translate("DockControl", "12", nullptr));
        ___qtreewidgetitem59->setText(0, QCoreApplication::translate("DockControl", "Maximum Cardinality", nullptr));
        QTreeWidgetItem *___qtreewidgetitem60 = ___qtreewidgetitem56->child(3);
        ___qtreewidgetitem60->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem60->setText(0, QCoreApplication::translate("DockControl", "Target Edge Length", nullptr));
        QTreeWidgetItem *___qtreewidgetitem61 = treeWidget_OptimizationProperties->topLevelItem(3);
        ___qtreewidgetitem61->setText(0, QCoreApplication::translate("DockControl", "HLBFGS Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem62 = ___qtreewidgetitem61->child(0);
        ___qtreewidgetitem62->setText(1, QCoreApplication::translate("DockControl", "50000", nullptr));
        ___qtreewidgetitem62->setText(0, QCoreApplication::translate("DockControl", "Maximum Iterations", nullptr));
        QTreeWidgetItem *___qtreewidgetitem63 = ___qtreewidgetitem61->child(1);
        ___qtreewidgetitem63->setText(1, QCoreApplication::translate("DockControl", "1.0e-6", nullptr));
        ___qtreewidgetitem63->setText(0, QCoreApplication::translate("DockControl", "Gradient Threshold", nullptr));
        QTreeWidgetItem *___qtreewidgetitem64 = ___qtreewidgetitem61->child(2);
        ___qtreewidgetitem64->setText(1, QCoreApplication::translate("DockControl", "1.0e-4", nullptr));
        ___qtreewidgetitem64->setText(0, QCoreApplication::translate("DockControl", "Gradient Division Threshold", nullptr));
        QTreeWidgetItem *___qtreewidgetitem65 = ___qtreewidgetitem61->child(3);
        ___qtreewidgetitem65->setText(1, QCoreApplication::translate("DockControl", "1.0e-4", nullptr));
        ___qtreewidgetitem65->setText(0, QCoreApplication::translate("DockControl", "Line-Search Function Tolerance", nullptr));
        QTreeWidgetItem *___qtreewidgetitem66 = ___qtreewidgetitem61->child(4);
        ___qtreewidgetitem66->setText(1, QCoreApplication::translate("DockControl", "1.0e-16", nullptr));
        ___qtreewidgetitem66->setText(0, QCoreApplication::translate("DockControl", "Line-Search Variable Tolerance", nullptr));
        QTreeWidgetItem *___qtreewidgetitem67 = ___qtreewidgetitem61->child(5);
        ___qtreewidgetitem67->setText(1, QCoreApplication::translate("DockControl", "0.9", nullptr));
        ___qtreewidgetitem67->setText(0, QCoreApplication::translate("DockControl", "Line-Search Gradient Tolerance", nullptr));
        QTreeWidgetItem *___qtreewidgetitem68 = ___qtreewidgetitem61->child(6);
        ___qtreewidgetitem68->setText(1, QCoreApplication::translate("DockControl", "1.0e-20", nullptr));
        ___qtreewidgetitem68->setText(0, QCoreApplication::translate("DockControl", "Line-Search Minimum Step", nullptr));
        QTreeWidgetItem *___qtreewidgetitem69 = ___qtreewidgetitem61->child(7);
        ___qtreewidgetitem69->setText(1, QCoreApplication::translate("DockControl", "1.0e+20", nullptr));
        ___qtreewidgetitem69->setText(0, QCoreApplication::translate("DockControl", "Line-Search Maximum Step", nullptr));
        QTreeWidgetItem *___qtreewidgetitem70 = ___qtreewidgetitem61->child(8);
        ___qtreewidgetitem70->setText(1, QCoreApplication::translate("DockControl", "20", nullptr));
        ___qtreewidgetitem70->setText(0, QCoreApplication::translate("DockControl", "Line-Search Maximum Iterations", nullptr));
        treeWidget_OptimizationProperties->setSortingEnabled(__sortingEnabled6);

        QTreeWidgetItem *___qtreewidgetitem71 = treeWidget_EnergyProperties->headerItem();
        ___qtreewidgetitem71->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem71->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled7 = treeWidget_EnergyProperties->isSortingEnabled();
        treeWidget_EnergyProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem72 = treeWidget_EnergyProperties->topLevelItem(0);
        ___qtreewidgetitem72->setText(0, QCoreApplication::translate("DockControl", "Energy Terms", nullptr));
        QTreeWidgetItem *___qtreewidgetitem73 = ___qtreewidgetitem72->child(0);
        ___qtreewidgetitem73->setText(0, QCoreApplication::translate("DockControl", "Polygon Regularity", nullptr));
        QTreeWidgetItem *___qtreewidgetitem74 = ___qtreewidgetitem72->child(1);
        ___qtreewidgetitem74->setText(0, QCoreApplication::translate("DockControl", "Side Length", nullptr));
        QTreeWidgetItem *___qtreewidgetitem75 = ___qtreewidgetitem72->child(2);
        ___qtreewidgetitem75->setText(0, QCoreApplication::translate("DockControl", "Polygon Separation", nullptr));
        QTreeWidgetItem *___qtreewidgetitem76 = ___qtreewidgetitem72->child(3);
        ___qtreewidgetitem76->setText(0, QCoreApplication::translate("DockControl", "Intersection Regularity", nullptr));
        QTreeWidgetItem *___qtreewidgetitem77 = ___qtreewidgetitem72->child(4);
        ___qtreewidgetitem77->setText(0, QCoreApplication::translate("DockControl", "Overlap Area", nullptr));
        QTreeWidgetItem *___qtreewidgetitem78 = ___qtreewidgetitem72->child(5);
        ___qtreewidgetitem78->setText(0, QCoreApplication::translate("DockControl", "Overlap Count", nullptr));
        treeWidget_EnergyProperties->setSortingEnabled(__sortingEnabled7);

        QTreeWidgetItem *___qtreewidgetitem79 = treeWidget_KonigProperties->headerItem();
        ___qtreewidgetitem79->setText(1, QCoreApplication::translate("DockControl", "Value", nullptr));
        ___qtreewidgetitem79->setText(0, QCoreApplication::translate("DockControl", "Property", nullptr));

        const bool __sortingEnabled8 = treeWidget_KonigProperties->isSortingEnabled();
        treeWidget_KonigProperties->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem80 = treeWidget_KonigProperties->topLevelItem(0);
        ___qtreewidgetitem80->setText(0, QCoreApplication::translate("DockControl", "Settings", nullptr));
        QTreeWidgetItem *___qtreewidgetitem81 = ___qtreewidgetitem80->child(0);
        ___qtreewidgetitem81->setText(1, QCoreApplication::translate("DockControl", "0.33", nullptr));
        ___qtreewidgetitem81->setText(0, QCoreApplication::translate("DockControl", "Base Ratio", nullptr));
        QTreeWidgetItem *___qtreewidgetitem82 = ___qtreewidgetitem80->child(1);
        ___qtreewidgetitem82->setText(0, QCoreApplication::translate("DockControl", "Optimize Size", nullptr));
        QTreeWidgetItem *___qtreewidgetitem83 = ___qtreewidgetitem80->child(2);
        ___qtreewidgetitem83->setText(0, QCoreApplication::translate("DockControl", "Optimize Edge Length", nullptr));
        QTreeWidgetItem *___qtreewidgetitem84 = ___qtreewidgetitem80->child(3);
        ___qtreewidgetitem84->setText(1, QCoreApplication::translate("DockControl", "1.0", nullptr));
        ___qtreewidgetitem84->setText(0, QCoreApplication::translate("DockControl", "Postprocess Scaling", nullptr));
        treeWidget_KonigProperties->setSortingEnabled(__sortingEnabled8);

        tabWidget->setTabText(tabWidget->indexOf(LayoutTab), QCoreApplication::translate("DockControl", "Layout", nullptr));
        groupBox_12->setTitle(QCoreApplication::translate("DockControl", "Similarity Measure", nullptr));
        label_18->setText(QCoreApplication::translate("DockControl", "Alpha:", nullptr));
        label_20->setText(QCoreApplication::translate("DockControl", "Gamma", nullptr));
        label_19->setText(QCoreApplication::translate("DockControl", "Beta:", nullptr));
        Options->setTitle(QCoreApplication::translate("DockControl", "Atomic Operation Generation", nullptr));
        checkBox_OperationNodeMerge->setText(QCoreApplication::translate("DockControl", "Node Merge", nullptr));
        checkBox_OperationNodeRemove->setText(QCoreApplication::translate("DockControl", "Node Remove", nullptr));
        checkBox_OperationEdgeRemove->setText(QCoreApplication::translate("DockControl", "Edge Remove", nullptr));
        checkBox_OperationEdgeMerge->setText(QCoreApplication::translate("DockControl", "Edge Merge", nullptr));
        pushButton_GenerateAtomicOperations->setText(QCoreApplication::translate("DockControl", "Generate Operations", nullptr));
        groupBox->setTitle(QCoreApplication::translate("DockControl", "Multiscale Control", nullptr));
        groupBox_9->setTitle(QCoreApplication::translate("DockControl", "Operation Restrictions", nullptr));
        checkBox_PreserveConnections->setText(QCoreApplication::translate("DockControl", "Preserve Connections", nullptr));
        checkBox_PreserveBranches->setText(QCoreApplication::translate("DockControl", "Preserve Branches", nullptr));
        checkBox_PruneOnly->setText(QCoreApplication::translate("DockControl", "Prune Only", nullptr));
        label_28->setText(QCoreApplication::translate("DockControl", "Max. Edge Merge Size:", nullptr));
        groupBox_13->setTitle(QCoreApplication::translate("DockControl", "Simplification Termination", nullptr));
        label_23->setText(QCoreApplication::translate("DockControl", "Min. Nodes:", nullptr));
        label_22->setText(QCoreApplication::translate("DockControl", "Min. Edges:", nullptr));
        label_21->setText(QCoreApplication::translate("DockControl", "Max. Operations:", nullptr));
        label_34->setText(QCoreApplication::translate("DockControl", "Priority Threshold:", nullptr));
        checkBox_optimizeInversions->setText(QCoreApplication::translate("DockControl", "Optimize Inversions", nullptr));
        pushButton_InvertOperation->setText(QCoreApplication::translate("DockControl", "Invert Last", nullptr));
        pushButton_ApplyOperation->setText(QCoreApplication::translate("DockControl", "Apply Next", nullptr));
        pushButton_ApplyAllOperations->setText(QCoreApplication::translate("DockControl", "Apply All Operations", nullptr));
        pushButton_InvertAllOperations->setText(QCoreApplication::translate("DockControl", "Invert All Operations", nullptr));
        pushButton_OptimizeFootprint->setText(QCoreApplication::translate("DockControl", "Optimize Footprint", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(MultiscaleTab), QCoreApplication::translate("DockControl", "Multiscale", nullptr));
        groupBox_CycleDecomp->setTitle(QCoreApplication::translate("DockControl", "Cycle Decomposition", nullptr));
        pushButton_HighlightAllTrees->setText(QCoreApplication::translate("DockControl", "Highlight All Bridges/Branches", nullptr));
        pushButton_ResetHighlights->setText(QCoreApplication::translate("DockControl", "Reset Highlights", nullptr));
        pushButton_HighlightAllBiComps->setText(QCoreApplication::translate("DockControl", "Highlight All Topo Blocks", nullptr));
        pushButton_HighlightNextTree->setText(QCoreApplication::translate("DockControl", "Highlight Next Bridge/Branch", nullptr));
        pushButton_HighlightAllCycles->setText(QCoreApplication::translate("DockControl", "Highlight All Cycles", nullptr));
        pushButton_HighlightAllClusters->setText(QCoreApplication::translate("DockControl", "Highlight All Forbidden Clusters", nullptr));
        pushButton_CycleDecompose->setText(QCoreApplication::translate("DockControl", "Run Decomposition", nullptr));
        pushButton_HighlightNextCluster->setText(QCoreApplication::translate("DockControl", "Highlight Next Forbidden Cluster", nullptr));
        pushButton_HighlightNextBiComp->setText(QCoreApplication::translate("DockControl", "Highlight Next Topo Block", nullptr));
        pushButton_HighlightNextCycle->setText(QCoreApplication::translate("DockControl", "Highlight Next Cycle", nullptr));
        groupBox_15->setTitle(QCoreApplication::translate("DockControl", "Decomposition Bipartite Operations", nullptr));
        pushButton_SimpAllTrees->setText(QCoreApplication::translate("DockControl", "Simplify All Bridges/Branches", nullptr));
        pushButton_RecordBipartOps->setText(QCoreApplication::translate("DockControl", "Start Operation Recording", nullptr));
        pushButton_SimpAllBicomps->setText(QCoreApplication::translate("DockControl", "Simplify All Topo Blocks", nullptr));
        pushButton_ApplyNextClusterOp->setText(QCoreApplication::translate("DockControl", "Apply Next Cycle Collapse", nullptr));
        pushButton_ApplyNextTreeOp->setText(QCoreApplication::translate("DockControl", "Apply Next Leaf Pruning", nullptr));
        pushButton_ApplyNextBicompOp->setText(QCoreApplication::translate("DockControl", "Apply Next Cycle Cut", nullptr));
        pushButton_SimpAllClusters->setText(QCoreApplication::translate("DockControl", "Simplify All Forbidden Clusters", nullptr));
        pushButton_SaveBipartOps->setText(QCoreApplication::translate("DockControl", "Save Recorded Operations", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("DockControl", "Saved Bipartite Operations", nullptr));
        pushButton_ApplyNextBipartOp->setText(QCoreApplication::translate("DockControl", "Apply Next Operation", nullptr));
        pushButton_InvertLastBipartOp->setText(QCoreApplication::translate("DockControl", "Invert Last Operation", nullptr));
        pushButton_LoadBipartOps->setText(QCoreApplication::translate("DockControl", "Load Operations", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(DecompositionTab), QCoreApplication::translate("DockControl", "Decomp", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DockControl: public Ui_DockControl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCKCONTROL_H
