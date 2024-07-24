/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[311];
    char stringdata0[7711];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 24), // "updateMouseLocationLabel"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 1), // "x"
QT_MOC_LITERAL(4, 39, 1), // "y"
QT_MOC_LITERAL(5, 41, 23), // "updateprimeVisSizeLabel"
QT_MOC_LITERAL(6, 65, 22), // "updatedualVisSizeLabel"
QT_MOC_LITERAL(7, 88, 16), // "updateERdataInfo"
QT_MOC_LITERAL(8, 105, 7), // "ERData*"
QT_MOC_LITERAL(9, 113, 17), // "focusedVisChanged"
QT_MOC_LITERAL(10, 131, 7), // "VISTYPE"
QT_MOC_LITERAL(11, 139, 5), // "mtype"
QT_MOC_LITERAL(12, 145, 19), // "updatefocusVisLabel"
QT_MOC_LITERAL(13, 165, 4), // "type"
QT_MOC_LITERAL(14, 170, 20), // "syncSelectedElements"
QT_MOC_LITERAL(15, 191, 13), // "ERDataChanged"
QT_MOC_LITERAL(16, 205, 10), // "resetModel"
QT_MOC_LITERAL(17, 216, 14), // "dualVisVisible"
QT_MOC_LITERAL(18, 231, 16), // "bipartVisVisible"
QT_MOC_LITERAL(19, 248, 10), // "getOpenDir"
QT_MOC_LITERAL(20, 259, 10), // "setOpenDir"
QT_MOC_LITERAL(21, 270, 8), // "filePath"
QT_MOC_LITERAL(22, 279, 21), // "selectionActionsFresh"
QT_MOC_LITERAL(23, 301, 10), // "OGLWidget*"
QT_MOC_LITERAL(24, 312, 10), // "currentVis"
QT_MOC_LITERAL(25, 323, 18), // "setVisSelectEntity"
QT_MOC_LITERAL(26, 342, 7), // "checked"
QT_MOC_LITERAL(27, 350, 10), // "OpenERFile"
QT_MOC_LITERAL(28, 361, 11), // "const char*"
QT_MOC_LITERAL(29, 373, 4), // "path"
QT_MOC_LITERAL(30, 378, 10), // "viewWidget"
QT_MOC_LITERAL(31, 389, 10), // "SaveERFile"
QT_MOC_LITERAL(32, 400, 33), // "on_actionSave_Nary_Data_trigg..."
QT_MOC_LITERAL(33, 434, 33), // "on_actionOpen_Nary_Data_trigg..."
QT_MOC_LITERAL(34, 468, 38), // "on_actionLoad_Nary_Locations_..."
QT_MOC_LITERAL(35, 507, 40), // "on_actionLoad_Hyperedge_Color..."
QT_MOC_LITERAL(36, 548, 37), // "on_actionSave_Visualization_t..."
QT_MOC_LITERAL(37, 586, 51), // "on_actionSave_Visualization_w..."
QT_MOC_LITERAL(38, 638, 20), // "actionDeleteEntities"
QT_MOC_LITERAL(39, 659, 25), // "actionDeleteRelationships"
QT_MOC_LITERAL(40, 685, 48), // "on_actionSepareteEnclosingRel..."
QT_MOC_LITERAL(41, 734, 47), // "on_actiondeleteEnclosingRelat..."
QT_MOC_LITERAL(42, 782, 33), // "on_actionFindConvexHull_trigg..."
QT_MOC_LITERAL(43, 816, 32), // "on_actionOutputSubdata_triggered"
QT_MOC_LITERAL(44, 849, 33), // "on_actionSave_ViewPoint_trigg..."
QT_MOC_LITERAL(45, 883, 33), // "on_actionLoad_ViewPoint_trigg..."
QT_MOC_LITERAL(46, 917, 49), // "on_actionSave_All_Color_Schem..."
QT_MOC_LITERAL(47, 967, 38), // "on_actionOutput_Color_Scheme_..."
QT_MOC_LITERAL(48, 1006, 36), // "on_actionLoad_Color_Scheme_tr..."
QT_MOC_LITERAL(49, 1043, 47), // "on_actionselectEnclosingRelat..."
QT_MOC_LITERAL(50, 1091, 44), // "on_actionClear_Gradient_Direc..."
QT_MOC_LITERAL(51, 1136, 39), // "on_actionMergeEnclosedPolygon..."
QT_MOC_LITERAL(52, 1176, 38), // "on_actionShow_Vis_Sample_Num_..."
QT_MOC_LITERAL(53, 1215, 42), // "on_actionCloseAllDockableWidg..."
QT_MOC_LITERAL(54, 1258, 31), // "on_actionFontSettings_triggered"
QT_MOC_LITERAL(55, 1290, 32), // "on_actionClear_Console_triggered"
QT_MOC_LITERAL(56, 1323, 31), // "on_actionSave_Console_triggered"
QT_MOC_LITERAL(57, 1355, 45), // "on_actionAssign_Relationships..."
QT_MOC_LITERAL(58, 1401, 40), // "on_actionAssign_Entities_Colo..."
QT_MOC_LITERAL(59, 1442, 38), // "on_actionSet_Selection_Color_..."
QT_MOC_LITERAL(60, 1481, 47), // "on_actionChange_Entity_Bounda..."
QT_MOC_LITERAL(61, 1529, 43), // "on_actionSet_Nodes_Boundary_W..."
QT_MOC_LITERAL(62, 1573, 45), // "on_actionSet_Polygon_Boundary..."
QT_MOC_LITERAL(63, 1619, 34), // "on_actionLights_Settings_trig..."
QT_MOC_LITERAL(64, 1654, 45), // "on_actionUse_Low_Sepecular_Li..."
QT_MOC_LITERAL(65, 1700, 45), // "on_actionUse_High_Specular_Li..."
QT_MOC_LITERAL(66, 1746, 40), // "on_actionRemove_PolygonsOfCar..."
QT_MOC_LITERAL(67, 1787, 50), // "on_actionRemoveBinaryRelation..."
QT_MOC_LITERAL(68, 1838, 42), // "on_actionRemoveUnvisibleEleme..."
QT_MOC_LITERAL(69, 1881, 43), // "on_actionRemove_Dangling_Enti..."
QT_MOC_LITERAL(70, 1925, 48), // "on_actionRemove_Dangling_Rela..."
QT_MOC_LITERAL(71, 1974, 41), // "on_actionRemove_Multiple_Diag..."
QT_MOC_LITERAL(72, 2016, 52), // "on_actionRemove_Non_repeated_..."
QT_MOC_LITERAL(73, 2069, 39), // "on_actionConvert_to_Bipartite..."
QT_MOC_LITERAL(74, 2109, 36), // "on_actionConvert_to_Linear_tr..."
QT_MOC_LITERAL(75, 2146, 35), // "on_actionConver_to_Simple_tri..."
QT_MOC_LITERAL(76, 2182, 38), // "on_actionConvert_to_2_Linear_..."
QT_MOC_LITERAL(77, 2221, 32), // "on_actionCreate_ER_Vis_triggered"
QT_MOC_LITERAL(78, 2254, 52), // "on_actionCreate_Relationships..."
QT_MOC_LITERAL(79, 2307, 52), // "on_actionCreate_Cliques_from_..."
QT_MOC_LITERAL(80, 2360, 52), // "on_actionAssign_Entity_Labels..."
QT_MOC_LITERAL(81, 2413, 58), // "on_actionAssign_Relationship_..."
QT_MOC_LITERAL(82, 2472, 33), // "on_actionClear_all_data_trigg..."
QT_MOC_LITERAL(83, 2506, 16), // "createDualERdata"
QT_MOC_LITERAL(84, 2523, 45), // "on_actionSwitch_PrimVis_and_D..."
QT_MOC_LITERAL(85, 2569, 39), // "on_actionAnimate_Optimization..."
QT_MOC_LITERAL(86, 2609, 40), // "on_actionAuto_Adjust_Viewpoin..."
QT_MOC_LITERAL(87, 2650, 40), // "on_actionDraw_Curved_Scaffold..."
QT_MOC_LITERAL(88, 2691, 40), // "on_actionDraw_Nodes_as_Sphere..."
QT_MOC_LITERAL(89, 2732, 42), // "on_actionDraw_Edges_as_Cylind..."
QT_MOC_LITERAL(90, 2775, 28), // "on_actionDraw_Axes_triggered"
QT_MOC_LITERAL(91, 2804, 28), // "on_actionDraw_Grid_triggered"
QT_MOC_LITERAL(92, 2833, 30), // "on_actionDraw_Cursor_triggered"
QT_MOC_LITERAL(93, 2864, 31), // "on_actionSelectEntity_triggered"
QT_MOC_LITERAL(94, 2896, 37), // "on_actionSelectRelationship_t..."
QT_MOC_LITERAL(95, 2934, 33), // "on_actionSelectEntities_trigg..."
QT_MOC_LITERAL(96, 2968, 29), // "on_actionMoveSelect_triggered"
QT_MOC_LITERAL(97, 2998, 28), // "on_actionaddEntity_triggered"
QT_MOC_LITERAL(98, 3027, 36), // "on_actionaddRandomEntities_tr..."
QT_MOC_LITERAL(99, 3064, 34), // "on_actionaddRelationship_trig..."
QT_MOC_LITERAL(100, 3099, 41), // "on_actionDeleteSelectedElemen..."
QT_MOC_LITERAL(101, 3141, 30), // "on_actionEditElement_triggered"
QT_MOC_LITERAL(102, 3172, 32), // "on_actionMergeElements_triggered"
QT_MOC_LITERAL(103, 3205, 37), // "on_actionDivideRelationship_t..."
QT_MOC_LITERAL(104, 3243, 49), // "on_actionRemoveElementsFromRe..."
QT_MOC_LITERAL(105, 3293, 23), // "on_actionSwap_triggered"
QT_MOC_LITERAL(106, 3317, 33), // "on_actionBringReToFront_trigg..."
QT_MOC_LITERAL(107, 3351, 32), // "on_actionBringReToBack_triggered"
QT_MOC_LITERAL(108, 3384, 35), // "on_action_optimize_layout_tri..."
QT_MOC_LITERAL(109, 3420, 35), // "on_actionstopOptimization_tri..."
QT_MOC_LITERAL(110, 3456, 25), // "on_actionManual_triggered"
QT_MOC_LITERAL(111, 3482, 37), // "on_actionCreateRelationship_t..."
QT_MOC_LITERAL(112, 3520, 32), // "on_actionSplitElements_triggered"
QT_MOC_LITERAL(113, 3553, 16), // "optimizeOrdering"
QT_MOC_LITERAL(114, 3570, 20), // "optimizeDualOrdering"
QT_MOC_LITERAL(115, 3591, 10), // "lineLayout"
QT_MOC_LITERAL(116, 3602, 23), // "reorderSelectedPolygons"
QT_MOC_LITERAL(117, 3626, 24), // "calcDegreeAndCardinality"
QT_MOC_LITERAL(118, 3651, 21), // "calcElementStatistics"
QT_MOC_LITERAL(119, 3673, 9), // "fix_verts"
QT_MOC_LITERAL(120, 3683, 14), // "focused_ERData"
QT_MOC_LITERAL(121, 3698, 11), // "vector<int>"
QT_MOC_LITERAL(122, 3710, 16), // "fixedVertIndices"
QT_MOC_LITERAL(123, 3727, 13), // "free_elements"
QT_MOC_LITERAL(124, 3741, 6), // "erdata"
QT_MOC_LITERAL(125, 3748, 15), // "freeVertIndices"
QT_MOC_LITERAL(126, 3764, 15), // "freePolyIndices"
QT_MOC_LITERAL(127, 3780, 17), // "showFixedVertInfo"
QT_MOC_LITERAL(128, 3798, 23), // "getCurrentColorGradient"
QT_MOC_LITERAL(129, 3822, 13), // "ColorGradient"
QT_MOC_LITERAL(130, 3836, 7), // "setZoom"
QT_MOC_LITERAL(131, 3844, 15), // "syncDualZooming"
QT_MOC_LITERAL(132, 3860, 20), // "syncDualElementColor"
QT_MOC_LITERAL(133, 3881, 16), // "updatePolyColors"
QT_MOC_LITERAL(134, 3898, 42), // "assignRelationshipColorsBySet..."
QT_MOC_LITERAL(135, 3941, 29), // "assignElementsColorPredefined"
QT_MOC_LITERAL(136, 3971, 26), // "assignElementsColorBrewer1"
QT_MOC_LITERAL(137, 3998, 26), // "assignElementsColorBrewer2"
QT_MOC_LITERAL(138, 4025, 33), // "assignElementColorEvenHueDist..."
QT_MOC_LITERAL(139, 4059, 19), // "assignEnColorByDual"
QT_MOC_LITERAL(140, 4079, 18), // "applyReColorScheme"
QT_MOC_LITERAL(141, 4098, 18), // "applyEnColorScheme"
QT_MOC_LITERAL(142, 4117, 13), // "scaleERLayout"
QT_MOC_LITERAL(143, 4131, 5), // "scale"
QT_MOC_LITERAL(144, 4137, 12), // "recenterView"
QT_MOC_LITERAL(145, 4150, 19), // "ResetMonogonsAngles"
QT_MOC_LITERAL(146, 4170, 23), // "DistributeMonogonAngles"
QT_MOC_LITERAL(147, 4194, 24), // "removeMonogonsInPrimeVis"
QT_MOC_LITERAL(148, 4219, 23), // "removeMonogonsInDualVis"
QT_MOC_LITERAL(149, 4243, 24), // "getIndexListFromLineEdit"
QT_MOC_LITERAL(150, 4268, 24), // "getLabelListFromLineEdit"
QT_MOC_LITERAL(151, 4293, 14), // "vector<string>"
QT_MOC_LITERAL(152, 4308, 25), // "select_entities_fromInput"
QT_MOC_LITERAL(153, 4334, 30), // "select_relationships_fromInput"
QT_MOC_LITERAL(154, 4365, 26), // "Highlight_selectedElements"
QT_MOC_LITERAL(155, 4392, 30), // "Highlight_selectedElementsOnce"
QT_MOC_LITERAL(156, 4423, 15), // "applyGridLayout"
QT_MOC_LITERAL(157, 4439, 9), // "resetView"
QT_MOC_LITERAL(158, 4449, 24), // "applyGridSeparatedLayout"
QT_MOC_LITERAL(159, 4474, 17), // "applyRandomLayout"
QT_MOC_LITERAL(160, 4492, 19), // "apply3DRandomLayout"
QT_MOC_LITERAL(161, 4512, 17), // "applyCircleLayout"
QT_MOC_LITERAL(162, 4530, 11), // "applyHLBFGS"
QT_MOC_LITERAL(163, 4542, 15), // "applyHLBFGSDual"
QT_MOC_LITERAL(164, 4558, 19), // "applyHLBFGSArithopt"
QT_MOC_LITERAL(165, 4578, 23), // "applyHLBFGSArithoptDual"
QT_MOC_LITERAL(166, 4602, 18), // "applyHLBFGSMonogon"
QT_MOC_LITERAL(167, 4621, 22), // "applyHLBFGSMonogonDual"
QT_MOC_LITERAL(168, 4644, 14), // "clearFixedVert"
QT_MOC_LITERAL(169, 4659, 12), // "setFixedVert"
QT_MOC_LITERAL(170, 4672, 16), // "fixSelectedVerts"
QT_MOC_LITERAL(171, 4689, 16), // "stopOptimization"
QT_MOC_LITERAL(172, 4706, 18), // "calcBoundingCircle"
QT_MOC_LITERAL(173, 4725, 17), // "calcAverageLength"
QT_MOC_LITERAL(174, 4743, 47), // "findEdgelyAdjacentRelationshi..."
QT_MOC_LITERAL(175, 4791, 11), // "assignColor"
QT_MOC_LITERAL(176, 4803, 16), // "set_ShowEntities"
QT_MOC_LITERAL(177, 4820, 3), // "res"
QT_MOC_LITERAL(178, 4824, 21), // "set_ShowRelationships"
QT_MOC_LITERAL(179, 4846, 16), // "set_EntityLabels"
QT_MOC_LITERAL(180, 4863, 5), // "index"
QT_MOC_LITERAL(181, 4869, 22), // "set_RelationshipLabels"
QT_MOC_LITERAL(182, 4892, 18), // "set_ShowEntityTips"
QT_MOC_LITERAL(183, 4911, 24), // "set_ShowRelationshipTips"
QT_MOC_LITERAL(184, 4936, 25), // "set_ShowRelationshipDecal"
QT_MOC_LITERAL(185, 4962, 22), // "set_EntityDegreeFilter"
QT_MOC_LITERAL(186, 4985, 28), // "set_RelationshipDegreeFilter"
QT_MOC_LITERAL(187, 5014, 27), // "set_EntityCardinalityFilter"
QT_MOC_LITERAL(188, 5042, 28), // "set_polygonCardinalityFilter"
QT_MOC_LITERAL(189, 5071, 21), // "set_EntityIndexFilter"
QT_MOC_LITERAL(190, 5093, 27), // "set_RelationshipIndexFilter"
QT_MOC_LITERAL(191, 5121, 23), // "set_InheritVisibilities"
QT_MOC_LITERAL(192, 5145, 17), // "initialize_2D_vis"
QT_MOC_LITERAL(193, 5163, 26), // "set_PrimeVisWidget_focused"
QT_MOC_LITERAL(194, 5190, 25), // "set_DualVisWidget_focused"
QT_MOC_LITERAL(195, 5216, 27), // "set_BipartVisWidget_focused"
QT_MOC_LITERAL(196, 5244, 22), // "vertexMinDegreeChanged"
QT_MOC_LITERAL(197, 5267, 4), // "arg1"
QT_MOC_LITERAL(198, 5272, 22), // "vertexMaxDegreeChanged"
QT_MOC_LITERAL(199, 5295, 20), // "polyMinDegreeChanged"
QT_MOC_LITERAL(200, 5316, 20), // "polyMaxDegreeChanged"
QT_MOC_LITERAL(201, 5337, 36), // "relationshipCardinalityBottom..."
QT_MOC_LITERAL(202, 5374, 33), // "relationshipCardinalityTopCha..."
QT_MOC_LITERAL(203, 5408, 30), // "entityCardinalityBottomChanged"
QT_MOC_LITERAL(204, 5439, 27), // "entityCardinalityTopChanged"
QT_MOC_LITERAL(205, 5467, 27), // "optimizeIterationMaxChanged"
QT_MOC_LITERAL(206, 5495, 24), // "entityIndexBottomChanged"
QT_MOC_LITERAL(207, 5520, 21), // "entityIndexTopChanged"
QT_MOC_LITERAL(208, 5542, 30), // "relationshipIndexBottomChanged"
QT_MOC_LITERAL(209, 5573, 27), // "relationshipIndexTopChanged"
QT_MOC_LITERAL(210, 5601, 37), // "doubleSpinBox_VertexSize_valu..."
QT_MOC_LITERAL(211, 5639, 30), // "doubleSpinBox_MaxScaffoldCurve"
QT_MOC_LITERAL(212, 5670, 36), // "doubleSpinBox_LensAngle_value..."
QT_MOC_LITERAL(213, 5707, 45), // "doubleSpinBox_MonogonAspectRa..."
QT_MOC_LITERAL(214, 5753, 38), // "doubleSpinBox_MonogonSize_val..."
QT_MOC_LITERAL(215, 5792, 32), // "on_actionSave_CSV_Data_triggered"
QT_MOC_LITERAL(216, 5825, 32), // "on_actionOpen_CSV_Data_triggered"
QT_MOC_LITERAL(217, 5858, 37), // "on_actionSave_Decomposition_t..."
QT_MOC_LITERAL(218, 5896, 37), // "on_actionLoad_Decomposition_t..."
QT_MOC_LITERAL(219, 5934, 31), // "on_actionPolygon_View_triggered"
QT_MOC_LITERAL(220, 5966, 29), // "on_actionKonig_View_triggered"
QT_MOC_LITERAL(221, 5996, 12), // "apply_layout"
QT_MOC_LITERAL(222, 6009, 29), // "get_layout_transform_settings"
QT_MOC_LITERAL(223, 6039, 26), // "get_random_layout_settings"
QT_MOC_LITERAL(224, 6066, 24), // "get_grid_layout_settings"
QT_MOC_LITERAL(225, 6091, 26), // "get_circle_layout_settings"
QT_MOC_LITERAL(226, 6118, 27), // "get_force_directed_settings"
QT_MOC_LITERAL(227, 6146, 25), // "get_optimization_settings"
QT_MOC_LITERAL(228, 6172, 21), // "get_subgraph_settings"
QT_MOC_LITERAL(229, 6194, 23), // "get_simplified_settings"
QT_MOC_LITERAL(230, 6218, 24), // "generateAtomicOperations"
QT_MOC_LITERAL(231, 6243, 35), // "generateAtomicOperationsComma..."
QT_MOC_LITERAL(232, 6279, 5), // "alpha"
QT_MOC_LITERAL(233, 6285, 4), // "beta"
QT_MOC_LITERAL(234, 6290, 5), // "gamma"
QT_MOC_LITERAL(235, 6296, 4), // "nrem"
QT_MOC_LITERAL(236, 6301, 4), // "erem"
QT_MOC_LITERAL(237, 6306, 6), // "nmerge"
QT_MOC_LITERAL(238, 6313, 6), // "emerge"
QT_MOC_LITERAL(239, 6320, 18), // "applyNextOperation"
QT_MOC_LITERAL(240, 6339, 19), // "invertLastOperation"
QT_MOC_LITERAL(241, 6359, 18), // "applyAllOperations"
QT_MOC_LITERAL(242, 6378, 29), // "applyAllOperationsCommandLine"
QT_MOC_LITERAL(243, 6408, 11), // "std::string"
QT_MOC_LITERAL(244, 6420, 11), // "output_name"
QT_MOC_LITERAL(245, 6432, 9), // "min_edges"
QT_MOC_LITERAL(246, 6442, 9), // "min_nodes"
QT_MOC_LITERAL(247, 6452, 17), // "invertAndOptimize"
QT_MOC_LITERAL(248, 6470, 28), // "invertAndOptimizeCommandLine"
QT_MOC_LITERAL(249, 6499, 8), // "interval"
QT_MOC_LITERAL(250, 6508, 8), // "op_total"
QT_MOC_LITERAL(251, 6517, 24), // "optimizeSubgraphManually"
QT_MOC_LITERAL(252, 6542, 22), // "applySelectedOperation"
QT_MOC_LITERAL(253, 6565, 25), // "optimizeInversionsChanged"
QT_MOC_LITERAL(254, 6591, 26), // "preserveConnectionsChanged"
QT_MOC_LITERAL(255, 6618, 23), // "preserveBranchesChanged"
QT_MOC_LITERAL(256, 6642, 16), // "pruneOnlyChanged"
QT_MOC_LITERAL(257, 6659, 31), // "applyBranchClusterDecomposition"
QT_MOC_LITERAL(258, 6691, 13), // "createCluster"
QT_MOC_LITERAL(259, 6705, 6), // "ENList"
QT_MOC_LITERAL(260, 6712, 6), // "enlist"
QT_MOC_LITERAL(261, 6719, 6), // "REList"
QT_MOC_LITERAL(262, 6726, 6), // "relist"
QT_MOC_LITERAL(263, 6733, 23), // "copyERDataMergeClusters"
QT_MOC_LITERAL(264, 6757, 6), // "source"
QT_MOC_LITERAL(265, 6764, 16), // "ERClusterDecomp*"
QT_MOC_LITERAL(266, 6781, 6), // "decomp"
QT_MOC_LITERAL(267, 6788, 24), // "getMatchingIndexEntities"
QT_MOC_LITERAL(268, 6813, 7), // "ENList*"
QT_MOC_LITERAL(269, 6821, 6), // "target"
QT_MOC_LITERAL(270, 6828, 29), // "getMatchingIndexRelationships"
QT_MOC_LITERAL(271, 6858, 7), // "REList*"
QT_MOC_LITERAL(272, 6866, 19), // "copyVertexLocations"
QT_MOC_LITERAL(273, 6886, 23), // "applyCycleDecomposition"
QT_MOC_LITERAL(274, 6910, 22), // "decompHighlightChanged"
QT_MOC_LITERAL(275, 6933, 25), // "updateVisSelectedElements"
QT_MOC_LITERAL(276, 6959, 18), // "highlightNextBComp"
QT_MOC_LITERAL(277, 6978, 18), // "highlightAllBComps"
QT_MOC_LITERAL(278, 6997, 18), // "highlightNextTComp"
QT_MOC_LITERAL(279, 7016, 18), // "highlightAllTComps"
QT_MOC_LITERAL(280, 7035, 18), // "highlightNextCycle"
QT_MOC_LITERAL(281, 7054, 18), // "highlightAllCycles"
QT_MOC_LITERAL(282, 7073, 20), // "highlightNextCluster"
QT_MOC_LITERAL(283, 7094, 20), // "highlightAllClusters"
QT_MOC_LITERAL(284, 7115, 21), // "resetDecompHighlights"
QT_MOC_LITERAL(285, 7137, 30), // "startRecordingBipartOperations"
QT_MOC_LITERAL(286, 7168, 20), // "applyBipartOperation"
QT_MOC_LITERAL(287, 7189, 14), // "BIPART_OP_TYPE"
QT_MOC_LITERAL(288, 7204, 20), // "saveBipartOperations"
QT_MOC_LITERAL(289, 7225, 20), // "loadBipartOperations"
QT_MOC_LITERAL(290, 7246, 25), // "invertLastBipartOperation"
QT_MOC_LITERAL(291, 7272, 24), // "applyNextBipartOperation"
QT_MOC_LITERAL(292, 7297, 18), // "applyBipartEdgeCut"
QT_MOC_LITERAL(293, 7316, 20), // "applyBipartEdgePrune"
QT_MOC_LITERAL(294, 7337, 22), // "applyBipartK22Collapse"
QT_MOC_LITERAL(295, 7360, 21), // "applyBipartChainMerge"
QT_MOC_LITERAL(296, 7382, 24), // "checkDecompAndOpManagers"
QT_MOC_LITERAL(297, 7407, 25), // "applyNextClusterOperation"
QT_MOC_LITERAL(298, 7433, 26), // "applyClusterSimplification"
QT_MOC_LITERAL(299, 7460, 19), // "simplifyAllClusters"
QT_MOC_LITERAL(300, 7480, 24), // "applyNextBicompOperation"
QT_MOC_LITERAL(301, 7505, 18), // "simplifyAllBicomps"
QT_MOC_LITERAL(302, 7524, 22), // "applyNextTreeOperation"
QT_MOC_LITERAL(303, 7547, 16), // "simplifyAllTrees"
QT_MOC_LITERAL(304, 7564, 27), // "writeOrderedIncidenceMatrix"
QT_MOC_LITERAL(305, 7592, 22), // "writeAdjacencyMatrices"
QT_MOC_LITERAL(306, 7615, 21), // "writeERDataStatistics"
QT_MOC_LITERAL(307, 7637, 7), // "outfile"
QT_MOC_LITERAL(308, 7645, 46), // "on_actionPrint_Euler_Characte..."
QT_MOC_LITERAL(309, 7692, 12), // "save_console"
QT_MOC_LITERAL(310, 7705, 5) // "fname"

    },
    "MainWindow\0updateMouseLocationLabel\0"
    "\0x\0y\0updateprimeVisSizeLabel\0"
    "updatedualVisSizeLabel\0updateERdataInfo\0"
    "ERData*\0focusedVisChanged\0VISTYPE\0"
    "mtype\0updatefocusVisLabel\0type\0"
    "syncSelectedElements\0ERDataChanged\0"
    "resetModel\0dualVisVisible\0bipartVisVisible\0"
    "getOpenDir\0setOpenDir\0filePath\0"
    "selectionActionsFresh\0OGLWidget*\0"
    "currentVis\0setVisSelectEntity\0checked\0"
    "OpenERFile\0const char*\0path\0viewWidget\0"
    "SaveERFile\0on_actionSave_Nary_Data_triggered\0"
    "on_actionOpen_Nary_Data_triggered\0"
    "on_actionLoad_Nary_Locations_triggered\0"
    "on_actionLoad_Hyperedge_Colors_triggered\0"
    "on_actionSave_Visualization_triggered\0"
    "on_actionSave_Visualization_with_HTML_Map_triggered\0"
    "actionDeleteEntities\0actionDeleteRelationships\0"
    "on_actionSepareteEnclosingRelationship_triggered\0"
    "on_actiondeleteEnclosingRelationships_triggered\0"
    "on_actionFindConvexHull_triggered\0"
    "on_actionOutputSubdata_triggered\0"
    "on_actionSave_ViewPoint_triggered\0"
    "on_actionLoad_ViewPoint_triggered\0"
    "on_actionSave_All_Color_Schemes_Results_triggered\0"
    "on_actionOutput_Color_Scheme_triggered\0"
    "on_actionLoad_Color_Scheme_triggered\0"
    "on_actionselectEnclosingRelationships_triggered\0"
    "on_actionClear_Gradient_Directions_triggered\0"
    "on_actionMergeEnclosedPolygon_triggered\0"
    "on_actionShow_Vis_Sample_Num_triggered\0"
    "on_actionCloseAllDockableWidgets_triggered\0"
    "on_actionFontSettings_triggered\0"
    "on_actionClear_Console_triggered\0"
    "on_actionSave_Console_triggered\0"
    "on_actionAssign_Relationships_Color_triggered\0"
    "on_actionAssign_Entities_Color_triggered\0"
    "on_actionSet_Selection_Color_triggered\0"
    "on_actionChange_Entity_Boundary_Color_triggered\0"
    "on_actionSet_Nodes_Boundary_Width_triggered\0"
    "on_actionSet_Polygon_Boundary_Width_triggered\0"
    "on_actionLights_Settings_triggered\0"
    "on_actionUse_Low_Sepecular_Lighting_triggered\0"
    "on_actionUse_High_Specular_Lighting_triggered\0"
    "on_actionRemove_PolygonsOfCard_triggered\0"
    "on_actionRemoveBinaryRelationshipsOfNary_triggered\0"
    "on_actionRemoveUnvisibleElements_triggered\0"
    "on_actionRemove_Dangling_Entities_triggered\0"
    "on_actionRemove_Dangling_Relationships_triggered\0"
    "on_actionRemove_Multiple_Diagon_triggered\0"
    "on_actionRemove_Non_repeated_Relationships_triggered\0"
    "on_actionConvert_to_Bipartite_triggered\0"
    "on_actionConvert_to_Linear_triggered\0"
    "on_actionConver_to_Simple_triggered\0"
    "on_actionConvert_to_2_Linear_triggered\0"
    "on_actionCreate_ER_Vis_triggered\0"
    "on_actionCreate_Relationships_From_Cliques_triggered\0"
    "on_actionCreate_Cliques_from_Relationships_triggered\0"
    "on_actionAssign_Entity_Labels_With_Indices_triggered\0"
    "on_actionAssign_Relationship_Labels_With_Indices_triggered\0"
    "on_actionClear_all_data_triggered\0"
    "createDualERdata\0"
    "on_actionSwitch_PrimVis_and_DualVis_triggered\0"
    "on_actionAnimate_Optimization_triggered\0"
    "on_actionAuto_Adjust_Viewpoint_triggered\0"
    "on_actionDraw_Curved_Scaffolds_triggered\0"
    "on_actionDraw_Nodes_as_Spheres_triggered\0"
    "on_actionDraw_Edges_as_Cylinders_triggered\0"
    "on_actionDraw_Axes_triggered\0"
    "on_actionDraw_Grid_triggered\0"
    "on_actionDraw_Cursor_triggered\0"
    "on_actionSelectEntity_triggered\0"
    "on_actionSelectRelationship_triggered\0"
    "on_actionSelectEntities_triggered\0"
    "on_actionMoveSelect_triggered\0"
    "on_actionaddEntity_triggered\0"
    "on_actionaddRandomEntities_triggered\0"
    "on_actionaddRelationship_triggered\0"
    "on_actionDeleteSelectedElements_triggered\0"
    "on_actionEditElement_triggered\0"
    "on_actionMergeElements_triggered\0"
    "on_actionDivideRelationship_triggered\0"
    "on_actionRemoveElementsFromRelationship_triggered\0"
    "on_actionSwap_triggered\0"
    "on_actionBringReToFront_triggered\0"
    "on_actionBringReToBack_triggered\0"
    "on_action_optimize_layout_triggered\0"
    "on_actionstopOptimization_triggered\0"
    "on_actionManual_triggered\0"
    "on_actionCreateRelationship_triggered\0"
    "on_actionSplitElements_triggered\0"
    "optimizeOrdering\0optimizeDualOrdering\0"
    "lineLayout\0reorderSelectedPolygons\0"
    "calcDegreeAndCardinality\0calcElementStatistics\0"
    "fix_verts\0focused_ERData\0vector<int>\0"
    "fixedVertIndices\0free_elements\0erdata\0"
    "freeVertIndices\0freePolyIndices\0"
    "showFixedVertInfo\0getCurrentColorGradient\0"
    "ColorGradient\0setZoom\0syncDualZooming\0"
    "syncDualElementColor\0updatePolyColors\0"
    "assignRelationshipColorsBySetIntersections\0"
    "assignElementsColorPredefined\0"
    "assignElementsColorBrewer1\0"
    "assignElementsColorBrewer2\0"
    "assignElementColorEvenHueDistance\0"
    "assignEnColorByDual\0applyReColorScheme\0"
    "applyEnColorScheme\0scaleERLayout\0scale\0"
    "recenterView\0ResetMonogonsAngles\0"
    "DistributeMonogonAngles\0"
    "removeMonogonsInPrimeVis\0"
    "removeMonogonsInDualVis\0"
    "getIndexListFromLineEdit\0"
    "getLabelListFromLineEdit\0vector<string>\0"
    "select_entities_fromInput\0"
    "select_relationships_fromInput\0"
    "Highlight_selectedElements\0"
    "Highlight_selectedElementsOnce\0"
    "applyGridLayout\0resetView\0"
    "applyGridSeparatedLayout\0applyRandomLayout\0"
    "apply3DRandomLayout\0applyCircleLayout\0"
    "applyHLBFGS\0applyHLBFGSDual\0"
    "applyHLBFGSArithopt\0applyHLBFGSArithoptDual\0"
    "applyHLBFGSMonogon\0applyHLBFGSMonogonDual\0"
    "clearFixedVert\0setFixedVert\0"
    "fixSelectedVerts\0stopOptimization\0"
    "calcBoundingCircle\0calcAverageLength\0"
    "findEdgelyAdjacentRelationshipIncidentOneEntity\0"
    "assignColor\0set_ShowEntities\0res\0"
    "set_ShowRelationships\0set_EntityLabels\0"
    "index\0set_RelationshipLabels\0"
    "set_ShowEntityTips\0set_ShowRelationshipTips\0"
    "set_ShowRelationshipDecal\0"
    "set_EntityDegreeFilter\0"
    "set_RelationshipDegreeFilter\0"
    "set_EntityCardinalityFilter\0"
    "set_polygonCardinalityFilter\0"
    "set_EntityIndexFilter\0set_RelationshipIndexFilter\0"
    "set_InheritVisibilities\0initialize_2D_vis\0"
    "set_PrimeVisWidget_focused\0"
    "set_DualVisWidget_focused\0"
    "set_BipartVisWidget_focused\0"
    "vertexMinDegreeChanged\0arg1\0"
    "vertexMaxDegreeChanged\0polyMinDegreeChanged\0"
    "polyMaxDegreeChanged\0"
    "relationshipCardinalityBottomChanged\0"
    "relationshipCardinalityTopChanged\0"
    "entityCardinalityBottomChanged\0"
    "entityCardinalityTopChanged\0"
    "optimizeIterationMaxChanged\0"
    "entityIndexBottomChanged\0entityIndexTopChanged\0"
    "relationshipIndexBottomChanged\0"
    "relationshipIndexTopChanged\0"
    "doubleSpinBox_VertexSize_valueChanged\0"
    "doubleSpinBox_MaxScaffoldCurve\0"
    "doubleSpinBox_LensAngle_valueChanged\0"
    "doubleSpinBox_MonogonAspectRatio_valueChanged\0"
    "doubleSpinBox_MonogonSize_valueChanged\0"
    "on_actionSave_CSV_Data_triggered\0"
    "on_actionOpen_CSV_Data_triggered\0"
    "on_actionSave_Decomposition_triggered\0"
    "on_actionLoad_Decomposition_triggered\0"
    "on_actionPolygon_View_triggered\0"
    "on_actionKonig_View_triggered\0"
    "apply_layout\0get_layout_transform_settings\0"
    "get_random_layout_settings\0"
    "get_grid_layout_settings\0"
    "get_circle_layout_settings\0"
    "get_force_directed_settings\0"
    "get_optimization_settings\0"
    "get_subgraph_settings\0get_simplified_settings\0"
    "generateAtomicOperations\0"
    "generateAtomicOperationsCommandLine\0"
    "alpha\0beta\0gamma\0nrem\0erem\0nmerge\0"
    "emerge\0applyNextOperation\0invertLastOperation\0"
    "applyAllOperations\0applyAllOperationsCommandLine\0"
    "std::string\0output_name\0min_edges\0"
    "min_nodes\0invertAndOptimize\0"
    "invertAndOptimizeCommandLine\0interval\0"
    "op_total\0optimizeSubgraphManually\0"
    "applySelectedOperation\0optimizeInversionsChanged\0"
    "preserveConnectionsChanged\0"
    "preserveBranchesChanged\0pruneOnlyChanged\0"
    "applyBranchClusterDecomposition\0"
    "createCluster\0ENList\0enlist\0REList\0"
    "relist\0copyERDataMergeClusters\0source\0"
    "ERClusterDecomp*\0decomp\0"
    "getMatchingIndexEntities\0ENList*\0"
    "target\0getMatchingIndexRelationships\0"
    "REList*\0copyVertexLocations\0"
    "applyCycleDecomposition\0decompHighlightChanged\0"
    "updateVisSelectedElements\0highlightNextBComp\0"
    "highlightAllBComps\0highlightNextTComp\0"
    "highlightAllTComps\0highlightNextCycle\0"
    "highlightAllCycles\0highlightNextCluster\0"
    "highlightAllClusters\0resetDecompHighlights\0"
    "startRecordingBipartOperations\0"
    "applyBipartOperation\0BIPART_OP_TYPE\0"
    "saveBipartOperations\0loadBipartOperations\0"
    "invertLastBipartOperation\0"
    "applyNextBipartOperation\0applyBipartEdgeCut\0"
    "applyBipartEdgePrune\0applyBipartK22Collapse\0"
    "applyBipartChainMerge\0checkDecompAndOpManagers\0"
    "applyNextClusterOperation\0"
    "applyClusterSimplification\0"
    "simplifyAllClusters\0applyNextBicompOperation\0"
    "simplifyAllBicomps\0applyNextTreeOperation\0"
    "simplifyAllTrees\0writeOrderedIncidenceMatrix\0"
    "writeAdjacencyMatrices\0writeERDataStatistics\0"
    "outfile\0on_actionPrint_Euler_Characteristics_triggered\0"
    "save_console\0fname"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     262,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2, 1324,    2, 0x0a /* Public */,
       5,    2, 1329,    2, 0x0a /* Public */,
       6,    2, 1334,    2, 0x0a /* Public */,
       7,    1, 1339,    2, 0x0a /* Public */,
       9,    1, 1342,    2, 0x0a /* Public */,
      12,    1, 1345,    2, 0x0a /* Public */,
      14,    1, 1348,    2, 0x0a /* Public */,
      15,    1, 1351,    2, 0x0a /* Public */,
      15,    0, 1354,    2, 0x2a /* Public | MethodCloned */,
      17,    0, 1355,    2, 0x0a /* Public */,
      18,    0, 1356,    2, 0x0a /* Public */,
      19,    0, 1357,    2, 0x0a /* Public */,
      20,    1, 1358,    2, 0x0a /* Public */,
      22,    1, 1361,    2, 0x0a /* Public */,
      25,    2, 1364,    2, 0x0a /* Public */,
      27,    2, 1369,    2, 0x0a /* Public */,
      31,    1, 1374,    2, 0x0a /* Public */,
      32,    0, 1377,    2, 0x0a /* Public */,
      33,    0, 1378,    2, 0x0a /* Public */,
      34,    0, 1379,    2, 0x0a /* Public */,
      35,    0, 1380,    2, 0x0a /* Public */,
      36,    0, 1381,    2, 0x0a /* Public */,
      37,    0, 1382,    2, 0x0a /* Public */,
      38,    0, 1383,    2, 0x0a /* Public */,
      39,    0, 1384,    2, 0x0a /* Public */,
      40,    0, 1385,    2, 0x0a /* Public */,
      41,    0, 1386,    2, 0x0a /* Public */,
      42,    0, 1387,    2, 0x0a /* Public */,
      43,    0, 1388,    2, 0x0a /* Public */,
      44,    0, 1389,    2, 0x0a /* Public */,
      45,    0, 1390,    2, 0x0a /* Public */,
      46,    0, 1391,    2, 0x0a /* Public */,
      47,    0, 1392,    2, 0x0a /* Public */,
      48,    0, 1393,    2, 0x0a /* Public */,
      49,    0, 1394,    2, 0x0a /* Public */,
      50,    0, 1395,    2, 0x0a /* Public */,
      51,    0, 1396,    2, 0x0a /* Public */,
      52,    0, 1397,    2, 0x0a /* Public */,
      53,    0, 1398,    2, 0x0a /* Public */,
      54,    0, 1399,    2, 0x0a /* Public */,
      55,    0, 1400,    2, 0x0a /* Public */,
      56,    0, 1401,    2, 0x0a /* Public */,
      57,    0, 1402,    2, 0x0a /* Public */,
      58,    0, 1403,    2, 0x0a /* Public */,
      59,    0, 1404,    2, 0x0a /* Public */,
      60,    0, 1405,    2, 0x0a /* Public */,
      61,    0, 1406,    2, 0x0a /* Public */,
      62,    0, 1407,    2, 0x0a /* Public */,
      63,    0, 1408,    2, 0x0a /* Public */,
      64,    0, 1409,    2, 0x0a /* Public */,
      65,    0, 1410,    2, 0x0a /* Public */,
      66,    0, 1411,    2, 0x0a /* Public */,
      67,    0, 1412,    2, 0x0a /* Public */,
      68,    0, 1413,    2, 0x0a /* Public */,
      69,    0, 1414,    2, 0x0a /* Public */,
      70,    0, 1415,    2, 0x0a /* Public */,
      71,    0, 1416,    2, 0x0a /* Public */,
      72,    0, 1417,    2, 0x0a /* Public */,
      73,    0, 1418,    2, 0x0a /* Public */,
      74,    0, 1419,    2, 0x0a /* Public */,
      75,    0, 1420,    2, 0x0a /* Public */,
      76,    0, 1421,    2, 0x0a /* Public */,
      77,    0, 1422,    2, 0x0a /* Public */,
      78,    0, 1423,    2, 0x0a /* Public */,
      79,    0, 1424,    2, 0x0a /* Public */,
      80,    0, 1425,    2, 0x0a /* Public */,
      81,    0, 1426,    2, 0x0a /* Public */,
      82,    0, 1427,    2, 0x0a /* Public */,
      83,    0, 1428,    2, 0x0a /* Public */,
      84,    0, 1429,    2, 0x0a /* Public */,
      85,    1, 1430,    2, 0x0a /* Public */,
      86,    1, 1433,    2, 0x0a /* Public */,
      87,    1, 1436,    2, 0x0a /* Public */,
      88,    1, 1439,    2, 0x0a /* Public */,
      89,    1, 1442,    2, 0x0a /* Public */,
      90,    1, 1445,    2, 0x0a /* Public */,
      91,    1, 1448,    2, 0x0a /* Public */,
      92,    1, 1451,    2, 0x0a /* Public */,
      93,    1, 1454,    2, 0x0a /* Public */,
      94,    1, 1457,    2, 0x0a /* Public */,
      95,    1, 1460,    2, 0x0a /* Public */,
      96,    1, 1463,    2, 0x0a /* Public */,
      97,    1, 1466,    2, 0x0a /* Public */,
      98,    0, 1469,    2, 0x0a /* Public */,
      99,    0, 1470,    2, 0x0a /* Public */,
     100,    0, 1471,    2, 0x0a /* Public */,
     101,    0, 1472,    2, 0x0a /* Public */,
     102,    0, 1473,    2, 0x0a /* Public */,
     103,    0, 1474,    2, 0x0a /* Public */,
     104,    0, 1475,    2, 0x0a /* Public */,
     105,    0, 1476,    2, 0x0a /* Public */,
     106,    0, 1477,    2, 0x0a /* Public */,
     107,    0, 1478,    2, 0x0a /* Public */,
     108,    0, 1479,    2, 0x0a /* Public */,
     109,    0, 1480,    2, 0x0a /* Public */,
     110,    0, 1481,    2, 0x0a /* Public */,
     111,    0, 1482,    2, 0x0a /* Public */,
     112,    0, 1483,    2, 0x0a /* Public */,
     113,    0, 1484,    2, 0x0a /* Public */,
     114,    0, 1485,    2, 0x0a /* Public */,
     115,    0, 1486,    2, 0x0a /* Public */,
     116,    0, 1487,    2, 0x0a /* Public */,
     117,    0, 1488,    2, 0x0a /* Public */,
     118,    0, 1489,    2, 0x0a /* Public */,
     119,    2, 1490,    2, 0x0a /* Public */,
     123,    3, 1495,    2, 0x0a /* Public */,
     123,    1, 1502,    2, 0x0a /* Public */,
     127,    1, 1505,    2, 0x0a /* Public */,
     128,    0, 1508,    2, 0x0a /* Public */,
     130,    0, 1509,    2, 0x0a /* Public */,
     131,    0, 1510,    2, 0x0a /* Public */,
     132,    0, 1511,    2, 0x0a /* Public */,
     133,    0, 1512,    2, 0x0a /* Public */,
     134,    0, 1513,    2, 0x0a /* Public */,
     135,    0, 1514,    2, 0x0a /* Public */,
     136,    0, 1515,    2, 0x0a /* Public */,
     137,    0, 1516,    2, 0x0a /* Public */,
     138,    0, 1517,    2, 0x0a /* Public */,
     139,    0, 1518,    2, 0x0a /* Public */,
     140,    0, 1519,    2, 0x0a /* Public */,
     141,    0, 1520,    2, 0x0a /* Public */,
     142,    2, 1521,    2, 0x0a /* Public */,
     144,    0, 1526,    2, 0x0a /* Public */,
     145,    0, 1527,    2, 0x0a /* Public */,
     146,    0, 1528,    2, 0x0a /* Public */,
     147,    0, 1529,    2, 0x0a /* Public */,
     148,    0, 1530,    2, 0x0a /* Public */,
     149,    0, 1531,    2, 0x0a /* Public */,
     150,    0, 1532,    2, 0x0a /* Public */,
     152,    0, 1533,    2, 0x0a /* Public */,
     153,    0, 1534,    2, 0x0a /* Public */,
     154,    0, 1535,    2, 0x0a /* Public */,
     155,    0, 1536,    2, 0x0a /* Public */,
     156,    0, 1537,    2, 0x0a /* Public */,
     157,    0, 1538,    2, 0x0a /* Public */,
     158,    0, 1539,    2, 0x0a /* Public */,
     159,    0, 1540,    2, 0x0a /* Public */,
     160,    0, 1541,    2, 0x0a /* Public */,
     161,    0, 1542,    2, 0x0a /* Public */,
     162,    0, 1543,    2, 0x0a /* Public */,
     163,    0, 1544,    2, 0x0a /* Public */,
     164,    0, 1545,    2, 0x0a /* Public */,
     165,    0, 1546,    2, 0x0a /* Public */,
     166,    0, 1547,    2, 0x0a /* Public */,
     167,    0, 1548,    2, 0x0a /* Public */,
     168,    0, 1549,    2, 0x0a /* Public */,
     169,    0, 1550,    2, 0x0a /* Public */,
     170,    0, 1551,    2, 0x0a /* Public */,
     171,    0, 1552,    2, 0x0a /* Public */,
     172,    0, 1553,    2, 0x0a /* Public */,
     173,    0, 1554,    2, 0x0a /* Public */,
     174,    0, 1555,    2, 0x0a /* Public */,
     175,    0, 1556,    2, 0x0a /* Public */,
     176,    1, 1557,    2, 0x0a /* Public */,
     178,    1, 1560,    2, 0x0a /* Public */,
     179,    1, 1563,    2, 0x0a /* Public */,
     181,    1, 1566,    2, 0x0a /* Public */,
     182,    1, 1569,    2, 0x0a /* Public */,
     183,    1, 1572,    2, 0x0a /* Public */,
     184,    1, 1575,    2, 0x0a /* Public */,
     185,    1, 1578,    2, 0x0a /* Public */,
     186,    1, 1581,    2, 0x0a /* Public */,
     187,    1, 1584,    2, 0x0a /* Public */,
     188,    1, 1587,    2, 0x0a /* Public */,
     189,    1, 1590,    2, 0x0a /* Public */,
     190,    1, 1593,    2, 0x0a /* Public */,
     191,    1, 1596,    2, 0x0a /* Public */,
     192,    0, 1599,    2, 0x0a /* Public */,
     193,    1, 1600,    2, 0x0a /* Public */,
     194,    1, 1603,    2, 0x0a /* Public */,
     195,    1, 1606,    2, 0x0a /* Public */,
     196,    1, 1609,    2, 0x0a /* Public */,
     198,    1, 1612,    2, 0x0a /* Public */,
     199,    1, 1615,    2, 0x0a /* Public */,
     200,    1, 1618,    2, 0x0a /* Public */,
     201,    1, 1621,    2, 0x0a /* Public */,
     202,    1, 1624,    2, 0x0a /* Public */,
     203,    1, 1627,    2, 0x0a /* Public */,
     204,    1, 1630,    2, 0x0a /* Public */,
     205,    1, 1633,    2, 0x0a /* Public */,
     206,    1, 1636,    2, 0x0a /* Public */,
     207,    1, 1639,    2, 0x0a /* Public */,
     208,    1, 1642,    2, 0x0a /* Public */,
     209,    1, 1645,    2, 0x0a /* Public */,
     210,    1, 1648,    2, 0x0a /* Public */,
     211,    1, 1651,    2, 0x0a /* Public */,
     212,    1, 1654,    2, 0x0a /* Public */,
     213,    1, 1657,    2, 0x0a /* Public */,
     214,    1, 1660,    2, 0x0a /* Public */,
     215,    0, 1663,    2, 0x0a /* Public */,
     216,    0, 1664,    2, 0x0a /* Public */,
     217,    0, 1665,    2, 0x0a /* Public */,
     218,    0, 1666,    2, 0x0a /* Public */,
     219,    0, 1667,    2, 0x0a /* Public */,
     220,    0, 1668,    2, 0x0a /* Public */,
     221,    0, 1669,    2, 0x0a /* Public */,
     222,    0, 1670,    2, 0x0a /* Public */,
     223,    0, 1671,    2, 0x0a /* Public */,
     224,    0, 1672,    2, 0x0a /* Public */,
     225,    0, 1673,    2, 0x0a /* Public */,
     226,    0, 1674,    2, 0x0a /* Public */,
     227,    0, 1675,    2, 0x0a /* Public */,
     228,    0, 1676,    2, 0x0a /* Public */,
     229,    0, 1677,    2, 0x0a /* Public */,
     230,    0, 1678,    2, 0x0a /* Public */,
     231,    7, 1679,    2, 0x0a /* Public */,
     239,    0, 1694,    2, 0x0a /* Public */,
     240,    0, 1695,    2, 0x0a /* Public */,
     241,    0, 1696,    2, 0x0a /* Public */,
     242,    3, 1697,    2, 0x0a /* Public */,
     247,    0, 1704,    2, 0x0a /* Public */,
     248,    3, 1705,    2, 0x0a /* Public */,
     248,    2, 1712,    2, 0x2a /* Public | MethodCloned */,
     248,    1, 1717,    2, 0x2a /* Public | MethodCloned */,
     251,    0, 1720,    2, 0x0a /* Public */,
     252,    0, 1721,    2, 0x0a /* Public */,
     253,    0, 1722,    2, 0x0a /* Public */,
     254,    0, 1723,    2, 0x0a /* Public */,
     255,    0, 1724,    2, 0x0a /* Public */,
     256,    0, 1725,    2, 0x0a /* Public */,
     257,    0, 1726,    2, 0x0a /* Public */,
     258,    2, 1727,    2, 0x0a /* Public */,
     263,    2, 1732,    2, 0x0a /* Public */,
     267,    2, 1737,    2, 0x0a /* Public */,
     270,    2, 1742,    2, 0x0a /* Public */,
     272,    2, 1747,    2, 0x0a /* Public */,
     273,    0, 1752,    2, 0x0a /* Public */,
     274,    0, 1753,    2, 0x0a /* Public */,
     275,    0, 1754,    2, 0x0a /* Public */,
     276,    0, 1755,    2, 0x0a /* Public */,
     277,    0, 1756,    2, 0x0a /* Public */,
     278,    0, 1757,    2, 0x0a /* Public */,
     279,    0, 1758,    2, 0x0a /* Public */,
     280,    0, 1759,    2, 0x0a /* Public */,
     281,    0, 1760,    2, 0x0a /* Public */,
     282,    0, 1761,    2, 0x0a /* Public */,
     283,    0, 1762,    2, 0x0a /* Public */,
     284,    0, 1763,    2, 0x0a /* Public */,
     285,    0, 1764,    2, 0x0a /* Public */,
     286,    1, 1765,    2, 0x0a /* Public */,
     288,    0, 1768,    2, 0x0a /* Public */,
     289,    0, 1769,    2, 0x0a /* Public */,
     290,    0, 1770,    2, 0x0a /* Public */,
     291,    0, 1771,    2, 0x0a /* Public */,
     292,    0, 1772,    2, 0x0a /* Public */,
     293,    0, 1773,    2, 0x0a /* Public */,
     294,    0, 1774,    2, 0x0a /* Public */,
     295,    0, 1775,    2, 0x0a /* Public */,
     296,    0, 1776,    2, 0x0a /* Public */,
     297,    0, 1777,    2, 0x0a /* Public */,
     298,    0, 1778,    2, 0x0a /* Public */,
     299,    0, 1779,    2, 0x0a /* Public */,
     300,    0, 1780,    2, 0x0a /* Public */,
     301,    0, 1781,    2, 0x0a /* Public */,
     302,    0, 1782,    2, 0x0a /* Public */,
     303,    0, 1783,    2, 0x0a /* Public */,
     304,    0, 1784,    2, 0x0a /* Public */,
     305,    0, 1785,    2, 0x0a /* Public */,
     306,    1, 1786,    2, 0x0a /* Public */,
     306,    0, 1789,    2, 0x2a /* Public | MethodCloned */,
     308,    0, 1790,    2, 0x0a /* Public */,
     309,    1, 1791,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   13,
    QMetaType::Void, 0x80000000 | 10,   13,
    QMetaType::Void, QMetaType::Bool,   16,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::QString,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, 0x80000000 | 23,   24,
    QMetaType::Void, 0x80000000 | 23, QMetaType::Bool,   24,   26,
    QMetaType::Bool, 0x80000000 | 28, 0x80000000 | 23,   29,   30,
    QMetaType::Bool, 0x80000000 | 28,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 121,  120,  122,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 121, 0x80000000 | 121,  124,  125,  126,
    QMetaType::Void, 0x80000000 | 8,  124,
    QMetaType::Void, 0x80000000 | 8,  124,
    0x80000000 | 129,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Double,  124,  143,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 121,
    0x80000000 | 151,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,  177,
    QMetaType::Void, QMetaType::Bool,  177,
    QMetaType::Void, QMetaType::Int,  180,
    QMetaType::Void, QMetaType::Int,  180,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Bool,   26,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Int,  197,
    QMetaType::Void, QMetaType::Double,  197,
    QMetaType::Void, QMetaType::Double,  197,
    QMetaType::Void, QMetaType::Double,  197,
    QMetaType::Void, QMetaType::Double,  197,
    QMetaType::Void, QMetaType::Double,  197,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool,  232,  233,  234,  235,  236,  237,  238,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, 0x80000000 | 243, QMetaType::Int, QMetaType::Int,  244,  245,  246,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 243, QMetaType::Int, QMetaType::Int,  244,  249,  250,
    QMetaType::Void, 0x80000000 | 243, QMetaType::Int,  244,  249,
    QMetaType::Void, 0x80000000 | 243,  244,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 259, 0x80000000 | 261,  260,  262,
    0x80000000 | 8, 0x80000000 | 8, 0x80000000 | 265,  264,  266,
    QMetaType::Bool, 0x80000000 | 268, 0x80000000 | 8,  264,  269,
    QMetaType::Bool, 0x80000000 | 271, 0x80000000 | 8,  264,  269,
    QMetaType::Bool, 0x80000000 | 8, 0x80000000 | 8,  264,  269,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 287,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 243,  307,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 243,  310,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateMouseLocationLabel((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->updateprimeVisSizeLabel((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->updatedualVisSizeLabel((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->updateERdataInfo((*reinterpret_cast< ERData*(*)>(_a[1]))); break;
        case 4: _t->focusedVisChanged((*reinterpret_cast< VISTYPE(*)>(_a[1]))); break;
        case 5: _t->updatefocusVisLabel((*reinterpret_cast< VISTYPE(*)>(_a[1]))); break;
        case 6: _t->syncSelectedElements((*reinterpret_cast< VISTYPE(*)>(_a[1]))); break;
        case 7: _t->ERDataChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->ERDataChanged(); break;
        case 9: { bool _r = _t->dualVisVisible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 10: { bool _r = _t->bipartVisVisible();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 11: { QString _r = _t->getOpenDir();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 12: _t->setOpenDir((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->selectionActionsFresh((*reinterpret_cast< OGLWidget*(*)>(_a[1]))); break;
        case 14: _t->setVisSelectEntity((*reinterpret_cast< OGLWidget*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 15: { bool _r = _t->OpenERFile((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< OGLWidget*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 16: { bool _r = _t->SaveERFile((*reinterpret_cast< const char*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 17: _t->on_actionSave_Nary_Data_triggered(); break;
        case 18: _t->on_actionOpen_Nary_Data_triggered(); break;
        case 19: _t->on_actionLoad_Nary_Locations_triggered(); break;
        case 20: _t->on_actionLoad_Hyperedge_Colors_triggered(); break;
        case 21: _t->on_actionSave_Visualization_triggered(); break;
        case 22: _t->on_actionSave_Visualization_with_HTML_Map_triggered(); break;
        case 23: _t->actionDeleteEntities(); break;
        case 24: _t->actionDeleteRelationships(); break;
        case 25: _t->on_actionSepareteEnclosingRelationship_triggered(); break;
        case 26: _t->on_actiondeleteEnclosingRelationships_triggered(); break;
        case 27: _t->on_actionFindConvexHull_triggered(); break;
        case 28: _t->on_actionOutputSubdata_triggered(); break;
        case 29: _t->on_actionSave_ViewPoint_triggered(); break;
        case 30: _t->on_actionLoad_ViewPoint_triggered(); break;
        case 31: _t->on_actionSave_All_Color_Schemes_Results_triggered(); break;
        case 32: _t->on_actionOutput_Color_Scheme_triggered(); break;
        case 33: _t->on_actionLoad_Color_Scheme_triggered(); break;
        case 34: _t->on_actionselectEnclosingRelationships_triggered(); break;
        case 35: _t->on_actionClear_Gradient_Directions_triggered(); break;
        case 36: _t->on_actionMergeEnclosedPolygon_triggered(); break;
        case 37: _t->on_actionShow_Vis_Sample_Num_triggered(); break;
        case 38: _t->on_actionCloseAllDockableWidgets_triggered(); break;
        case 39: _t->on_actionFontSettings_triggered(); break;
        case 40: _t->on_actionClear_Console_triggered(); break;
        case 41: _t->on_actionSave_Console_triggered(); break;
        case 42: _t->on_actionAssign_Relationships_Color_triggered(); break;
        case 43: _t->on_actionAssign_Entities_Color_triggered(); break;
        case 44: _t->on_actionSet_Selection_Color_triggered(); break;
        case 45: _t->on_actionChange_Entity_Boundary_Color_triggered(); break;
        case 46: _t->on_actionSet_Nodes_Boundary_Width_triggered(); break;
        case 47: _t->on_actionSet_Polygon_Boundary_Width_triggered(); break;
        case 48: _t->on_actionLights_Settings_triggered(); break;
        case 49: _t->on_actionUse_Low_Sepecular_Lighting_triggered(); break;
        case 50: _t->on_actionUse_High_Specular_Lighting_triggered(); break;
        case 51: _t->on_actionRemove_PolygonsOfCard_triggered(); break;
        case 52: _t->on_actionRemoveBinaryRelationshipsOfNary_triggered(); break;
        case 53: _t->on_actionRemoveUnvisibleElements_triggered(); break;
        case 54: _t->on_actionRemove_Dangling_Entities_triggered(); break;
        case 55: _t->on_actionRemove_Dangling_Relationships_triggered(); break;
        case 56: _t->on_actionRemove_Multiple_Diagon_triggered(); break;
        case 57: _t->on_actionRemove_Non_repeated_Relationships_triggered(); break;
        case 58: _t->on_actionConvert_to_Bipartite_triggered(); break;
        case 59: _t->on_actionConvert_to_Linear_triggered(); break;
        case 60: _t->on_actionConver_to_Simple_triggered(); break;
        case 61: _t->on_actionConvert_to_2_Linear_triggered(); break;
        case 62: _t->on_actionCreate_ER_Vis_triggered(); break;
        case 63: _t->on_actionCreate_Relationships_From_Cliques_triggered(); break;
        case 64: _t->on_actionCreate_Cliques_from_Relationships_triggered(); break;
        case 65: _t->on_actionAssign_Entity_Labels_With_Indices_triggered(); break;
        case 66: _t->on_actionAssign_Relationship_Labels_With_Indices_triggered(); break;
        case 67: _t->on_actionClear_all_data_triggered(); break;
        case 68: _t->createDualERdata(); break;
        case 69: _t->on_actionSwitch_PrimVis_and_DualVis_triggered(); break;
        case 70: _t->on_actionAnimate_Optimization_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 71: _t->on_actionAuto_Adjust_Viewpoint_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 72: _t->on_actionDraw_Curved_Scaffolds_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 73: _t->on_actionDraw_Nodes_as_Spheres_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 74: _t->on_actionDraw_Edges_as_Cylinders_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 75: _t->on_actionDraw_Axes_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 76: _t->on_actionDraw_Grid_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 77: _t->on_actionDraw_Cursor_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 78: _t->on_actionSelectEntity_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 79: _t->on_actionSelectRelationship_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 80: _t->on_actionSelectEntities_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 81: _t->on_actionMoveSelect_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 82: _t->on_actionaddEntity_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 83: _t->on_actionaddRandomEntities_triggered(); break;
        case 84: _t->on_actionaddRelationship_triggered(); break;
        case 85: _t->on_actionDeleteSelectedElements_triggered(); break;
        case 86: _t->on_actionEditElement_triggered(); break;
        case 87: _t->on_actionMergeElements_triggered(); break;
        case 88: _t->on_actionDivideRelationship_triggered(); break;
        case 89: _t->on_actionRemoveElementsFromRelationship_triggered(); break;
        case 90: _t->on_actionSwap_triggered(); break;
        case 91: _t->on_actionBringReToFront_triggered(); break;
        case 92: _t->on_actionBringReToBack_triggered(); break;
        case 93: _t->on_action_optimize_layout_triggered(); break;
        case 94: _t->on_actionstopOptimization_triggered(); break;
        case 95: _t->on_actionManual_triggered(); break;
        case 96: _t->on_actionCreateRelationship_triggered(); break;
        case 97: _t->on_actionSplitElements_triggered(); break;
        case 98: _t->optimizeOrdering(); break;
        case 99: _t->optimizeDualOrdering(); break;
        case 100: _t->lineLayout(); break;
        case 101: _t->reorderSelectedPolygons(); break;
        case 102: _t->calcDegreeAndCardinality(); break;
        case 103: _t->calcElementStatistics(); break;
        case 104: _t->fix_verts((*reinterpret_cast< ERData*(*)>(_a[1])),(*reinterpret_cast< vector<int>(*)>(_a[2]))); break;
        case 105: _t->free_elements((*reinterpret_cast< ERData*(*)>(_a[1])),(*reinterpret_cast< vector<int>(*)>(_a[2])),(*reinterpret_cast< vector<int>(*)>(_a[3]))); break;
        case 106: _t->free_elements((*reinterpret_cast< ERData*(*)>(_a[1]))); break;
        case 107: _t->showFixedVertInfo((*reinterpret_cast< ERData*(*)>(_a[1]))); break;
        case 108: { ColorGradient _r = _t->getCurrentColorGradient();
            if (_a[0]) *reinterpret_cast< ColorGradient*>(_a[0]) = std::move(_r); }  break;
        case 109: _t->setZoom(); break;
        case 110: _t->syncDualZooming(); break;
        case 111: _t->syncDualElementColor(); break;
        case 112: _t->updatePolyColors(); break;
        case 113: _t->assignRelationshipColorsBySetIntersections(); break;
        case 114: _t->assignElementsColorPredefined(); break;
        case 115: _t->assignElementsColorBrewer1(); break;
        case 116: _t->assignElementsColorBrewer2(); break;
        case 117: _t->assignElementColorEvenHueDistance(); break;
        case 118: _t->assignEnColorByDual(); break;
        case 119: _t->applyReColorScheme(); break;
        case 120: _t->applyEnColorScheme(); break;
        case 121: _t->scaleERLayout((*reinterpret_cast< ERData*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 122: _t->recenterView(); break;
        case 123: _t->ResetMonogonsAngles(); break;
        case 124: _t->DistributeMonogonAngles(); break;
        case 125: _t->removeMonogonsInPrimeVis(); break;
        case 126: _t->removeMonogonsInDualVis(); break;
        case 127: { vector<int> _r = _t->getIndexListFromLineEdit();
            if (_a[0]) *reinterpret_cast< vector<int>*>(_a[0]) = std::move(_r); }  break;
        case 128: { vector<string> _r = _t->getLabelListFromLineEdit();
            if (_a[0]) *reinterpret_cast< vector<string>*>(_a[0]) = std::move(_r); }  break;
        case 129: _t->select_entities_fromInput(); break;
        case 130: _t->select_relationships_fromInput(); break;
        case 131: _t->Highlight_selectedElements(); break;
        case 132: _t->Highlight_selectedElementsOnce(); break;
        case 133: _t->applyGridLayout(); break;
        case 134: _t->resetView(); break;
        case 135: _t->applyGridSeparatedLayout(); break;
        case 136: _t->applyRandomLayout(); break;
        case 137: _t->apply3DRandomLayout(); break;
        case 138: _t->applyCircleLayout(); break;
        case 139: _t->applyHLBFGS(); break;
        case 140: _t->applyHLBFGSDual(); break;
        case 141: _t->applyHLBFGSArithopt(); break;
        case 142: _t->applyHLBFGSArithoptDual(); break;
        case 143: _t->applyHLBFGSMonogon(); break;
        case 144: _t->applyHLBFGSMonogonDual(); break;
        case 145: _t->clearFixedVert(); break;
        case 146: _t->setFixedVert(); break;
        case 147: _t->fixSelectedVerts(); break;
        case 148: _t->stopOptimization(); break;
        case 149: _t->calcBoundingCircle(); break;
        case 150: _t->calcAverageLength(); break;
        case 151: _t->findEdgelyAdjacentRelationshipIncidentOneEntity(); break;
        case 152: _t->assignColor(); break;
        case 153: _t->set_ShowEntities((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 154: _t->set_ShowRelationships((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 155: _t->set_EntityLabels((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 156: _t->set_RelationshipLabels((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 157: _t->set_ShowEntityTips((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 158: _t->set_ShowRelationshipTips((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 159: _t->set_ShowRelationshipDecal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 160: _t->set_EntityDegreeFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 161: _t->set_RelationshipDegreeFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 162: _t->set_EntityCardinalityFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 163: _t->set_polygonCardinalityFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 164: _t->set_EntityIndexFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 165: _t->set_RelationshipIndexFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 166: _t->set_InheritVisibilities((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 167: _t->initialize_2D_vis(); break;
        case 168: _t->set_PrimeVisWidget_focused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 169: _t->set_DualVisWidget_focused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 170: _t->set_BipartVisWidget_focused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 171: _t->vertexMinDegreeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 172: _t->vertexMaxDegreeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 173: _t->polyMinDegreeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 174: _t->polyMaxDegreeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 175: _t->relationshipCardinalityBottomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 176: _t->relationshipCardinalityTopChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 177: _t->entityCardinalityBottomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 178: _t->entityCardinalityTopChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 179: _t->optimizeIterationMaxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 180: _t->entityIndexBottomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 181: _t->entityIndexTopChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 182: _t->relationshipIndexBottomChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 183: _t->relationshipIndexTopChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 184: _t->doubleSpinBox_VertexSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 185: _t->doubleSpinBox_MaxScaffoldCurve((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 186: _t->doubleSpinBox_LensAngle_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 187: _t->doubleSpinBox_MonogonAspectRatio_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 188: _t->doubleSpinBox_MonogonSize_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 189: _t->on_actionSave_CSV_Data_triggered(); break;
        case 190: _t->on_actionOpen_CSV_Data_triggered(); break;
        case 191: _t->on_actionSave_Decomposition_triggered(); break;
        case 192: _t->on_actionLoad_Decomposition_triggered(); break;
        case 193: _t->on_actionPolygon_View_triggered(); break;
        case 194: _t->on_actionKonig_View_triggered(); break;
        case 195: _t->apply_layout(); break;
        case 196: _t->get_layout_transform_settings(); break;
        case 197: _t->get_random_layout_settings(); break;
        case 198: _t->get_grid_layout_settings(); break;
        case 199: _t->get_circle_layout_settings(); break;
        case 200: _t->get_force_directed_settings(); break;
        case 201: _t->get_optimization_settings(); break;
        case 202: _t->get_subgraph_settings(); break;
        case 203: _t->get_simplified_settings(); break;
        case 204: _t->generateAtomicOperations(); break;
        case 205: _t->generateAtomicOperationsCommandLine((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< bool(*)>(_a[7]))); break;
        case 206: _t->applyNextOperation(); break;
        case 207: _t->invertLastOperation(); break;
        case 208: _t->applyAllOperations(); break;
        case 209: { int _r = _t->applyAllOperationsCommandLine((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 210: _t->invertAndOptimize(); break;
        case 211: _t->invertAndOptimizeCommandLine((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 212: _t->invertAndOptimizeCommandLine((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 213: _t->invertAndOptimizeCommandLine((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 214: _t->optimizeSubgraphManually(); break;
        case 215: _t->applySelectedOperation(); break;
        case 216: _t->optimizeInversionsChanged(); break;
        case 217: _t->preserveConnectionsChanged(); break;
        case 218: _t->preserveBranchesChanged(); break;
        case 219: _t->pruneOnlyChanged(); break;
        case 220: _t->applyBranchClusterDecomposition(); break;
        case 221: _t->createCluster((*reinterpret_cast< ENList(*)>(_a[1])),(*reinterpret_cast< REList(*)>(_a[2]))); break;
        case 222: { ERData* _r = _t->copyERDataMergeClusters((*reinterpret_cast< ERData*(*)>(_a[1])),(*reinterpret_cast< ERClusterDecomp*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< ERData**>(_a[0]) = std::move(_r); }  break;
        case 223: { bool _r = _t->getMatchingIndexEntities((*reinterpret_cast< ENList*(*)>(_a[1])),(*reinterpret_cast< ERData*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 224: { bool _r = _t->getMatchingIndexRelationships((*reinterpret_cast< REList*(*)>(_a[1])),(*reinterpret_cast< ERData*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 225: { bool _r = _t->copyVertexLocations((*reinterpret_cast< ERData*(*)>(_a[1])),(*reinterpret_cast< ERData*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 226: _t->applyCycleDecomposition(); break;
        case 227: { bool _r = _t->decompHighlightChanged();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 228: _t->updateVisSelectedElements(); break;
        case 229: _t->highlightNextBComp(); break;
        case 230: _t->highlightAllBComps(); break;
        case 231: _t->highlightNextTComp(); break;
        case 232: _t->highlightAllTComps(); break;
        case 233: _t->highlightNextCycle(); break;
        case 234: _t->highlightAllCycles(); break;
        case 235: _t->highlightNextCluster(); break;
        case 236: _t->highlightAllClusters(); break;
        case 237: _t->resetDecompHighlights(); break;
        case 238: _t->startRecordingBipartOperations(); break;
        case 239: _t->applyBipartOperation((*reinterpret_cast< BIPART_OP_TYPE(*)>(_a[1]))); break;
        case 240: _t->saveBipartOperations(); break;
        case 241: _t->loadBipartOperations(); break;
        case 242: _t->invertLastBipartOperation(); break;
        case 243: _t->applyNextBipartOperation(); break;
        case 244: _t->applyBipartEdgeCut(); break;
        case 245: _t->applyBipartEdgePrune(); break;
        case 246: _t->applyBipartK22Collapse(); break;
        case 247: _t->applyBipartChainMerge(); break;
        case 248: { bool _r = _t->checkDecompAndOpManagers();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 249: { bool _r = _t->applyNextClusterOperation();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 250: _t->applyClusterSimplification(); break;
        case 251: _t->simplifyAllClusters(); break;
        case 252: _t->applyNextBicompOperation(); break;
        case 253: _t->simplifyAllBicomps(); break;
        case 254: _t->applyNextTreeOperation(); break;
        case 255: _t->simplifyAllTrees(); break;
        case 256: _t->writeOrderedIncidenceMatrix(); break;
        case 257: _t->writeAdjacencyMatrices(); break;
        case 258: _t->writeERDataStatistics((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        case 259: _t->writeERDataStatistics(); break;
        case 260: _t->on_actionPrint_Euler_Characteristics_triggered(); break;
        case 261: _t->save_console((*reinterpret_cast< std::string(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 262)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 262;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 262)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 262;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
