#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QActionGroup>
#include <QProgressBar>
#include <QLabel>
#include <omp.h>

#include <utility> 
#include <queue>
#include <memory>
#include "string"

#include "JPMath/Vector3.h"
#include "JPMath/Vector4.h"
#include "ERData.h"
#include "globalVariables.h"
#include "OptimizationHLBFGS.h"
#include "EnergyEvaluation.h"
#include "ERProgressive.h"
#include "ERClusters.h"
#include "ERDecomposition.h"
#include "BipartOpManager.h"

namespace Ui {
class MainWindow;
}

class OGLWidget;
class DockControl;
class FontSettingDialog;
class ModifyElementDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    Ui::MainWindow *ui;

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



	//////////////// Objects ////////////////

	// Data
	ERData *m_ERData = NULL;
	ERData *m_Dual_ERData = NULL;
	ERProgressive* m_ERProgressive = NULL;
	ERCycleDecomp* m_ERCycleDecomp = NULL;
	BipartOpManager* m_BOptManager = NULL;
	std::string m_PATH = "..//Data";
	std::string m_FILENAME;

	// Custom Widgets
	OGLWidget *primeVis;
	OGLWidget *dualVis;
	OGLWidget* bipartVis;
	OGLWidget *focusedVis = NULL;
	DockControl *control_dock = NULL;
	FontSettingDialog *fontsettingDlg;
	ModifyElementDialog *modifySelectedElementsDlg;	
	QProgressBar *progressBar;
	QLabel *MouseLocationLabel;
	QLabel *ERDataInfoLabel = NULL;
	QLabel *primeVisSizeLabel;
	QLabel *dualVisSizeLabel;
	QLabel *focusVisLabel;
	QActionGroup* selectionActionGroup;



	//////////////// flags ////////////////

	// File I/O flags
	bool flag_saveImageAlpha = true;

	// Visualization flags
	bool flag_showPolyBoundary = true;
	bool flag_showPolyFill = true;
	bool flag_drawPolygonWithShading = false;
    bool flag_drawPolyCurvedScaffolds = true;
	bool flag_drawEdgeCylinder = false;
	bool flag_drawNodeSphere = false;
	bool flag_drawPlaneAxes = false;
	bool flag_drawPlaneGrid = false;
	bool flag_drawCursor = false;
    bool flag_drawVertexPrincipleDirection = false;
	bool flag_drawCrossOnFixedElements = true;

	// Optimization Flags
	bool flag_stopOptimization = false;
	bool flag_animateOptimization = true;
	bool flag_autoAdjustViewpoint = false;
	bool flag_HLBFGS_stopWhenConverged = true;
	bool flag_optimizeWithMonogons = true;
	bool flag_optimizeWithReordering = false;
	bool flag_jointOptimize = true;
	bool flag_showGradientDirs = false;
	bool flag_showLSearchDirs = false;
	bool flag_showOptimizationInfo = true;
	
	// Multiscale Flags
	bool flag_save_multiscale = false;
	bool flag_optimizeInversions = true;
	bool flag_preserveConnections = true;
	bool flag_preserveBranches = true;
	bool flag_pruneOnly = true;
	bool flag_showCommandLineMultiscaleProgress = false;

	// Decomposition Flags
	bool flag_applyLayoutsToClusterMergedCopy = true;
	bool flag_subgraphsInheritColor = true;


	//////////////// Settings ////////////////

	// Visualization Settings
	double m_scaffoldMaxCurve = 0.1;

	// Layout Transform Settings
	bool TR_translation = false;
	double TR_translate_x = 0.0;
	double TR_translate_y = 0.0;
	bool TR_rotation = false;
	double TR_rotate_angle = 0.0;
	bool TR_scaling = false;
	double TR_scale_x = 1.0;
	double TR_scale_y = 1.0;
	bool TR_reflection = false;
	double TR_reflect_angle = 0.0;

	// Grid Layout Settings
	int GR_dimension = 10;
	bool GR_col_wise = true;
	double GR_spacing_x = 0;
	double GR_spacing_y = 0;

	// Circle Layout Settings
	double CL_radius = 5.0;
	ELEMENT_STATISTIC CL_sort = ELEM_INDEX;
	bool CL_ascending = true;
	double CL_start_angle = 0.0;

	// Random Layout Settings
	bool RD_rect_domain = false;
	double RD_rad_min = 0.0;
	double RD_rad_max = 5.0;

	// Force Directed Settings
	double FD_step = 0.5;
	double FD_relative_coef = 0.2;		// regulate the repulsive force
	double FD_edge_length = 0.5;		// optimal edge length
	int FD_iterations = 500;
	double FD_div_pow = 2.8;
	double FD_tolerance = 0.00001;		// energy tolerance

	// HLBFGS optimization Settings
	double HLBFGS_linesearch_ftol = 1.0e-8;
	double HLBFGS_linesearch_xtol = 1.0e-16;
	double HLBFGS_linesearch_gtol = 0.9;
	double HLBFGS_linesearch_stepmin = 1.0e-20;
	double HLBFGS_linesearch_stepmax = 1.0e20;
	int HLBFGS_linesearch_IterMax = 20;
	int HLBFGS_UpdateNum = 7;
    int ITERMAX = 50000;
	double GNormThreshold = 1.0e-6;
	double GNormDivisionThreshold = 0.0001;
	double endiv_threshold = 1e-16;	//energy division threshold, used to avoid divide zero
	double expectedLength = 1.0;
	double VertexSepBuffer = 2;
	double PS_distance_buffer = 0.1;
	double PS_angle_buffer = 30.0;
	double AD_Digon_Len_scale = 1.0;
	int PA_max_card = 12;

	double weight_PR = 0.3;		// polygon aspect ratio
	double weight_PA = 0.16;	// polygon area / side length 
	double weight_PS = 0.36;	// polygon separation
	double weight_PI = 0.18;	// intersection aspect ratio
	double weight_DD = 1.0;		// dual distance
	double weight_DL = 1.0;		// digon length
	
	vector<Vec2> gradientDirections;

	// Multiscale Settings
	float share_power = 2.0;

	// Decomposition Settings
	ELEMENT_STATISTIC CLUSTER_ENSORT = ELEM_INCIDENCE;
	ELEMENT_STATISTIC CLUSTER_RESORT = ELEM_INCIDENCE;
	

	//////////////// functions ////////////////
	
	// Widget functions
	void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
	void set_ERData(ERData *src, OGLWidget *vis = NULL, bool deleteOld = true);
	bool hasFocusedVis();
	void updateWidgets();
	void repaintWidgets();
	void reset_sceneToContent(OGLWidget *vis = NULL);
	void updateVisVariablesUIs(OGLWidget *vis);
	void syncUIs();
    void showProgressBar(bool show = true, int maxvalue = 0);
	void refreshOpenGLWindow();

	// Utility
	bool OpenColorScheme(const char* path);
	void generateDataHierarchicalMethod(int entityNum, vector<std::pair<int, int>> relationNums);
	
	// optimization
	void Optimize_layout(ERData *focused_ERData, double &time, double &finalEnergy, int &iterNum, int &HLBFGSEvaluations, string solvername = "HLBFGS", ERData *dualERdata = NULL);
	void getGradientDirections(ERData* merdata, int N, double* g);

	// Timing and evaluation counts
	void resetEnergyTiming();
	int EnergyEvalCount = 0;
	void increaseEnergyEvalCount();
	double PolySeparationTime = 0.0;
	void addPolySeparationTime(double time);
	double PolyRegularityTime = 0.0;
	void addPolyRegularityTime(double time);
	double PolyAreaTime = 0.0;
	void addPolyAreaTime(double time);
	double PolyIntersectionTime = 0.0;
	void addPolyIntersectionTime(double time);
	double AdeptEvalTime = 0.0;
	void addAdeptEvalTime(double time);
	double HLBFGSEvalTime = 0.0;
	void addHLBFGSEvalTime(double time);
	double SwapEvalTime = 0.0;
	void addSwapEvalTime(double time);
	void printEnergyTiming(int num_ens, int num_res);



public slots:

	void updateMouseLocationLabel(double x, double y);
	void updateprimeVisSizeLabel(double x, double y);
	void updatedualVisSizeLabel(double x, double y);
	void updateERdataInfo(ERData *);
	void focusedVisChanged(VISTYPE mtype);
	void updatefocusVisLabel(VISTYPE type);
	void syncSelectedElements(VISTYPE type);
	void ERDataChanged(bool resetModel = true);
	bool dualVisVisible();
	bool bipartVisVisible();
	QString getOpenDir();
	void setOpenDir(QString filePath);
	void selectionActionsFresh(OGLWidget*currentVis);
	void setVisSelectEntity(OGLWidget*currentVis, bool checked);
	bool OpenERFile(const char* path, OGLWidget *viewWidget);
	bool SaveERFile(const char* path);

	void on_actionSave_Nary_Data_triggered();
	void on_actionOpen_Nary_Data_triggered();
	void on_actionLoad_Nary_Locations_triggered();
	void on_actionLoad_Hyperedge_Colors_triggered();
	void on_actionSave_Visualization_triggered();
	void on_actionSave_Visualization_with_HTML_Map_triggered();


	void actionDeleteEntities();
	void actionDeleteRelationships();
	void on_actionSepareteEnclosingRelationship_triggered();
	void on_actiondeleteEnclosingRelationships_triggered();
	void on_actionFindConvexHull_triggered();
	void on_actionOutputSubdata_triggered();
	void on_actionSave_ViewPoint_triggered();
	void on_actionLoad_ViewPoint_triggered();
	void on_actionSave_All_Color_Schemes_Results_triggered();
	void on_actionOutput_Color_Scheme_triggered();
	void on_actionLoad_Color_Scheme_triggered();
	void on_actionselectEnclosingRelationships_triggered();
	void on_actionClear_Gradient_Directions_triggered();
	void on_actionMergeEnclosedPolygon_triggered();
	void on_actionShow_Vis_Sample_Num_triggered();
	void on_actionCloseAllDockableWidgets_triggered();
	void on_actionFontSettings_triggered();
	void on_actionClear_Console_triggered();
	void on_actionSave_Console_triggered();
	void on_actionAssign_Relationships_Color_triggered();
	void on_actionAssign_Entities_Color_triggered();
	void on_actionSet_Selection_Color_triggered();
	void on_actionChange_Entity_Boundary_Color_triggered();
	void on_actionSet_Nodes_Boundary_Width_triggered();
	void on_actionSet_Polygon_Boundary_Width_triggered();
	void on_actionLights_Settings_triggered();
	void on_actionUse_Low_Sepecular_Lighting_triggered();
	void on_actionUse_High_Specular_Lighting_triggered();
	void on_actionRemove_PolygonsOfCard_triggered();
	void on_actionRemoveBinaryRelationshipsOfNary_triggered();
	void on_actionRemoveUnvisibleElements_triggered();
	void on_actionRemove_Dangling_Entities_triggered();
	void on_actionRemove_Dangling_Relationships_triggered();
	void on_actionRemove_Multiple_Diagon_triggered();
	void on_actionRemove_Non_repeated_Relationships_triggered();
	void on_actionConvert_to_Bipartite_triggered();
	void on_actionConvert_to_Linear_triggered();
	void on_actionConver_to_Simple_triggered();
	void on_actionConvert_to_2_Linear_triggered();
	void on_actionCreate_ER_Vis_triggered();
	void on_actionCreate_Relationships_From_Cliques_triggered();
	void on_actionCreate_Cliques_from_Relationships_triggered();
	void on_actionAssign_Entity_Labels_With_Indices_triggered();
	void on_actionAssign_Relationship_Labels_With_Indices_triggered();
	void on_actionClear_all_data_triggered();
	void createDualERdata();
	void on_actionSwitch_PrimVis_and_DualVis_triggered();

	void on_actionAnimate_Optimization_triggered(bool checked);
	void on_actionAuto_Adjust_Viewpoint_triggered(bool checked);

	void on_actionDraw_Curved_Scaffolds_triggered(bool checked);
	void on_actionDraw_Nodes_as_Spheres_triggered(bool checked);
	void on_actionDraw_Edges_as_Cylinders_triggered(bool checked);
	void on_actionDraw_Axes_triggered(bool checked);
	void on_actionDraw_Grid_triggered(bool checked);
	void on_actionDraw_Cursor_triggered(bool checked);

	/*tool bar*/
	void on_actionSelectEntity_triggered(bool checked);
	void on_actionSelectRelationship_triggered(bool checked);
	void on_actionSelectEntities_triggered(bool checked);
	void on_actionMoveSelect_triggered(bool checked);
	void on_actionaddEntity_triggered(bool checked);
	void on_actionaddRandomEntities_triggered();
	void on_actionaddRelationship_triggered();
	void on_actionDeleteSelectedElements_triggered();
	void on_actionEditElement_triggered();
	void on_actionMergeElements_triggered();
	void on_actionDivideRelationship_triggered();
	void on_actionRemoveElementsFromRelationship_triggered();
	void on_actionSwap_triggered();
	void on_actionBringReToFront_triggered();
	void on_actionBringReToBack_triggered();
	void on_action_optimize_layout_triggered();
	void on_actionstopOptimization_triggered();
	void on_actionManual_triggered();
	void on_actionCreateRelationship_triggered();
	void on_actionSplitElements_triggered();

	void optimizeOrdering();    /*Optimization*/
	void optimizeDualOrdering();
	void lineLayout();
	void reorderSelectedPolygons();
	void calcDegreeAndCardinality();
	void calcElementStatistics();
    void fix_verts(ERData *focused_ERData, vector<int>fixedVertIndices);
	void free_elements(ERData* erdata, vector<int> freeVertIndices, vector<int> freePolyIndices);
	void free_elements(ERData* erdata);
	void showFixedVertInfo(ERData *erdata);
	ColorGradient getCurrentColorGradient();/*change elemetns colors*/
    
    
	void setZoom();
    void syncDualZooming();
	void syncDualElementColor();
	void updatePolyColors();
	void assignRelationshipColorsBySetIntersections();
	void assignElementsColorPredefined();
	void assignElementsColorBrewer1();
	void assignElementsColorBrewer2();
	void assignElementColorEvenHueDistance();
	void assignEnColorByDual();

	void applyReColorScheme();
	void applyEnColorScheme();

	void scaleERLayout(ERData* erdata, double scale);
	void recenterView();
	void ResetMonogonsAngles();
	void DistributeMonogonAngles();
	void removeMonogonsInPrimeVis();
	void removeMonogonsInDualVis();
    
	vector<int> getIndexListFromLineEdit();
	vector<string> getLabelListFromLineEdit();
	void select_entities_fromInput();
	void select_relationships_fromInput();
	void Highlight_selectedElements();
	void Highlight_selectedElementsOnce();

    void applyGridLayout();
	void resetView();
    void applyGridSeparatedLayout();
	void applyRandomLayout();
	void apply3DRandomLayout();
	void applyCircleLayout();
	void applyHLBFGS();
	void applyHLBFGSDual();
	void applyHLBFGSArithopt();
	void applyHLBFGSArithoptDual();
	void applyHLBFGSMonogon();
	void applyHLBFGSMonogonDual();
	void clearFixedVert();
	void setFixedVert();
	void fixSelectedVerts();
	void stopOptimization();
	void calcBoundingCircle();
	void calcAverageLength();
	void findEdgelyAdjacentRelationshipIncidentOneEntity();
	void assignColor();

	/*view settings*/
    void set_ShowEntities(bool res);
	void set_ShowRelationships(bool res);
	void set_EntityLabels(int index);
	void set_RelationshipLabels(int index);
	void set_ShowEntityTips(bool checked);
	void set_ShowRelationshipTips(bool checked);
	void set_ShowRelationshipDecal(bool checked);
	

	/*filter*/
	void set_EntityDegreeFilter(bool checked);
	void set_RelationshipDegreeFilter(bool checked);
	void set_EntityCardinalityFilter(bool checked);
    void set_polygonCardinalityFilter(bool checked);
	void set_EntityIndexFilter(bool checked);
	void set_RelationshipIndexFilter(bool checked);
	void set_InheritVisibilities(bool checked);

	/*view box*/
	void initialize_2D_vis();
	//void initializeComposedDualVis(bool checked);

	/*View window selection*/
	void set_PrimeVisWidget_focused(bool checked);
	void set_DualVisWidget_focused(bool checked);
	void set_BipartVisWidget_focused(bool checked);

    void vertexMinDegreeChanged(int arg1);
    void vertexMaxDegreeChanged(int arg1);
    void polyMinDegreeChanged(int arg1);
    void polyMaxDegreeChanged(int arg1);
    void relationshipCardinalityBottomChanged(int arg1);
    void relationshipCardinalityTopChanged(int arg1);
	void entityCardinalityBottomChanged(int arg1);
	void entityCardinalityTopChanged(int arg1);
	void optimizeIterationMaxChanged(int arg1);
	void entityIndexBottomChanged(int arg1);
	void entityIndexTopChanged(int arg1);
	void relationshipIndexBottomChanged(int arg1);
	void relationshipIndexTopChanged(int arg1);


	void doubleSpinBox_VertexSize_valueChanged(double arg1);
	void doubleSpinBox_MaxScaffoldCurve(double arg1);
	void doubleSpinBox_LensAngle_valueChanged(double arg1);
	void doubleSpinBox_MonogonAspectRatio_valueChanged(double arg1);
	void doubleSpinBox_MonogonSize_valueChanged(double arg1);

	// New UI

	// File Dropdown Menu
	void on_actionSave_CSV_Data_triggered();
	void on_actionOpen_CSV_Data_triggered();
	void on_actionSave_Decomposition_triggered();
	void on_actionLoad_Decomposition_triggered();

	// View Dropdown Menu
	void on_actionPolygon_View_triggered();
	void on_actionKonig_View_triggered();

	// Layout Control Panel
	void apply_layout();
	void get_layout_transform_settings();
	void get_random_layout_settings();
	void get_grid_layout_settings();
	void get_circle_layout_settings();
	void get_force_directed_settings();
	void get_optimization_settings();
	void get_subgraph_settings();
	void get_simplified_settings();
	
	// Multiscale Control Panel
	void generateAtomicOperations();
	void generateAtomicOperationsCommandLine(double alpha, double beta, double gamma, bool nrem, bool erem, bool nmerge, bool emerge);
	void applyNextOperation();
	void invertLastOperation();
	void applyAllOperations();
	int applyAllOperationsCommandLine(std::string output_name, int min_edges, int min_nodes);
	void invertAndOptimize();
	void invertAndOptimizeCommandLine(std::string output_name, int interval = 1, int op_total = 0);
	void optimizeSubgraphManually();
	void applySelectedOperation();
	void optimizeInversionsChanged();
	void preserveConnectionsChanged();
	void preserveBranchesChanged();
	void pruneOnlyChanged();

	// Hypergraph Branch Cluster Decomposition
	void applyBranchClusterDecomposition();
	void createCluster(ENList enlist, REList relist);
	ERData* copyERDataMergeClusters(ERData* source, ERClusterDecomp* decomp);
	bool getMatchingIndexEntities(ENList* source, ERData* target);
	bool getMatchingIndexRelationships(REList* source, ERData* target);
	bool copyVertexLocations(ERData* source, ERData* target);

	// Hypergraph Cycle Decomposition
	void applyCycleDecomposition();

	bool decompHighlightChanged();
	void updateVisSelectedElements();
	void highlightNextBComp();
	void highlightAllBComps();
	void highlightNextTComp();
	void highlightAllTComps();
	void highlightNextCycle();
	void highlightAllCycles();
	void highlightNextCluster();
	void highlightAllClusters();
	void resetDecompHighlights();

	// Bipartite Simplification Operations
	void startRecordingBipartOperations();
	void applyBipartOperation(BIPART_OP_TYPE type);
	void saveBipartOperations();
	void loadBipartOperations();
	void invertLastBipartOperation();
	void applyNextBipartOperation();
	void applyBipartEdgeCut();
	void applyBipartEdgePrune();
	void applyBipartK22Collapse();
	void applyBipartChainMerge();

	// Decomposition-based simplification
	bool checkDecompAndOpManagers();
	bool applyNextClusterOperation();
	void applyClusterSimplification();
	void simplifyAllClusters();
	void applyNextBicompOperation();
	void simplifyAllBicomps();
	void applyNextTreeOperation();
	void simplifyAllTrees();

	// debugging
	void writeOrderedIncidenceMatrix();
	void writeAdjacencyMatrices();
	void writeERDataStatistics(std::string outfile = "");
	void on_actionPrint_Euler_Characteristics_triggered();
	void save_console(std::string fname);

};

extern MainWindow *gmainwindow;

#endif // MAINWINDOW_H
