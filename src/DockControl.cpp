#include "DockControl.h"
#include "oglwidget.h"
#include "mainwindow.h"
#include "LogConsole.h"
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QStyledItemDelegate>
#include <QComboBox>
#include "EnergyEvaluation.h"

class NoEditDelegate : public QStyledItemDelegate 
{
public:

	explicit NoEditDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {}

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		return 0;
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		painter->save();
		painter->setPen(QColor(200, 200, 200, 255));
		painter->drawRect(option.rect);
		painter->restore();

		QStyledItemDelegate::paint(painter, option, index);
	}
};

class FloatEditDelegate : public QStyledItemDelegate
{
public:

	explicit FloatEditDelegate(QObject* parent = 0) : QStyledItemDelegate(parent) {};
	
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QLineEdit* lineEdit = new QLineEdit(parent);

		QDoubleValidator* validator = new QDoubleValidator(-1000000.0, 1000000.0, 6, lineEdit);
		lineEdit->setValidator(validator);

		return lineEdit;
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		painter->save();
		painter->setPen(QColor(200, 200, 200, 255));
		painter->drawRect(option.rect);
		painter->restore();

		QStyledItemDelegate::paint(painter, option, index);
	}
};

class ComboBoxDelegate : public QStyledItemDelegate
{
public:

	explicit ComboBoxDelegate(QStringList items, QObject* parent = 0) : QStyledItemDelegate(parent)
	{
		m_items = items;
	};

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		if (index.column() == 0)
			return 0;

		QComboBox* editor = new QComboBox(parent);
		editor->addItems(m_items);
		return editor;
	}

	void setEditorData(QWidget* editor, const QModelIndex& index) const
	{
		if (index.column() == 0)
			return;

		QComboBox* box = static_cast<QComboBox*>(editor);
		int value = index.model()->data(index, Qt::EditRole).toUInt();
		box->setCurrentIndex(value);
	}

	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
	{
		if (index.column() == 0)
			return;

		QComboBox* box = static_cast<QComboBox*>(editor);
		model->setData(index, box->currentIndex(), Qt::EditRole);
	}

	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		if (index.column() == 0)
			return;

		editor->setGeometry(option.rect);
	}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{		
		painter->save();
		painter->setPen(QColor(200, 200, 200, 255));
		painter->drawRect(option.rect);
		painter->restore();

		if (index.column() == 0)
			QStyledItemDelegate::paint(painter, option, index);
		else
		{
			QStyleOptionViewItem style = option;
			QString text = m_items[0];
			style.text = text;
			QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &style, painter);
		}
	}

private:

	QStringList m_items;

};

DockControl::DockControl(OGLWidget *vis1, OGLWidget *vis2)
	: QWidget(NULL), ui(new Ui::DockControl), primeVis(vis1), dualVis(vis2)
{
	ui->setupUi(this);
	Element::initPolyFontColors(Predefined_1_PolyFontColor);
	Element::initPolyColors(ColorBrewer2);
	initializeColorSchemeComboBox();
	ui->comboBox_ColorScheme->setCurrentIndex(5);
	syncUIs();

	ui->treeWidget_TransformProperties->expandAll();
	ui->treeWidget_RandomProperties->expandAll();
	ui->treeWidget_GridProperties->expandAll();
	ui->treeWidget_CircleProperties->expandAll();
	ui->treeWidget_MonogonProperties->expandAll();
	ui->treeWidget_FDProperties->expandAll();
	ui->treeWidget_OptimizationProperties->expandAll();
	ui->treeWidget_EnergyProperties->expandAll();
	ui->treeWidget_KonigProperties->expandAll();
	
	ui->treeWidget_DefaultProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_TransformProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_RandomProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_GridProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_CircleProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_MonogonProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_FDProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_OptimizationProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_EnergyProperties->setItemDelegateForColumn(1, new FloatEditDelegate);
	ui->treeWidget_KonigProperties->setItemDelegateForColumn(1, new FloatEditDelegate);

	ui->treeWidget_DefaultProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_TransformProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_RandomProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_GridProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_CircleProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_MonogonProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_FDProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_OptimizationProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_EnergyProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	ui->treeWidget_KonigProperties->setItemDelegateForColumn(0, new NoEditDelegate);
	
	ComboBoxDelegate* comboBox_CircleSort = new ComboBoxDelegate({ "Index","Incidence" }, ui->treeWidget_CircleProperties);
	ui->treeWidget_CircleProperties->setItemDelegateForRow(1, comboBox_CircleSort);
	QTreeWidgetItemIterator it(ui->treeWidget_CircleProperties); it += 2;
	ui->treeWidget_CircleProperties->openPersistentEditor(*it, 1);
	
	ui->comboBox_LayoutSelector->setCurrentIndex(0);
	ui->stackedWidget_LayoutPages->setCurrentIndex(0);
    
	// connect the ui with slots in mainwindow
	connect(ui->pushButton_CalcDegreeAndCardinality, &QPushButton::clicked, gmainwindow, &MainWindow::calcElementStatistics);
	connect(ui->pushButton_FixSelectedVerts, &QPushButton::clicked, gmainwindow, &MainWindow::fixSelectedVerts);
	connect(ui->pushButton_ClearFixedVert, &QPushButton::clicked, gmainwindow, &MainWindow::clearFixedVert);
	connect(ui->pushButton_ApplyEnColorScheme, &QPushButton::clicked, gmainwindow, &MainWindow::applyEnColorScheme);
	connect(ui->pushButton_ApplyReColorScheme, &QPushButton::clicked, gmainwindow, &MainWindow::applyReColorScheme);
	connect(ui->pushButton_SyncDualElements, &QPushButton::clicked, gmainwindow, &MainWindow::syncDualElementColor);
	connect(ui->pushButton_SyncViewZooming, &QPushButton::clicked, gmainwindow, &MainWindow::syncDualZooming);
	connect(ui->pushButton_SetZoom, &QPushButton::clicked, gmainwindow, &MainWindow::setZoom);

	connect(ui->pushButton_RecenterView, &QPushButton::clicked, gmainwindow, &MainWindow::recenterView);
	
    connect(ui->checkBox_ShowEntities, &QCheckBox::clicked, gmainwindow, &MainWindow::set_ShowEntities);
	connect(ui->checkBox_ShowRelationships, &QCheckBox::clicked, gmainwindow, &MainWindow::set_ShowRelationships);
	connect(ui->comboBox_entityLabelContent, SIGNAL(currentIndexChanged(int)), gmainwindow, SLOT(set_EntityLabels(int)));
	connect(ui->comboBox_relationshipLabelContent, SIGNAL(currentIndexChanged(int)), gmainwindow, SLOT(set_RelationshipLabels(int)));
	connect(ui->checkBox_EntityDegreeFilter, &QCheckBox::clicked, gmainwindow, &MainWindow::set_EntityDegreeFilter);
	connect(ui->checkBox_RelationshipDegreeFilter, &QCheckBox::clicked, gmainwindow, &MainWindow::set_RelationshipDegreeFilter);
	connect(ui->checkBox_EntityCardinalityFilter, &QCheckBox::clicked, gmainwindow, &MainWindow::set_EntityCardinalityFilter);
	connect(ui->checkBox_polygonCardinalityFilter, &QCheckBox::clicked, gmainwindow, &MainWindow::set_polygonCardinalityFilter);
	connect(ui->checkBox_EntityIndexFilter, &QCheckBox::clicked, gmainwindow, &MainWindow::set_EntityIndexFilter);
	connect(ui->checkBox_RelationshipIndexFilter, &QCheckBox::clicked, gmainwindow, &MainWindow::set_RelationshipIndexFilter);
    
	connect(ui->radioButton_PrimeVisWidget, &QRadioButton::toggled, gmainwindow, &MainWindow::set_PrimeVisWidget_focused);
	connect(ui->radioButton_DualVisWidget, &QRadioButton::toggled, gmainwindow, &MainWindow::set_DualVisWidget_focused);
	connect(ui->radioButton_BipartVisWidget, &QRadioButton::toggled, gmainwindow, &MainWindow::set_BipartVisWidget_focused);

	connect(ui->spinBox_VertexMinDegree, SIGNAL(valueChanged(int)), gmainwindow, SLOT(vertexMinDegreeChanged(int)));
	connect(ui->spinBox_VertexMaxDegree, SIGNAL(valueChanged(int)), gmainwindow, SLOT(vertexMaxDegreeChanged(int)));
	connect(ui->spinBox_PolyMinDegree, SIGNAL(valueChanged(int)), gmainwindow, SLOT(polyMinDegreeChanged(int)));
	connect(ui->spinBox_PolyMaxDegree, SIGNAL(valueChanged(int)), gmainwindow, SLOT(polyMaxDegreeChanged(int)));
	connect(ui->spinBox_RelationshipCardinalityBottom, SIGNAL(valueChanged(int)), gmainwindow, SLOT(relationshipCardinalityBottomChanged(int)));
	connect(ui->spinBox_RelationshipCardinalityTop, SIGNAL(valueChanged(int)), gmainwindow, SLOT(relationshipCardinalityTopChanged(int)));
	connect(ui->spinBox_EntityCardinalityBottom, SIGNAL(valueChanged(int)), gmainwindow, SLOT(entityCardinalityBottomChanged(int)));
	connect(ui->spinBox_EntityCardinalityTop, SIGNAL(valueChanged(int)), gmainwindow, SLOT(entityCardinalityTopChanged(int)));
	connect(ui->spinBox_EntityIndexBottom, SIGNAL(valueChanged(int)), gmainwindow, SLOT(entityIndexBottomChanged(int)));
	connect(ui->spinBox_EntityIndexTop, SIGNAL(valueChanged(int)), gmainwindow, SLOT(entityIndexTopChanged(int)));
	connect(ui->spinBox_RelationshipIndexBottom, SIGNAL(valueChanged(int)), gmainwindow, SLOT(relationshipIndexTopChanged(int)));
	connect(ui->spinBox_RelationshipIndexTop, SIGNAL(valueChanged(int)), gmainwindow, SLOT(relationshipIndexTopChanged(int)));

	connect(ui->doubleSpinBox_VertexSize, SIGNAL(valueChanged(double)), gmainwindow, SLOT(doubleSpinBox_VertexSize_valueChanged(double)));
	connect(ui->doubleSpinBox_LensAngle, SIGNAL(valueChanged(double)), gmainwindow, SLOT(doubleSpinBox_LensAngle_valueChanged(double)));
	connect(ui->doubleSpinBox_MonogonAspectRatio, SIGNAL(valueChanged(double)), gmainwindow, SLOT(doubleSpinBox_MonogonAspectRatio_valueChanged(double)));
	connect(ui->doubleSpinBox_MonogonSize, SIGNAL(valueChanged(double)), gmainwindow, SLOT(doubleSpinBox_MonogonSize_valueChanged(double)));

	connect(ui->pushButton_GenerateAtomicOperations, &QPushButton::clicked, gmainwindow, &MainWindow::generateAtomicOperations);
	connect(ui->pushButton_OptimizeFootprint, &QPushButton::clicked, gmainwindow, &MainWindow::optimizeSubgraphManually);
	connect(ui->pushButton_ApplyOperation, &QPushButton::clicked, gmainwindow, &MainWindow::applyNextOperation);
	connect(ui->pushButton_InvertOperation, &QPushButton::clicked, gmainwindow, &MainWindow::invertLastOperation);
	connect(ui->pushButton_ApplyAllOperations, &QPushButton::clicked, gmainwindow, &MainWindow::applyAllOperations);
	connect(ui->pushButton_InvertAllOperations, &QPushButton::clicked, gmainwindow, &MainWindow::invertAndOptimize);
	connect(ui->checkBox_optimizeInversions, &QCheckBox::clicked, gmainwindow, &MainWindow::optimizeInversionsChanged);
	connect(ui->checkBox_PreserveConnections, &QCheckBox::clicked, gmainwindow, &MainWindow::preserveConnectionsChanged);
	connect(ui->checkBox_PreserveBranches, &QCheckBox::clicked, gmainwindow, &MainWindow::preserveBranchesChanged);
	connect(ui->checkBox_PruneOnly, &QCheckBox::clicked, gmainwindow, &MainWindow::pruneOnlyChanged);

	connect(ui->pushButton_Apply, &QPushButton::clicked, gmainwindow, &MainWindow::apply_layout);
	connect(ui->pushButton_Stop, &QPushButton::clicked, gmainwindow, &MainWindow::stopOptimization);
	
	connect(ui->pushButton_CycleDecompose, &QPushButton::clicked, gmainwindow, &MainWindow::applyCycleDecomposition);
	
	connect(ui->pushButton_HighlightNextBiComp, &QPushButton::clicked, gmainwindow, &MainWindow::highlightNextBComp);
	connect(ui->pushButton_HighlightAllBiComps, &QPushButton::clicked, gmainwindow, &MainWindow::highlightAllBComps);
	connect(ui->pushButton_HighlightNextTree, &QPushButton::clicked, gmainwindow, &MainWindow::highlightNextTComp);
	connect(ui->pushButton_HighlightAllTrees, &QPushButton::clicked, gmainwindow, &MainWindow::highlightAllTComps);
	connect(ui->pushButton_HighlightNextCycle, &QPushButton::clicked, gmainwindow, &MainWindow::highlightNextCycle);
	connect(ui->pushButton_HighlightAllCycles, &QPushButton::clicked, gmainwindow, &MainWindow::highlightAllCycles);
	connect(ui->pushButton_HighlightNextCluster, &QPushButton::clicked, gmainwindow, &MainWindow::highlightNextCluster);
	connect(ui->pushButton_HighlightAllClusters, &QPushButton::clicked, gmainwindow, &MainWindow::highlightAllClusters);
	connect(ui->pushButton_ResetHighlights, &QPushButton::clicked, gmainwindow, &MainWindow::resetDecompHighlights);

	connect(ui->pushButton_ApplyNextClusterOp, &QPushButton::clicked, gmainwindow, &MainWindow::applyClusterSimplification);
	connect(ui->pushButton_SimpAllClusters, &QPushButton::clicked, gmainwindow, &MainWindow::simplifyAllClusters);
	connect(ui->pushButton_ApplyNextBicompOp, &QPushButton::clicked, gmainwindow, &MainWindow::applyNextBicompOperation);
	connect(ui->pushButton_SimpAllBicomps, &QPushButton::clicked, gmainwindow, &MainWindow::simplifyAllBicomps);
	connect(ui->pushButton_ApplyNextTreeOp, &QPushButton::clicked, gmainwindow, &MainWindow::applyNextTreeOperation);
	connect(ui->pushButton_SimpAllTrees, &QPushButton::clicked, gmainwindow, &MainWindow::simplifyAllTrees);
	
	connect(ui->pushButton_RecordBipartOps, &QPushButton::clicked, gmainwindow, &MainWindow::startRecordingBipartOperations);
	connect(ui->pushButton_SaveBipartOps, &QPushButton::clicked, gmainwindow, &MainWindow::saveBipartOperations);
	connect(ui->pushButton_ApplyNextBipartOp, &QPushButton::clicked, gmainwindow, &MainWindow::applyNextBipartOperation);
	connect(ui->pushButton_InvertLastBipartOp, &QPushButton::clicked, gmainwindow, &MainWindow::invertLastBipartOperation);
	connect(ui->pushButton_LoadBipartOps, &QPushButton::clicked, gmainwindow, &MainWindow::loadBipartOperations);
}

DockControl::~DockControl()
{

}

void DockControl::resizeEvent(QResizeEvent * event)
{
}

void DockControl::initializeColorSchemeComboBox()
{
	/*Set up color scheme combo*/
	double iconwidth = 100;
	double iconheight = 15;

	//#0
	{
		QPixmap colorIcon(iconwidth, iconheight);
		QPainter p(&colorIcon);

		QLinearGradient gradient1 = QLinearGradient(QPointF(0, 0), QPointF(iconwidth, 0));
		gradient1.setColorAt(0, Qt::blue);
		gradient1.setColorAt(0.25, Qt::cyan);
		gradient1.setColorAt(0.5, Qt::green);
		gradient1.setColorAt(0.75, Qt::yellow);
		gradient1.setColorAt(1, Qt::red);
		p.fillRect(colorIcon.rect(), gradient1);

		ui->comboBox_ColorScheme->setIconSize(QSize(iconwidth, iconheight));
		ui->comboBox_ColorScheme->addItem(colorIcon, QString("Rainbow"));
	}

	//#1
	{
		QPixmap colorIcon(iconwidth, iconheight);
		QPainter p(&colorIcon);

		QLinearGradient gradient1 = QLinearGradient(QPointF(0, 0), QPointF(iconwidth, 0));
		gradient1.setColorAt(0, Qt::blue);
		gradient1.setColorAt(1, Qt::red);
		p.fillRect(colorIcon.rect(), gradient1);

		ui->comboBox_ColorScheme->setIconSize(QSize(iconwidth, iconheight));
		ui->comboBox_ColorScheme->addItem(colorIcon, QString("Red Blue"));
	}

	//#2
	{
		QPixmap colorIcon(iconwidth, iconheight);
		QPainter p(&colorIcon);

		QLinearGradient gradient1 = QLinearGradient(QPointF(0, 0), QPointF(iconwidth, 0));
		gradient1.setColorAt(0, Qt::blue);
		gradient1.setColorAt(0.5, Qt::white);
		gradient1.setColorAt(1, Qt::red);
		p.fillRect(colorIcon.rect(), gradient1);

		ui->comboBox_ColorScheme->setIconSize(QSize(iconwidth, iconheight));
		ui->comboBox_ColorScheme->addItem(colorIcon, QString("Red White Blue"));
	}

	auto addItem = [iconwidth, iconheight](const Vec4 *colorlist, QComboBox *combo, QString name) {
		QPixmap colorIcon(iconwidth, iconheight);
		colorIcon.fill(Qt::white);
		QPainter p(&colorIcon);

		double w = iconwidth / COLORCOUNT;
		for (int i = 0; i < COLORCOUNT; i++) {
			QColor color(colorlist[i].R, colorlist[i].G, colorlist[i].B, colorlist[i].Alpha);
			if (i == COLORCOUNT - 1)
			{
				p.fillRect(0 + i*w, 0, iconwidth - (i - 1)*w, iconheight, color);
			}
			else
				p.fillRect(0 + i*w, 0, w, iconheight, color);
		}

		combo->setIconSize(QSize(iconwidth, iconheight));
		combo->addItem(colorIcon, name);
	};

	//#3
	addItem(TolColor_Bright, ui->comboBox_ColorScheme, "TolColor Bright");

	//#4
	addItem(TolColor_Vibrant, ui->comboBox_ColorScheme, "TolColor Vibrant");

	//#5
	addItem(TolColor_Muted, ui->comboBox_ColorScheme, "TolColor Muted");

	//#6
	addItem(TolColor_Light, ui->comboBox_ColorScheme, "TolColor Light");

	//#7
	addItem(Predefined_1_PolyColor, ui->comboBox_ColorScheme, "ColorBrewer0");

	//#8
	addItem(ColorBrewer1, ui->comboBox_ColorScheme, "ColorBrewer1");

	//#9
	addItem(ColorBrewer2, ui->comboBox_ColorScheme, "ColorBrewer2");

	//#10
	{
		QPixmap colorIcon(iconwidth, iconheight);
		colorIcon.fill(Qt::white);
		QPainter p(&colorIcon);

		double w = iconwidth / COLORCOUNT;
		for (int i = 0; i<COLORCOUNT; i++) {
			QColor color;
			int hue = (360.0 / (COLORCOUNT - 1))*i;
			int s = 255;
			int l = 0.5*255;
			color.setHsl(hue, s, l);
			if (i == COLORCOUNT - 1)
			{
				p.fillRect(0 + i*w, 0, iconwidth - (i-1)*w, iconheight, color);
			}
			else
				p.fillRect(0 + i*w, 0, w, iconheight, color);
		}

		ui->comboBox_ColorScheme->setIconSize(QSize(iconwidth, iconheight));
		ui->comboBox_ColorScheme->addItem(colorIcon, QString("Equvalent Hue"));
	}

	auto addItems = [iconwidth, iconheight](int count, const Vec4 *colorlist, QComboBox *combo, QString name){
	
		for (int ci = 0; ci < count; ci++) {
			QPixmap colorIcon(iconwidth, iconheight);
			colorIcon.fill(Qt::white);
			QPainter p(&colorIcon);

			double w = iconwidth / COLORCOUNT;
			for (int i = 0; i<COLORCOUNT; i++) {
				//QColor color(colorlist[ci][i].R, colorlist[ci][i].G, colorlist[ci][i].B, colorlist[ci][i].Alpha);
				Vec4 c = *((colorlist + ci*COLORCOUNT) + i);
				QColor color(c.R, c.G, c.B, c.Alpha);

				if (i == COLORCOUNT - 1)
				{
					p.fillRect(0 + i*w, 0, iconwidth - (i - 1)*w, iconheight, color);
				}
				else
					p.fillRect(0 + i*w, 0, w, iconheight, color);
			}

			combo->setIconSize(QSize(iconwidth, iconheight));
			QString str = QString("%1 %2").arg(name).arg(ci);
			combo->addItem(colorIcon, str);
		}
	};

	//#11 - #22
	addItems(12, &ColorBrewer_sequential_multihue[0][0], ui->comboBox_ColorScheme, "ColorBrewer Sequential Multihue");

	//#23 - #28
	addItems(6, &ColorBrewer_sequential_singleHue[0][0], ui->comboBox_ColorScheme, "ColorBrewer Sequential Singlehue");

	//#29 - #37
	addItems(9, &ColorBrewer_diverging[0][0], ui->comboBox_ColorScheme, "ColorBrewer Diverging");

	for (int i = 0; i < ui->comboBox_ColorScheme->count(); i++) {
		QString name = ui->comboBox_ColorScheme->itemText(i);
		ui->comboBox_ColorScheme->setItemData(i, name, Qt::ToolTipRole);
	}
}

//void DockControl::polygonSeparationTypeChanged(int i)
//{
//	flag_PSType = (PSTYPE)i;
//	LogConsole::writeGreenConsole("PS Type %s", PSTypeNames[flag_PSType].c_str());
//}

void DockControl::digonWeightChanged(double val)
{
	gmainwindow->weight_DL = val;
	//LogConsole::writeGreenConsole("Digon weights:%f", gmainwindow->weight_DL);
}

void DockControl::digonScaleChanged(double val)
{
	gmainwindow->AD_Digon_Len_scale = val;
	//LogConsole::writeGreenConsole("Digon length scale:%f", gmainwindow->AD_Digon_Len_scale);
}

void DockControl::on_spinBox_MaximumPolygonAreaVerticesCount_valueChanged(int arg1)
{
	gmainwindow->PA_max_card = arg1;
	//LogConsole::writeGreenConsole("Max area card for cutoff:%f", gmainwindow->PA_max_card);
}

void DockControl::on_doubleSpinBox_PS_EXPECT_GAMMA_valueChanged(double val)
{
	gmainwindow->PS_angle_buffer = val;
	//LogConsole::writeGreenConsole("PS expected gamma scale:%f", gmainwindow->PS_angle_buffer);
}

void DockControl::on_doubleSpinBox_CircleRadiusRatio_valueChanged(double arg)
{
	gmainwindow->RD_rad_max = arg;
	//LogConsole::writeGreenConsole("Circle Ratio:%f", gmainwindow->CircleRatio);
}

void DockControl::on_doubleSpinBox_ExpectedLength_valueChanged(double arg1)
{
	gmainwindow->expectedLength = arg1;
}

void DockControl::on_doubleSpinBox_OCBufferScale_valueChanged(double arg1)
{
	gmainwindow->PS_distance_buffer = arg1;
	//LogConsole::writeGreenConsole("OC Buffer scale: %f", gmainwindow->PS_distance_buffer);
}

void DockControl::nsBufferScaleChanged(double arg1)
{
	gmainwindow->VertexSepBuffer = arg1;
	LogConsole::writeGreenConsole("Vertex Separation Buffer: %f", gmainwindow->VertexSepBuffer);
}

void DockControl::multipleDigonAngleOffsetChanged(double arg1)
{
	Element::m_MultipleDigonOffset = arg1;

	primeVis->currentERData->update2D();
	dualVis->currentERData->update2D();

	gmainwindow->updateWidgets();
}

void DockControl::HLBFGSLinesearchITERMAXChanged(int val)
{
	gmainwindow->HLBFGS_linesearch_IterMax = val;
	LogConsole::writeGreenConsole("HLBFG linesearch IterMax:%d", gmainwindow->HLBFGS_linesearch_IterMax);
}

void DockControl::HLBFGSUpdateNumChanged(int val)
{
	gmainwindow->HLBFGS_UpdateNum = val;
}

void DockControl::energyDivisionThresholdChanged(double val)
{
	gmainwindow->endiv_threshold = val;
	LogConsole::writeGreenConsole("Energy Division Threshold: %.4e", gmainwindow->endiv_threshold);
}

void DockControl::toggleOptimizationButtons(bool res)
{
	ui->pushButton_Apply->setEnabled(res);
}

/*sync UI with associated values*/
void DockControl::syncUIs()
{
	ui->doubleSpinBox_VertexSize->setValue(Element::m_VertexSize);
	ui->doubleSpinBox_LensAngle->setValue(Element::m_DigonLensAngle);
	ui->doubleSpinBox_MonogonAspectRatio->setValue(Element::m_MonogonLength);
	ui->doubleSpinBox_MonogonSize->setValue(Element::m_MonogonSize);
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

// Layout Tab
void DockControl::on_comboBox_LayoutSelector_currentIndexChanged(int index)
{
	ui->stackedWidget_LayoutPages->setCurrentIndex(index);
	ui->treeWidget_OptimizationProperties->expandAll();
}