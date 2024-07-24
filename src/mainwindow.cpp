#include "mainwindow.h"
#include "oglwidget.h"
#include "ui_mainwindow.h"
#include "ModifyElementDialog.h"
#include "colorsettingdialog.h"
#include "changePolygonColorDialog.h"
#include "fontsettingdialog.h"
#include "LightSettingDialog.h"
#include "helpdialog.h"
#include "DockTextEdit.h"
#include "DockControl.h"
#include "DockSelectElement.h"
#include "EnergyEvaluation.h"
#include "Energy.h"
#include <QTime>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QSizePolicy>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentRun>
#include <QDirIterator>
#include <QColorDialog>
#include <QScrollArea>
#include <QFileInfo>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>
#include <regex>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream> 
#include <typeinfo>
#include <cctype>
#include "FileIO.h"
#include "LogConsole.h"
#include "ElementUtils.h"
#include "lodepng.h"
#include "ColorPalette.h"
#include <adept.h>
#include "OptimizationOrder.h"
#include <stdio.h>
#include <omp.h>

using namespace std;
MainWindow *gmainwindow;
DockTextEdit *gconsole;

static int stencilBufferOutput = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	gmainwindow = this;

    ui->setupUi(this);
	primeVis = ui->visWdgt;
	dualVis = ui->dualWdgt;
	bipartVis = ui->bipartWdgt;
	gconsole = ui->dockWidgetContents_Console;
	setCentralWidget(ui->dockWidget_PrimeVis);
	splitDockWidget(ui->dockWidget_BipartVis, ui->dockWidget_Control, Qt::Horizontal);
	splitDockWidget(ui->dockWidget_DualVis, ui->dockWidget_BipartVis, Qt::Horizontal);
	ui->dockWidget_Control->show();
	ui->dockWidget_Control->raise();

	//set the size of docks after set maximized
	setWindowState(Qt::WindowMaximized);

	/*Dialogues*/
	fontsettingDlg = new FontSettingDialog(primeVis, dualVis);
	modifySelectedElementsDlg = new ModifyElementDialog();

	this->set_ERData(new ERData(), primeVis);
	this->set_ERData(new ERData(), dualVis);
	this->set_ERData(new ERData(), bipartVis);

	primeVis->widgetType = VISTYPE::PRIMEVIS;
	dualVis->widgetType = VISTYPE::DUALVIS;
	bipartVis->widgetType = VISTYPE::BIPARTVIS;

	primeVis->infoDock = ui->dockWidgetContents_Information;
	dualVis->infoDock = ui->dockWidgetContents_Information;

	focusedVis = primeVis;

	/*action groups*/
	selectionActionGroup = new QActionGroup(this);
	selectionActionGroup->addAction(ui->actionSelectEntities);
	selectionActionGroup->addAction(ui->actionSelectEntity);
	selectionActionGroup->addAction(ui->actionSelectRelationship);
	selectionActionGroup->addAction(ui->actionMoveSelect);
	selectionActionGroup->addAction(ui->actionaddEntity);

	ERDataInfoLabel = new QLabel(this);
	ERDataInfoLabel->setMinimumWidth(200);
	ERDataInfoLabel->setAlignment(Qt::AlignLeft);
	updateERdataInfo(m_ERData);
	ui->statusBar->addPermanentWidget(ERDataInfoLabel);

	MouseLocationLabel = new QLabel(this);
	MouseLocationLabel->setMinimumWidth(200);
	MouseLocationLabel->setAlignment(Qt::AlignLeft);
	updateMouseLocationLabel(0, 0);
	ui->statusBar->addPermanentWidget(MouseLocationLabel);

	primeVisSizeLabel = new QLabel(this);
	primeVisSizeLabel->setMinimumWidth(120);
	primeVisSizeLabel->setAlignment(Qt::AlignCenter);
	ui->statusBar->addPermanentWidget(primeVisSizeLabel);

	dualVisSizeLabel = new QLabel(this);
	dualVisSizeLabel->setMinimumWidth(120);
	dualVisSizeLabel->setAlignment(Qt::AlignCenter);
	ui->statusBar->addPermanentWidget(dualVisSizeLabel);
	
	focusVisLabel = new QLabel(this);
	focusVisLabel->setMinimumWidth(80);
	focusVisLabel->setAlignment(Qt::AlignCenter);
	ui->statusBar->addPermanentWidget(focusVisLabel);
	focusVisLabel->setStyleSheet("QLabel { color : red; }");
	updatefocusVisLabel(VISTYPE::PRIMEVIS);

	connect(primeVis, SIGNAL(mouseLocChanged(double, double)), this, SLOT(updateMouseLocationLabel(double, double)));
	connect(dualVis, SIGNAL(mouseLocChanged(double, double)), this, SLOT(updateMouseLocationLabel(double, double)));
	connect(bipartVis, SIGNAL(mouseLocChanged(double, double)), this, SLOT(updateMouseLocationLabel(double, double)));

	connect(primeVis, SIGNAL(visSizeChanged(double, double)), this, SLOT(updateprimeVisSizeLabel(double, double)));
	connect(dualVis, SIGNAL(visSizeChanged(double, double)), this, SLOT(updatedualVisSizeLabel(double, double)));
	connect(bipartVis, SIGNAL(visSizeChanged(double, double)), this, SLOT(updatedualVisSizeLabel(double, double)));

	connect(ui->actionCreate_Dual_VisWidget, &QAction::triggered, this, &MainWindow::createDualERdata);

	//progressive bar
	progressBar = new QProgressBar(NULL);
	progressBar->setMaximumHeight(16);
	progressBar->setMaximumWidth(200);
	progressBar->setTextVisible(false);
	this->statusBar()->addPermanentWidget(progressBar, 0);

	progressBar->hide();
	progressBar->setValue(0);
	progressBar->setMinimum(0);
	progressBar->setMaximum(100);
	progressBar->setMaximumWidth(100);

	QMenu *menuWindow = ui->menuWindows;
	QList<QDockWidget *> dockWidgets = findChildren<QDockWidget *>();
	for (int i = 0; i < dockWidgets.size(); i++) {
		QDockWidget *dw = dockWidgets[i];
		QString dname = dw->windowTitle();

		QAction *addNew = new QAction(menuWindow);
		addNew->setText("Show " + dname + " window");
		menuWindow->addAction(addNew);
		connect(addNew, &QAction::triggered, dw, &QDockWidget::show);
	}

	control_dock = new DockControl(primeVis, dualVis);
	ui->dockWidget_Control->setWidget(control_dock);


	/*connect*/
	connect(primeVis, &OGLWidget::changeFocusedVis, this, &MainWindow::focusedVisChanged);
	connect(dualVis, &OGLWidget::changeFocusedVis, this, &MainWindow::focusedVisChanged);
	connect(bipartVis, &OGLWidget::changeFocusedVis, this, &MainWindow::focusedVisChanged);
	connect(primeVis, &OGLWidget::selectedElementsChanged, this, &MainWindow::syncSelectedElements);
	connect(dualVis, &OGLWidget::selectedElementsChanged, this, &MainWindow::syncSelectedElements);
	connect(bipartVis, &OGLWidget::selectedElementsChanged, this, &MainWindow::syncSelectedElements);
	connect(ui->dockWidgetContents_Information->ui.pushButton_SelectEntities, &QPushButton::clicked, this, &MainWindow::select_entities_fromInput);
	connect(ui->dockWidgetContents_Information->ui.pushButton_SelectRelationships, &QPushButton::clicked, this, &MainWindow::select_relationships_fromInput);
	connect(ui->dockWidgetContents_Information->ui.pushButton_HighlightSelectedElements, &QPushButton::clicked, this, &MainWindow::Highlight_selectedElements);

	ui->actionSelectEntity->trigger();

	syncUIs();
	updateVisVariablesUIs(primeVis);
	ui->dockWidget_DualVis->hide();
	ui->dockWidget_BipartVis->hide();
	ui->dockWidget_Console->hide();
	ui->dockWidget_Information->hide();
}

MainWindow::~MainWindow()
{
	delete m_ERData;
	delete m_Dual_ERData;
	delete m_ERProgressive;
	delete m_ERCycleDecomp;
	delete m_BOptManager;
	delete selectionActionGroup;
	delete fontsettingDlg;
	delete modifySelectedElementsDlg;
    delete ui;
}



///////////////////////////////////////////////////////////////
//////////////////////// functions ////////////////////////////
///////////////////////////////////////////////////////////////

// Widget functions

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);

	if (!primeVis)
		return;

	int h = primeVis->win_height;
	int w = primeVis->win_width;

	if (!ui->dualWdgt)
		return;

	int dualh = dualVis->win_height;
	int dualw = dualVis->win_width;

	char buffer[100];
	sprintf(buffer, "Vis Window: %d x %d; Dual Window: %d x %d", w, h, dualw, dualh);

	QString status = QString(buffer);
	statusBar()->showMessage(status, 2000);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		QApplication::focusWidget()->clearFocus();
	}

	if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace)
	{
		actionDeleteEntities();
		actionDeleteRelationships();
		LogConsole::writeConsole("Relationship and Entities are deleted");
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
}

void MainWindow::set_ERData(ERData* src, OGLWidget* vis, bool deleteOld)
{
	if (vis == NULL)
		vis = primeVis;

	ERClusterDecomp* decomp = vis->getERClusterDecomp();
	if (decomp && deleteOld)
		vis->clearERClusterDecomp();

	if (vis == primeVis)
	{
		if (m_ERData && deleteOld)
			delete(m_ERData);

		m_ERData = src;
		primeVis->currentERData = src;
		ERDataChanged();
	}
	else if (vis == dualVis) {

		if (m_Dual_ERData && deleteOld)
			delete(m_Dual_ERData);

		m_Dual_ERData = src;
		dualVis->currentERData = src;
	}
	else if (vis == bipartVis)
	{
		bipartVis->currentERData = src;
	}


}

bool MainWindow::hasFocusedVis()
{
	if (!focusedVis) {
		LogConsole::showError("No focused Vis");
		return false;
	}

	if (!focusedVis->currentERData) {
		LogConsole::showError("No focused E_R data");
		return false;
	}

	return true;
}

void MainWindow::updateWidgets()
{
	if (primeVis->isVisible())
		primeVis->update();
	if (dualVis->isVisible())
		dualVis->update();
	if (bipartVis->isVisible())
		bipartVis->update();
}

void MainWindow::repaintWidgets()
{
	if (primeVis->isVisible())
		primeVis->repaint();
	if (dualVis->isVisible())
		dualVis->repaint();
	if (bipartVis->isVisible())
		bipartVis->repaint();
}

void MainWindow::reset_sceneToContent(OGLWidget* vis)
{
	if (vis == NULL)
		vis = focusedVis;

	if (vis)
	{
		vis->currentERData->calculateBoundingCirle();

		double minX = vis->currentERData->minX;
		double minY = vis->currentERData->minY;
		double maxX = vis->currentERData->maxX;
		double maxY = vis->currentERData->maxY;

		vis->reset_sceneToContent(minX, minY, maxX, maxY);
	}
}

void MainWindow::updateVisVariablesUIs(OGLWidget* vis)
{
	control_dock->ui->checkBox_ShowEntities->setChecked(vis->flag_showEntities);
	control_dock->ui->checkBox_ShowRelationships->setChecked(vis->flag_showRelationships);
	control_dock->ui->comboBox_entityLabelContent->setCurrentIndex((int)vis->entityLabels);
	control_dock->ui->comboBox_relationshipLabelContent->setCurrentIndex((int)vis->relationshipLabels);
	control_dock->ui->checkBox_EntityIndexFilter->setChecked(vis->flag_entityIndexFilter);
	control_dock->ui->checkBox_RelationshipIndexFilter->setChecked(vis->flag_relationshipIndexFilter);
	control_dock->ui->checkBox_EntityDegreeFilter->setChecked(vis->flag_entityDegreeFilter);
	control_dock->ui->checkBox_RelationshipDegreeFilter->setChecked(vis->flag_relationshipDegreeFilter);
	control_dock->ui->checkBox_EntityCardinalityFilter->setChecked(vis->flag_entityCardinalityFilter);
	control_dock->ui->checkBox_polygonCardinalityFilter->setChecked(vis->flag_relationshipCardinalityFilter);
}

void MainWindow::syncUIs()
{
	ui->actionDraw_Curved_Scaffolds->setChecked(flag_drawPolyCurvedScaffolds);
	ui->actionDraw_Edges_as_Cylinders->setChecked(flag_drawEdgeCylinder);
	ui->actionDraw_Nodes_as_Spheres->setChecked(flag_drawNodeSphere);
	ui->actionAnimate_Optimization->setChecked(flag_animateOptimization);
	ui->actionAuto_Adjust_Viewpoint->setChecked(flag_autoAdjustViewpoint);

	control_dock->ui->checkBox_optimizeInversions->setChecked(flag_optimizeInversions);
	control_dock->ui->checkBox_PreserveConnections->setChecked(flag_preserveConnections);
	control_dock->ui->checkBox_PreserveBranches->setChecked(flag_preserveBranches);
}

void MainWindow::showProgressBar(bool show, int max) {
	
	if(!show)
		progressBar->hide();
	else {
		progressBar->show();
		progressBar->setRange(0, max);
	}
}

void MainWindow::refreshOpenGLWindow() {

	primeVis->repaint();

	if (dualVisVisible())
		dualVis->repaint();

	if (bipartVisVisible())
		bipartVis->repaint();
}

// Utility

bool MainWindow::OpenColorScheme(const char* path)
{

	if (!hasFocusedVis())
		return false;

	QFile myfile(path);

	char chararray[100];
	int primitiveNums[4];

	if (!myfile.open(QIODevice::ReadOnly))
	{
		LogConsole::showError("File does not exist.");
		return false;
	}

	QTextStream in(&myfile);

	int lineNum = 0;
	while (!in.atEnd())
	{
		QString line = in.readLine();

		stringstream ss(line.toStdString());
		string temp;
		for (int i = 0; i < 4; i++) {
			getline(ss, temp, ' ');
			primitiveNums[i] = stof(temp);
		}

		Element::polyColor[lineNum] = Vec4(primitiveNums[0], primitiveNums[1], primitiveNums[2], primitiveNums[3]);
		lineNum++;

	}

	myfile.close();

	if (focusedVis->currentERData) {
		focusedVis->currentERData->assignRelationshipColors();
	}

	updateWidgets();

	return true;
}

void MainWindow::generateDataHierarchicalMethod(int entityNum, vector<std::pair<int, int>> relationNums) {

	if (sqrt(entityNum) - (int)sqrt(entityNum) != 0) {
		LogConsole::writeConsole("please make sure your vertex num is a squared int");
		return;
	}

	set_ERData(new ERData(), primeVis);

	int w = sqrt(entityNum);
	int h = w;

	int max_degree = 10;

	int num = 1;
	queue<Entity*> vertexqueue;
	Entity* vertex = new Entity();
	m_ERData->addEntity(vertex);
	vertexqueue.push(vertex);

	while (num <= entityNum && !vertexqueue.empty()) {

		Entity* vertex = vertexqueue.front();
		vertexqueue.pop();

		float r = (float)rand() / (float)RAND_MAX;
		int degree = r * max_degree + 1;

		for (int i = 0; i < degree; i++) {
			Entity* entity1 = new Entity();
			entity1->setIndex(i);
			vertexqueue.push(entity1);

			vector<Entity*> connection;
			connection.push_back(vertex);
			connection.push_back(entity1);

			if (!m_ERData->isInsideRelationshipList(connection)) {

				m_ERData->addEntity(entity1);

				Relationship* edge = new Relationship(connection);
				m_ERData->addRelationship(edge);

			}
		}
		num += degree;
	}

	for (int i = 0; i < relationNums.size(); i++) {
		m_ERData->generateRandomRelationship(relationNums[i].first, relationNums[i].second);
	}

	m_ERData->update2D();
	ui->visWdgt->initializeSelection();
	LogConsole::writeConsole("number of entities is: %d", m_ERData->getEntityNum());
	LogConsole::writeConsole("number of relationships is: %d", m_ERData->getRelationshipNum());
	//m_ERData->writeEulerCharacteristics();
	ui->visWdgt->reset_sceneToCenterVisualization();
	update();
}

// Optimization

void MainWindow::Optimize_layout(ERData* focused_ERData, double& execution_time, double& finalEnergy, int& iterNum, int& HLBFGSEvaluations, string solverName, ERData* dualERdata)
{
	flag_stopOptimization = false;
	//resetEnergyTiming();

	control_dock->toggleOptimizationButtons(false);
	showProgressBar(true, 0);

	LogConsole::writeConsole("\n------Start %s Optimization------", solverName.c_str());

#ifdef USE_OPENMP
	double time1 = omp_get_wtime();
#else
	clock_t time1 = clock();
#endif

	if (solverName == "Reordering")
	{
		OptimizationOrder optsolver;
		optsolver.reordering_polygons(focused_ERData);
	}
	else if (solverName == "DualReordering")
	{
		OptimizationOrder optsolver;
		optsolver.dual_reordering_polygons(focused_ERData, dualERdata);
		/*optenergy = optsolver.m_energy;
		optEnergyRecords = optsolver.m_optEnergyRecords;*/
	}
	else if (solverName == "PolygonHLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.optimize_polygons_HLBFGS(focused_ERData);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
		/*optenergy = m_optHLBFGSSolver.m_energy;
		optEnergyRecords = m_optHLBFGSSolver.m_optEnergyRecords;*/
	}
	else if (solverName == "DualPolygonHLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.dual_optimize_polygons_HLBFGS(focused_ERData, dualERdata);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
		/*optenergy = m_optHLBFGSSolver.m_energy;
		optEnergyRecords = m_optHLBFGSSolver.m_optEnergyRecords;*/
	}
	else if (solverName == "MonogonHLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.optimize_monogons_HLBFGS(focused_ERData);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
		/*optenergy = m_optHLBFGSSolver.m_energy;
		optEnergyRecords = m_optHLBFGSSolver.m_optEnergyRecords;*/
	}
	else if (solverName == "MonogonHLBFGSDual")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.optimize_monogons_HLBFGS(focused_ERData, true);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
		/*optenergy = m_optHLBFGSSolver.m_energy;
		optEnergyRecords = m_optHLBFGSSolver.m_optEnergyRecords;*/
	}
	else if (solverName == "HLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.optimize_HLBFGS(focused_ERData);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
		/*optenergy = m_optHLBFGSSolver.m_energy;
		optEnergyRecords = m_optHLBFGSSolver.m_optEnergyRecords;*/
	}
	else if (solverName == "DualHLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.dual_optimize_HLBFGS(focused_ERData, dualERdata);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
		/*optenergy = m_optHLBFGSSolver.m_energy;
		optEnergyRecords = m_optHLBFGSSolver.m_optEnergyRecords;*/
	}
	else if (solverName == "SubgraphHLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.optimize_subgraph_HLBFGS(focused_ERData);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
	}
	else if (solverName == "DualSubgraphHLBFGS")
	{
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.dual_optimize_subgraph_HLBFGS(focused_ERData, dualERdata);
		finalEnergy = m_optHLBFGSSolver.m_energy.total;
		iterNum = m_optHLBFGSSolver.HLBFGSIterations;
		HLBFGSEvaluations = m_optHLBFGSSolver.HLBFGSEvaluations;
	}

#ifdef USE_OPENMP
	double time2 = omp_get_wtime();
	execution_time = time2 - time1;
#else
	clock_t time2 = clock();
	execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
#endif

	// if (dualERdata)
	// {
	// 	int num = focused_ERData->getEntityNum() + focused_ERData->getRelationshipNum();
	// 	printEnergyTiming(num, num);
	// }
	// else
	// {
	// 	printEnergyTiming(focused_ERData->getEntityNum(), focused_ERData->getRelationshipNum());
	// }
	LogConsole::writeConsole("Optimization costs: %.3lf sec.", execution_time);
	//LogConsole::writeConsole("Evaluation Times: %d.", EnergyEvalCount);
	LogConsole::writeConsole("--------END--------\n");

	/*
	showEnergyInfo(optenergy);
	focused_ERData->saveCurrentLayout();
	*/

	control_dock->toggleOptimizationButtons(true);
	showProgressBar(false);
	updateWidgets();
}

void MainWindow::getGradientDirections(ERData* merdata, int N, double* g)
{
	gradientDirections.clear();

	if (merdata->fixedVertComp.size() != 0)
	{
		assert(N == merdata->getFreeVertCompNum());
	}

	for (int i = 0; i < merdata->getEntities().size(); i++) {

		int vi = merdata->getEntity(i)->getIndex();

		int listindex_x, listindex_y;
		if (!merdata->fixedVertComp.empty())
		{
			listindex_x = vertex_index_x_to_list_index(vi, merdata->getSortedFixedCompIndices());
			listindex_y = vertex_index_y_to_list_index(vi, merdata->getSortedFixedCompIndices());
		}
		else {
			listindex_x = 2 * vi;
			listindex_y = 2 * vi + 1;
		}

		Vec2 grad;
		if (listindex_x == -1 || listindex_y == -1) {
			grad = Vec2(0, 0);
		}
		else {
			grad = Vec2(g[listindex_x], g[listindex_y]);
		}

		if (flag_showGradientDirs)
			gradientDirections.push_back(-grad);
		else
			gradientDirections.push_back(grad);
	}
}

// timing

void MainWindow::resetEnergyTiming()
{
	EnergyEvalCount = 0;
	PolySeparationTime = 0.0;
	PolyRegularityTime = 0.0;
	PolyAreaTime = 0.0;
	PolyIntersectionTime = 0.0;
	AdeptEvalTime = 0.0;
	HLBFGSEvalTime = 0.0;
	SwapEvalTime = 0.0;
}

void MainWindow::increaseEnergyEvalCount()
{
	EnergyEvalCount++;
}

void MainWindow::addPolySeparationTime(double time)
{
	PolySeparationTime += time;
}

void MainWindow::addPolyRegularityTime(double time)
{
	PolyRegularityTime += time;
}

void MainWindow::addPolyAreaTime(double time)
{
	PolyAreaTime += time;
}

void MainWindow::addPolyIntersectionTime(double time)
{
	PolyIntersectionTime += time;
}

void MainWindow::addAdeptEvalTime(double time)
{
	AdeptEvalTime += time;
}

void MainWindow::addHLBFGSEvalTime(double time)
{
	HLBFGSEvalTime += time;
}

void MainWindow::addSwapEvalTime(double time)
{
	SwapEvalTime += time;
}

void MainWindow::printEnergyTiming(int num_ens, int num_res)
{
	LogConsole::writeConsole("Footprint Entities: %d.", num_ens);
	LogConsole::writeConsole("Footprint Relationships: %d.", num_res);
	LogConsole::writeConsole("Energy Evaluations: %d.", EnergyEvalCount);
	LogConsole::writeConsole("Polygon Separation Time: %.3lf sec.", PolySeparationTime);
	LogConsole::writeConsole("Polygon Regularity Time: %.3lf sec.", PolyRegularityTime);
	LogConsole::writeConsole("Polygon Area Time: %.3lf sec.", PolyAreaTime);
	LogConsole::writeConsole("Polygon Intersection Time: %.3lf sec.", PolyIntersectionTime);
	LogConsole::writeConsole("Adept Evaluation Time: %.3lf sec.", AdeptEvalTime);
	LogConsole::writeConsole("HLBFGS Evaluation Time: %.3lf sec.", HLBFGSEvalTime);
	LogConsole::writeConsole("Vertex Swap Evaluation Time: %.3lf sec.", SwapEvalTime);
}



///////////////////////////////////////////////////////////////
/////////////////////////// slots /////////////////////////////
///////////////////////////////////////////////////////////////

void MainWindow::on_actionClear_Console_triggered()
{
	ui->dockWidgetContents_Console->clearConsole();
}

void MainWindow::on_actionSave_Console_triggered()
{
    QString filter = "Text files (*.txt)";
    QString fileName = QFileDialog::getSaveFileName(0, "Save Console", getOpenDir(), filter);
	ui->dockWidgetContents_Console->saveConsole(fileName);
}

void MainWindow::applyGridLayout()
{
	if (!hasFocusedVis())
		return;

	focusedVis->currentERData->designLayout_Grid(RD_rad_max*expectedLength);
	for(int i = 0; i<5; i++)
		focusedVis->currentERData->jitterLocations();
	
	focusedVis->reset_sceneToCenterVisualization();
	updateWidgets();
}

void MainWindow::applyRandomLayout() {

	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	LogConsole::writeConsole("random layout created with radius:%.3e", RD_rad_max*expectedLength);
	//focused_ERData->designLayout_Random(CircleRatio*expectedLength);
	focused_ERData->designLayout_Random(RD_rad_max * expectedLength);

	focused_ERData->update2D();
	reset_sceneToContent(gmainwindow->primeVis);
	updateWidgets();
}

void MainWindow::applyCircleLayout()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	LogConsole::writeConsole("circle layout created with CR:%.3e", RD_rad_max);
	focused_ERData->designLayout_GridCircle(0.5*RD_rad_max*expectedLength, 0.5*expectedLength);

	focusedVis->reset_sceneToCenterVisualization();

	updateWidgets();
}

void MainWindow::lineLayout()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	LogConsole::writeConsole("linear layout created with CR:%.3e", RD_rad_max);
	focused_ERData->designLayout_linear(RD_rad_max*expectedLength);

	focusedVis->reset_sceneToCenterVisualization();

	updateWidgets();
}

void MainWindow::apply3DRandomLayout() {

	m_ERData->designLayout_Random3D();

	updateWidgets();
}

void MainWindow::resetView()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	focused_ERData->update2D();
	focusedVis->reset_scene();
	updateWidgets();
	LogConsole::writeConsole("The scene is reset");
}

void MainWindow::recenterView()
{
	if (!focusedVis)
	{
		LogConsole::showError("No focused vis");
		return;
	}

	focusedVis->reset_sceneToCenterVisualization();

	updateWidgets();
	LogConsole::writeConsole("The scene is recentered");
}

void MainWindow::on_actionSave_ViewPoint_triggered()
{
	auto wid = focusedVis;
	if(wid)
	{

		QString filter = "view files (*.view *.txt)";

		QString fileName = QFileDialog::getSaveFileName(0, "Save View Point", getOpenDir(), filter);
		string str = fileName.toStdString();

		if (!str.size())
			return;

		const char* path = str.c_str();

		bool res = wid->saveTransformation(path);
		if (res)
			LogConsole::writeConsole("view point is saved");
	}
}

void MainWindow::on_actionLoad_ViewPoint_triggered()
{
	auto wid = focusedVis;
	if (wid) {

		QString filter = "view files (*.view *.txt)";

		QString pathOftext = QFileDialog::getOpenFileName(this, "Select a View Point file", getOpenDir(), filter);
		string str = pathOftext.toStdString();

		if (!str.size())
			return;

		const char* path = str.c_str();

		bool res = wid->openTransformation(path);
		if (res)
			LogConsole::writeConsole("view point is loaded");

		updateWidgets();
	}
}

void MainWindow::updateMouseLocationLabel(double x, double y)
{
	QString status = QString("OpenGL Coord(%1,%2)").arg(x, 0, 'f', 4).arg(y, 0, 'f', 4);
	MouseLocationLabel->setText(status);
}

void MainWindow::updateprimeVisSizeLabel(double x, double y)
{
	QString status = QString("PrimeVis: %1 X %2").arg(x, 4).arg(y, 4);
	primeVisSizeLabel->setText(status);
}

void MainWindow::updatedualVisSizeLabel(double x, double y)
{
	QString status = QString("DualVis: %1 X %2").arg(x, 4).arg(y, 4);
	dualVisSizeLabel->setText(status);
}

void MainWindow::updateERdataInfo(ERData *mdata)
{
	if (mdata == NULL || ERDataInfoLabel == NULL)
		return;

	QString status = QString("#EN: %1, #RE: %2").arg(mdata->getEntityNum()).arg(mdata->getRelationshipNum());
	ERDataInfoLabel->setText(status);
}

void MainWindow::updatefocusVisLabel(VISTYPE type)
{
	if (type == VISTYPE::PRIMEVIS)
		focusVisLabel->setText(QString("PRIME"));
	else if (type == VISTYPE::DUALVIS)
		focusVisLabel->setText(QString("DUAL"));
	else if (type == VISTYPE::BIPARTVIS)
		focusVisLabel->setText(QString("BIPART"));
}

void MainWindow::syncSelectedElements(VISTYPE type)
{
	if(type == VISTYPE::PRIMEVIS || type == VISTYPE::DUALVIS)
	{
		OGLWidget *otherVis = NULL;
		OGLWidget *changedVis = NULL;

		if (type == VISTYPE::PRIMEVIS) {
			changedVis = primeVis;
			otherVis = dualVis;
		}
		else if (type == VISTYPE::DUALVIS) {
			changedVis = dualVis;
			otherVis = primeVis;
		}

		if (!otherVis)
		{
			LogConsole::showError("Invalid vis");
			return;
		}

		bool otherVisIsVisible = otherVis->isVisible();

		otherVis->removeSelectedEntities();
		otherVis->removeSelectedRelationships();

		for (auto e : changedVis->getSelectedEntities()) {
            
			if (otherVisIsVisible) {
				if (!e->getDualRelationship()) {
					LogConsole::showError("Entity %d has no dual element.", e->getIndex());
					continue;
				}
				else {
					otherVis->addSelectRelationshipList(e->getDualRelationship());
				}
			}
		}
		
		for (auto r : changedVis->getSelectedRelationships()) {

			if (otherVisIsVisible) {
				if (!r->getDualEntity()) {
					LogConsole::showError("Relation %d has no dual element.", r->getIndex());
					continue;
				}
				else {
					otherVis->addSelectEntityList(r->getDualEntity());
				}
			}
		}

		updateWidgets();
	}
}

bool MainWindow::dualVisVisible()
{
	return ui->dockWidget_DualVis->isVisible();
}

bool MainWindow::bipartVisVisible()
{
	return ui->dockWidget_BipartVis->isVisible();
}

QString MainWindow::getOpenDir()
{
	QString pathrelative = QString::fromStdString(m_PATH);
	return pathrelative;
}

void MainWindow::setOpenDir(QString filePath)
{
	if (filePath.isEmpty())
		return;

	QFileInfo fi(filePath);
	QString folder_name = fi.path();
	m_PATH = folder_name.toStdString();
}

void MainWindow::on_actionSave_Visualization_triggered()
{
	if (!hasFocusedVis())
		return;

	QString dir = getOpenDir();
	std::string file_name = m_FILENAME.substr(0, m_FILENAME.find_first_of('.'));
	QString target = dir + "/" + QString::fromStdString(file_name) + ".png";

	QString imagePath = QFileDialog::getSaveFileName(this,
		tr("Save Image"), target, tr("Image Files (*.png *.jpg *.bmp)"));

	string str = imagePath.toStdString();
	char filename[1024];
	strcpy(filename, str.c_str());

	if (!imagePath.isEmpty())
	{
		setOpenDir(imagePath);
		focusedVis->saveToImage(filename);
		LogConsole::writeConsole("Image saved %s", str.c_str());
	}
}

void MainWindow::on_actionSave_Visualization_with_HTML_Map_triggered()
{
	if (!hasFocusedVis())
		return;

	QString dir = getOpenDir();
	std::string file_name = m_FILENAME.substr(0, m_FILENAME.find_first_of('.'));
	QString target = dir + "/" + QString::fromStdString(file_name) + ".png";

	QString imagePath = QFileDialog::getSaveFileName(this,
		tr("Save Image"), target, tr("Image Files (*.png *.jpg *.bmp)"));

	string image_str = imagePath.toStdString();
	string html_str = image_str.substr(0, image_str.find_last_of('.')) + ".html";

	if (!imagePath.isEmpty())
	{
		setOpenDir(imagePath);
		focusedVis->saveToImage(image_str.c_str());
		focusedVis->saveImageHtmlMap(html_str);
		LogConsole::writeConsole("Image saved %s", image_str.c_str());
	}
}



void MainWindow::set_ShowEntities(bool res)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_showEntities = res;
	focusedVis->updateEntitiesVisibility();
	updateWidgets();
}

void MainWindow::set_ShowRelationships(bool res) {

	if (!hasFocusedVis())
		return;

	focusedVis->flag_showRelationships = res;
	focusedVis->updateRelationshipsVisibility();
	updateWidgets();
}

void MainWindow::set_EntityLabels(int index)
{
	if (!hasFocusedVis())
		return;

	focusedVis->entityLabels = (LABEL_TYPE)index;
	if (index != 0) 
	{
		if (focusedVis->getSelectedEntities().size() != 0) 
		{
			LogConsole::writeConsole("only the label of selected entities are shown");
			focusedVis->currentERData->updateEntityLabelVisibilities(focusedVis->getSelectedEntities());
		}
		else 
		{
			focusedVis->currentERData->updateEntityLabelVisibilities();
		}
	}
		
	updateWidgets();
}

void MainWindow::set_RelationshipLabels(int index)
{
	if (!hasFocusedVis())
		return;

	focusedVis->relationshipLabels = (LABEL_TYPE)index;
	if (index != 0)
	{
		if (focusedVis->getSelectedRelationships().size() != 0)
		{
			LogConsole::writeConsole("Only the label of selected relations are shown");
			focusedVis->currentERData->updateRelationshipsLabelVisibilities(focusedVis->getSelectedRelationships());
		}
		else
		{
			focusedVis->currentERData->updateRelationshipsLabelVisibilities();
		}
	}

	updateWidgets();
}

void MainWindow::set_ShowEntityTips(bool checked)
{
	if (!hasFocusedVis())
		return;
	
	focusedVis->flag_showEntityTips = checked;
}

void MainWindow::set_ShowRelationshipTips(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_showRelationshipTips = checked;
}

void MainWindow::set_ShowRelationshipDecal(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_showRelationshipDecal = checked;

	focusedVis->decaltextures.clear();
	focusedVis->createDecalTexture("../Resources/textures/friendship.png");
	updateWidgets();

}

void MainWindow::applyGridSeparatedLayout()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	bool ok;
	int i = QInputDialog::getInt(this, tr("Get Column Number"),
		tr("columns:"), 10, 1, 1000, 1, &ok);

	bool sort = LogConsole::showConfirm("Do you want to sort");

	focused_ERData->designLayout_GridSeparatedTree(i, sort);
	updateWidgets();
}

void MainWindow::initialize_2D_vis() {
	ui->visWdgt->view_dimension = VIEWDIMENSION::TWO_D;
	updateWidgets();
}

void MainWindow::on_actionRemove_PolygonsOfCard_triggered()
{
	if (!hasFocusedVis()) {
		return;
	}
	ERData *data = focusedVis->currentERData;

	bool ok;
	int num = QInputDialog::getInt(NULL, "Delete Polys",
		"Cardinality of:", 2, 1, 100, 1, &ok);

	if (!ok)
		return;

	data->removePolygonsByCard(num);
	ERDataChanged();
	updateWidgets();
}


void MainWindow::createDualERdata()
{
	if (!m_ERData || m_ERData->getEntityNum() == 0)
	{
		LogConsole::showError("No data to process");
		return;
	}

	if (m_Dual_ERData) {
		delete m_Dual_ERData;
		m_Dual_ERData = NULL;
	}

	m_ERData->updateEntityIndices();
	m_ERData->updateRelationshipIndices();
	m_ERData->calculateBoundingCirle();
	m_Dual_ERData = m_ERData->convertToDualERData();
	Element::initPolyColors(ColorBrewer2, COLORCOUNT);
	m_Dual_ERData->assignRelationshipColors();
	m_Dual_ERData->syncColorsWithDualElements();
	m_Dual_ERData->initializeMonogonOrientations();
	m_Dual_ERData->update2D();

	set_ERData(m_Dual_ERData, dualVis, false);
	if (!ui->dockWidget_DualVis->isVisible())
		ui->dockWidget_DualVis->show();
	
	set_ERData(m_Dual_ERData, bipartVis, false);
	if (!ui->dockWidget_BipartVis->isVisible())
		ui->dockWidget_BipartVis->show();
	bipartVis->viewStyle = VIEWSTYLE::BIPARTITE;
	
	primeVis->reset_sceneToCenterVisualization();
	dualVis->reset_sceneToCenterVisualization();
	bipartVis->reset_sceneToCenterVisualization();

	updateWidgets();
	LogConsole::writeBlueConsole("Dual Data created");
}

void MainWindow::on_actionSwitch_PrimVis_and_DualVis_triggered()
{
	ERData *oldER = m_ERData;
	ERData *oldDualER = m_Dual_ERData;
	{
		m_ERData = oldDualER;
		primeVis->currentERData = m_ERData;
		primeVis->update();
		ERDataChanged();
        
		m_Dual_ERData = oldER;
		dualVis->currentERData = m_Dual_ERData;
		dualVis->update();
	}
}

// call this when the size of ER data is changed, The model's row count will be changed by reset the model
void MainWindow::ERDataChanged(bool resetModel)
{
	updateERdataInfo(m_ERData);
}

void MainWindow::on_actionSave_Nary_Data_triggered()
{

	if (!hasFocusedVis())
		return;

	QString filters("Nary Data files (*.er);; ALL(*.*)");
	QString defaultFilter("Nary Data files (*.er)");

	QString dir = getOpenDir();
	QString target = dir + "/" + QString::fromStdString(m_FILENAME) + ".er";

	QString fileName = QFileDialog::getSaveFileName(0, "Save Nary Data file", target, filters, &defaultFilter);

	if (fileName.isEmpty())
		return;

	setOpenDir(fileName);

	string str = fileName.toStdString();
	bool res = FileIO::writeERFile(str.c_str(), focusedVis->currentERData);
	

	if (!res)
		LogConsole::writeConsole("fail to write the data");
	else
		LogConsole::writeConsole("save to " + fileName);
}

void MainWindow::on_actionOpen_Nary_Data_triggered()
{
	if (!hasFocusedVis())
		return;

	QString filters("Nary Relationship files (*.er);; ALL(*.*)");
	QString defaultFilter("Nary Relationship files (*.er)");

	QString fileName = QFileDialog::getOpenFileName(0, "Open Nary Data", getOpenDir(), filters, &defaultFilter);
	
	if (fileName.isEmpty())
		return;

	setOpenDir(fileName);

	if (fileName.isEmpty())
		return;

	string str = fileName.toStdString();
	const char* path = str.c_str();

	if(OpenERFile(path, focusedVis))
	{
		focusedVis->initializeSelection();
		focusedVis->reset_sceneToCenterVisualization();
	}
}

void MainWindow::on_actionLoad_Nary_Locations_triggered()
{

	if (!hasFocusedVis())
		return;

	QString filters("Nary Relationship files (*.er);; ALL(*.*)");
	QString defaultFilter("Nary Relationship files (*.er)");

	QString fileName = QFileDialog::getOpenFileName(0, "Open Nary Data to set locations", getOpenDir(), filters, &defaultFilter);

	if (fileName.isEmpty())
		return;

	setOpenDir(fileName);

	string str = fileName.toStdString();
	const char* path = str.c_str();

	if (FileIO::readERFileLayout(path, focusedVis->currentERData)) 
	{
		calcDegreeAndCardinality();
		focusedVis->initializeSelection();
		focusedVis->reset_sceneToCenterVisualization();
	}
}

void MainWindow::on_actionLoad_Hyperedge_Colors_triggered()
{
	if (!hasFocusedVis())
		return;

	QString filters("Nary Relationship files (*.er);; ALL(*.*)");
	QString defaultFilter("Nary Relationship files (*.er)");

	QString fileName = QFileDialog::getOpenFileName(0, "Open Nary Data to set hyperedge colors", getOpenDir(), filters, &defaultFilter);

	if (fileName.isEmpty())
		return;

	setOpenDir(fileName);

	string str = fileName.toStdString();
	const char* path = str.c_str();

	ERData* colorData = FileIO::readERFile(path);
	if (colorData == NULL)
		return;
	ERData* applyData = focusedVis->currentERData;
	
	for (Relationship* target : applyData->getRelationships())
	{
		string tlabel = target->getLabel();
		for (Relationship* source : colorData->getRelationships())
		{
			string slabel = source->getLabel();
			if (slabel.back() == char(')'))
			{
				while (slabel.back() != char('('))
					slabel.pop_back();
				slabel.pop_back();
				slabel.pop_back();
			}
			if (slabel.find(tlabel) != std::string::npos ||
				tlabel.find(slabel) != std::string::npos)
				target->setColor(source->getColor());
		}
	}

	delete (colorData);
	updateWidgets();
}

void MainWindow::on_actionOutputSubdata_triggered()
{
	QString filters("Nary Data files (*.er);; ALL(*.*)");
	QString defaultFilter("Nary Data files (*.er)");
	QString pathrelative = QDir::currentPath();
	QString fileName = QFileDialog::getExistingDirectory(0, "Select one folder", getOpenDir());
	if (fileName.isEmpty())
		return;
	string str = fileName.toStdString();
	bool res = m_ERData->writeERSubsets(str.c_str());
	if (!res)
		LogConsole::showError("fail to write the data");
}

bool MainWindow::OpenERFile(const char * path, OGLWidget * viewWidget)
{
	string fpath(path);
	string fname = fpath.substr(fpath.find_last_of("/"), fpath.length());
	m_FILENAME = fname;
	string name = "Nary Visualization File: " + fname;
	QString qname = QString::fromStdString(name);
	QWidget::setWindowTitle(qname);

	ERData *newData = FileIO::readERFile(path);
	if (newData == NULL)
	{
		return false;
	}

	set_ERData(newData, viewWidget);
	newData->update2D();
    
	string visName = "Prime Vis";
	if (viewWidget->widgetType == DUALVIS)
		visName = "Dual Vis";

	LogConsole::writeConsole(path);
	LogConsole::writeBlueConsole("[%s] Number of entities is: %d", visName.c_str(), newData->getEntityNum());
	LogConsole::writeBlueConsole("[%s] Number of relationships is: %d", visName.c_str(), newData->getRelationshipNum());
	//m_ERData->writeEulerCharacteristics(visName);
	//LogConsole::writeBlueConsole("%s", newData->FILECOMMENTS.c_str());
	calcDegreeAndCardinality();
	return true;
}

bool MainWindow::SaveERFile(const char* path)
{
	FILE *this_file = fopen(path, "w");

	if (this_file)
	{
		fprintf(this_file, "Nary Relationship Data 2D\n");
		fprintf(this_file, "Comma Delimiter\n");

		fprintf(this_file, "%d,%d\n", m_ERData->getEntityNum(), m_ERData->getRelationshipNum());

		for (int i = 0; i < m_ERData->getEntityNum(); i++) {
			Entity *v = m_ERData->getEntity(i);
			//Vertex *v = dynamic_cast<Vertex*>(n->m_primitive);

			//id	label	x y z
			fprintf(this_file, "%d,%s,%f,%f\n", i, v->getLabel().c_str(), v->getLocation().x, v->getLocation().y);
		}

		//relationships
		for (int i = 0; i < m_ERData->getRelationshipNum(); i++) {
			Relationship *re = m_ERData->getRelationship(i);

			//id	label	caridnality		entitiesIds
			fprintf(this_file, "%d,%s,%d", i, re->getLabel().c_str(), re->getCardinality());

			for (int n = 0; n < re->getCardinality(); n++) {
				fprintf(this_file, ",%d", re->getIncidentEntity(n)->getIndex());
			}

			fprintf(this_file, "\n");
		}

		fclose(this_file);

		return true;
	}
	
	return false;
}

void MainWindow::reorderSelectedPolygons()
{
	if (!focusedVis)
	{
		LogConsole::showError("No focused vis");
		return;
	}


	if (focusedVis->getSelectedRelationships().size()<1) {
		LogConsole::showError("No relationships are selected to reoder");
		return;
	}

	if (focusedVis->getSelectedRelationship(0)->getIncidentEntities().size()<3) {
		LogConsole::showError("No way to reorder a binary relationship");
		return;
	}

	focusedVis->getSelectedRelationship(0)->randomeCreateNewObserver();
	updateWidgets();
}

void MainWindow::ResetMonogonsAngles()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	vector<Entity*> enlist = focusedVis->getSelectedEntities();
	focused_ERData->resetMonogonAngles(enlist);

	focused_ERData->update2D();
	updateWidgets();
}

void MainWindow::DistributeMonogonAngles()
{
	if (!hasFocusedVis())
		return;
	ERData* erdata = focusedVis->currentERData;

	erdata->distributeMonogonAngles(erdata->getEntities());
	erdata->update2D();
}

void MainWindow::calcDegreeAndCardinality() {

	if (!hasFocusedVis())
		return;

	ERData *focused_ERData = focusedVis->currentERData;

	focused_ERData->calculateEntityDegree();
	focused_ERData->calculateRelationshipDegree();
	focused_ERData->calculateEntityCard();
	focused_ERData->calculateRelationshipCard();


	/*entity degree filters*/
	control_dock->ui->spinBox_VertexMinDegree->setMaximum(focused_ERData->maxEntityDegree);
	control_dock->ui->spinBox_VertexMinDegree->setMinimum(focused_ERData->minEntityDegree);
	control_dock->ui->spinBox_VertexMinDegree->setValue(focused_ERData->minEntityDegree);

	control_dock->ui->spinBox_VertexMaxDegree->setMaximum(focused_ERData->maxEntityDegree);
	control_dock->ui->spinBox_VertexMaxDegree->setMinimum(focused_ERData->minEntityDegree);
	control_dock->ui->spinBox_VertexMaxDegree->setValue(focused_ERData->maxEntityDegree);

	focusedVis->FilterEntityDegreeBottom = control_dock->ui->spinBox_VertexMinDegree->value();
	focusedVis->FilterEntityDegreeTop = control_dock->ui->spinBox_VertexMaxDegree->value();

	/*relationship degree filters*/
	control_dock->ui->spinBox_PolyMinDegree->setMaximum(focused_ERData->maxRelationshipDegree);
	control_dock->ui->spinBox_PolyMinDegree->setMinimum(focused_ERData->minRelationshipDegree);
	control_dock->ui->spinBox_PolyMinDegree->setValue(focused_ERData->minRelationshipDegree);

	control_dock->ui->spinBox_PolyMaxDegree->setMaximum(focused_ERData->maxRelationshipDegree);
	control_dock->ui->spinBox_PolyMaxDegree->setMinimum(focused_ERData->minRelationshipDegree);
	control_dock->ui->spinBox_PolyMaxDegree->setValue(focused_ERData->maxRelationshipDegree);

	focusedVis->FilterRelationshipDegreeBottom = control_dock->ui->spinBox_PolyMinDegree->value();
	focusedVis->FilterRelationshipDegreeTop = control_dock->ui->spinBox_PolyMaxDegree->value();

	/*entity cardinality filters*/
	control_dock->ui->spinBox_EntityCardinalityBottom->setMaximum(focused_ERData->maxEntityCardinality);
	control_dock->ui->spinBox_EntityCardinalityBottom->setMinimum(focused_ERData->minEntityCardinality);
	control_dock->ui->spinBox_EntityCardinalityBottom->setValue(focused_ERData->minEntityCardinality);

	control_dock->ui->spinBox_EntityCardinalityTop->setMaximum(focused_ERData->maxEntityCardinality);
	control_dock->ui->spinBox_EntityCardinalityTop->setMinimum(focused_ERData->minEntityCardinality);
	control_dock->ui->spinBox_EntityCardinalityTop->setValue(focused_ERData->maxEntityCardinality);

	focusedVis->FilterEntityCardBottom = control_dock->ui->spinBox_EntityCardinalityBottom->value();
	focusedVis->FilterEntityCardTop = control_dock->ui->spinBox_EntityCardinalityTop->value();

	/*relationship cardinality filters*/
	control_dock->ui->spinBox_RelationshipCardinalityBottom->setMaximum(focused_ERData->maxRelationshipCardinality);
	control_dock->ui->spinBox_RelationshipCardinalityBottom->setMinimum(focused_ERData->minRelationshipCardinality);
	control_dock->ui->spinBox_RelationshipCardinalityBottom->setValue(focused_ERData->minRelationshipCardinality);

	control_dock->ui->spinBox_RelationshipCardinalityTop->setMaximum(focused_ERData->maxRelationshipCardinality);
	control_dock->ui->spinBox_RelationshipCardinalityTop->setMinimum(focused_ERData->minRelationshipCardinality);
	control_dock->ui->spinBox_RelationshipCardinalityTop->setValue(focused_ERData->maxRelationshipCardinality);

	focusedVis->FilterRelationshipCardBottom = control_dock->ui->spinBox_RelationshipCardinalityBottom->value();
	focusedVis->FilterRelationshipCardTop = control_dock->ui->spinBox_RelationshipCardinalityTop->value();

    /*Relationship index filters*/
    int rcount = focused_ERData->getRelationshipNum();
    rcount -= 1;
	control_dock->ui->spinBox_RelationshipIndexBottom->setMaximum(rcount);
	control_dock->ui->spinBox_RelationshipIndexTop->setMaximum(rcount);

	control_dock->ui->spinBox_RelationshipIndexBottom->setMinimum(0);
	control_dock->ui->spinBox_RelationshipIndexTop->setMinimum(0);

	control_dock->ui->spinBox_RelationshipIndexBottom->setValue(0);
	control_dock->ui->spinBox_RelationshipIndexTop->setValue(rcount);

    /*Entity index filters*/
    int ecount = focused_ERData->getEntityNum();
    ecount -= 1;
	control_dock->ui->spinBox_EntityIndexBottom->setMaximum(ecount);
	control_dock->ui->spinBox_EntityIndexTop->setMaximum(ecount);

	control_dock->ui->spinBox_EntityIndexBottom->setMinimum(0);
	control_dock->ui->spinBox_EntityIndexTop->setMinimum(0);

	control_dock->ui->spinBox_EntityIndexBottom->setValue(0);
	control_dock->ui->spinBox_EntityIndexTop->setValue(ecount);

	/*simplifiction operation limit*/
	int op_max = rcount + ecount + 2;
	control_dock->ui->spinBox_MaxOps->setMaximum(op_max);
	control_dock->ui->spinBox_MaxOps->setValue(op_max);

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::calcElementStatistics()
{
	if (!hasFocusedVis())
		return;
	ERData* erdata = focusedVis->currentERData;

	erdata->calcAllStatsLocally();
}

void MainWindow::entityIndexBottomChanged(int arg1)
{

	if (!focusedVis)
	{
		LogConsole::showError("No focused vis");
		return;
	}

    int currentMax = control_dock->ui->spinBox_EntityIndexTop->value();
    if (arg1 > currentMax)
    {
		control_dock->ui->spinBox_EntityIndexTop->setValue(arg1);
    }
	
	focusedVis->FilterEntityIndexBottom = control_dock->ui->spinBox_EntityIndexBottom->value();
	focusedVis->FilterEntityIndexTop = control_dock->ui->spinBox_EntityIndexTop->value();
	focusedVis->updateVisibility();
    updateWidgets();
}

void MainWindow::entityIndexTopChanged(int arg1)
{

	if (!focusedVis)
	{
		LogConsole::showError("No focused vis");
		return;
	}

    int currentMin = control_dock->ui->spinBox_EntityIndexBottom->value();
    if (arg1 < currentMin)
    {
		control_dock->ui->spinBox_EntityIndexBottom->setValue(arg1);
    }

    focusedVis->FilterEntityIndexBottom = control_dock->ui->spinBox_EntityIndexBottom->value();
    focusedVis->FilterEntityIndexTop = control_dock->ui->spinBox_EntityIndexTop->value();
    focusedVis->updateVisibility();
    updateWidgets();
}

void MainWindow::set_EntityIndexFilter(bool checked)
{
	if (!focusedVis)
	{
		LogConsole::showError("No focused vis");
		return;
	}

	focusedVis->flag_entityIndexFilter = checked;

	focusedVis->FilterEntityIndexBottom = control_dock->ui->spinBox_EntityIndexBottom->value();
	focusedVis->FilterEntityIndexTop = control_dock->ui->spinBox_EntityIndexTop->value();

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::relationshipIndexBottomChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

    int currentMax = control_dock->ui->spinBox_RelationshipIndexTop->value();
    if (arg1 > currentMax){
		control_dock->ui->spinBox_RelationshipIndexTop->setValue(arg1);
    }

    focusedVis->FilterRelationshipIndexBottom = control_dock->ui->spinBox_RelationshipIndexBottom->value();
    focusedVis->FilterRelationshipIndexTop = control_dock->ui->spinBox_RelationshipIndexTop->value();
    focusedVis->updateVisibility();
    updateWidgets();
}

void MainWindow::relationshipIndexTopChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

    int currentMin = control_dock->ui->spinBox_RelationshipIndexBottom->value();
    if (arg1 < currentMin){
		control_dock->ui->spinBox_RelationshipIndexBottom->setValue(arg1);
    }

    focusedVis->FilterRelationshipIndexBottom = control_dock->ui->spinBox_RelationshipIndexBottom->value();
    focusedVis->FilterRelationshipIndexTop = control_dock->ui->spinBox_RelationshipIndexTop->value();
    focusedVis->updateVisibility();
    updateWidgets();
}

void MainWindow::set_RelationshipIndexFilter(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_relationshipIndexFilter = checked;
	focusedVis->FilterRelationshipIndexBottom = control_dock->ui->spinBox_RelationshipIndexBottom->value();
	focusedVis->FilterRelationshipIndexTop = control_dock->ui->spinBox_RelationshipIndexTop->value();
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::vertexMinDegreeChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

	int currentMax = control_dock->ui->spinBox_VertexMaxDegree->value();
    if (arg1 > currentMax)
    {
		control_dock->ui->spinBox_VertexMaxDegree->setValue(arg1);
    }

	focusedVis->FilterEntityDegreeBottom = control_dock->ui->spinBox_VertexMinDegree->value();
	focusedVis->FilterEntityDegreeTop = control_dock->ui->spinBox_VertexMaxDegree->value();

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::vertexMaxDegreeChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

	int currentMin = control_dock->ui->spinBox_VertexMinDegree->value();
	if (arg1 < currentMin)
    {
		control_dock->ui->spinBox_VertexMinDegree->setValue(arg1);
    }

	focusedVis->FilterEntityDegreeBottom = control_dock->ui->spinBox_VertexMinDegree->value();
	focusedVis->FilterEntityDegreeTop = control_dock->ui->spinBox_VertexMaxDegree->value();

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::set_EntityDegreeFilter(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_entityDegreeFilter = checked;
	focusedVis->FilterEntityDegreeBottom = control_dock->ui->spinBox_VertexMinDegree->value();
	focusedVis->FilterEntityDegreeTop = control_dock->ui->spinBox_VertexMaxDegree->value();

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::polyMinDegreeChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

	int currentMax = control_dock->ui->spinBox_PolyMaxDegree->value();
	if (arg1 > currentMax)
    {
		control_dock->ui->spinBox_PolyMaxDegree->setValue(arg1);
    }

	focusedVis->FilterRelationshipDegreeBottom = control_dock->ui->spinBox_PolyMinDegree->value();
	focusedVis->FilterRelationshipDegreeTop = control_dock->ui->spinBox_PolyMaxDegree->value();

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::polyMaxDegreeChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

	int currentMin = control_dock->ui->spinBox_PolyMinDegree->value();
	if (arg1 < currentMin)
    {
		control_dock->ui->spinBox_PolyMinDegree->setValue(arg1);
    }

	focusedVis->FilterRelationshipDegreeBottom = control_dock->ui->spinBox_PolyMinDegree->value();
	focusedVis->FilterRelationshipDegreeTop = control_dock->ui->spinBox_PolyMaxDegree->value();
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::set_RelationshipDegreeFilter(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_relationshipDegreeFilter = checked;
	focusedVis->FilterRelationshipDegreeBottom = control_dock->ui->spinBox_PolyMinDegree->value();
	focusedVis->FilterRelationshipDegreeTop = control_dock->ui->spinBox_PolyMaxDegree->value();
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::entityCardinalityBottomChanged(int arg1) {

	if (!hasFocusedVis())
		return;

	int currentMax = control_dock->ui->spinBox_EntityCardinalityTop->value();
	if (arg1 > currentMax)
    {
		control_dock->ui->spinBox_EntityCardinalityTop->setValue(arg1);
    }

	focusedVis->FilterEntityCardBottom = control_dock->ui->spinBox_EntityCardinalityBottom->value();
	focusedVis->FilterEntityCardTop = control_dock->ui->spinBox_EntityCardinalityTop->value();

	focusedVis->updateVisibility();
	updateWidgets();

}

void MainWindow::entityCardinalityTopChanged(int arg1) {

	if (!hasFocusedVis())
		return;

	int currentMin = control_dock->ui->spinBox_EntityCardinalityBottom->value();
	if (arg1 < currentMin)
    {
		control_dock->ui->spinBox_EntityCardinalityBottom->setValue(arg1);
    }


	focusedVis->FilterEntityCardBottom = control_dock->ui->spinBox_EntityCardinalityBottom->value();
	focusedVis->FilterEntityCardTop = control_dock->ui->spinBox_EntityCardinalityTop->value();

	focusedVis->updateVisibility();
	updateWidgets();

}

void MainWindow::set_EntityCardinalityFilter(bool checked) {

	if (!hasFocusedVis())
		return;

	focusedVis->flag_entityCardinalityFilter = checked;
	focusedVis->FilterEntityCardBottom = control_dock->ui->spinBox_EntityCardinalityBottom->value();
	focusedVis->FilterEntityCardTop = control_dock->ui->spinBox_EntityCardinalityTop->value();

	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::relationshipCardinalityBottomChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

	int currentMin = control_dock->ui->spinBox_RelationshipCardinalityTop->value();
	if (arg1 > currentMin)
    {
		control_dock->ui->spinBox_RelationshipCardinalityTop->setValue(arg1);
    }

	focusedVis->FilterRelationshipCardTop = control_dock->ui->spinBox_RelationshipCardinalityTop->value();
	focusedVis->FilterRelationshipCardBottom = control_dock->ui->spinBox_RelationshipCardinalityBottom->value();
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::relationshipCardinalityTopChanged(int arg1)
{
	if (!hasFocusedVis())
		return;

	int currentMin = control_dock->ui->spinBox_RelationshipCardinalityBottom->value();
	if (arg1 < currentMin)
    {
		control_dock->ui->spinBox_RelationshipCardinalityBottom->setValue(arg1);
    }

	focusedVis->FilterRelationshipCardTop = control_dock->ui->spinBox_RelationshipCardinalityTop->value();
	focusedVis->FilterRelationshipCardBottom = control_dock->ui->spinBox_RelationshipCardinalityBottom->value();
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::set_polygonCardinalityFilter(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_relationshipCardinalityFilter = checked;
	focusedVis->FilterRelationshipCardBottom = control_dock->ui->spinBox_RelationshipCardinalityBottom->value();
	focusedVis->FilterRelationshipCardTop = control_dock->ui->spinBox_RelationshipCardinalityTop->value();
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::on_actionBringReToFront_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	if (focusedVis->getSelectedRelationships().size()<1) {
		LogConsole::showError("No relationships are selected");
		return;
	}

	//rendered front, ordered later
	//focused_ERData->bringRelationshipBackward(focusedVis->getSelectedRelationship(0));
	focused_ERData->sendRelationshipsToBack(focusedVis->getSelectedRelationships());

	focused_ERData->assignIndices();
	updateWidgets();
}

void MainWindow::on_actionBringReToBack_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	if (focusedVis->getSelectedRelationships().size()<1) {
		LogConsole::showError("No relationships are selected");
		return;
	}

	//rendered back, ordered first
	//focused_ERData->bringRelationshipForward(focusedVis->getSelectedRelationship(0));
	focused_ERData->sendRelationshipsToFront(focusedVis->getSelectedRelationships());

	focused_ERData->assignIndices();
	updateWidgets();
}

void MainWindow::on_actionFontSettings_triggered() {
	fontsettingDlg->setWindowModality(Qt::WindowModal);
	fontsettingDlg->show();
}

void MainWindow::set_InheritVisibilities(bool checked)
{
	if (!hasFocusedVis())
		return;

	focusedVis->flag_inheritVisibility = checked;
	focusedVis->updateVisibility();
	updateWidgets();
}

void MainWindow::on_actionRemoveBinaryRelationshipsOfNary_triggered()
{

	int ret = LogConsole::showConfirm("Removing binary relationships inside each N-ary relationship is an irreversible process, do you still want to preceed?");
	if (ret) {
		int re_b = m_ERData->removeBinaryEnclosedByRelationships();
		LogConsole::writeConsole("Removed %d binary relationships", re_b);
		
		ERDataChanged();
		updateWidgets();
	}
}

void MainWindow::on_actionRemove_Multiple_Diagon_triggered()
{
	int ret = LogConsole::showConfirm("Removing Multiple relationships inside each N-ary relationship is an irreversible process, do you still want to preceed?");
	if (ret) {
		int re_b = m_ERData->removeMultipleRelationships();
		LogConsole::writeConsole("Removed %d Multiple Relationships", re_b);
		ERDataChanged();
		updateWidgets();
	}
}

void MainWindow::on_actionRemove_Non_repeated_Relationships_triggered()
{
	int ret = LogConsole::showConfirm("Removing Multiple relationships inside each N-ary relationship is an irreversible process, do you still want to preceed?");
	if (ret) {
		int re_b = m_ERData->removeNonMultipleRelationships();
		LogConsole::writeConsole("Removed %d Multiple Relationships", re_b);
		ERDataChanged();
		updateWidgets();
	}
}

void MainWindow::on_actionConvert_to_Bipartite_triggered()
{
	/*create relationships from cliques*/
	if (!hasFocusedVis())
		return;
	ERData* focused_ERData = focusedVis->currentERData;

	focused_ERData->convertToBipartiteGraphData();
	focused_ERData->update2D();

	updateWidgets();
}

void MainWindow::on_actionConvert_to_Linear_triggered()
{
	int ret = LogConsole::showConfirm("Converting to linear hypergraph is an irreversible process, do you still want to preceed?");
	if (ret) {
		m_ERData->convertToLinear();
		ERDataChanged();
		updateWidgets();
		m_ERData->update2D();
	}
}

void MainWindow::on_actionConver_to_Simple_triggered()
{
	m_ERData->convertToSimple();
	ERDataChanged();
	m_ERData->update2D();

	updateWidgets();
}

void MainWindow::on_actionConvert_to_2_Linear_triggered()
{
	int ret = LogConsole::showConfirm("Converting to 2-linear hypergraph is an irreversible process, do you still want to preceed?");
	if (ret) {
		m_ERData->convertToLinear(2);
		m_ERData->update2D();
		ERDataChanged();
		updateWidgets();
	}
}

void MainWindow::on_actionRemoveUnvisibleElements_triggered()
{
	int ret = LogConsole::showConfirm("Removing elements is an irreversible process, do you still want to preceed?");
	if (ret) {
		int re_e, re_r;
		m_ERData->removeUnvisibleElements(re_e, re_r);
		LogConsole::writeConsole("removed entities:%d", re_e);
		LogConsole::writeConsole("removed relationships:%d", re_r);
		ERDataChanged();
		updateWidgets();
	}
}

void MainWindow::selectionActionsFresh(OGLWidget*widget) {

	if (widget)
	{

		widget->m_selectState = SELECTSTATE::FINISHED;
		widget->clearSelectRegion();
		widget->clearSwapList();
		updateWidgets();
	}
}

void MainWindow::setVisSelectEntity(OGLWidget * widget, bool checked)
{
	if (widget)
	{
		if (checked && widget->viewStyle == VIEWSTYLE::POLYGON) {
			widget->m_mouseAction = MOUSEACTION::SELECT;
			widget->m_selectType = SELECTTYPE::SELECTENTITY;
		}
		else
			widget->m_selectType = SELECTTYPE::NONSELECT;

		widget->setCursor(QCursor(Qt::CrossCursor));
		selectionActionsFresh(widget);
	}
}

void MainWindow::on_actionSelectEntities_triggered(bool checked)
{
	auto widget = focusedVis;
	if (widget) 
	{

		if (checked) {
			widget->m_mouseAction = MOUSEACTION::SELECT;
			widget->m_selectType = SELECTTYPE::SELECTREGION;
		}
		else {
			widget->m_selectType = SELECTTYPE::NONSELECT;
		}

		widget->setCursor(QCursor(Qt::CrossCursor));
		selectionActionsFresh(widget);
	}
}

void MainWindow::on_actionSelectEntity_triggered(bool checked)
{
	auto widget = focusedVis;
	setVisSelectEntity(widget, checked);
}

void MainWindow::on_actionSelectRelationship_triggered(bool checked)
{
	auto widget = focusedVis;
	if (widget)
	{
		if (checked) {
			widget->m_mouseAction = MOUSEACTION::SELECT;
			widget->m_selectType = SELECTTYPE::SELECTRELATIONSHIP;
		}
		else
			widget->m_selectType = SELECTTYPE::NONSELECT;

		widget->setCursor(QCursor(Qt::CrossCursor));
		selectionActionsFresh(widget);
	}
}

void MainWindow::on_actionselectEnclosingRelationships_triggered()
{
	if (primeVis->getSelectedEntities().size() == 0)
	{
		LogConsole::showConfirm("please select nodes first");
		return;
	}

	vector<Relationship*> rlist = m_ERData->getEntityEnclosingRelationships(ui->visWdgt->getSelectedEntities());

	for (auto re : rlist) {
		primeVis->addSelectRelationshipList(re);
	}

	m_ERData->update2D();
	updateWidgets();

	primeVis->removeSelectedEntities();
	primeVis->clearSelectRegion();
}

void MainWindow::on_actionaddEntity_triggered(bool checked)
{
	auto widget = primeVis;

	if(focusedVis == primeVis)
	{
		if (checked) {
			widget->m_mouseAction = MOUSEACTION::ADDENTITY;
		}
		else {
			widget->m_mouseAction = MOUSEACTION::NOACTION;
		}

		widget->setCursor(QCursor(Qt::CrossCursor));

		selectionActionsFresh(widget);
	}
	else {
		LogConsole::showError("cannot change data in the dual view");
	}
}

void MainWindow::on_actionShow_Vis_Sample_Num_triggered()
{
	QSurfaceFormat fmt = primeVis->format();
	LogConsole::writeConsole("Number of samples: %d", fmt.samples());

	fmt.setSamples(16);
	primeVis->setFormat(fmt);

	fmt = primeVis->format();
	LogConsole::writeConsole("Number of samples is changed to: %d", fmt.samples());
}

void MainWindow::on_actionaddRandomEntities_triggered()
{
	if (focusedVis == primeVis)
	{
		bool ok;

		int num = QInputDialog::getInt(NULL, "Input",
			"Entity Num:",
			1, 0, 1000, 1, &ok);

		if (!ok)
			return;

		QString text = QInputDialog::getText(NULL, "Input",
			"Entity Label:", QLineEdit::Normal,
			"", &ok);
		if (!ok)
			return;
			
		m_ERData->addEntities(num, text.toStdString());

		ERDataChanged();
		updateWidgets();
	}
	else {
		LogConsole::showError("cannot change data in the dual view");
	}
}

void MainWindow::on_actionaddRelationship_triggered()
{
	if (focusedVis == primeVis)
	{
		bool ok;

		int card = QInputDialog::getInt(NULL, "Input",
			"Relationship Cardinality:",
			3, 1, 100, 1, &ok);

		if (!ok)
			return;

		QString currentRelationIndex = QString::number(m_ERData->getRelationshipNum());
		QString text = QInputDialog::getText(NULL, "Input",
			"Relationship Label:", QLineEdit::Normal,
			"R" + currentRelationIndex, &ok);
		if (!ok || text.isEmpty())
			return;

		double edgeLen = expectedLength;
		double ccr = edgeLen / (2.0* sin(M_PI / (double)card));

		Relationship* newR = m_ERData->addRelationship(card, text.toStdString(), ccr);

		Vec3 pos = primeVis->m_cursorPos;
		newR->translateLocation(pos.x, pos.y);

		ERDataChanged();
		updateWidgets();
	}
	else {
		LogConsole::showError("cannot change data in the dual view");
	}
}

void MainWindow::on_actionCreateRelationship_triggered()
{
	if (focusedVis == primeVis)
	{
		REList relist = primeVis->getSelectedRelationships();
		vector<int> indicesOfPolysToMerge;

		for (Relationship* re : relist)
		{
			indicesOfPolysToMerge.push_back(re->getIndex());
			re->setSelected(false);
		}
		primeVis->clearSelectRelationshipList();
		relist.clear();

		vector<int> indicesOfEntitiesToMerge = m_ERData->getEntityIndicesOfRelationships(indicesOfPolysToMerge);
		for (int i = 0; i < indicesOfEntitiesToMerge.size(); i++) {
			primeVis->addSelectEntityList(m_ERData->getEntity(indicesOfEntitiesToMerge[i]));
		}

		if (primeVis->getSelectedEntities().size() == 0) {
			LogConsole::showError("please select at least one entity.");
			return;
		}

		string name = "R" + to_string(m_ERData->getRelationshipNum());

		//bool ok;
		//QString currentRelationIndex = QString::number(m_ERData->getRelationshipNum());
		//string name;
		//QString text = QInputDialog::getText(NULL, "Input",
		//	"Relationship Label:", QLineEdit::Normal,
		//	"R" + currentRelationIndex, &ok);
		//if (ok && !text.isEmpty())
		//	name = text.toStdString();
		//else
		//	return;

		ENList enlist = primeVis->getSelectedEntities();
		bool res = m_ERData->mergeToRelationship(enlist, relist, name);

		if (res) 
		{
			primeVis->removeSelectedEntities();
			primeVis->clearSelectRegion();
			primeVis->m_selectState = SELECTSTATE::FINISHED;
		}
		else 
		{
			LogConsole::writeConsole("Merging fails");
		}

		ERDataChanged();
		m_ERData->update2D();
		updateWidgets();
	}
}

void MainWindow::on_actionMoveSelect_triggered(bool checked)
{
	auto widget = focusedVis; 
	if(widget)
	{

		if (checked) {
			widget->m_mouseAction = MOUSEACTION::MOVEELEMENTS;
			widget->setCursor(QCursor(Qt::SizeAllCursor));
		}
		else {
			widget->m_mouseAction = MOUSEACTION::NOACTION;
			widget->setCursor(QCursor(Qt::CrossCursor));
		}


		widget->clearSelectRegion();
		widget->m_selectState = SELECTSTATE::FINISHED;
		updateWidgets();
	}
}

void MainWindow::on_actionEditElement_triggered()
{

	if (focusedVis == primeVis)
	{
		if (primeVis->getSelectedEntities().size() > 1 || primeVis->getSelectedRelationships().size() > 1) {

			int ret = LogConsole::showConfirm("More than one elements is selected, all of them will be modified to the same. Do you want to proceed?");
			if (!ret)
				return;
		}
		else if(primeVis->getSelectedEntities().size() + primeVis->getSelectedRelationships().size() == 0){
			LogConsole::showError("Please select elements first!");
			return;
		}

		modifySelectedElementsDlg->setWindowModality(Qt::WindowModal);
		modifySelectedElementsDlg->init(primeVis->getSelectedEntities(), primeVis->getSelectedRelationships());
		modifySelectedElementsDlg->show();

	}
	else {
		LogConsole::showError("cannot modify data in the dual vis");
	}
}

void MainWindow::on_actionManual_triggered()
{
	HelpDialog helpdia;
	helpdia.exec();
}

void MainWindow::on_actionCloseAllDockableWidgets_triggered()
{
	QList<QDockWidget*> dockwidgets = findChildren<QDockWidget*>();
	for (auto dw : dockwidgets)
		dw->close();
}

void MainWindow::actionDeleteEntities()
{
	ENList entities = focusedVis->getSelectedEntities();
	if (entities.size() < 1)
		return;

	ERData* erdata = focusedVis->currentERData;
	ERData* dualerdata = (erdata == m_ERData) ? m_Dual_ERData : m_ERData;

	for (Entity* en : entities)
	{
		Relationship* re = en->getDualRelationship();
		if (re)
		{
			dualerdata->deleteRelationship(re, false);
		}
		erdata->deleteEntity(en, false);
	}

	focusedVis->clearSelectEntityList();

	erdata->update2D();
	ERDataChanged();
	updateWidgets();
}

void MainWindow::on_actionMergeElements_triggered() 
{
	ERData* erdata = focusedVis->currentERData;
	ERData* dualerdata = (erdata == m_ERData) ? m_Dual_ERData : m_ERData;

	ENList enlist = focusedVis->getSelectedEntities();
	REList relist = focusedVis->getSelectedRelationships();

	bool name_ok;
	if (enlist.size() > 1)
	{
		// get name of new entity
		QString currentEntityIndex = QString::number(erdata->getEntityNum());
		string name;
		QString text = QInputDialog::getText(NULL, "Input",
			"Entity Label:", QLineEdit::Normal,
			"E" + currentEntityIndex, &name_ok);
		if (name_ok && !text.isEmpty())
			name = text.toStdString();
		else
			return;

		REList dual_relist;
		for (Entity* en : enlist)
		{
			Relationship* dual_re = en->getDualRelationship();
			if (dual_re)
				dual_relist.push_back(dual_re);
		}
		
		Entity* newEn = erdata->mergeEntities(enlist, name);
		Relationship* newRe = NULL;
		if (enlist.size() == dual_relist.size())
		{
			newRe = dualerdata->mergeRelationships(dual_relist, name);
		}

		bool can_distribute = newEn->distributeMonogons();
		if (!can_distribute)
		{
			for (auto adjRe : newEn->getIncidentRelationships()) {

				if (adjRe->getCardinality() == 1)
				{
					adjRe->initializeMonogonOrientation();
				}
			}
		}

		newEn->setDualElement(newRe);
		if (newRe)
			newRe->setDualElement(newEn);
	}

	if (relist.size() > 1)
	{
		// get name of new relationship
		QString currentEntityIndex = QString::number(erdata->getEntityNum());
		string name;
		QString text = QInputDialog::getText(NULL, "Input",
			"Entity Label:", QLineEdit::Normal,
			"R" + currentEntityIndex, &name_ok);
		if (name_ok && !text.isEmpty())
			name = text.toStdString();
		else
			return;

		ENList dual_enlist;
		for (Relationship* re : relist)
		{
			Entity* dual_en = re->getDualEntity();
			if (dual_en)
				dual_enlist.push_back(dual_en);
		}

		Relationship* newRe = erdata->mergeRelationships(relist, name);
		Entity* newEn = NULL;
		if (relist.size() == dual_enlist.size())
		{
			newEn = dualerdata->mergeEntities(dual_enlist, name);
			bool can_distribute = newEn->distributeMonogons();
			if (!can_distribute)
			{
				for (auto adjRe : newEn->getIncidentRelationships()) {

					if (adjRe->getCardinality() == 1)
					{
						adjRe->initializeMonogonOrientation();
					}
				}
			}
		}

		newRe->setDualElement(newEn);
		if (newEn)
			newEn->setDualElement(newRe);
	}

	focusedVis->clearSelectRelationshipList();
	focusedVis->removeSelectedEntities();
	focusedVis->clearSelectRegion();
	focusedVis->m_selectState = SELECTSTATE::FINISHED;
	
	erdata->update2D();
	ERDataChanged();
	if (dualerdata)
		dualerdata->update2D();
	updateWidgets();
}

void MainWindow::on_actionSplitElements_triggered()
{

}

void MainWindow::on_actionDeleteSelectedElements_triggered()
{
	actionDeleteEntities();
	actionDeleteRelationships();
}

vector<int> MainWindow::getIndexListFromLineEdit()
{
	vector<int> intlist;
	QString text = ui->dockWidgetContents_Information->ui.lineEdit_SelectInput->text();
	QStringList list = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);

	for (int i = 0; i < list.size(); ++i) {
		int idex = list.at(i).toInt();

		if (idex <0)
			continue;

		intlist.push_back(idex);
		LogConsole::writeConsole("index: %d", idex);
	}

	return intlist;
}

vector<string> MainWindow::getLabelListFromLineEdit()
{
	vector<string> labellist;

	QString text = ui->dockWidgetContents_Information->ui.lineEdit_SelectInput->text();
	QStringList list = text.split(QRegExp("\\s+"), QString::SkipEmptyParts);

	int max = m_ERData->getRelationshipNum();
	for (int i = 0; i < list.size(); ++i) {

		labellist.push_back(list[i].toStdString());
		LogConsole::writeConsole("label:%s", list[i].toStdString().c_str());
	}

	return labellist;
}

void MainWindow::select_entities_fromInput()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	if (ui->dockWidgetContents_Information->ui.radioButton_indices->isChecked()) {

		vector<int> indicesOfEntitiesToMerge = getIndexListFromLineEdit();

		for (int i = 0; i < indicesOfEntitiesToMerge.size(); i++) {
			focusedVis->addSelectEntityList(focused_ERData->getEntity(indicesOfEntitiesToMerge[i]));
			focusedVis->showSelectedElementInfo();
		}
	}

	if (ui->dockWidgetContents_Information->ui.radioButton_Lables->isChecked()) {

		vector<string> labelsOfEntitiesToMerge = getLabelListFromLineEdit();

		for (int i = 0; i < labelsOfEntitiesToMerge.size(); i++) {
			focusedVis->addSelectEntityList(focused_ERData->getEntity(labelsOfEntitiesToMerge[i]));
			focusedVis->showSelectedElementInfo();
		}
	}

	updateWidgets();
}

void MainWindow::select_relationships_fromInput()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	if (ui->dockWidgetContents_Information->ui.radioButton_indices->isChecked()) {

		vector<int> indicesOfResToMerge = getIndexListFromLineEdit();

		for (int i = 0; i < indicesOfResToMerge.size(); i++) {
			focusedVis->addSelectRelationshipList(focused_ERData->getRelationship(indicesOfResToMerge[i]));
			focusedVis->showSelectedElementInfo();
		}
	}

	if (ui->dockWidgetContents_Information->ui.radioButton_Lables->isChecked()) {

		vector<string> labelsOfResToMerge = getLabelListFromLineEdit();

		for (int i = 0; i < labelsOfResToMerge.size(); i++) {
			focusedVis->addSelectRelationshipList(focused_ERData->getRelationship(labelsOfResToMerge[i]));
			focusedVis->showSelectedElementInfo();
		}
	}

	updateWidgets();
}

void MainWindow::Highlight_selectedElements()
{
	for (int i = 0; i < 4; i++) {
		QTimer::singleShot(1000 * i, this, SLOT(Highlight_selectedElementsOnce()));
	}
}

void MainWindow::Highlight_selectedElementsOnce()
{
	double scale = 1.25;

	for (int i = 0; i < 5; i++) {
		QCoreApplication::processEvents();

		primeVis->Highlight_selectedElementsOneFrame(scale);
		dualVis->Highlight_selectedElementsOneFrame(scale);

		primeVis->repaint();
		dualVis->repaint();
	}

	for (int i = 0; i < 5; i++) {
		QCoreApplication::processEvents();

		primeVis->Highlight_selectedElementsOneFrame(1.0/scale);
		dualVis->Highlight_selectedElementsOneFrame(1.0 / scale);

		primeVis->repaint();
		dualVis->repaint();
	}
}

void MainWindow::on_actionMergeEnclosedPolygon_triggered()
{
	if (focusedVis == primeVis)
	{
		if (primeVis->getSelectedEntities().size() == 0) {
			LogConsole::showError("Please select entities at first");
			return;
		}

		vector<Relationship*> rlist = m_ERData->getEntityEnclosingRelationships(primeVis->getSelectedEntities());

		bool res = m_ERData->mergeToRelationship(vector<Entity*>(), rlist);

		if (res) {
			primeVis->clearSelectRelationshipList();
			primeVis->removeSelectedEntities();
			primeVis->clearSelectRegion();
			primeVis->m_selectState = SELECTSTATE::FINISHED;
		}
		else {
			LogConsole::writeConsole("Merging fails");
		}

		ERDataChanged();
		updateWidgets();
	}
	else {
		LogConsole::showError("cannot modify data in the dual vis");
	}
}

void MainWindow::on_actionDivideRelationship_triggered() 
{
	REList relist = focusedVis->getSelectedRelationships();
	ENList enlist = focusedVis->getSelectedEntities();

	if (relist.size() != 1)
	{
		LogConsole::showError("Select only one relationship.");
		return;
	}

	if (enlist.size() == 0)
	{
		LogConsole::showError("Select at least one entity.");
		return;
	}

	bool res = m_ERData->divideEntitiesFromRelationship(enlist, relist.front());

	relist.front()->setSelected(false);
	primeVis->removeSelectedEntities();
	primeVis->removeSelectedRelationships();
	dualVis->removeSelectedEntities();
	dualVis->removeSelectedRelationships();

	ERDataChanged();
	updateWidgets();
}

void MainWindow::actionDeleteRelationships() 
{
	REList relationships = focusedVis->getSelectedRelationships();
	if (relationships.size() < 1)
		return;

	ERData* erdata = focusedVis->currentERData;
	ERData* dualerdata = (erdata == m_ERData) ? m_Dual_ERData : m_ERData;

	for (Relationship* re : relationships)
	{
		Entity* en = re->getDualEntity();
		if (en)
		{
			dualerdata->deleteEntity(en, false);
		}
		erdata->deleteRelationship(re, false);
	}

	focusedVis->clearSelectRelationshipList();
	erdata->update2D();
	ERDataChanged();
	updateWidgets();


	/*if (focusedVis == primeVis)
	{
		if (focusedVis->getSelectedRelationships().size() < 1) {
			return;
		}

		bool res = m_ERData->deleteRelationships(ui->visWdgt->getSelectedRelationships());

		if (!res)
			LogConsole::showError("Something wrong with deleting entities");
		else
			focusedVis->clearSelectRelationshipList();

		focusedVis->currentERData->update2D();
		ERDataChanged();
		updateWidgets();
	}
	else {
		LogConsole::writeConsole("Cannot modify data in the dual vis");
	}*/
}

void MainWindow::on_actionRemoveElementsFromRelationship_triggered() {

	//remove the entities from selected relationship
	REList relist = focusedVis->getSelectedRelationships();
	ENList enlist = focusedVis->getSelectedEntities();

	if (relist.size() != 1)
	{
		LogConsole::showError("Select only one relationship.");
		return;
	}

	if (enlist.size() == 0)
	{
		LogConsole::showError("Select at least one entity.");
		return;
	}

	bool res = m_ERData->removeEntitiesFromRelationship(enlist, relist.front());

	relist.front()->setSelected(false);
	primeVis->removeSelectedEntities();
	primeVis->removeSelectedRelationships();
	dualVis->removeSelectedEntities();
	dualVis->removeSelectedRelationships();

	ERDataChanged();
	updateWidgets();
}

void MainWindow::on_actionSepareteEnclosingRelationship_triggered() {

	//divide the selected one relationship into two if it has a enclosing relationship
	if (focusedVis == primeVis)
	{
		if (focusedVis->getSelectedRelationships().size() != 1) {
			LogConsole::showError("please select one relationship");
			return;
		}

		if (focusedVis->getSelectedRelationship(0)->getCardinality() == 2) {
			LogConsole::showError("please do not remove elements from Binary relationship");
			return;
		}

		bool res = m_ERData->separateRelationship(focusedVis->getSelectedRelationship(0));

		if (res) {
			focusedVis->clearSelectRelationshipList();
			focusedVis->clearSwapList();
		}
		else {
			LogConsole::writeConsole("separate the enclosing relationship fails");
		}

		ERDataChanged();
		updateWidgets();
	}
	else {
		LogConsole::writeConsole("Cannot modify data in the dual vis");
	}
}

void MainWindow::on_actiondeleteEnclosingRelationships_triggered()
{
	if (focusedVis == primeVis)
	{
		//delete all enclosing relationship of the selected one relationship
		if (focusedVis->getSelectedRelationships().size() == 0) {
			LogConsole::showError("please select at least one relationship");
			return;
		}

		if (focusedVis->getSelectedRelationship(0)->getCardinality() < 3) {
			LogConsole::showError("please do not remove elements from Binary relationship");
			return;
		}

		bool res = false;
		for (int i = 0; i < focusedVis->getSelectedRelationships().size(); i++) {
			bool ires = m_ERData->removeEnclosingRelationships(focusedVis->getSelectedRelationship(i));
			if (ires)
				res = true;
		}

		if (res) {
			ERDataChanged();
			updateWidgets();
		}
	}
	else {
		LogConsole::writeConsole("Cannot modify data in the dual vis");
	}
}

void MainWindow::on_actionSwap_triggered()
{
	if (focusedVis == primeVis)
	{
		if (focusedVis->getSelectedRelationships().size() != 1) {
			LogConsole::showError("please select one relationship");
			return;
		}

		if (focusedVis->getSwapList().size() != 2) {
			LogConsole::showError("please select two entities to swap");
			return;
		}

		bool res = focusedVis->getSelectedRelationship(0)->swapEntities(focusedVis->getSwapList());
		if(res)
			focusedVis->clearSwapList();

		updateWidgets();
	}
	else {
		LogConsole::writeConsole("Cannot modify data in the dual vis");
	}
}

void MainWindow::on_actionRemove_Dangling_Entities_triggered()
{
	if (!hasFocusedVis()) {
		return;
	}
	ERData *data = focusedVis->currentERData;

	data->removeDagglingEntities();
	ERDataChanged();
	updateWidgets();
}

void MainWindow::on_actionRemove_Dangling_Relationships_triggered()
{
	if (!hasFocusedVis()) {
		return;
	}
	ERData *data = focusedVis->currentERData;

	data->removeDagglingRelationships();
	ERDataChanged();
	updateWidgets();
}

void MainWindow::on_actionAssign_Entity_Labels_With_Indices_triggered()
{
	if (!hasFocusedVis()) {
		return;
	}
	ERData *data = focusedVis->currentERData;

	data->assignEntityLabelsWithIndices();
	updateWidgets();
}

void MainWindow::on_actionAssign_Relationship_Labels_With_Indices_triggered()
{
	if (!hasFocusedVis()) {
		return;
	}
	ERData *data = focusedVis->currentERData;

	data->assignRelationshipLabelsWithIndices();
	updateWidgets();
}

void MainWindow::on_actionSet_Selection_Color_triggered()
{
	if (!hasFocusedVis())
		return;
	
	ENList entities = focusedVis->getSelectedEntities();
	REList relationships = focusedVis->getSelectedRelationships();

	if (entities.size() == 0 && relationships.size() == 0)
		return;

	QColorDialog cd(QColor(255, 255, 255, 255), this);
	cd.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	int dialogCode = cd.exec();

	if (dialogCode == QDialog::Accepted)
	{
		int R, G, B, A;
		QColor qcol = cd.currentColor();
		qcol.getRgb(&R, &G, &B, &A);
		Color4_F color((double)R/255.0, (double)G/255.0, (double)B/255.0, (double)A/255.0);

		for (Entity* en : entities)
			en->setColor(color);
		for (Relationship* re : relationships)
			re->setColor(color);

		this->updateWidgets();
	}
}

ColorGradient MainWindow::getCurrentColorGradient()
{
	ColorGradient cg;
	int currentIndex = control_dock->ui->comboBox_ColorScheme->currentIndex();
	int colorIndex;
	Color4_8BIT colors[11];

	switch (currentIndex)
	{
	case 0:
		cg.createRainbowHeatMapGradient();
		break;
	case 1:
		cg.createRainbowHeatMapGradient();
		break;
	case 2:
		cg.createRedWhiteBlueHeatMapGradient();
		break;
	case 3:
		cg.createColorRamp(6, TolColor_Bright);
		break;
	case 4:
		cg.createColorRamp(6, TolColor_Vibrant);
		break;
	case 5:
		cg.createColorRamp(9, TolColor_Muted);
		break;
	case 6:
		cg.createColorRamp(8, TolColor_Light);
		break;
	case 7:
		cg.createColorRamp(10, Predefined_1_PolyColor);
		break;
	case 8:
		cg.createColorRamp(11, ColorBrewer1);
		break;
	case 9: // default
		cg.createColorRamp(11, ColorBrewer2);
		break;
	case 10:
		for (int i = 0; i < COLORCOUNT; i++) {
			QColor c;
			int hue = (360.0 / (COLORCOUNT - 1)) * i;
			int s = 255;
			int l = 0.5 * 255;
			c.setHsl(hue, s, l);
			colors[i] = Vec4(c.red(), c.green(), c.blue(), 127);
		}
		cg.createColorRamp(11, colors);
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22: //#11 - #22
		colorIndex = currentIndex - 11;
		cg.createColorRamp(9, ColorBrewer_sequential_multihue[colorIndex]);
		break;
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28: //#23 - #28
		colorIndex = currentIndex - 23;
		cg.createColorRamp(9, ColorBrewer_sequential_singleHue[colorIndex]);
		break;
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37: //#29 - #37
		colorIndex = currentIndex - 29;
		cg.createColorRamp(11, ColorBrewer_diverging[currentIndex - 25]);
		break;
	default:
		cg.createColorRamp(11, ColorBrewer2);
		break;
	}

	return cg;
}


void MainWindow::assignRelationshipColorsBySetIntersections()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	focused_ERData->assignRelationshipColorsBySetIntersections();
	updateWidgets();
}

void MainWindow::assignElementsColorPredefined()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	Element::initPolyColors(Predefined_1_PolyColor);

	focused_ERData->assignRelationshipColors();
	focused_ERData->assignEntitiesColors(Vec4(0.0, 0.0, 1.0, 1.0));

	updateWidgets();
}

void MainWindow::assignElementsColorBrewer1()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	Element::initPolyColors(ColorBrewer1, COLORCOUNT);
	focused_ERData->assignRelationshipColors();
	updateWidgets();
}

void MainWindow::assignElementsColorBrewer2()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	Element::initPolyColors(ColorBrewer2, COLORCOUNT);
	focused_ERData->assignRelationshipColors();
	updateWidgets();
}

void MainWindow::assignElementColorEvenHueDistance()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	for (int i = 0; i < COLORCOUNT; i++) {
		QColor c;
		c.setHsl((360.0/ COLORCOUNT)*i, 255, 0.75*255);
		Element::polyColor[i] = Vec4(c.red(), c.green(), c.blue(), 127);
	}
	
	focused_ERData->assignRelationshipColors();
	updateWidgets();
}

void MainWindow::setZoom()
{
	if (!hasFocusedVis())
		return;

	double zoom = control_dock->ui->doubleSpinBox_Zoom->value();
	focusedVis->zoom = zoom;
	focusedVis->update();
}

void MainWindow::syncDualZooming()
{
	if (!hasFocusedVis())
		return;

	double zoom = focusedVis->zoom;
	
	if (focusedVis->widgetType == VISTYPE::PRIMEVIS)
	{
		dualVis->zoom = zoom;
		LogConsole::writeConsole("dual vis zoom is changed to %f", dualVis->zoom);
		dualVis->update();

		bipartVis->zoom = zoom;
		LogConsole::writeConsole("barpart vis zoom is changed to %f", bipartVis->zoom);
		bipartVis->update();
	}
	else if (focusedVis->widgetType == VISTYPE::DUALVIS)
	{
		primeVis->zoom = zoom;
		LogConsole::writeConsole("dual vis zoom is changed to %f", primeVis->zoom);
		primeVis->update();

		bipartVis->zoom = zoom;
		LogConsole::writeConsole("barpart vis zoom is changed to %f", bipartVis->zoom);
		bipartVis->update();
	}
	else if (focusedVis->widgetType == VISTYPE::BIPARTVIS)
	{
		primeVis->zoom = zoom;
		LogConsole::writeConsole("dual vis zoom is changed to %f", primeVis->zoom);
		primeVis->update();

		dualVis->zoom = zoom;
		LogConsole::writeConsole("dual vis zoom is changed to %f", dualVis->zoom);
		dualVis->update();
	}
}

void MainWindow::syncDualElementColor()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	focused_ERData->syncColorsWithDualElements();

	updateWidgets();
}

void MainWindow::updatePolyColors()
{
	int currentIndex = control_dock->ui->comboBox_ColorScheme->currentIndex();
	int colorIndex;

	switch (currentIndex)
	{
	case 3:
		Element::initPolyColors(TolColor_Bright);
		break;
	case 4:
		Element::initPolyColors(TolColor_Vibrant);
		break;
	case 5:
		Element::initPolyColors(TolColor_Muted);
		break;
	case 6:
		Element::initPolyColors(TolColor_Light);
		break;
	case 7:
		Element::initPolyColors(Predefined_1_PolyColor);
		break;
	case 8:
		Element::initPolyColors(ColorBrewer1);
		break;
	case 9: // default
		Element::initPolyColors(ColorBrewer2);
		break;
	case 10:
		for (int i = 0; i < COLORCOUNT; i++) {
			QColor c;
			c.setHsl((360.0 / COLORCOUNT) * i, 255, 0.75 * 255);
			Element::polyColor[i] = Vec4(c.red(), c.green(), c.blue(), 127);
		}
		break;
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 21:
	case 22: //#11 - #22
		colorIndex = currentIndex - 11;
		Element::initPolyColors(ColorBrewer_sequential_multihue[colorIndex]);
		break;
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28: //#23 - #28
		colorIndex = currentIndex - 23;
		Element::initPolyColors(ColorBrewer_sequential_singleHue[colorIndex]);
		break;
	case 29:
	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37: //#29 - #37
		colorIndex = currentIndex - 29;
		Element::initPolyColors(ColorBrewer_diverging[colorIndex]);
		break;
	default:
		Element::initPolyColors(ColorBrewer2);
		break;
	}
}

void MainWindow::applyReColorScheme()
{
	if (!hasFocusedVis())
		return;
	ERData* focused_ERData = focusedVis->currentERData;

	int scheme = control_dock->ui->comboBox_RelationshipColor->currentIndex();
	ELEMENT_COLORSCHEME colorScheme = static_cast<ELEMENT_COLORSCHEME>(scheme);
	if (colorScheme == -1)
	{
		vector<std::string> substrings{"(USA)", "(Germany)", "(China)"};
		/*vector<std::string> substrings{"(USA)", "(Germany)", "(China)", "(Hong Kong)", "(Netherlands)", "(UK)", "(Canada)",
			"(Australia)", "(Austria)", "(Switzerland)", "(France)", "(Brazil)", "(Saudi Arabia)", "(Denmark)", "(Finland)",
			"(Sweden)", "(Israel)", "(Taiwan)", "(South Korea)", "(Norway)"};*/
		focused_ERData->assignRelationshipColorsBySubstring(substrings);
	}
	else
	{
		bool continuous = LogConsole::showConfirm("Use continuous color palette?");

		if (!continuous)
		{
			updatePolyColors();
			focused_ERData->assignRelationshipColors(colorScheme);
		}
		else
			focused_ERData->assignRelationshipColors(colorScheme, getCurrentColorGradient());
	}

	updateWidgets();
}

void MainWindow::applyEnColorScheme()
{
	if (!hasFocusedVis())
		return;
	ERData* focused_ERData = focusedVis->currentERData;

	int scheme = control_dock->ui->comboBox_EntityColor->currentIndex();
	ELEMENT_COLORSCHEME colorScheme = static_cast<ELEMENT_COLORSCHEME>(scheme);
	if (colorScheme == -1)
	{
		vector<std::string> substrings{"(USA)", "(Germany)", "(China)"};
		/*vector<std::string> substrings{"(USA)", "(Germany)", "(China)", "(Hong Kong)", "(Netherlands)", "(UK)", "(Canada)",
			"(Australia)", "(Austria)", "(Switzerland)", "(France)", "(Brazil)", "(Saudi Arabia)", "(Denmark)", "(Finland)",
			"(Sweden)", "(Israel)", "(Taiwan)", "(South Korea)", "(Norway)"};*/
		focused_ERData->assignEntitiesColorsBySubstring(substrings);
	}
	else
	{
		bool continuous = LogConsole::showConfirm("Use continuous color palette?");

		if (!continuous)
		{
			updatePolyColors();
			focused_ERData->assignEntitiesColors(colorScheme);
		}
		else
			focused_ERData->assignEntitiesColors(colorScheme, getCurrentColorGradient());
	}

	updateWidgets();
}

void MainWindow::assignEnColorByDual()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	updatePolyColors();

	focused_ERData->assignEntitiesColorsDual();
	updateWidgets();
}

void MainWindow::on_actionFindConvexHull_triggered()
{
	if (focusedVis)
	{
		if (focusedVis->getSelectedEntities().size() < 3) {
			LogConsole::showError("Not enough entities are selected to detect convex hull, please select at lease three vertices");
			return;
		}
		vector<Vec3> convexHull;
		bool res = getConvexHull(focusedVis->getSelectedEntities(), convexHull);

		if (!res)
			LogConsole::showError("Something wrong when constructing convex hull");
		else {
			focusedVis->setConvexHull(convexHull);
		}

		updateWidgets();
	}
}

void MainWindow::on_actionAnimate_Optimization_triggered(bool checked)
{
	flag_animateOptimization = checked;
}

void MainWindow::on_actionAuto_Adjust_Viewpoint_triggered(bool checked)
{
	flag_autoAdjustViewpoint = checked;
}

void MainWindow::on_actionAssign_Relationships_Color_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	changePolygonColorDialog dia(focused_ERData);
	dia.setWindowModality(Qt::WindowModal);
	dia.setUIValues(Element::polyColor);

	dia.exec();

}

void MainWindow::on_actionAssign_Entities_Color_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;


    ColorSettingDialog *add = new ColorSettingDialog();
    add->exec();

    if (add->flag_accepted)
    {
        Vec4 color = Vec4(add->R_channel, add->G_channel, add->B_channel, add->A_channel);

        for (int i = 0; i < focused_ERData->getEntities().size(); i++) {
            Entity *e = focused_ERData->getEntity(i);
            e->setColor(color);
        }
    }
	delete(add);
}

void MainWindow::on_actionChange_Entity_Boundary_Color_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	ColorSettingDialog *add = new ColorSettingDialog();
	add->initUI(focusedVis->EntityBoundaryColor);
	add->setWindowModality(Qt::WindowModal);
	add->exec();

	if (add->flag_accepted)
	{
		Vec4 color = Vec4(add->R_channel, add->G_channel, add->B_channel, add->A_channel);
		focusedVis->EntityBoundaryColor = color;
		focusedVis->update();
	}
	delete(add);

}

void MainWindow::on_actionClear_all_data_triggered()
{
    bool res = LogConsole::showConfirm("Are you sure to delete the ER data in the prime vis and dual vis?");
    if (res) {
		set_ERData(new ERData(), primeVis);
		set_ERData(new ERData(), dualVis);
		updateWidgets();
    }
}

void MainWindow::showFixedVertInfo(ERData *erdata) {

	if (erdata == NULL)
		return;

	if (erdata->fixedVertComp.empty())
	{
		LogConsole::writeConsole("no fixed verts");
	}
	else {

		struct FixVertInfo {
			int vindex;
			bool xfixed = false;
			bool yfixed = false;
		};
		vector<FixVertInfo> vert_fixed_infos;
		for (int i = 0; i < erdata->fixedVertComp.size(); i++) {
			int fixedVertComp = erdata->fixedVertComp[i];
			int vIndex = fixedVertComp / 2;

			/*find whether this vertex already saved in vert_fixed_infos*/
			bool existed = false;
			for (int k = 0; k < vert_fixed_infos.size(); k++) {
				if (vert_fixed_infos[k].vindex == vIndex) {
					FixVertInfo &fvinfo = vert_fixed_infos[k];
					fvinfo.vindex = vIndex;
					int mod = fixedVertComp % 2;
					if (mod == 0)
						fvinfo.xfixed = true;
					else
						fvinfo.yfixed = true;
					existed = true;
					break;
				}
			}

			if (!existed) {
				FixVertInfo fvinfo;
				fvinfo.vindex = vIndex;
				int mod = fixedVertComp % 2;
				if (mod == 0)
					fvinfo.xfixed = true;
				else
					fvinfo.yfixed = true;
				vert_fixed_infos.push_back(fvinfo);
			}
		}

		QString bothFixedVIndices = "verts";
		int bothfixedVertCount = 0;
		for (int i = 0; i < vert_fixed_infos.size(); i++) {
			FixVertInfo &fvinfo = vert_fixed_infos[i];
			if (fvinfo.xfixed && fvinfo.yfixed) {
				bothFixedVIndices += " " + QString::number(fvinfo.vindex);
				bothfixedVertCount++;
			}
		}
		bothFixedVIndices += " are fixed";
		LogConsole::writeConsole(bothFixedVIndices);

		if (bothfixedVertCount != vert_fixed_infos.size()) {
			for (int i = 0; i < vert_fixed_infos.size(); i++) {
				FixVertInfo &fvinfo = vert_fixed_infos[i];
				if (fvinfo.xfixed && !fvinfo.yfixed) {
					LogConsole::writeConsole("vert %d's x is fixed", fvinfo.vindex);
				}
				else if (!fvinfo.xfixed && fvinfo.yfixed) {
					LogConsole::writeConsole("vert %d's y is fixed", fvinfo.vindex);
				}
			}
		}
	}
}

void MainWindow::setFixedVert()
{
	if (!hasFocusedVis())
		return;

	ERData *focused_ERData = focusedVis->currentERData;
	for (int i = 0; i < focused_ERData->getEntities().size(); i++) {
		focused_ERData->getEntity(i)->flag_Optimizable = true;
	}

	/*fix first vertex's x and y componenet*/
	focused_ERData->fixedVertComp.clear();
	vector<int> vi;
	vector<EN*>selectedVerts = focusedVis->getSelectedEntities();
	bool ok = true;
	if (selectedVerts.size() == 2) {
		ok = LogConsole::showConfirm("Fix selected verts?");
		if (ok)
		{
			for (int i = 0; i < selectedVerts.size(); i++) {
				int index = selectedVerts[i]->getIndex();
				vi.push_back(index);
			}
		}
	}

	if (selectedVerts.size() != 2 || !ok)
	{
		vi = focused_ERData->getTwosVertsToBeFixed();
	}

	fix_verts(focused_ERData, vi);
}

void MainWindow::fixSelectedVerts()
{
	if (!hasFocusedVis())
		return;

	ERData* focused_ERData = focusedVis->currentERData;
	vector<int> vi;
	vector<EN*>selectedVerts = focusedVis->getSelectedEntities();
	for (int i = 0; i < selectedVerts.size(); i++) 
	{
		int index = selectedVerts[i]->getIndex();
		vi.push_back(index);
	}

	fix_verts(focused_ERData, vi);
}

void MainWindow::free_elements(ERData* erdata, vector<int> freeVertIndices, vector<int> freePolyIndices)
{
	erdata->fixedVertComp.clear();
	for (Entity* en : erdata->getEntities())
	{
		int idx = en->getIndex();
		if (std::find(freeVertIndices.begin(), freeVertIndices.end(), idx) == freeVertIndices.end() /*|| en->flag_Locked*/)
		{
			erdata->fixedVertComp.push_back(2 * idx);
			erdata->fixedVertComp.push_back(2 * idx + 1);
			en->flag_Optimizable = false;
		}
		else
		{
			en->flag_Optimizable = true;
		}	
	}

	erdata->fixedMonoComp.clear();
	for (Relationship* re : erdata->getMonogons())
	{
		int idx = re->getIndex();
		if (std::find(freePolyIndices.begin(), freePolyIndices.end(), idx) == freePolyIndices.end() /*|| re->flag_Locked*/)
		{
			int compIdx = erdata->getFreeVertCompNum() + erdata->getMonogonIndex(re);
			erdata->fixedMonoComp.push_back(compIdx);
			re->flag_Optimizable = false;
		}
		else
		{
			re->flag_Optimizable = true;
		}
	}
}

void MainWindow::free_elements(ERData* erdata)
{
	erdata->fixedVertComp.clear();
	for (Entity* en : erdata->getEntities())
		en->flag_Optimizable = true;
	erdata->fixedMonoComp.clear();
	for (Relationship* re : erdata->getRelationships())
		re->flag_Optimizable = true;
}

void MainWindow::fix_verts(ERData *merdata, vector<int> fixedVertIndices)
{
	for (int i = 0; i < merdata->getEntities().size(); i++) {
		merdata->getEntity(i)->flag_Optimizable = true;
	}

	merdata->fixedVertComp.clear();

	for (int i = 0; i < fixedVertIndices.size(); i++)
	{
		int index = fixedVertIndices[i];
		merdata->fixedVertComp.push_back(2 * index);
		merdata->fixedVertComp.push_back(2 * index + 1);
	}

	showFixedVertInfo(merdata);

	/*set fixed verts to not optimizable*/
	for (int i = 0; i < merdata->fixedVertComp.size(); i++) {
		int fixedVertComp = merdata->fixedVertComp[i];
		int vIndex = fixedVertComp / 2;
		merdata->getEntity(vIndex)->flag_Optimizable = false;
	}

	updateWidgets();
}

void MainWindow::clearFixedVert()
{
	if (!hasFocusedVis())
		return;

	ERData *focused_ERData = focusedVis->currentERData;

	/*fix first vertex's x and y componenet*/
	focused_ERData->fixedVertComp.clear();

	for (int i = 0; i < focused_ERData->getEntities().size(); i++) {
		focused_ERData->getEntity(i)->flag_Optimizable = true;
	}

	LogConsole::writeConsole("no fixed verts");
	updateWidgets();
}

void MainWindow::stopOptimization()
{
	flag_stopOptimization = true;
	control_dock->toggleOptimizationButtons(true);
	LogConsole::writeConsole("Optimization is stopped");
}


void MainWindow::applyHLBFGS()
{
	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	if (!hasFocusedVis())
		return;
	ERData* focused_ERData = focusedVis->currentERData;
	
	//vector<int> monogon_indices = focused_ERData->convertMonogonsToDigons();

	//Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSTime, "PolygonHLBFGS");

	//focused_ERData->convertDigonsToMonogons(monogon_indices);

	Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "HLBFGS");
}

void MainWindow::applyHLBFGSDual()
{
	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	if (!primeVis->currentERData) {
		LogConsole::showError("No prime ERdata");
		return;
	}

	if (!dualVis->currentERData) {
		LogConsole::showError("No dual ERdata");
		return;
	}
	
	/*ERData* primeData = primeVis->currentERData;
	ERData* dualData = dualVis->currentERData;*/

	//vector<int> prime_monogons = primeData->convertMonogonsToDigons();
	//vector<int> dual_monogons = dualData->convertMonogonsToDigons();

	//Optimize_layout(primeData, costtime, finalEnergy, iterNum, HLBFGSTime, "DualPolygonHLBFGS", dualData);

	//primeData->convertDigonsToMonogons(prime_monogons);
	//dualData->convertDigonsToMonogons(dual_monogons);

	//Optimize_layout(primeVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSTime, "DualHLBFGS", dualVis->currentERData);
	Optimize_layout(dualVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "DualHLBFGS", primeVis->currentERData);
}

void MainWindow::applyHLBFGSArithopt()
{
	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "PolygonHLBFGS");

}

void MainWindow::on_action_optimize_layout_triggered()
{
	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	if (!hasFocusedVis())
		return;

	if (dualVisVisible())
	{
		if (!primeVis->currentERData) {
			LogConsole::showError("No prime ERdata");
			return;
		}

		if (!dualVis->currentERData) {
			LogConsole::showError("No dual ERdata");
			return;
		}

		//Optimize_layout(primeVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSTime, "DualPolygonHLBFGS", dualVis->currentERData);
		//Optimize_layout(primeVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSTime, "MonogonHLBFGSDual");
		//Optimize_layout(dualVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSTime, "MonogonHLBFGSDual");

		Optimize_layout(primeVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "DualHLBFGS", dualVis->currentERData);
	}
	else 
	{
		ERData* focused_ERData = focusedVis->currentERData;

		//Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSTime, "PolygonHLBFGS");
		//Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSTime, "MonogonHLBFGS");

		Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "HLBFGS");
	}
}

void MainWindow::on_actionstopOptimization_triggered()
{
	stopOptimization();
}

void MainWindow::on_actionDraw_Curved_Scaffolds_triggered(bool checked)
{
	flag_drawPolyCurvedScaffolds = checked;

	if (primeVis->isVisible())
	{
		primeVis->currentERData->update2D();
		primeVis->update();
	}
	if (dualVis->isVisible())
	{
		dualVis->currentERData->update2D();
		dualVis->update();
	}
}

void MainWindow::on_actionDraw_Nodes_as_Spheres_triggered(bool checked)
{
	flag_drawNodeSphere = checked;

	if (primeVis->isVisible())
		primeVis->update();
	if (dualVis->isVisible())
		dualVis->update();
	if (bipartVis->isVisible())
		bipartVis->update();
}

void MainWindow::on_actionDraw_Edges_as_Cylinders_triggered(bool checked)
{
	flag_drawEdgeCylinder = checked;

	if (primeVis->isVisible())
		primeVis->update();
	if (dualVis->isVisible())
		dualVis->update();
	if (bipartVis->isVisible())
		bipartVis->update();
}

void MainWindow::on_actionDraw_Axes_triggered(bool checked)
{
	flag_drawPlaneAxes = checked;

	if (primeVis->isVisible())
		primeVis->update();
	if (dualVis->isVisible())
		dualVis->update();
	if (bipartVis->isVisible())
		bipartVis->update();
}

void MainWindow::on_actionDraw_Grid_triggered(bool checked)
{
	flag_drawPlaneGrid = checked;

	if (primeVis->isVisible())
		primeVis->update();
	if (dualVis->isVisible())
		dualVis->update();
	if (bipartVis->isVisible())
		bipartVis->update();
}

void MainWindow::on_actionDraw_Cursor_triggered(bool checked)
{
	flag_drawCursor = checked;

	if (primeVis->isVisible())
		primeVis->update();
	if (dualVis->isVisible())
		dualVis->update();
	if (bipartVis->isVisible())
		bipartVis->update();
}

void MainWindow::applyHLBFGSArithoptDual()
{
	if (!primeVis->currentERData) {
		LogConsole::showError("No prime ERdata");
		return;
	}

	if (!dualVis->currentERData) {
		LogConsole::showError("No dual ERdata");
		return;
	}

	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	Optimize_layout(primeVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "DualPolygonHLBFGS", dualVis->currentERData);
	//Optimize_layout(dualVis->currentERData, costtime, finalEnergy, iterNum, HLBFGSTime, "DualPolygonHLBFGS", primeVis->currentERData);

	updateWidgets();
}

void MainWindow::applyHLBFGSMonogon()
{
	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "MonogonHLBFGS");

	updateWidgets();
}

void MainWindow::applyHLBFGSMonogonDual()
{
	double costtime;
	double finalEnergy;
	int iterNum;
	int HLBFGSEvaluations;

	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "MonogonHLBFGSDual");

	updateWidgets();
}

void MainWindow::optimizeOrdering()
{

	double costtime;
	double finalEnergy;
	int iterNum;
	int OptTime;

	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, OptTime, "Reordering");

	updateWidgets();
}

void MainWindow::optimizeDualOrdering()
{

	double costtime;
	double finalEnergy;
	int iterNum;
	int OptTime;

	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;
	Optimize_layout(focused_ERData, costtime, finalEnergy, iterNum, OptTime, "DualReordering", dualVis->currentERData);

	updateWidgets();
}

void MainWindow::on_actionClear_Gradient_Directions_triggered()
{
	gradientDirections.clear();
	LogConsole::writeConsole("gradient directions debug is cleared");

	updateWidgets();
}

void MainWindow::scaleERLayout(ERData* erdata, double scale)
{
	erdata->calculateBoundingCirle(false, false);
	erdata->scaleEntities(Vec3(scale, scale, 0), erdata->BC_center);
	erdata->update2D();
}

void MainWindow::doubleSpinBox_VertexSize_valueChanged(double arg1)
{
    Element::m_VertexSize = arg1;
	m_ERData->setEntitiesSize(arg1);
	m_Dual_ERData->setEntitiesSize(arg1);
    updateWidgets();
}

void MainWindow::doubleSpinBox_MaxScaffoldCurve(double arg1)
{
	m_scaffoldMaxCurve = arg1;

	m_ERData->create2DScaffolds();

	if(dualVisVisible())
		m_Dual_ERData->create2DScaffolds();

	updateWidgets();
}

void MainWindow::doubleSpinBox_MonogonSize_valueChanged(double arg1)
{
	Element::m_MonogonSize = arg1;

	m_ERData->setMonogonsSize(arg1);
	m_Dual_ERData->setMonogonsSize(arg1);

	m_ERData->create2DScaffolds(1);
	m_Dual_ERData->create2DScaffolds(1);

	primeVis->buildMonogonPipe(primeVis->boundaryWidth);
	dualVis->buildMonogonPipe(primeVis->boundaryWidth);

	updateWidgets();
}

void MainWindow::doubleSpinBox_LensAngle_valueChanged(double arg1)
{
	Element::m_DigonLensAngle = arg1;
	m_ERData->create2DScaffolds(2);
	m_Dual_ERData->create2DScaffolds(2);
	updateWidgets();
}

void MainWindow::doubleSpinBox_MonogonAspectRatio_valueChanged(double arg1)
{
	Element::m_MonogonLength = arg1;

	m_ERData->setMonogonLength(arg1);
	m_Dual_ERData->setMonogonLength(arg1);

	m_ERData->create2DScaffolds(1);
	m_Dual_ERData->create2DScaffolds(1);

	primeVis->buildMonogonPipe(primeVis->boundaryWidth);
	dualVis->buildMonogonPipe(primeVis->boundaryWidth);

	updateWidgets();
}

void MainWindow::optimizeIterationMaxChanged(int arg1)
{
    ITERMAX = arg1;
}

void MainWindow::calcBoundingCircle()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	focused_ERData->calculateBoundingCirle(false, false);
    updateWidgets();
}

void MainWindow::on_actionCreate_ER_Vis_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	LogConsole::writeConsole("start to find cliques");

	int found = focused_ERData->findCliques(focused_ERData->getEntities());
	LogConsole::writeGreenConsole("added %d relationships", found);

	int re_b = focused_ERData->removeBinaryEnclosedByRelationships();
	LogConsole::writeGreenConsole("Removed %d binary relationships", re_b);

	focused_ERData->update2D();
	updateWidgets();
}

void MainWindow::on_actionCreate_Relationships_From_Cliques_triggered()
{
	/*create relationships from cliques*/
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	bool res = false;
	if (focusedVis->getSelectedEntities().size() > 0) {
		res = LogConsole::showConfirm("Do you want only find cliques from the selected elements?");
	}

	if (res) {
		LogConsole::writeConsole("start to find cliques");
		focused_ERData->findCliques(focusedVis->getSelectedEntities());
	}
	else {
		LogConsole::writeConsole("start to find cliques");
		focused_ERData->findCliques(focused_ERData->getEntities());
	}

	focused_ERData->update2D();
	updateWidgets();
}

void MainWindow::on_actionCreate_Cliques_from_Relationships_triggered()
{
	/*create relationships from cliques*/
	if (!hasFocusedVis())
		return;
	ERData *focused_ERData = focusedVis->currentERData;

	focused_ERData->convertToGraphData();
	focused_ERData->update2D();

	updateWidgets();
}

void MainWindow::calcAverageLength()
{
	double averageL = m_ERData->getAverageEdgeLenth();
    expectedLength = averageL;
	LogConsole::writeConsole("Average edge length: %f", expectedLength);
}

void MainWindow::findEdgelyAdjacentRelationshipIncidentOneEntity()
{
    if (ui->visWdgt->getSelectedRelationships().size() != 1) {
        LogConsole::showError("please select one relationship");
        return;
    }

    if (ui->visWdgt->getSwapList().size() == 0) {
        LogConsole::showError("please use right click to select one entity");
        return;
    }

    Entity *en1 = ui->visWdgt->getSwapList()[0];
    vector<Relationship*> res;
    res = getEdgelyAdjacentRelationshipIncidentToEntity(ui->visWdgt->getSelectedRelationship(0), en1);

    for (int i = 0; i < res.size(); i++) {
        LogConsole::writeConsole("edgely index of adjacent polygon of entity %d is %d", en1->getIndex(), res[i]->getIndex());
    }

}

void MainWindow::on_actionSave_All_Color_Schemes_Results_triggered()
{

	QString filters("Polygon Color Schemes (*.PolyColors);; ALL(*.*)");
	QString defaultFilter("Polygon Color Schemes (*.PolyColors)");

	QStringList fileNames = QFileDialog::getOpenFileNames(0, "Select Color Schemes", getOpenDir(), filters, &defaultFilter);

	if (fileNames.size() == 0)
		return;

	QString dir = QFileDialog::getExistingDirectory(0, "Select one folder save results", getOpenDir());

	string str = dir.toStdString();
	if (dir.isEmpty())
		return;

	for (int i = 0; i < fileNames.size(); i++) {
		QString fileName = fileNames[i];
		OpenColorScheme(fileName.toStdString().c_str());

		QFileInfo fi(fileName);
		QString folder_name = fi.path();
		QString base = fi.baseName();
		QString outputName = dir + "/" + QString::fromStdString(m_FILENAME) + "_" + base + ".png";
		primeVis->savePNGImage_fitAspectRatio(outputName.toStdString().c_str());
	}

	LogConsole::writeConsole("THE RENDERING IS FINISHED");
}



void MainWindow::on_actionSet_Nodes_Boundary_Width_triggered()
{
	bool ok;
	double w = QInputDialog::getDouble(this, tr("Set Node Boundary Width"),
		tr("Line Width:"), focusedVis->nodeBoundaryWidth, 0.0, 10, 4, &ok);

	if (ok)
	{
		for (OGLWidget* vis : { primeVis, dualVis, bipartVis })
			vis->nodeBoundaryWidth = w;
		updateWidgets();
	}
}

void MainWindow::on_actionSet_Polygon_Boundary_Width_triggered()
{
	bool ok;
	double w = QInputDialog::getDouble(this, tr("Set Polygons Boundary Width"),
		tr("Line Width:"), focusedVis->boundaryWidth, 0.0, 10, 4, &ok);

	if (ok) 
	{
		for (OGLWidget* vis : { primeVis, dualVis, bipartVis })
		{
			vis->boundaryWidth = w;
			vis->buildMonogonPipe(focusedVis->boundaryWidth);
		}
		updateWidgets();
	}
}

void MainWindow::removeMonogonsInPrimeVis()
{
	OGLWidget *vis = primeVis;
	OGLWidget *dualv = dualVis;

	ERData *da = vis->currentERData;
	ERData *dualda = dualv->currentERData;

	da->removeMonogons();
	dualda->removeEntitiesWithoutDualElements();

	updateWidgets();

	LogConsole::writeColorConsole(0.0, 0.0, 1.0, "Monogons in prime are removed");
}

void MainWindow::removeMonogonsInDualVis()
{
	OGLWidget *vis = dualVis;
	OGLWidget *dualv = primeVis;

	ERData *da = vis->currentERData;
	ERData *dualda = dualv->currentERData;

	da->removeMonogons();
	dualda->removeEntitiesWithoutDualElements();

	updateWidgets();

	LogConsole::writeColorConsole(0.0, 0.0, 1.0, "Monogons in dual are removed");

}

void MainWindow::set_PrimeVisWidget_focused(bool checked)
{
	if (checked) {
		focusedVis = primeVis;

		ui->toolBar->setEnabled(true);

		ui->actionSelectEntity->setChecked(true);
		setVisSelectEntity(focusedVis, true);
		focusedVis->removeSelectedEntities();
		focusedVis->removeSelectedRelationships();

		OGLWidget *nonfocusedVis = dualVis;
		setVisSelectEntity(nonfocusedVis, false);
		nonfocusedVis->removeSelectedEntities();
		nonfocusedVis->removeSelectedRelationships();

		updatefocusVisLabel(VISTYPE::PRIMEVIS);
		updateVisVariablesUIs(focusedVis);
		calcDegreeAndCardinality();
	}
}

void MainWindow::set_DualVisWidget_focused(bool checked)
{
	if (checked && !dualVisVisible())
	{
		LogConsole::showError("please show the dual view first!");
		control_dock->ui->radioButton_PrimeVisWidget->setChecked(true);
		return;
	}

	if (checked) {
		focusedVis = dualVis;
		ui->toolBar->setEnabled(true);

		/*clear the current selection first*/
		ui->actionSelectEntity->setChecked(true);
		setVisSelectEntity(focusedVis, true);

		OGLWidget *nonfocusedVis = primeVis;
		setVisSelectEntity(primeVis, false);
		nonfocusedVis->removeSelectedEntities();
		nonfocusedVis->removeSelectedRelationships();

		updatefocusVisLabel(VISTYPE::DUALVIS);
		updateVisVariablesUIs(focusedVis);
		calcDegreeAndCardinality();
	}
}

void MainWindow::set_BipartVisWidget_focused(bool checked)
{
	if (checked && !bipartVisVisible())
	{
		LogConsole::showError("please show the bipartite view first!");
		return;
	}

	if (checked)
	{
		focusedVis = bipartVis;
		ui->toolBar->setEnabled(true);

		/*clear the current selection first*/
		ui->actionSelectEntity->setChecked(true);
		setVisSelectEntity(bipartVis, true);

		setVisSelectEntity(primeVis, false);
		primeVis->removeSelectedEntities();
		primeVis->removeSelectedRelationships();

		setVisSelectEntity(dualVis, false);
		dualVis->removeSelectedEntities();
		dualVis->removeSelectedRelationships();

		updatefocusVisLabel(VISTYPE::DUALVIS);
		updateVisVariablesUIs(focusedVis);
		calcDegreeAndCardinality();
	}
}

void MainWindow::focusedVisChanged(VISTYPE mtype) 
{
	if (mtype == VISTYPE::PRIMEVIS && focusedVis != primeVis)
		control_dock->ui->radioButton_PrimeVisWidget->setChecked(true);
	else if (mtype == VISTYPE::DUALVIS && focusedVis != dualVis)
		control_dock->ui->radioButton_DualVisWidget->setChecked(true);
	else if (mtype == VISTYPE::BIPARTVIS && focusedVis != bipartVis)
		control_dock->ui->radioButton_BipartVisWidget->setChecked(true);
}	

void MainWindow::on_actionOutput_Color_Scheme_triggered()
{
	QString filter = "Polygon Colors Scheme (*.PolyColors)";

	QString pathOftext = QFileDialog::getSaveFileName(this, "Save Color Scheme to a txt...", getOpenDir(), filter);
	setOpenDir(pathOftext);

	string str = pathOftext.toStdString();

	if (!str.size())
		return;

	/*write to file*/
	FILE *this_file = fopen(str.c_str(), "w");

	if (this_file)
	{
		for (int i = 0; i < 11; i++) {
			fprintf(this_file, "%f %f %f %f;\n", Element::polyColor[i].R, Element::polyColor[i].G, Element::polyColor[i].B, Element::polyColor[i].Alpha);
		}
		fclose(this_file);
	}

	LogConsole::writeConsole("Energies are saved");
}

void MainWindow::on_actionLoad_Color_Scheme_triggered()
{
	QString filters("Polygon Colors Scheme (*.PolyColors)");
	QString defaultFilter("Polygon Colors Scheme (*.PolyColors)");

	QString fileName = QFileDialog::getOpenFileName(0, "Open Color Scheme file", getOpenDir(), filters, &defaultFilter);
	setOpenDir(fileName);

	if (fileName.isEmpty())
		return;

	string str = fileName.toStdString();
	const char* path = str.c_str();

	OpenColorScheme(path);

	return;

}

void MainWindow::assignColor()
{

	vector<Relationship*> rlist = primeVis->getSelectedRelationships();
	if (rlist.empty())
		return;

	ColorSettingDialog add;
	add.exec();

	if (add.flag_accepted)
	{
		Vec4 color = Vec4(add.R_channel, add.G_channel, add.B_channel, add.A_channel);
		for (auto r : rlist) {
			r->setColor(color);
		}
	}
	else {
		return;
	}
}

void MainWindow::on_actionLights_Settings_triggered()
{
	if (!hasFocusedVis())
		return;

	LightSettingDialog dia(focusedVis);
	dia.setWindowModality(Qt::WindowModal);
	dia.exec();
}

void MainWindow::on_actionUse_Low_Sepecular_Lighting_triggered()
{
	primeVis->light0.specular[0] = 0;
	primeVis->light0.specular[1] = 0;
	primeVis->light0.specular[2] = 0;
	primeVis->light0.specular[3] = 0;
	primeVis->SphereShininess = 30;
	primeVis->updateLights();

	if (dualVisVisible()) {

		dualVis->light0.specular[0] = 0;
		dualVis->light0.specular[1] = 0;
		dualVis->light0.specular[2] = 0;
		dualVis->light0.specular[3] = 0;
		dualVis->SphereShininess = 30;
		dualVis->updateLights();
	}

	updateWidgets();
}

void MainWindow::on_actionUse_High_Specular_Lighting_triggered()
{
	primeVis->initLights();
	primeVis->SphereShininess = 10;
	if (dualVisVisible()) {
		dualVis->initLights();
		dualVis->SphereShininess = 10;
	}
	updateWidgets();
}




// New UI

// file Dropdown Menu

void MainWindow::on_actionSave_CSV_Data_triggered()
{
	QString filter = "CSV files (*.csv)";
	QString fileName = QFileDialog::getSaveFileName(0, "Save CSV Data", getOpenDir(), filter);
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			ERData* erdata = focusedVis->currentERData;
			for (Relationship* re : erdata->getRelationships())
			{
				stream << re->getIndex() << re->getLabel().c_str();
				for (Entity* en : re->getIncidentEntities())
					stream << "," << en->getIndex() << en->getLabel().c_str();
				stream << endl;
			}

			file.flush();
			file.close();
		}
	}
}

void MainWindow::on_actionOpen_CSV_Data_triggered()
{
	QString filter = "CSV files (*.csv)";
	QString fileName = QFileDialog::getOpenFileName(0, "Open CSV Data", getOpenDir(), filter);
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly))
		{
			ERData* erdata = new ERData();

			QTextStream stream(&file);
			while (!stream.atEnd())
			{
				QStringList list = stream.readLine().split(",");
				ENList enlist;
				for (int i = 1; i < list.length(); i++)
				{
					Entity* en = erdata->addEntityUnique(list[i].toStdString());
					enlist.push_back(en);
				}
				Relationship* newRe = new Relationship(enlist);
				newRe->setLabel(list[0].toStdString());
				erdata->addRelationship(newRe);
				newRe->update2D();
			}

			set_ERData(erdata, focusedVis);
			erdata->update2D();

			string visName = "Prime Vis";
			if (focusedVis->widgetType == DUALVIS)
				visName = "Dual Vis";

			LogConsole::writeConsole(fileName);
			LogConsole::writeBlueConsole("[%s] Number of entities is: %d", visName.c_str(), erdata->getEntityNum());
			LogConsole::writeBlueConsole("[%s] Number of relationships is: %d", visName.c_str(), erdata->getRelationshipNum());
			calcDegreeAndCardinality();

			focusedVis->initializeSelection();
			focusedVis->reset_sceneToCenterVisualization();
		}
	}
}

void MainWindow::on_actionSave_Decomposition_triggered()
{
	ERClusterDecomp* decomp = focusedVis->getERClusterDecomp();
	if (!decomp)
	{
		LogConsole::writeError("Data does not have decomposition.");
		return;
	}
	vector<Cluster*> clusters = decomp->getClusters();
	vector<Bridge*> bridges = decomp->getBridges();

	QString filter = "Text files (*.txt)";
	QString fileName = QFileDialog::getSaveFileName(0, "Save ER Decomposition", getOpenDir(), filter);
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			stream << clusters.size() << "," << bridges.size() << endl;

			auto printSubgraphElements = [&](ERSubgraph* subgraph)
			{
				stream << subgraph->getNumElements(ENTITY) << "," << subgraph->getNumElements(RELATIONSHIP);
				for (int idx : subgraph->getElementSet(ENTITY))
					stream << "," << idx;
				for (int idx : subgraph->getElementSet(RELATIONSHIP))
					stream << "," << idx;
				stream << endl;
			};
			
			for (Cluster* c : clusters)
				printSubgraphElements(c);
			for (Bridge* b : bridges)
				printSubgraphElements(b);

			file.flush();
			file.close();
		}
	}
}

void MainWindow::on_actionLoad_Decomposition_triggered()
{
	QString filter = "Text files (*.txt)";
	QString fileName = QFileDialog::getOpenFileName(0, "Load ER Decomposition", getOpenDir(), filter);
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly))
		{
			ERData* erdata = focusedVis->currentERData;
			ERClusterDecomp* decomp = new ERClusterDecomp(erdata);
			decomp->initEmpty();

			QTextStream stream(&file);
			QStringList list = stream.readLine().split(",");
			int num_clusters = list[0].toInt();
			int num_bridges = list[1].toInt();
			int num_ens, num_res;

			for (int i = 0; i < num_clusters; i++)
			{
				Cluster* c = new Cluster();
				list = stream.readLine().split(",");
				num_ens = list[0].toInt();
				num_res = list[1].toInt();
				for (int j = 0; j < num_ens; j++)
					decomp->addToCluster(list[j + 2].toInt(), c, ENTITY);
				for (int j = 0; j < num_res; j++)
					decomp->addToCluster(list[j + 2 + num_ens].toInt(), c, RELATIONSHIP);
			}
			
			for (int i = 0; i < num_bridges; i++)
			{
				Bridge* b = new Bridge();
				list = stream.readLine().split(",");
				num_ens = list[0].toInt();
				num_res = list[1].toInt();
				for (int j = 0; j < num_ens; j++)
					decomp->addToBridge(list[j + 2].toInt(), b, ENTITY);
				for (int j = 0; j < num_res; j++)
					decomp->addToBridge(list[j + 2 + num_ens].toInt(), b, RELATIONSHIP);
			}

			focusedVis->setERClusterDecomp(decomp);
			decomp->createClusterLayouts();
			/*ColorGradient ccolors = getCurrentColorGradient();
			ColorGradient bcolors = ColorGradient();
			decomp->assignColors(ccolors,bcolors);*/

			file.flush();
			file.close();
		}
	}
}


// View Dropdown Menu

void MainWindow::on_actionPolygon_View_triggered()
{
	ui->actionKonig_View->setChecked(false);
	ui->toolBar->setEnabled(true);

	focusedVis->m_selectType = SELECTTYPE::SELECTENTITY;
	focusedVis->m_mouseAction = MOUSEACTION::SELECT;
	focusedVis->viewStyle = VIEWSTYLE::POLYGON;
	if (focusedVis->isVisible())
		focusedVis->update();
}

void MainWindow::on_actionKonig_View_triggered()
{
	ui->actionPolygon_View->setChecked(false);
	
	focusedVis->m_selectType = SELECTTYPE::SELECTENTITY;
	focusedVis->m_mouseAction = MOUSEACTION::SELECT;
	//vis->currentERData->constructKonigGraph();
	focusedVis->viewStyle = VIEWSTYLE::BIPARTITE;
	if (focusedVis->isVisible())
		focusedVis->update();
}



// Layout Control Panel

void MainWindow::apply_layout()
{	
	if (!hasFocusedVis())
		return;

	int layout = control_dock->ui->comboBox_LayoutSelector->currentIndex();

	// get current selection
	ERData* erdata = focusedVis->currentERData;
	/*ENList en_selection;
	REList re_selection;
	{
		ENList enlist = focusedVis->getSelectedEntities();
		REList relist = focusedVis->getSelectedRelationships();
		for (Entity* en : enlist)
		{
			if (en->flag_Optimizable)
				en_selection.push_back(en);
			for (Relationship* re : en->getIncidentRelationships())
				if (!isRelationshipInsideRelationshipList(re,re_selection))
					re_selection.push_back(re);
		}

		for (Relationship* re : relist)
		{
			if (re->flag_Optimizable)
				re_selection.push_back(re);
			for (Entity* en : re->getIncidentEntities())
				if (!isEntityInsideEntityList(en, en_selection))
					en_selection.push_back(en);
			for (Relationship* adj : re->getAdjacentRelationships())
				if (adj->getCardinality() == 1)
					re_selection.push_back(adj);
		}

		if (en_selection.empty() && re_selection.empty())
		{
			en_selection = erdata->getFreeEntities();
			re_selection = erdata->getFreeRelationships();
		}
	}*/
	ENList en_selection = focusedVis->getSelectedEntities(true);
	REList re_selection = focusedVis->getSelectedRelationships(true);
	
	ERClusterDecomp* decomp = focusedVis->getERClusterDecomp();
	if (decomp && flag_applyLayoutsToClusterMergedCopy)
	{
		erdata = copyERDataMergeClusters(erdata, decomp);
		getMatchingIndexEntities(&en_selection, erdata);
		getMatchingIndexRelationships(&re_selection, erdata);
		set_ERData(erdata, focusedVis, false);
	}

	if (en_selection.empty() && re_selection.empty())
	{
		en_selection = erdata->getFreeEntities();
		re_selection = erdata->getFreeRelationships();
	}
	
	// apply the layout
	switch (layout)
	{

	case 1: // transform layout //////////////////////////////////
	{
		// get settings from UI
		get_layout_transform_settings();

		Vec3 pivot = focusedVis->m_cursorPos;
		if (TR_translation)
			erdata->moveEntities(TR_translate_x, TR_translate_y, &en_selection);
		if (TR_rotation)
		{
			erdata->rotateEntities(TR_rotate_angle, pivot, &en_selection);
			erdata->rotateMonogons(TR_rotate_angle, &re_selection);
		}
		if (TR_scaling)
			erdata->scaleEntities(Vec3(TR_scale_x, TR_scale_y, 0), pivot, &en_selection);
		if (TR_reflection)
			erdata->reflectEntities(TR_reflect_angle, pivot, &en_selection);

		break;
	}

	case 2: // random layout /////////////////////////////////////
	{
		// get settings from UI
		get_random_layout_settings();

		Vec3 pivot = focusedVis->m_cursorPos;
		if (RD_rect_domain)
			erdata->designLayout_RandomRectangular(pivot.x, pivot.y, RD_rad_min, RD_rad_max, en_selection);
		else
			erdata->designLayout_RandomCircular(pivot.x, pivot.y, RD_rad_min, RD_rad_max, en_selection);

		break;
	}

	case 3: // Grid Layout ///////////////////////////////////////
	{
		// get settings from UI
		get_grid_layout_settings();

		Vec3 pivot = focusedVis->m_cursorPos;
		erdata->designLayout_RectGrid(GR_dimension, GR_col_wise, pivot.x, pivot.y, GR_spacing_x, GR_spacing_y, en_selection);
		break;
	}

	case 4: // Circle Layout /////////////////////////////////////
	{
		// get settings from UI
		get_circle_layout_settings();

		Vec3 pivot = focusedVis->m_cursorPos;
		erdata->designLayout_CircGrid(pivot.x, pivot.y, CL_radius, CL_start_angle, en_selection, CL_sort, CL_ascending);
		break;
	}

	case 5: // adjust monogons ///////////////////////////////////
	{
		bool distribute_evenly = false;
		QTreeWidget* properties = control_dock->ui->treeWidget_MonogonProperties;
		QTreeWidgetItemIterator property(properties);
		while (*property)
		{
			QString text = (*property)->text(0);
			if (text == "Distribute Evenly")
				distribute_evenly = (*property)->checkState(1);
			property++;
		}

		if (distribute_evenly)
			erdata->resetMonogonAngles(en_selection);
		else
			erdata->distributeMonogonAngles(en_selection);
		erdata->update2D();

		break;
	}

	case 6: // force directed ////////////////////////////////////
	{
		// get settings from UI
		get_force_directed_settings();

		double execution_time;
		LogConsole::writeConsole("\n-------- Started Force Directed Algorithm --------");

#ifdef USE_OPENMP
		double time1 = omp_get_wtime();
#else
		clock_t time1 = clock();
#endif

		erdata->designLayout_ForceDirected(en_selection, FD_step, FD_edge_length, FD_relative_coef, FD_div_pow, FD_iterations, FD_tolerance, true);

#ifdef USE_OPENMP
		double time2 = omp_get_wtime();
		execution_time = time2 - time1;
#else
		clock_t time2 = clock();
		execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
#endif

		LogConsole::writeConsole("Layout cost: %.3lf sec.", execution_time);
		LogConsole::writeConsole("----------------- END ----------------\n");

		break;
	}

	case 7: // layout optimization ///////////////////////////////
	{
		// get optimization properties from UI
		get_optimization_settings();

		// get elements to be optimized
		bool subgraph = false;
		std::vector<int> enlist, relist;
		if (en_selection.size() < erdata->getEntityNum())
		{
			subgraph = true;
			for (Entity* en : en_selection)
				enlist.push_back(en->getIndex());
			for (Relationship* re : re_selection)
				if (re)
					relist.push_back(re->getIndex());
		}

		// call optimization function
		double costtime;
		double finalEnergy;
		int iterNum;
		int HLBFGSEvaluations;
		if (dualVisVisible() && flag_jointOptimize)
		{
			if (!primeVis->currentERData) {
				LogConsole::showError("No prime ERdata");
				return;
			}

			if (!dualVis->currentERData) {
				LogConsole::showError("No dual ERdata");
				return;
			}		
			
			if (subgraph)
			{
				free_elements(erdata, enlist, relist);
				free_elements(erdata->getDualERData(), relist, enlist);
				Optimize_layout(erdata, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "DualSubgraphHLBFGS", erdata->getDualERData());
			}
			else if (flag_optimizeWithMonogons)
				Optimize_layout(erdata, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "DualHLBFGS", erdata->getDualERData());
			else
				Optimize_layout(erdata, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "DualPolygonHLBFGS", erdata->getDualERData());
		}
		else
		{
			if (subgraph)
			{
				free_elements(erdata, enlist, relist);
				Optimize_layout(erdata, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "SubgraphHLBFGS");
			}
			else if (flag_optimizeWithMonogons)
				Optimize_layout(erdata, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "HLBFGS");
			else
				Optimize_layout(erdata, costtime, finalEnergy, iterNum, HLBFGSEvaluations, "PolygonHLBFGS");
		}
		
		break;
	}

	case 8: // layout evaluation /////////////////////////////////
	{
		bool polyReg, sideLen, polySep, interReg, overArea, overCount;
		QTreeWidget* properties = control_dock->ui->treeWidget_EnergyProperties;
		QTreeWidgetItemIterator property(properties);
		while (*property)
		{
			QString text = (*property)->text(0);
			if (text == "Polygon Regularity")
				polyReg = (*property)->checkState(1);
			else if (text == "Side Length")
				sideLen = (*property)->checkState(1);
			else if (text == "Polygon Separation")
				polySep = (*property)->checkState(1);
			else if (text == "Intersection Regularity")
				interReg = (*property)->checkState(1);
			else if (text == "Overlap Area")
				overArea = (*property)->checkState(1);
			else if (text == "Overlap Count")
				overCount = (*property)->checkState(1);
			property++;
		}

		LogConsole::writeConsole("Unavoidable Overlaps: %d", erdata->countNAdjacentClusters());

		calcAverageLength();
		OptimizationHLBFGS m_optHLBFGSSolver;
		m_optHLBFGSSolver.evaluate_layout_energy(erdata, polyReg, sideLen, polySep, interReg, overArea, overCount);

		break;
	}
	
	case 9: // konig layout //////////////////////////////////////
	{
		double scale, base_ratio;
		bool size_optim, side_optim;
		QTreeWidget* properties = control_dock->ui->treeWidget_KonigProperties;
		QTreeWidgetItemIterator property(properties);
		while (*property)
		{
			QString text = (*property)->text(0);
			if (text == "Postprocess Scaling")
				scale = (*property)->text(1).toDouble();
			else if (text == "Base Ratio")
				base_ratio = (*property)->text(1).toDouble();
			else if (text == "Optimize Size")
				size_optim = (*property)->checkState(1);
			else if (text == "Optimize Edge Length")
				side_optim = (*property)->checkState(1);
			property++;
		}

		bool result = erdata->applyKonigLayout(base_ratio, size_optim, side_optim, scale);
		if (result)
			LogConsole::writeGreenConsole("Konig layout applied.");
		else
			LogConsole::writeRedConsole("Konig graph is not planar.");

		if (primeVis->isVisible())
			reset_sceneToContent(primeVis);
		if (dualVis->isVisible())
			reset_sceneToContent(dualVis);
		if (bipartVis->isVisible())
			reset_sceneToContent(bipartVis);

		break;
	}

	default:
		break;
	}

	if (decomp && flag_applyLayoutsToClusterMergedCopy)
	{
		copyVertexLocations(erdata, decomp->getERData());
		set_ERData(decomp->getERData(), focusedVis, false);
		delete(erdata);
		decomp->updateClusterLayouts();
	}

	updateWidgets();
	//if (primeVis->isVisible())
	//	reset_sceneToContent(primeVis);
	//if (dualVis->isVisible())
	//	reset_sceneToContent(dualVis);
}

void MainWindow::get_layout_transform_settings()
{
	QTreeWidget* properties = control_dock->ui->treeWidget_TransformProperties;
	QTreeWidgetItemIterator property(properties);
	while (*property)
	{
		QString text = (*property)->text(0);
		if (text == "Use Translation")
			TR_translation = (*property)->checkState(1);
		else if (text == "Translation X")
			TR_translate_x = (*property)->text(1).toDouble();
		else if (text == "Translation Y")
			TR_translate_y = (*property)->text(1).toDouble();
		else if (text == "Use Rotation")
			TR_rotation = (*property)->checkState(1);
		else if (text == "Angle")
			TR_rotate_angle = (*property)->text(1).toDouble() * M_PI / 180;
		else if (text == "Use Scaling")
			TR_scaling = (*property)->checkState(1);
		else if (text == "Scale X")
			TR_scale_x = (*property)->text(1).toDouble();
		else if (text == "Scale Y")
			TR_scale_y = (*property)->text(1).toDouble();
		else if (text == "Use Reflection")
			TR_reflection = (*property)->checkState(1);
		else if (text == "Axis Angle")
			TR_reflect_angle = (*property)->text(1).toDouble() * M_PI / 180;
		property++;
	}

	LogConsole::writeConsole("\n------ Layout Transform Settings ------");
	if (TR_translation)
	{
		LogConsole::writeConsole("TR_translate_x: %.2f", TR_translate_x);
		LogConsole::writeConsole("TR_translate_y: %.2f", TR_translate_y);
	}
	if (TR_rotation)
	{
		LogConsole::writeConsole("TR_rotate_angle: %.2f", TR_rotate_angle);
	}
	if (TR_scaling)
	{
		LogConsole::writeConsole("TR_scale_x: %.2f", TR_scale_x);
		LogConsole::writeConsole("TR_scale_y: %.2f", TR_scale_y);
	}
	if (TR_reflection)
	{
		LogConsole::writeConsole("TR_reflect_angle: %.2f", TR_reflect_angle);
	}
	LogConsole::writeConsole("-------------------------------------");
}

void MainWindow::get_random_layout_settings()
{
	QTreeWidget* properties = control_dock->ui->treeWidget_RandomProperties;
	QTreeWidgetItemIterator property(properties);
	while (*property)
	{
		QString text = (*property)->text(0);
		if (text == "(Rectangular Domain)")
			RD_rect_domain = (*property)->checkState(1);
		else if (text == "Radius Min (Width)")
			RD_rad_min = (*property)->text(1).toDouble();
		else if (text == "Radius Max (Height)")
			RD_rad_max = (*property)->text(1).toDouble();
		property++;
	}

	LogConsole::writeConsole("\n------ Random Layout Settings ------");
	LogConsole::writeConsole("RD_rect_domain: %s", RD_rect_domain ? "true" : "false");
	LogConsole::writeConsole("RD_rad_min: %.2f", RD_rad_min);
	LogConsole::writeConsole("RD_rad_max: %.2f", RD_rad_max);
	LogConsole::writeConsole("-------------------------------------");
}

void MainWindow::get_grid_layout_settings()
{
	QTreeWidget* properties = control_dock->ui->treeWidget_GridProperties;
	QTreeWidgetItemIterator property(properties);
	while (*property)
	{
		QString text = (*property)->text(0);
		if (text == "Min Dimension")
			GR_dimension = (*property)->text(1).toInt();
		else if (text == "Column-Wise")
			GR_col_wise = (*property)->checkState(1);
		else if (text == "Grid Spacing X")
			GR_spacing_x = (*property)->text(1).toDouble();
		else if (text == "Grid Spacing Y")
			GR_spacing_y = (*property)->text(1).toDouble();
		property++;
	}

	LogConsole::writeConsole("\n------ Grid Layout Settings ------");
	LogConsole::writeConsole("GR_dimension: %d", GR_dimension);
	LogConsole::writeConsole("GR_col_wise: %s", GR_col_wise ? "true" : "false");
	LogConsole::writeConsole("GR_spacing_x: %.2f", GR_spacing_x);
	LogConsole::writeConsole("GR_spacing_y: %.2f", GR_spacing_y);
	LogConsole::writeConsole("-------------------------------------");
}

void MainWindow::get_circle_layout_settings()
{
	QTreeWidget* properties = control_dock->ui->treeWidget_CircleProperties;
	QTreeWidgetItemIterator property(properties);
	while (*property)
	{
		QString text = (*property)->text(0);
		if (text == "Radius")
			CL_radius = (*property)->text(1).toDouble();
		else if (text == "Sort By")
			CL_sort = (ELEMENT_STATISTIC)(*property)->text(1).toInt();
		else if (text == "Sort Ascending")
			CL_ascending = (*property)->checkState(1);
		else if (text == "Start Angle")
			CL_start_angle = (*property)->text(1).toDouble() * M_PI / 180;
		property++;
	}

	LogConsole::writeConsole("\n------ Circle Layout Settings ------");
	LogConsole::writeConsole("CL_radius: %.2f", CL_radius);
	LogConsole::writeConsole("CL_sort: %d", CL_sort);
	LogConsole::writeConsole("CL_start_angle: %.2f", CL_start_angle);
	LogConsole::writeConsole("-------------------------------------");
}

void MainWindow::get_force_directed_settings()
{
	QTreeWidget* properties = control_dock->ui->treeWidget_FDProperties;
	QTreeWidgetItemIterator property(properties);
	while (*property)
	{
		QString text = (*property)->text(0);
		if (text == "Initial Step Size")
			FD_step = (*property)->text(1).toDouble();
		if (text == "Target Edge Length")
			FD_edge_length = (*property)->text(1).toDouble();
		if (text == "Repulsion Coefficient")
			FD_relative_coef = (*property)->text(1).toDouble();
		if (text == "Repulsion Normalization Power")
			FD_div_pow = (*property)->text(1).toDouble();
		if (text == "Number Iterations")
			FD_iterations = (*property)->text(1).toInt();
		if (text == "Convergence Tolerance")
			FD_tolerance = (*property)->text(1).toDouble();
		property++;
	}

	LogConsole::writeConsole("\n------ Force Directed Settings ------");
	LogConsole::writeConsole("FD_step: %.4f", FD_step);
	LogConsole::writeConsole("FD_edge_length: %.4f", FD_edge_length);
	LogConsole::writeConsole("FD_relative_coef: %.4f", FD_relative_coef);
	LogConsole::writeConsole("FD_div_pow: %.4f", FD_div_pow);
	LogConsole::writeConsole("FD_iterations: %d", FD_iterations);
	LogConsole::writeConsole("FD_tolerance: %.4f", FD_tolerance);
	LogConsole::writeConsole("-------------------------------------");
}

void MainWindow::get_optimization_settings()
{
	QTreeWidget* properties = control_dock->ui->treeWidget_OptimizationProperties;
	QTreeWidgetItemIterator property(properties);
	while (*property)
	{
		QString text = (*property)->text(0);
		if (text == "Use Monogons")
			flag_optimizeWithMonogons = (*property)->checkState(1);
		else if (text == "Joint Optimize")
			flag_jointOptimize = (*property)->checkState(1);
		else if (text == "Polygon Regularity (PR)")
			weight_PR = (*property)->text(1).toDouble();
		else if (text == "Polygon Area (PA)")
			weight_PA = (*property)->text(1).toDouble();
		else if (text == "Digon Length (DL)")
			weight_DL = (*property)->text(1).toDouble();
		else if (text == "Polygon Separation (PS)")
			weight_PS = (*property)->text(1).toDouble();
		else if (text == "Polygon Interation (PI)")
			weight_PI = (*property)->text(1).toDouble();
		else if (text == "Dual Distance (DD)")
			weight_DD = (*property)->text(1).toDouble();
		else if (text == "Separation Angle Buffer")
			PS_angle_buffer = (*property)->text(1).toDouble();
		else if (text == "Separation Distance Buffer")
			PS_distance_buffer = (*property)->text(1).toDouble();
		else if (text == "Maximum Cardinality")
			PA_max_card = (*property)->text(1).toInt();
		else if (text == "Target Edge Length")
			expectedLength = (*property)->text(1).toDouble();
		else if (text == "Maximum Iterations")
			ITERMAX = (*property)->text(1).toInt();
		else if (text == "Gradient Threshold")
			GNormThreshold = (*property)->text(1).toDouble();
		else if (text == "Gradient Division Threshold")
			GNormDivisionThreshold = (*property)->text(1).toDouble();
		else if (text == "Line-Search Function Tolerance")
			HLBFGS_linesearch_ftol = (*property)->text(1).toDouble();
		else if (text == "Line-Search Variable Tolerance")
			HLBFGS_linesearch_xtol = (*property)->text(1).toDouble();
		else if (text == "Line-Search Gradient Tolerance")
			HLBFGS_linesearch_gtol = (*property)->text(1).toDouble();
		else if (text == "Line-Search Minimum Step")
			HLBFGS_linesearch_stepmin = (*property)->text(1).toDouble();
		else if (text == "Line-Search Maximum Step")
			HLBFGS_linesearch_stepmax = (*property)->text(1).toDouble();
		else if (text == "Line-Search Maximum Iterations")
			HLBFGS_linesearch_IterMax = (*property)->text(1).toDouble();

		property++;
	}

	LogConsole::writeConsole("\n------ Optimization Settings ------");
	LogConsole::writeConsole("weight_PA: %.4f",weight_PA);
	LogConsole::writeConsole("weight_PR: %.4f",weight_PR);
	LogConsole::writeConsole("weight_PS: %.4f",weight_PS);
	LogConsole::writeConsole("weight_PI: %.4f",weight_PI);
	LogConsole::writeConsole("weight_DL: %.4f",weight_DL);
	LogConsole::writeConsole("weight_DD: %.4f",weight_DD);
	LogConsole::writeConsole("PS_angle_buffer: %.4f",PS_angle_buffer);
	LogConsole::writeConsole("PS_distance_buffer: %.4f",PS_distance_buffer);
	LogConsole::writeConsole("HLBFGS_linesearch_ftol: %.2e",HLBFGS_linesearch_ftol);
	LogConsole::writeConsole("HLBFGS_linesearch_xtol: %.2e",HLBFGS_linesearch_xtol);
	LogConsole::writeConsole("HLBFGS_linesearch_gtol: %.2e",HLBFGS_linesearch_gtol);
	LogConsole::writeConsole("HLBFGS_linesearch_stepmin: %.2e",HLBFGS_linesearch_stepmin);
	LogConsole::writeConsole("HLBFGS_linesearch_stepmax: %.2e",HLBFGS_linesearch_stepmax);
	LogConsole::writeConsole("GNormDivisionThreshold: %.2e",GNormDivisionThreshold);
	LogConsole::writeConsole("GNormThreshold: %.2e",GNormThreshold);
	LogConsole::writeConsole("HLBFGS_linesearch_IterMax: %.2e",HLBFGS_linesearch_IterMax);
	LogConsole::writeConsole("ITERMAX: %.2e",ITERMAX);
	LogConsole::writeConsole("-----------------------------------\n");
}

void MainWindow::get_subgraph_settings()
{
	flag_optimizeWithMonogons = true;
	flag_jointOptimize = true;
	weight_PR = 0.3;
	weight_PA = 0.16;
	weight_DL = 1.0;
	weight_PS = 0.36;
	weight_PI = 0.18;
	weight_DD = 1.0;
	PS_angle_buffer = 60.0;
	PS_distance_buffer = 0.5;
	PA_max_card = 1;
	expectedLength = 1.0;
	ITERMAX = 8;
	GNormThreshold = 0.1;
	GNormDivisionThreshold = 0.01;
	HLBFGS_linesearch_ftol = 0.0001;
	HLBFGS_linesearch_xtol = 0.0000000000000001;
	HLBFGS_linesearch_gtol = 0.9;
	HLBFGS_linesearch_stepmin = 0.00000000000000000001;
	HLBFGS_linesearch_stepmax = 100000000000000000000.0;
	HLBFGS_linesearch_IterMax = 4;

	LogConsole::writeConsole("\n------ Optimization Settings ------");
	LogConsole::writeConsole("weight_PA: %.4f",weight_PA);
	LogConsole::writeConsole("weight_PR: %.4f",weight_PR);
	LogConsole::writeConsole("weight_PS: %.4f",weight_PS);
	LogConsole::writeConsole("weight_PI: %.4f",weight_PI);
	LogConsole::writeConsole("weight_DL: %.4f",weight_DL);
	LogConsole::writeConsole("weight_DD: %.4f",weight_DD);
	LogConsole::writeConsole("PS_angle_buffer: %.4f",PS_angle_buffer);
	LogConsole::writeConsole("PS_distance_buffer: %.4f",PS_distance_buffer);
	LogConsole::writeConsole("HLBFGS_linesearch_ftol: %.2e",HLBFGS_linesearch_ftol);
	LogConsole::writeConsole("HLBFGS_linesearch_xtol: %.2e",HLBFGS_linesearch_xtol);
	LogConsole::writeConsole("HLBFGS_linesearch_gtol: %.2e",HLBFGS_linesearch_gtol);
	LogConsole::writeConsole("HLBFGS_linesearch_stepmin: %.2e",HLBFGS_linesearch_stepmin);
	LogConsole::writeConsole("HLBFGS_linesearch_stepmax: %.2e",HLBFGS_linesearch_stepmax);
	LogConsole::writeConsole("GNormDivisionThreshold: %.2e",GNormDivisionThreshold);
	LogConsole::writeConsole("GNormThreshold: %.2e",GNormThreshold);
	LogConsole::writeConsole("HLBFGS_linesearch_IterMax: %d",HLBFGS_linesearch_IterMax);
	LogConsole::writeConsole("ITERMAX: %d",ITERMAX);
	LogConsole::writeConsole("-----------------------------------\n");
}

void MainWindow::get_simplified_settings()
{
	flag_optimizeWithMonogons = true;
	flag_jointOptimize = true;
	weight_PR = 0.2;
	weight_PA = 0.4;
	weight_DL = 1.0;
	weight_PS = 0.4;
	weight_PI = 0.0;
	weight_DD = 1.0;
	PS_angle_buffer = 60.0;
	PS_distance_buffer = 0.5;
	PA_max_card = 1;
	expectedLength = 1.0;
	ITERMAX = 64;
	GNormThreshold = 0.000001;
	GNormDivisionThreshold = 0.0001;
	HLBFGS_linesearch_ftol = 0.00000001;
	HLBFGS_linesearch_xtol = 0.0000000000000001;
	HLBFGS_linesearch_gtol = 0.9;
	HLBFGS_linesearch_stepmin = 0.00000000000000000001;
	HLBFGS_linesearch_stepmax = 100000000000000000000.0;
	HLBFGS_linesearch_IterMax = 8;

	LogConsole::writeConsole("\n------ Optimization Settings ------");
	LogConsole::writeConsole("weight_PA: %.4f",weight_PA);
	LogConsole::writeConsole("weight_PR: %.4f",weight_PR);
	LogConsole::writeConsole("weight_PS: %.4f",weight_PS);
	LogConsole::writeConsole("weight_PI: %.4f",weight_PI);
	LogConsole::writeConsole("weight_DL: %.4f",weight_DL);
	LogConsole::writeConsole("weight_DD: %.4f",weight_DD);
	LogConsole::writeConsole("PS_angle_buffer: %.4f",PS_angle_buffer);
	LogConsole::writeConsole("PS_distance_buffer: %.4f",PS_distance_buffer);
	LogConsole::writeConsole("HLBFGS_linesearch_ftol: %.2e",HLBFGS_linesearch_ftol);
	LogConsole::writeConsole("HLBFGS_linesearch_xtol: %.2e",HLBFGS_linesearch_xtol);
	LogConsole::writeConsole("HLBFGS_linesearch_gtol: %.2e",HLBFGS_linesearch_gtol);
	LogConsole::writeConsole("HLBFGS_linesearch_stepmin: %.2e",HLBFGS_linesearch_stepmin);
	LogConsole::writeConsole("HLBFGS_linesearch_stepmax: %.2e",HLBFGS_linesearch_stepmax);
	LogConsole::writeConsole("GNormDivisionThreshold: %.2e",GNormDivisionThreshold);
	LogConsole::writeConsole("GNormThreshold: %.2e",GNormThreshold);
	LogConsole::writeConsole("HLBFGS_linesearch_IterMax: %d",HLBFGS_linesearch_IterMax);
	LogConsole::writeConsole("ITERMAX: %d",ITERMAX);
	LogConsole::writeConsole("-----------------------------------\n");
}



// Multiscale Control Panel

void MainWindow::generateAtomicOperations()
{
	double alpha = control_dock->ui->doubleSpinBox_Alpha->value();
	double beta = control_dock->ui->doubleSpinBox_Beta->value();
	double gamma = control_dock->ui->doubleSpinBox_Gamma->value();

	bool edge_remove = control_dock->ui->checkBox_OperationEdgeRemove->isChecked();
	bool edge_merge = control_dock->ui->checkBox_OperationEdgeMerge->isChecked();
	bool node_remove = control_dock->ui->checkBox_OperationNodeRemove->isChecked();
	bool node_merge = control_dock->ui->checkBox_OperationNodeMerge->isChecked();

	clock_t time1 = clock();

	generateAtomicOperationsCommandLine(alpha, beta, gamma, node_remove, edge_remove, node_merge, edge_merge);

	clock_t time2 = clock();
	double execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
	LogConsole::writeConsole("Operation generation time: %.4f", execution_time);

	

	//if (!hasFocusedVis())
	//	return;
	//ERData* focused_ERData = focusedVis->currentERData;
	//
	//delete m_ERProgressive;
	//m_ERProgressive = new ERProgressive();

	//double alpha = control_dock->ui->doubleSpinBox_Alpha->value();
	//double beta = control_dock->ui->doubleSpinBox_Beta->value();
	//double gamma = control_dock->ui->doubleSpinBox_Gamma->value();

	//bool edge_remove = control_dock->ui->checkBox_OperationEdgeRemove->isChecked();
	//bool edge_merge = control_dock->ui->checkBox_OperationEdgeMerge->isChecked();
	//bool node_remove = control_dock->ui->checkBox_OperationNodeRemove->isChecked();
	//bool node_merge = control_dock->ui->checkBox_OperationNodeMerge->isChecked();

	////double share_power = 2.0;
	////int max_emerge = control_dock->ui->spinBox_MaxEdgeMergeSize->value();

	//m_ERProgressive->setSimilarityCoefficients(alpha, beta, gamma);
	//m_ERProgressive->setOps(edge_remove, edge_merge, node_remove, node_merge);

	//focused_ERData->generateAtomicOperations(node_remove, node_merge, edge_remove, edge_merge, alpha, beta, gamma);
	////bool result = m_ERProgressive->generate_operations(focused_ERData, max_emerge);
	//bool result = m_ERProgressive->initialize_operation_queue(focused_ERData);

	//if (!result)
	//{
	//	LogConsole::showError("Operations could not be generated");
	//	delete m_ERProgressive;
	//	m_ERProgressive = NULL;
	//}
	//else
	//	LogConsole::writeGreenConsole("Operations generated");
}

void MainWindow::generateAtomicOperationsCommandLine(double alpha, double beta, double gamma, bool nrem, bool erem, bool nmerge, bool emerge)
{
	if (!hasFocusedVis())
		return;
	ERData* focused_ERData = focusedVis->currentERData;
	
	delete m_ERProgressive;
	m_ERProgressive = new ERProgressive();
	m_ERProgressive->setOps(erem, emerge, nrem, nmerge);
	m_ERProgressive->setSimilarityCoefficients(alpha, beta, gamma);
	focused_ERData->generateAtomicOperations(nrem, nmerge, erem, emerge, alpha, beta, gamma);
	m_ERProgressive->initialize_operation_queue(focused_ERData);
}

void MainWindow::applyNextOperation()
{
	if (m_ERProgressive == NULL)
	{
		LogConsole::showError("No operations generated");
		return;
	}

	int max_ops = control_dock->ui->spinBox_MaxOps->value();
	double threshold = control_dock->ui->doubleSpinBox_PriorityThreshold->value();
	int min_edges = control_dock->ui->spinBox_MinEdges->value();
	int min_nodes = control_dock->ui->spinBox_MinNodes->value();
	int max_emerge = control_dock->ui->spinBox_MaxEdgeMergeSize->value();

	bool res = m_ERProgressive->apply_operation(max_ops,threshold,min_edges, min_nodes);
	if (!res)
		LogConsole::showError("No valid operations to apply");

	ERDataChanged();
	updateWidgets();
}

void MainWindow::invertLastOperation()
{
	if (m_ERProgressive == NULL)
	{
		LogConsole::showError("No operations generated");
		return;
	}

	ERData* erdata = m_ERProgressive->getERData();
	std::vector<int> enlist, relist;
	flag_optimizeInversions = control_dock->ui->checkBox_optimizeInversions->isChecked();
	bool res = m_ERProgressive->invert_operation(&enlist, &relist);

	if (!res)
		LogConsole::showError("No valid operations to invert");
	else
	{
		ERData* pdata = m_ERProgressive->getERData();
		free_elements(pdata, enlist, relist);
		ERData* ddata = pdata->getDualERData();
		if (ddata)
			free_elements(ddata, relist, enlist);

		if (flag_optimizeInversions && enlist.size() > 0)
		{
			LogConsole::writeConsole("\n------Start Subgraph Optimization------");
			get_subgraph_settings();
			flag_stopOptimization = false;
			//resetEnergyTiming();
			OptimizationHLBFGS m_solver;
			if (ddata)
				m_solver.dual_optimize_subgraph_HLBFGS(pdata, ddata);
			else
				m_solver.optimize_subgraph_HLBFGS(pdata);
			//printEnergyTiming(enlist.size(), relist.size());
			LogConsole::writeConsole("--------END--------\n");
		}
		else
		{
			ERDataChanged();
			updateWidgets();
		}
	}
}

void MainWindow::applyAllOperations()
{
	clock_t time1, time2;
	//time1 = clock();

	if (m_ERProgressive == NULL)
		generateAtomicOperations();

	int max_ops = control_dock->ui->spinBox_MaxOps->value();
	double threshold = control_dock->ui->doubleSpinBox_PriorityThreshold->value();
	int min_edges = control_dock->ui->spinBox_MinEdges->value();
	int min_nodes = control_dock->ui->spinBox_MinNodes->value();
	flag_optimizeInversions = control_dock->ui->checkBox_optimizeInversions->isChecked();
	ERData* erdata = m_ERProgressive->getERData();
	ERData* dual_erdata = erdata->getDualERData();
	
	//Eigen::VectorXd evals_full = erdata->getIncidenceEigenvals();
	//std::vector<Eigen::VectorXd> evals_all = { evals_full };

	//Eigen::VectorXd evals_en_full = erdata->getEnAdjEigenvals();
	//Eigen::VectorXd evals_re_full = erdata->getReAdjEigenvals();
	//std::vector<Eigen::VectorXd> evals_en_all = { evals_en_full };
	//std::vector<Eigen::VectorXd> evals_re_all = { evals_re_full };

	time1 = clock();

	bool result = true;
	int opcount = 0;
	while (result)
	{
		opcount++;
		result = m_ERProgressive->apply_operation(max_ops,threshold,min_edges, min_nodes);
				
		//Eigen::VectorXd evals_simp = erdata->getIncidenceEigenvals();
		//evals_all.push_back(evals_simp);

		//Eigen::VectorXd evals_en_simp = erdata->getEnAdjEigenvals();
		//Eigen::VectorXd evals_re_simp = erdata->getReAdjEigenvals();
		//evals_en_all.push_back(evals_en_simp);
		//evals_re_all.push_back(evals_re_simp);
	}
	LogConsole::writeConsole("Applied %d Operations",opcount);

	time2 = clock();
	double execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
	LogConsole::writeConsole("Simplification Time: %.4f", execution_time);

	// initialize with force-directed konig layout
	//erdata->applyKonigLayout(0.33, true, true,1.25);
	////erdata->reCenterScaleData(1.25);
	//erdata->syncDualLayout();

	//if (primeVis->isVisible())
	//{
	//	primeVis->reset_sceneToCenterVisualization();
	//	primeVis->update();
	//}
	//if (dualVis->isVisible())
	//{
	//	dualVis->reset_sceneToCenterVisualization();
	//	dualVis->update();
	//}
	
	if (flag_optimizeInversions)
	{
		erdata->applyKonigLayout(0.33, true, true,1.25);
		erdata->syncDualLayout();

		get_simplified_settings();
		LogConsole::writeConsole("\n------Start Subgraph Optimization------");
		flag_stopOptimization = false;
		//resetEnergyTiming();
		OptimizationHLBFGS m_solver;
		if (dual_erdata)
			m_solver.dual_optimize_subgraph_HLBFGS(erdata, dual_erdata);
		else
			m_solver.optimize_subgraph_HLBFGS(erdata);
		//printEnergyTiming(erdata->getEntityNum(),erdata->getRelationshipNum());
		LogConsole::writeConsole("--------END--------\n");
	}

	//time2 = clock();
	//LogConsole::writeConsole("Total time: %.4f", (double(time2 - time1) / CLOCKS_PER_SEC));

	//QString fileName = QFileDialog::getSaveFileName(0,"Save Simplification Data");
	//if (fileName.isEmpty())
	//{
	//	LogConsole::writeError("Unable to save simplification data");
	//	return;
	//}
	//else
	//{
	//	std::string fname = fileName.toStdString();
	//	std::ofstream file(fname);
	//	if (file.is_open())
	//	{
	//		Eigen::IOFormat row_format(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ");
	//		for (int i = 0; i < evals_all.size(); i++)
	//		{
	//			file << evals_all[i].format(row_format) << std::endl;
	//		}
	//	}
	//	file.close();

	//	std::string fname = fileName.toStdString();
	//	std::ofstream file(fname + "_entity.txt");
	//	if (file.is_open())
	//	{
	//		Eigen::IOFormat row_format(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ");
	//		for (int i = 0; i < evals_en_all.size(); i++)
	//		{
	//			file << evals_en_all[i].format(row_format) << std::endl;
	//		}
	//	}
	//	file.close();

	//	std::ofstream file2(fname + "_relationship.txt");
	//	if (file2.is_open())
	//	{
	//		Eigen::IOFormat row_format(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ");
	//		for (int i = 0; i < evals_re_all.size(); i++)
	//		{
	//			file2 << evals_re_all[i].format(row_format) << std::endl;
	//		}
	//	}
	//	file2.close();
	//}
}

int MainWindow::applyAllOperationsCommandLine(std::string output_name, int min_edges, int min_nodes)
{
	ERData* erdata = m_ERProgressive->getERData();
	int max_ops = m_ERProgressive->getERData()->getEntityNum() + m_ERProgressive->getERData()->getRelationshipNum();
	double threshold = -DBL_MAX;

	bool result = true;
	int opcount = 0;
	while (result)
	{
		result = m_ERProgressive->apply_operation(max_ops,threshold,min_edges, min_nodes);
		opcount++;
	}

	// initialize with force-directed konig layout
	erdata->applyKonigLayout(0.33, true, true,1.25);
	//erdata->reCenterScaleData(1.25);

	if (flag_showCommandLineMultiscaleProgress)
		std::cout << "Optimizing Scale " << opcount << "/" << opcount << std::endl;
	get_simplified_settings();
	LogConsole::writeConsole("\n------Start Subgraph Optimization------");
	flag_stopOptimization = false;
	//resetEnergyTiming();
	OptimizationHLBFGS m_solver;
	m_solver.optimize_subgraph_HLBFGS(erdata);
	//printEnergyTiming(erdata->getEntityNum(), erdata->getRelationshipNum());
	//LogConsole::writeConsole("--------END--------\n");

	// std::string fname = output_name + std::to_string(opcount);
	// bool saved = SaveERFile((fname+".er").c_str());
	//if (saved)
	//{
	//	std::string msg = "saved " + fname;
	//	LogConsole::writeConsole(msg.c_str());
	//}
	//else
	//{
	//	std::string msg = "error saving " + fname;
	//	LogConsole::writeError(msg.c_str());
	//}
	//writeERDataStatistics(fname);

	return opcount;
}

void MainWindow::invertAndOptimize()
{
	if (m_ERProgressive == NULL)
	{
		LogConsole::showError("No operations generated");
		return;
	}

	flag_optimizeInversions = control_dock->ui->checkBox_optimizeInversions->isChecked();
	ERData* erdata = m_ERProgressive->getERData();
	ERData* dual_erdata = erdata->getDualERData();

	string file_name;
	if (flag_save_multiscale)
	{
		QString filters("Nary Data files (*.er);; ALL(*.*)");
		QString defaultFilter("Nary Data files (*.er)");
		QString dir = getOpenDir();
		QString target = dir + "/" + QString::fromStdString(m_FILENAME);
		QString fileName = QFileDialog::getSaveFileName(0, "Save Nary Data file", target, filters, &defaultFilter);
		setOpenDir(fileName);
		file_name = fileName.toStdString();
		file_name = file_name.substr(0, file_name.find("."));
		string str = file_name + "_" + std::to_string(m_ERProgressive->getAppliedOperationNum()) + ".er";
		FileIO::writeERFile(str.c_str(), focusedVis->currentERData);
	}


	bool result = true;
	while (result)
	{
		std::vector<int> enlist, relist;
		//result = m_ERProgressive->invert_last_operation();
		result = m_ERProgressive->invert_operation(&enlist, &relist);
		free_elements(erdata, enlist, relist);
		//free_elements(erdata);
		if (dual_erdata)
		{
			free_elements(dual_erdata, enlist, relist);
			//free_elements(dual_erdata);
		}


		if (!result)
			LogConsole::showError("No more operations to invert");
		else
		{
			if (flag_optimizeInversions && enlist.size() > 0)
			{
				get_subgraph_settings();
				LogConsole::writeConsole("\n------Start Subgraph Optimization------");
				flag_stopOptimization = false;
				//resetEnergyTiming();
				OptimizationHLBFGS m_solver;
				if (dual_erdata)
					m_solver.dual_optimize_subgraph_HLBFGS(erdata, dual_erdata);
				else
					m_solver.optimize_subgraph_HLBFGS(erdata);
				//printEnergyTiming(enlist.size(),relist.size());
				LogConsole::writeConsole("--------END--------\n");
			}
			else
			{
				ERDataChanged();
				updateWidgets();
			}

			if (flag_save_multiscale)
			{
				string str = file_name + "_" + std::to_string(m_ERProgressive->getAppliedOperationNum()) + ".er";
				FileIO::writeERFile(str.c_str(), focusedVis->currentERData);
			}
		}
	}
}

void MainWindow::invertAndOptimizeCommandLine(std::string output_name, int interval, int op_total)
{
	ERData* erdata = m_ERProgressive->getERData();
	ERData* dual_erdata = erdata->getDualERData();
	int scale = op_total;

	bool result = true;
	get_subgraph_settings();
	while (result)
	{
		std::vector<int> enlist, relist;
		result = m_ERProgressive->invert_operation(&enlist, &relist);
		free_elements(erdata, enlist, relist);
		if (dual_erdata)
			free_elements(erdata, enlist, relist);
		
		if (!result)
			break;
		scale--;

		if (flag_showCommandLineMultiscaleProgress)
			std::cout << "Optimizing Scale " << scale << "/" << op_total <<  " ..." << std::endl;
		if (enlist.size() > 0)
		{
			LogConsole::writeConsole("\n------Start Subgraph Optimization------");
			flag_stopOptimization = false;
			//resetEnergyTiming();
			OptimizationHLBFGS m_solver;
			if (dual_erdata)
				m_solver.dual_optimize_subgraph_HLBFGS(erdata, dual_erdata);
			else
				m_solver.optimize_subgraph_HLBFGS(erdata);
			//printEnergyTiming(enlist.size(), relist.size());
			LogConsole::writeConsole("--------END--------\n");
		}
		else
		{
			ERDataChanged();
		}
		
		// int num_size = std::to_string(op_total).size();
		// std::string num_str = std::to_string(scale);
		// while (num_str.size() < num_size)
		// 	num_str = "0" + num_str;
		// std::string fname = output_name + num_str;
		// bool saved = SaveERFile((fname+".er").c_str());
		/*if (saved)
		{
			std::string msg = "saved " + fname;
			LogConsole::writeConsole(msg.c_str());
		}
		else
		{
			std::string msg = "error saving " + fname;
			LogConsole::writeError(msg.c_str());
		}
		writeERDataStatistics(fname);*/
	}
}

void MainWindow::optimizeSubgraphManually()
{
	ERData* pdata = NULL;
	ERData* ddata = NULL;
	if (m_ERProgressive)
	{
		pdata = m_ERProgressive->getERData();
		if (pdata->getDualERData())
			ddata = pdata->getDualERData();
	}
	else if (dualVisVisible())
	{
		if (!primeVis->currentERData) 
		{
			LogConsole::showError("No prime ERdata");
			return;
		}

		if (!dualVis->currentERData)
		{
			LogConsole::showError("No dual ERdata");
			return;
		}

		pdata = primeVis->currentERData;
		ddata = dualVis->currentERData;
	}
	else
	{
		if (!hasFocusedVis())
			return;
		pdata = focusedVis->currentERData;
	}

	LogConsole::writeConsole("\n------Start Subgraph Optimization------");
	//get_subgraph_settings();
	get_simplified_settings();
	flag_stopOptimization = false;
	//resetEnergyTiming();
	OptimizationHLBFGS m_solver;
	if (ddata)
		m_solver.dual_optimize_subgraph_HLBFGS(pdata, ddata);
	else
		m_solver.optimize_subgraph_HLBFGS(pdata);
	//printEnergyTiming(0,0);
	LogConsole::writeConsole("--------END--------\n");
}

void MainWindow::applySelectedOperation()
{
	if (!hasFocusedVis())
		return;

	if (m_ERProgressive == NULL)
	{
		LogConsole::showError("No operations generated");
		return;
	}

	vector<Entity*> selected_ens = focusedVis->getSelectedEntities();
	vector<Relationship*> selected_res = focusedVis->getSelectedRelationships();

	if (selected_ens.empty() && selected_res.empty())
		LogConsole::showError("No element selected");

	if (selected_ens.size() > 2 ||
		selected_res.size() > 2 ||
		(selected_ens.size() > 0 && selected_res.size() > 0))
		LogConsole::showError("Too many elements selected.");

	int max_ops = control_dock->ui->spinBox_MaxOps->value();
	double threshold = control_dock->ui->doubleSpinBox_PriorityThreshold->value();
	int min_edges = control_dock->ui->spinBox_MinEdges->value();
	int min_nodes = control_dock->ui->spinBox_MinNodes->value();
	OperationRecord* sop = NULL;
	bool res;

	if (selected_ens.size() == 1)
	{
		sop = selected_ens[0]->getRemovalOp();
	}
	if (selected_ens.size() == 2)
	{
		Entity* en1 = selected_ens[0];
		Entity* en2 = selected_ens[1];
		for (OperationRecord* op : en1->getMergerOps())
		{
			int id1 = op->getRemovedIndex();
			int id2 = op->getRetainedIndex();
			if ((en1->getOpidx() == id1 && en2->getOpidx() == id2) ||
				(en2->getOpidx() == id1 && en1->getOpidx() == id2))
			{
				sop = op;
				break;
			}
		}
	}
	if (selected_res.size() == 1)
	{
		sop = selected_res[0]->getRemovalOp();
	}
	if (selected_res.size() == 2)
	{
		Relationship* re1 = selected_res[0];
		Relationship* re2 = selected_res[1];
		for (OperationRecord* op : re1->getMergerOps())
		{
			int id1 = op->getRemovedIndex();
			int id2 = op->getRetainedIndex();
			if ((re1->getOpidx() == id1 && re2->getOpidx() == id2) ||
				(re2->getOpidx() == id1 && re1->getOpidx() == id2))
			{
				sop = op;
				break;
			}
		}
	}
	
	if (sop)
	{
		if (sop->isLegal())
		{
			res = m_ERProgressive->apply_operation(max_ops, threshold, min_edges, min_nodes, sop);
			if (!res)
				LogConsole::showError("Selected operation failed.");
		}
		else
			LogConsole::showError("Selected operation is illegal.");
	}
	else
		LogConsole::showError("Selected operation does not exist.");

}

void MainWindow::optimizeInversionsChanged()
{
	flag_optimizeInversions = control_dock->ui->checkBox_optimizeInversions->isChecked();
}

void MainWindow::preserveConnectionsChanged()
{
	flag_preserveConnections = control_dock->ui->checkBox_PreserveConnections->isChecked();
}

void MainWindow::preserveBranchesChanged()
{
	flag_preserveBranches = control_dock->ui->checkBox_PreserveBranches->isChecked();
}

void MainWindow::pruneOnlyChanged()
{
	flag_pruneOnly = control_dock->ui->checkBox_PruneOnly->isChecked();
}







// Hypergraph Cluster Decomposition

void MainWindow::applyBranchClusterDecomposition()
{
	if (!hasFocusedVis())
		return;

	ERData* data = focusedVis->currentERData;

	ERClusterDecomp* decomp = new ERClusterDecomp(data);
	focusedVis->setERClusterDecomp(decomp);

	decomp->initDecomp();
	decomp->Decompose();
	decomp->createClusterLayouts();
	
	ColorGradient ccolors = getCurrentColorGradient();
	ColorGradient bcolors = ColorGradient();
	bcolors.clearGradient();
	bcolors.addColorPoint(0.0, 0.0, 0.0, 0.0);
	bcolors.addColorPoint(0.9,0.9,0.9, 1.0);
	decomp->assignColors(ccolors, bcolors);

	focusedVis->update();
}

void MainWindow::createCluster(ENList enlist, REList relist)
{
	ERClusterDecomp* decomp = focusedVis->getERClusterDecomp();
	if (!decomp)
	{
		ERData* data = focusedVis->currentERData;
		decomp = new ERClusterDecomp(data);
		focusedVis->setERClusterDecomp(decomp);
	}

	decomp->initEmpty();
	Cluster * cluster = decomp->createCluster(enlist, relist);
	decomp->createClusterLayout(cluster);
	
	//ColorGradient ccolors = getCurrentColorGradient();
	//ColorGradient bcolors = ColorGradient();
	//decomp->assignColors(ccolors, bcolors);
}

ERData* MainWindow::copyERDataMergeClusters(ERData* source, ERClusterDecomp* decomp)
{
	ERData* copy_data = new ERData();

	for (int i = 0; i < source->getEntityNum(); i++)
	{
		Entity* en = source->getEntity(i);
		Entity* copy_en = new Entity(en->getLocation());
		copy_en->flag_Optimizable = en->flag_Optimizable;
		copy_en->setSelected(en->getSelected());
		copy_data->addEntity(copy_en);
	}

	for (int i = 0; i < source->getRelationshipNum(); i++)
	{
		Relationship* re = source->getRelationship(i);
		if (decomp->getElemCluster(re->getIndex(), RELATIONSHIP))
			continue;
		
		ENList copy_entities;
		for (Entity* en : re->getIncidentEntities())
			copy_entities.push_back(copy_data->getEntity(en->getIndex()));

		Relationship* copy_re = new Relationship(copy_entities);
		copy_re->setColor(re->getColor());
		copy_re->setMonogonRotation(re->getMonogonRotation());
		copy_re->flag_Optimizable = re->flag_Optimizable;
		copy_re->setSelected(re->getSelected());
		copy_data->addRelationship(copy_re);
	}

	for (Cluster* c : decomp->getClusters())
	{
		ENList copy_entities;
		for (Entity* en : decomp->getSubgraphIncidentEntities(c))
			copy_entities.push_back(copy_data->getEntity(en->getIndex()));

		Relationship* copy_re = new Relationship(copy_entities);
		copy_re->setColor(c->getColor());
		copy_re->flag_Optimizable = true;
		copy_data->addRelationship(copy_re);
	}

	return copy_data;
}

bool MainWindow::getMatchingIndexEntities(ENList* source, ERData* target)
{
	int target_ens = target->getEntityNum();
	bool result = true;

	for (int i = 0; i < source->size(); i++)
	{
		int idx = source->at(i)->getIndex();
		if (idx >= target_ens)
		{
			source->at(i) = nullptr;
			result = false;
		}
		else
		{
			source->at(i) = target->getEntity(idx);
		}
	}
	
	return result;
}

bool MainWindow::getMatchingIndexRelationships(REList* source, ERData* target)
{
	int target_res = target->getRelationshipNum();
	bool result = true;

	for (int i = 0; i < source->size(); i++)
	{
		int idx = source->at(i)->getIndex();
		if (idx >= target_res)
		{
			source->at(i) = nullptr;
			result = false;
		}
		else
		{
			source->at(i) = target->getRelationship(idx);
		}
	}

	return result;
}

bool MainWindow::copyVertexLocations(ERData* source, ERData* target)
{
	int source_ens = source->getEntityNum();
	int source_res = source->getRelationshipNum();
	int target_ens = target->getEntityNum();
	int target_res = target->getRelationshipNum();
	bool result = (source_ens == target_ens && source_res == target_res);
	int num_ens = std::min(source_ens, target_ens);
	int num_res = std::min(source_res, target_res);

	for (int i = 0; i < num_ens; i++)
	{
		Vec3 loc = source->getEntity(i)->getLocation();
		target->getEntity(i)->setLocation(loc);
	}

	for (int i = 0; i < num_res; i++)
	{
		double angle = source->getRelationship(i)->getMonogonRotation();
		target->getRelationship(i)->setMonogonRotation(angle);
	}

	target->update2D();
	return result;
}






// Hypergraph Cycle Decomposition

void MainWindow::applyCycleDecomposition()
{
	if (m_ERCycleDecomp)
	{
		delete(m_ERCycleDecomp);
		m_ERCycleDecomp = NULL;
	}

	clock_t time1 = clock();

	m_ERCycleDecomp = new ERCycleDecomp(m_ERData);
	m_ERCycleDecomp->Decompose();

	clock_t time2 = clock();
	double execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
	LogConsole::writeConsole("Cycle decomposition time: %.4f", execution_time);
}


bool MainWindow::decompHighlightChanged()
{
	if (!m_ERCycleDecomp)
	{
		LogConsole::writeError("Run cycle decomposition first");
		return false;
	}

	if (m_ERCycleDecomp->getERData() != primeVis->currentERData)
	{
		LogConsole::writeError("Cycle decomposition data does not match visualizaiton data");
		return false;
	}

	primeVis->removeSelectedEntities();
	primeVis->removeSelectedRelationships();
	dualVis->removeSelectedEntities();
	dualVis->removeSelectedRelationships();

	return true;
}

void MainWindow::updateVisSelectedElements()
{
	primeVis->updateSelectionFromData();
	dualVis->updateSelectionFromData();
	bipartVis->repaint();
}

void MainWindow::highlightNextBComp()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightNextBComp();
	updateVisSelectedElements();
}

void MainWindow::highlightAllBComps()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightAllBComps();
	updateVisSelectedElements();
}

void MainWindow::highlightNextTComp()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightNextTComp();
	updateVisSelectedElements();
}

void MainWindow::highlightAllTComps()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightAllTComps();
	updateVisSelectedElements();
}

void MainWindow::highlightNextCycle()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightNextCycle();
	updateVisSelectedElements();
}

void MainWindow::highlightAllCycles()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightAllCycles();
	updateVisSelectedElements();
}

void MainWindow::highlightNextCluster()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightNextCluster();
	updateVisSelectedElements();
}

void MainWindow::highlightAllClusters()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->highlightAllClusters();
	updateVisSelectedElements();
}

void MainWindow::resetDecompHighlights()
{
	if (!decompHighlightChanged())
		return;
	m_ERCycleDecomp->resetHighlights();
	updateVisSelectedElements();
}





// Bipartite Simplification Operations

void MainWindow::startRecordingBipartOperations()
{
	if (m_BOptManager)
	{
		delete m_BOptManager;
		m_BOptManager = NULL;
	}
	m_BOptManager = new BipartOpManager(m_ERData);

	LogConsole::writeGreenConsole("Started Recording Bipartite Operations");
}

void MainWindow::applyBipartOperation(BIPART_OP_TYPE type)
{
	if (!m_BOptManager)
	{
		LogConsole::writeError("Start recording operations first.");
		return;
	}
	if (m_BOptManager->getERData() != primeVis->currentERData)
	{
		LogConsole::writeError("Vis data does not match recording data.");
		return;
	}

	ENList enlist = primeVis->getSelectedEntities(false);
	REList relist = primeVis->getSelectedRelationships(false);
	
	bool result = m_BOptManager->createOperation(enlist, relist, type);
	if (result)
		result = m_BOptManager->applyNextOperation();

	if (result)
	{
		ERDataChanged();
		updateWidgets();
	}
}

void MainWindow::saveBipartOperations()
{
	if (!m_BOptManager)
	{
		LogConsole::writeError("No bipartite operations to save.");
		return;
	}
	
	std::vector<BipartOperation*> bops = m_BOptManager->getAppliedOps();
	if (bops.empty())
	{
		LogConsole::writeError("No bipartite operations have been applied.");
		return;
	}

	QString filter = "Text files (*.txt)";
	QString fileName = QFileDialog::getSaveFileName(0, "Save Bipartite Operations", getOpenDir(), filter);
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly))
		{
			QTextStream stream(&file);
			stream << bops.size() << endl;

			auto printBipartOp = [&](BipartOperation* op)
			{
				stream << op->getType() << "," << op->getElemType();
				for (std::pair<Entity*, Relationship*> edge : op->getEdges())
				{
					stream << "," << edge.first->getOpidx() << "," << edge.second->getOpidx();
				}
				stream << endl;
			};

			for (BipartOperation* bop : bops)
				printBipartOp(bop);

			file.flush();
			file.close();
		}
	}
}

void MainWindow::loadBipartOperations()
{
	QString filter = "Text files (*.txt)";
	QString fileName = QFileDialog::getOpenFileName(0, "Load Bipartite Operations", getOpenDir(), filter);
	if (!fileName.isEmpty())
	{
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly))
		{
			startRecordingBipartOperations();

			QTextStream stream(&file);
			QStringList list = stream.readLine().split(",");
			int num_ops = list[0].toInt();
			for (int i = 0; i < num_ops; i++)
			{
				list = stream.readLine().split(",");
				m_BOptManager->addReadItem(list);
			}

			LogConsole::writeBlueConsole("%d operations loaded", num_ops);

			file.flush();
			file.close();
		}
	}
}

void MainWindow::invertLastBipartOperation()
{
	if (!m_BOptManager)
	{
		LogConsole::writeError("No bipartite operations to invert.");
		return;
	}

	BipartOperation* op = m_BOptManager->invertLastOperation();

	if (op)
	{
		ERDataChanged();
		updateWidgets();

		if (flag_optimizeInversions)
		{
			std::vector<int> enlist, relist;
			op->getElementIndices(&enlist, &relist);
			ERData* pdata = m_BOptManager->getERData();
			free_elements(pdata, enlist, relist);
			ERData* ddata = pdata->getDualERData();
			if (ddata)
				free_elements(ddata, relist, enlist);

			LogConsole::writeConsole("\n------Start Subgraph Optimization------");
			get_subgraph_settings();
			flag_stopOptimization = false;
			OptimizationHLBFGS m_solver;
			if (ddata)
				m_solver.dual_optimize_subgraph_HLBFGS(pdata, ddata);
			else
				m_solver.optimize_subgraph_HLBFGS(pdata);
			LogConsole::writeConsole("--------END--------\n");
		}

		updateWidgets();
	}
}

void MainWindow::applyNextBipartOperation()
{
	if (!m_BOptManager)
	{
		LogConsole::writeError("No bipartite operations to apply.");
		return;
	}

	bool result = m_BOptManager->applyNextOperation();

	if (result)
	{
		ERDataChanged();
		primeVis->update();
		dualVis->update();
		bipartVis->update();
	}
}


void MainWindow::applyBipartEdgeCut()
{
	applyBipartOperation(BIPART_OP_TYPE::EDGE_CUT);
}

void MainWindow::applyBipartEdgePrune()
{
	applyBipartOperation(BIPART_OP_TYPE::EDGE_PRUNE);
}

void MainWindow::applyBipartK22Collapse()
{
	applyBipartOperation(BIPART_OP_TYPE::K22_COLLAPSE);
}

void MainWindow::applyBipartChainMerge()
{
	applyBipartOperation(BIPART_OP_TYPE::CHAIN_MERGE);
}





// Decomposition-Based Simplification

bool MainWindow::checkDecompAndOpManagers()
{
	if (!m_BOptManager)
	{
		LogConsole::writeError("Start recording operations first.");
		return false;
	}
	if (!m_ERCycleDecomp)
	{
		LogConsole::writeError("Run cycle decomposition first.");
		return false;
	}
	if (m_ERCycleDecomp->getERData() != m_BOptManager->getERData())
	{
		LogConsole::writeError("Decomp data does not match recording data.");
		return false;
	}

	return true;
}

bool MainWindow::applyNextClusterOperation()
{
	ERData* erdata = m_ERCycleDecomp->getERData();
	BipartOperation* bopt = NULL;
	while (!bopt)
	{
		bopt = m_ERCycleDecomp->getNextClusterOp();
		if (m_ERCycleDecomp->m_clusterSimpIndex >= m_ERCycleDecomp->getNumClusters())
			return false;
	}

	// apply operation and update cycle adjacency
	m_BOptManager->applyOperation(bopt);
	m_ERCycleDecomp->updateCycleAdjacency();

	return true;
}

//bool MainWindow::applyNextClusterOperation()
//{
//	ERData* erdata = m_ERCycleDecomp->getERData();
//	ERForbidden* fb = m_ERCycleDecomp->getNextForbiddenSimp();
//	if (!fb)
//		return false;
//
//	// mark cycles that will be deleted
//	if (fb->getType() == FORBIDDENTYPE::NADJACENT)
//	{
//		for (ERCycle* c : fb->getCycles())
//		{
//			c->to_delete = true;
//			m_ERCycleDecomp->nullifyCycle(c);
//		}
//	}
//	else
//	{
//		ERCycle* c = fb->getCycle(0);
//		c->to_delete = true;
//		m_ERCycleDecomp->nullifyCycle(c);
//	}
//
//
//	// get elements for merge operation
//	std::vector<std::pair<Entity*, Relationship*>> edges;
//	std::vector<Element*> ens; std::vector<Element*> res;
//	ERCycle* first = fb->getCycle(0);
//	for (Element* e : first->getElements())
//	{
//		if (e->getType() == ELEMENTTYPE::ENTITY)
//			ens.push_back(e);
//		else
//			res.push_back(e);
//	}
//	edges.push_back(std::make_pair(erdata->getEntity(ens[0]->getIndex()), erdata->getRelationship(res[0]->getIndex())));
//	edges.push_back(std::make_pair(erdata->getEntity(ens[0]->getIndex()), erdata->getRelationship(res[1]->getIndex())));
//	edges.push_back(std::make_pair(erdata->getEntity(ens[1]->getIndex()), erdata->getRelationship(res[0]->getIndex())));
//	edges.push_back(std::make_pair(erdata->getEntity(ens[1]->getIndex()), erdata->getRelationship(res[1]->getIndex())));
//
//	// determine direction of merge operation
//	ELEMENTTYPE type; // this should be more sophisticated to ensure topology preservation
//	if (fb->getType() == FORBIDDENTYPE::NADJACENT)
//	{
//		int num_res = 0;
//		int num_ens = 0;
//		for (Element* e : fb->getCommonElements())
//		{
//			if (e->getType() == ELEMENTTYPE::ENTITY)
//				num_ens++;
//			else
//				num_res++;
//		}
//
//		if (num_ens > num_res)
//			type = ELEMENTTYPE::ENTITY;
//		else
//			type = ELEMENTTYPE::RELATIONSHIP;
//	}
//	else
//		type = fb->getCommonElement(0)->getType();
//
//	// create bipartite operation to apply
//	BipartOperation* bopt = new BipartOperation(edges, BIPART_OP_TYPE::K22_COLLAPSE, type);
//	if (type == ELEMENTTYPE::ENTITY)
//	{
//		// initialize replacement element
//		Vec3 loc = (edges[0].first->getLocation() + edges[3].first->getLocation()) / 2.0;
//		Entity* en = new Entity(loc);
//		en->setLabel(edges[0].first->getLabel() + edges[3].first->getLabel());
//		en->setOpidx(edges[0].first->getOpidx());
//		bopt->setEnResult(en);
//		Element* result = bopt->getResult();
//		m_ERCycleDecomp->replaceElements(ens, result);
//	}
//	else if (type == ELEMENTTYPE::RELATIONSHIP)
//	{
//		// initialize replacement element
//		Relationship* re = new Relationship(edges[0].second->getIncidentEntities(), false);
//		re->setLabel(edges[0].second->getLabel() + edges[3].second->getLabel());
//		re->setOpidx(edges[0].second->getOpidx());
//		bopt->setReResult(re);
//		Element* result = bopt->getResult();
//		m_ERCycleDecomp->replaceElements(res, result);
//	}
//
//	// apply operation and update cycle adjacency
//	m_BOptManager->applyOperation(bopt);
//	m_ERCycleDecomp->updateCycleAdjacency();
//
//	return true;
//}

void MainWindow::applyClusterSimplification()
{
	if (!checkDecompAndOpManagers())
		return;

	bool result = applyNextClusterOperation();
	if (!result)
		LogConsole::writeError("All clusters are already simplified.");

	ERDataChanged();
	updateWidgets();
}

void MainWindow::simplifyAllClusters()
{
	if (!checkDecompAndOpManagers())
		return;

	clock_t time1 = clock();

	bool result = true;
	int count = 0;
	while (result)
	{
		count++;
		result = applyNextClusterOperation();
	}
	
	clock_t time2 = clock();
		
	LogConsole::writeConsole("All clusters simplified");
	LogConsole::writeConsole("Ops applied: %d", count - 1);
	double execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
	LogConsole::writeConsole("Cluster simplification time: %.4f", execution_time);

	ERDataChanged();
	updateWidgets();
}

void MainWindow::applyNextBicompOperation()
{
	if (!checkDecompAndOpManagers())
		return;

	std::pair<Entity*, Relationship*> cut = m_ERCycleDecomp->getNextCutEdge();
	if (!cut.first)
	{
		LogConsole::writeConsole("No more edge cuts to apply.");
		return;
	}

	std::vector<std::pair<Entity*, Relationship*>> edges{cut};
	BipartOperation* bopt = new BipartOperation(edges, BIPART_OP_TYPE::EDGE_CUT, ELEMENTTYPE::ENTITY);
	m_BOptManager->applyOperation(bopt);
	
	ERDataChanged();
	updateWidgets();
}

void MainWindow::simplifyAllBicomps()
{
	if (!checkDecompAndOpManagers())
		return;

	clock_t time1 = clock();

	int count = 0;
	while (true)
	{
		count++;

		std::pair<Entity*, Relationship*> cut = m_ERCycleDecomp->getNextCutEdge();
		if (!cut.first)
			break;
		
		std::vector<std::pair<Entity*, Relationship*>> edges{cut};
		BipartOperation* bopt = new BipartOperation(edges, BIPART_OP_TYPE::EDGE_CUT, ELEMENTTYPE::ENTITY);
		m_BOptManager->applyOperation(bopt);
	}

	clock_t time2 = clock();

	LogConsole::writeConsole("%d Edges cut", count-1);
	double execution_time = (double)(time2 - time1) / CLOCKS_PER_SEC;
	LogConsole::writeConsole("Edge cut time: %.4f", execution_time);

	ERDataChanged();
	updateWidgets();
}

void MainWindow::applyNextTreeOperation()
{

}

void MainWindow::simplifyAllTrees()
{

}





// Debugging

void MainWindow::writeOrderedIncidenceMatrix()
{
	if (!hasFocusedVis())
		return;
	ERData* focused_ERData = focusedVis->currentERData;
	
	double share_power = 2.0;
	focused_ERData->constructIncidenceMatrix();
	/*focused_ERData->constructReShareMatrix(share_power);

	vector<int> resorted, ensorted;
	focused_ERData->getSortedIncidenceMatrix(&resorted, &ensorted);
	focused_ERData->constructReClusterMatrix(resorted);
	
	const int encount = ensorted.size();
	const int recount = resorted.size();

	Eigen::MatrixXi oim(recount, encount);
	Eigen::MatrixXi hsm(recount, recount);
	oim.setZero();
	hsm.setZero();

	int re_idx, en_idx, incident;
	for (int i = 0; i < recount; i++)
	{
		re_idx = resorted.at(i);
		for (int j = 0; j < encount; j++)
		{
			en_idx = ensorted.at(j);
			incident = focused_ERData->m_IncidenceMatrix.coeff(re_idx, en_idx);
			if (incident == 1)
			{
				oim(i, j) = 1;
			}
		}
	}*/

	QString fileName = QFileDialog::getSaveFileName(0, "Save Incidence Data");
	if (fileName.isEmpty())
	{
		LogConsole::writeError("Unable to save incidence data");
		return;
	}
	else
	{
		std::string fname = fileName.toStdString();
		std::ofstream file(fname+".txt");
		if (file.is_open())
		{
			file << std::endl << focused_ERData->m_IncidenceMatrix << std::endl << std::endl;
			file.close();
		}
	}
}

void MainWindow::writeAdjacencyMatrices()
{
	ERData* erdata = focusedVis->currentERData;
	erdata->constructEnAdjMatrix();
	erdata->constructReAdjMatrix();

	QString fileName = QFileDialog::getSaveFileName(0, "Save Simplification Data");

	if (fileName.isEmpty())
	{
		LogConsole::writeError("Unable to save simplification data");
		return;
	}
	else
	{
		std::string fname = fileName.toStdString();
		std::ofstream file(fname + "_entity.txt");
		if (file.is_open())
		{
			file << erdata->m_enAdjMatrix;
		}
		file.close();

		std::ofstream file2(fname + "_relationship.txt");
		if (file2.is_open())
		{
			file2 << erdata->m_reAdjMatrix;
		}
		file2.close();
	}
}

void MainWindow::writeERDataStatistics(std::string outfile)
{
	ERData* erdata = focusedVis->currentERData;
	int nverts = erdata->getEntityNum();
	int nedges = erdata->getRelationshipNum();
	Eigen::VectorXd evals = erdata->getIncidenceEigenvals();

	int* entityIncidence = new int[nverts + 2];
	int* entityAdjacency = new int[nverts + 2];
	int* entityAdjacencyFactor = new int[nverts + 2];
	float* entityBetweennessCentrality = new float[nverts + 2];
	float* entityEccentricity = new float[nverts + 2];
	float* entityClosenessCentrality = new float[nverts + 2];

	int* relationshipIncidence = new int[nedges + 2];
	int* relationshipAdjacency = new int[nedges + 2];
	int* relationshipAdjacencyFactor = new int[nedges + 2];
	float* relationshipBetweennessCentrality = new float[nedges + 2];
	float* relationshipEccentricity = new float[nedges + 2];
	float* relationshipClosenessCentrality = new float[nedges + 2];

	int** incidenceMatrix = new int* [nedges];
	for (int i = 0; i < nedges; i++)
		incidenceMatrix[i] = new int[nverts];

	int** entityAdjacencyMatrix = new int* [nverts];
	for (int i = 0; i < nverts; i++)
		entityAdjacencyMatrix[i] = new int[nverts];

	int** relationshipAdjacencyMatrix = new int* [nedges];
	for (int i = 0; i < nedges; i++)
		relationshipAdjacencyMatrix[i] = new int[nedges];

	erdata->calcStatistics
	(
		entityIncidence,
		relationshipIncidence,
		entityAdjacency,
		relationshipAdjacency,
		entityAdjacencyFactor,
		relationshipAdjacencyFactor
	);
	erdata->calcCentralities
	(
		entityBetweennessCentrality,
		entityEccentricity,
		entityClosenessCentrality,
		relationshipBetweennessCentrality,
		relationshipEccentricity,
		relationshipClosenessCentrality
	);

	// print statistics to a files
	if (outfile.empty())
	{
		outfile = m_FILENAME.substr(0,m_FILENAME.find_last_of('.'));
		outfile = "./output/statistics" + outfile;
	}

	std::string fname = outfile + "_entity_stats.csv";
	std::ofstream file(fname);
	if (file.is_open())
	{
		file << "X,Y,IN,AD,AF,BC,EC,CC,EI" << std::endl;
		for (int i = 0; i < nverts; i++)
		{
			Vec3 loc = erdata->getEntity(i)->getLocation();
			file << loc.x << ",";
			file << loc.y << ",";
			file << entityIncidence[i] << ",";
			file << entityAdjacency[i] << ",";
			file << entityAdjacencyFactor[i] << ",";
			file << entityBetweennessCentrality[i] << ",";
			file << entityEccentricity[i] << ",";
			file << entityClosenessCentrality[i] << ",";
			if (i < evals.size())
				file << evals(i) << std::endl;
			else
				file << "0" << std::endl;
		}
	}
	file.close();

	fname = outfile + "_relationship_stats.csv";
	file.open(fname);
	if (file.is_open())
	{
		file << "IN,AD,AF,BC,EC,CC" << std::endl;
		for (int i = 0; i < nedges; i++)
		{
			file << relationshipIncidence[i] << ",";
			file << relationshipAdjacency[i] << ",";
			file << relationshipAdjacencyFactor[i] << ",";
			file << relationshipBetweennessCentrality[i] << ",";
			file << relationshipEccentricity[i] << ",";
			file << relationshipClosenessCentrality[i] << std::endl;
		}
	}
	file.close();
}

void MainWindow::on_actionPrint_Euler_Characteristics_triggered()
{
	if (!hasFocusedVis())
		return;
	ERData* primer = primeVis->currentERData;
	ERData* dualer = dualVis->currentERData;
	if (primer)
		primer->writeEulerCharacteristics("prime");
	if (dualer)
		dualer->writeEulerCharacteristics("dual");
}

void MainWindow::save_console(std::string fname)
{
	QString qfname = QString::fromStdString(fname);
	ui->dockWidgetContents_Console->saveConsole(qfname);
}