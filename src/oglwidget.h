#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <memory>
#include <QWidget>
#include <QMouseEvent>
#include <QDropEvent>
#include <QWheelEvent>
#include <QGuiApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QPainter>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QTime>
#include "globalVariables.h"
#include "JPGLError.h"
#include "JPOGLWrite.h"
#include "JPMath/JPMath.h"
#include "OGLFuncs.h"
#include "Entity.h"
#include "Relationship.h"
#include "JPMath/Vector3.h"
#include "JPMath/Vector4.h"
#include "tmatrix.h"
#include "DockSelectElement.h"
#include "ColorGradient.h"
#include "Pipe.h"
#include "ERClusters.h"

class ERData;

struct OGLWidgetCreateFunctions
{
	std::unique_ptr<QOpenGLFunctions_2_1> OGLFunctions;

	~OGLWidgetCreateFunctions()
	{
		gl = NULL;
	}
};

struct OGLLight
{
	bool enabled;
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat position[4];
};

class OGLWidget : public QOpenGLWidget, private OGLWidgetCreateFunctions
{
	Q_OBJECT

public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();
	
	ERData *currentERData = NULL;
	VISTYPE widgetType;
	VIEWSTYLE viewStyle = VIEWSTYLE::POLYGON;
	DockSelectElement *infoDock = NULL;



	//////////////// flags ////////////////

	// Visualization
	bool flag_showEntities = true;
	bool flag_showRelationships = true;

	bool flag_showEntityTips = false;
	bool flag_showRelationshipTips = false;
	bool flag_showRelationshipDecal = false;
	bool flag_inheritVisibility = false;
	bool flag_ShowLight = false;

	bool flag_drawPolygonObserver = false;
	bool flag_drawDigonAsLine = false;
	bool flag_hideMonogon = false;
	bool flag_renderSelectedPolysLast = true;
	
	bool flag_drawClusters = true;
	bool flag_drawClusterBarcodes = true;
	bool flag_drawClusterEntityBubbles = true;
	bool flag_drawClusterSuperPolygons = true;
	bool flag_drawClusterCircleLayout = true;
	bool flag_alignClusterBarcodeWithEntities = true;
	bool flag_hideClusterInteralEntities = false;

	//filter
	bool flag_layerFilter = false;
    bool flag_attributeFilter = false;
	bool flag_entityDegreeFilter = false;
	bool flag_entityCardinalityFilter = false;
	bool flag_relationshipDegreeFilter = false;
	bool flag_relationshipCardinalityFilter = false;
    bool flag_relationshipIndexFilter = false;
    bool flag_entityIndexFilter = false;
	


	//////////////// Settings ////////////////
	
	// Filter
	int FilterEntityAttributeIndex = 1;
	int FilterEntityDegreeBottom;
	int FilterEntityDegreeTop;
	int FilterRelationshipDegreeBottom;
	int FilterRelationshipDegreeTop;
	int FilterEntityCardBottom;
	int FilterEntityCardTop;
	int FilterRelationshipCardBottom;
	int FilterRelationshipCardTop;
	int FilterLayerBottom;
	int FilterLayerTop;
    int FilterRelationshipIndexBottom;
    int FilterRelationshipIndexTop;
    int FilterEntityIndexBottom;
    int FilterEntityIndexTop;

	// Entity-Relationship Drawing
	double m_reheight = 0.0;
	double m_clheight = 1.0;
	double m_enheight = 2.0;
	double m_gradientArrowScale = 1.0;

	//view control
	double zoffset = 100;
    Vec4 m_backgroundColor = Vec4(1.0, 1.0, 1.0, 0.0);
	int win_width, win_height;
	QPoint m_lastPos;
	QPoint m_pressPos;
	QPoint m_pressGlobalPos;
	float last_s, last_t;
	float translation[2];
	float rotmat[4][4];
	float zoom;
	Vec3 m_cursorPos = Vec3(0.0, 0.0, 0.0);
	VIEWDIMENSION view_dimension = VIEWDIMENSION::TWO_D;
	bool saveTransformation(const char* fName);
	bool openTransformation(const char* fName);

	void initializeSelection();

	void showEntityInformation(Entity* entity);
	void showRelationshipInformation(Relationship* relationship);

	int processHits(GLint hits, GLuint buffer[]);

	int pick_Entity(int x, int y, bool showInfo = true);
	int pick_Relationship(int x, int y, bool showInfo = true);
	void pick_EntitiesInsideSelectRegion();
	int pick_Scaffold(int x, int y);

	void displayEntities(GLenum mode);
	void displayRelationships(GLenum mode);
	void displayScaffolds(GLenum mode);
	void updateScaffoldList();
	
	void set_scene(GLenum mode, int w, int h);
	void reset_scene();
	void reset_sceneToCenterVisualization(bool alignRightBottom = false);
	void reset_sceneToContent(double minX, double minY, double maxX, double maxY);
	
	void update_translation(int x, int y);
	void update_orientation(int x, int y);
	void update_zoom(int x, int y);
	void record_mouse_pos(int x, int y);
	
	void updateVisibility();
	void updateEntitiesVisibility();
	void updateRelationshipsVisibility();

	void updateEntitiesInheiredVisibility();
	void updateRelationshipsInheiredVisibility();
	bool getEntityInheiredVisibility(Entity *e);
	bool getRelationshipInheiredVisibility(Relationship *r);

	void updateEntityVisibility(Entity *e);
	void updateRelationshipVisibility(Relationship *r);
	bool checkEntityVisibility(Entity *en);
	bool checkRelationshipVisibility(Relationship* re);

	//save image
	unsigned char* results_pixles;
	void saveToImage(const char* fName);
	void savePNGImage_fitAspectRatio(const char * fName, int mimdim = 1920);
	void saveImageHtmlMap(std::string fname);
	
	double *antiA_4[2];
	double *antiA_16[2];
	double *antiA_64[2];
	double antiA_radius;
	void initAntiA();
	void freeAntiA();

	void updateLights();
	void initLights();
	
	OGLLight light0;
	OGLLight light1;
	Color4_F EntityBoundaryColor = Color4_F(1, 1, 1, 1);
	
	/*Draw Light*/
	float cylinderShininess = 30.0;
	float SphereShininess = 10.0;
	double lightsize = 1.0;
	void drawLights(double radius = 1.0);

	//Draw Labels
	QPen textPen = QPen(Qt::black);
	QFont textFont;
	void testText(int fontsize, QPoint winloc, string str, Vec3 color, bool bold = false, Qt::Alignment flags = Qt::AlignVCenter | Qt::AlignHCenter, int maxPixelWidth = -1);
	void renderText(int fontsize, QPoint winloc, string str, Vec3 color, bool bold = false, Qt::Alignment flags = Qt::AlignVCenter | Qt::AlignHCenter, int maxPixelWidth = -1);
	void renderTextWithOutline(int fontSize, QPoint loc, string str, Color4_F color, Color4_F boundaryColor, bool bold = false);
	void renderText(int fontsize, double x, double y, double z, string str, Vec3 color, bool bold = false, Qt::Alignment flags = Qt::AlignVCenter | Qt::AlignHCenter, int maxPixelWidth = -1);
	void renderTextWithOutline(int fontsize, double x, double y, double z, string str, Color4_F color, Color4_F boundaryColor, bool bold = false);
	QPoint GLCoordToWindowCoord(double x, double y, double z);
	Vec3 WindowCoordToGLCoord(int x, int y);
	double getPixelWidth();
	int GLSizeToPointSize(double glwidth, double dpi);
	int GLSizeToPixel(double glsize);
	int PixelToPointSize(int pixel, double dpi);

	int renderText(int fontSize, int x, int y, string str, Vec3 color = Vec3(), bool bold = false);


	bool flag_toopTipsShown = false;
	void startToolTips(QString tooltipContent, QPoint toolTipPos);

	QString createTipContent(Entity *en);
	QString createTipContent(Relationship *en);
	QString tooltipContent;
	QPoint toolTipPos;
	void drawToolTips();
	QTimer *toolTipTimer;
	QFont toolTipFont;

	/*FPS*/
	QTime fpsTimer;
	int frameCount = 0;

	/*scaffold width*/
	double boundaryWidth = 3.0;
	double nodeBoundaryWidth = 1.5;

	/*labels*/
	//bool flag_showEnityLabels = false;
	//bool flag_showRelationshipLabels = false;
	LABEL_TYPE entityLabels = LABEL_NONE;
	LABEL_TYPE relationshipLabels = LABEL_NONE;
	double EfontScale = 0.7;
	int EfontSize = 15;
	int RfontSize = 15;
	int CfontSize = 8;
	bool centerELabel = false;
	bool autoresizeELabel = false;
	bool blackELable = false;
	bool blackRLable = false;
	bool EfontBold = false;
	bool RfontBold = false;
	bool CfontBold = false;

	bool flag_showCaption = true;	/*Captions*/
	int MaximumCaptionLines = 20;
	vector<string> m_captions;
	void addCaption(QString str);
	void addCaption(string str);
	void DrawCaptions();
	void clearCaptions();

	void DrawLabels();
	void DrawEntityLabels();
	void DrawRelationshipLabels();
	void DrawEntityLabel(Entity *entity);
	void DrawRelationshipLabel(Relationship *re);

	//Draw 2D Elements
	void DrawERData2D();
	void DrawBipartite2D();
	void DrawComposed2D();

	/*Draw the Dual data with ERdata together*/
	//bool flag_HightlightVRe = true;
	//bool flag_drawComposedDual = false;
	//bool flag_drawComposedDualUseOriginalColor = false;
	
	//bool flag_drawComposedDual_showDualPolyBoundary = true;
	
	//void DrawERDataComposedDual();
	//void DrawERDataComposedNoChange();
	//ColorGradient composedDualBackgroundColors;
	//ColorGradient composedDualFrontgroundColors;

	void draw2D(Entity *en, double z = 0);
	void draw2D(Relationship *re, double z = 0);
	void drawEdges(Relationship *re, double z = 0);
	void drawScaffold(Scaffold*scaffold, Vec4 color, double width, double z = 0);
	void drawNary2D(Relationship *re, bool flag_Opt = false, double z = 0);
	void drawBipart2D(Relationship* re, double z = 0);
	void drawLinks(Relationship* re, double z = 0);
	//void drawNaryOpt2D(Relationship *re);
	void drawCenter(Relationship *re, double z = 0);
	void drawObserver(Relationship *re, double z = 0);

	/*Draw decal*/
	void encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height);
	void decodeOneStep(const char* filename, unsigned char** data, unsigned &width, unsigned &height);

	void DrawCircleTexture(GLuint texId, float x, float y, float radius, int num_vertex = 50);
	vector<GLuint> decaltextures;
	void createDecalTexture(const char *textureFileName);


	//Draw Control related
	void drawSelectRegion();
	void drawConvexHull();
	void DrawArrow(Vec2 start, Vec2 end, double thickness, Color4_F color, double headlen, double headtailraitio = 1.0, double angle = 30);
	
    
	void DrawDisk(double x, double y, double radius, Vec4 color, int segments = 32, bool do_shading = false);
	void DrawDisk(Vec3 loc, double radius, Vec4 color, int segments = 32, bool do_shading = false);
	void DrawPartialDisk(Vec3 center, double radius, double startAngle, double endAngle, Vec4 color, double radian_step = 0.2, bool do_shading = false);
	void DrawCircle(double x, double y, double radius, Vec4 color, double lw = 1.0, int segments = 32);
	void DrawCircle(Vec3 loc, double radius, Vec4 color, double lw = 1.0, int segments = 32);
	void DrawArc(Vec3 center, double radius, double startanlge, double endangle, Vec4 color, double lw = 1.0, double radian_step = 0.2);
	void DrawLine(Vec3 start, Vec3 end, Vec4 color, double width = 1.0);
	void DrawLine(Vec2 start, Vec2 end, Vec4 color, double width = 1.0);
	void DrawLine3D(Vec3 start, Vec3 end, Vec4 color, double width = 1.0);
    void DrawCross(double x, double y, double radius, Vec3 color = Vec3(0, 0, 0), double rotation = 0);
	void DrawCross(Vec3 loc, double radius, Vec4 color, double rotation = 0);
	void DrawSquare(double x, double y, double radius, Vec4 color);
	void DrawSphere(Vec3 Point0, Vec4 Color, double radius);
	void DrawCylinder(Vec3 start, Vec3 end, Vec4 color, double radius);
	void DrawCylidnerCircle(Vec3 loc, double radius, Vec4 color, double width, int segments = 32);
	void DrawCylinderArc(Vec3 center, double radius, double startanlge, double endangle, Vec4 color, double width, int segments = 32);
	void DrawPolygonFill(ENList locs, Vec3 center, Vec4 color, double height, bool do_shading = false);
	void DrawPolygonLine(ENList locs, Vec4 color, double height, bool drawCylinder = false);
	void DrawPillShape(Vec3 start, Vec3 end, double radius, Vec4 color, double radian_step = 0.2, bool do_shading = false);
	void DrawBubbleLine(vector<Vec3> locs, double radius, Vec4 color, double radian_step = 0.2, bool do_shading = false);

	void DrawBoundingCircle();
	void DrawRelationshipBoundingBox();
	void DrawLayoutBoundingBox();

	void DrawPlaneAxes();
	void DrawPlaneGrid(int numlines);
	void DrawCursor();

	//selections
	SELECTTYPE m_selectType = SELECTTYPE::NONSELECT;
	SELECTSTATE m_selectState;
	MOUSEACTION m_mouseAction = MOUSEACTION::NOACTION;

	void updateSelectionFromData();

	void addSelectEntityList(Entity *e);				//toggle selected state of e and update the selectedEntityList
	void addEntitiesToSelectList(const vector<Entity*> &enlist);
	void addSelectRelationshipList(Relationship *r);	//toggle selected state of r and update the selectedRelationshipList
	void addRelationsToSelectList(const vector<Relationship*> &enlist);

	void selectSingleEntity(Entity *e);
	void selectSingleRelationship(Relationship *r);

	void clearSelectEntityList();				//only clear the list
	void clearSelectRelationshipList();			//only clear the list
	void removeSelectedEntities();				//clear the list with toggle the m_selected flag
	void removeSelectedRelationships();			//clear the list with toggle the m_selected flag

	vector<Entity*> getSelectedEntities(bool get_incident = false);
	vector<Relationship*> getSelectedRelationships(bool get_incident = false);

	Relationship* getSelectedRelationship(int i);
	Entity* getSelectedEntity(int i);
	
	void Highlight_selectedElements();
	void clearSelectRegion();
	void addVertexToSelectRegion(Vec3 v);

	void clearSwapList();
	vector<Entity*> getSwapList();
	void addEntityToSwapList(Entity *e);
	void clearScaffoldList();

	void clearConvexHull();
	void setConvexHull(vector<Vec3>Poitlist);

	/*Clipping test*/
	bool flag_drawClippedPolygons = true;
	bool flag_showSubjectPolyVertices = false;
	bool flag_showClipPolyVertices = false;
	vector<vector<Vec2>> cps;	/*clipping polys*/
	vector<Vec3> cpsColors;


	/*circle line segment intersection test*/
	vector<Vec2> circleLineIntersectionsTestPoints;	/*0th: circle center, 1th: point on circle, 2th & 3th: line segment 4&5: intersection*/
	void drawTestCircleLineIntersections();


    void setViewProjection(VIEWDIMENSION view_dimension, double ratio, double zoom);

	/*gradient debug*/
	void drawGradentDirectionsForEntities();

	/*Events*/
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
	void displayScene(int w, int h);
	void getFitWindowSize(int &w, int &h, int mimdim = 2048);
	void updateMouseOpenGLCoordinates(int x, int y);
	void contextMenuEvent(QContextMenuEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent * e) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent * event) override;
	void wheelEvent(QWheelEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void focusOutEvent(QFocusEvent* event);
	void focusInEvent(QFocusEvent* event);
	void cleanup();


	void moveRelationship(Relationship* re, int dx, int dy);
	void moveEntity(Entity *en, int dx, int dy);
    void setEntityLocation(Entity *en, int dx, int dy);
	void moveScaffold(Scaffold *s, int x, int y);

	PipeMesh::Pipe mMonogonPipe;
	std::vector<PipeMesh::Vector3> baseCircle;
	void drawMonogonScaffold(Vec3 loc, double rotation, Vec4 color, double width, double z = 0.0);
	void drawReMonogonScaffold(PipeMesh::Pipe* mMonogonPipe, Vec3 loc, double rotation, Vec4 color, double width, double z = 0.0);
	void buildMonogonPipe(double width);

	/*Variables*/
	Vec2 selectionRegionMouseLoc;
	vector<Vec3> selectionRegion;
	vector<Vec3> convexHull;
	vector<Entity*> swapList;							//right click to select entities from a relationship
	vector<Entity*> selectedEntityList;				
	vector<Relationship*> selectedRelationshipList;
	vector<Scaffold*>scaffoldList;
	int selectedScaffoldIndex;








	// Drawing Cluster details
private:

	ERClusterDecomp* m_bcdecomp = NULL;
	void drawClusters(double reheight, double enheight, double clheight);
	void drawClusterBarcode(Cluster* c, double enheight, double clheight);
	void drawClusterSuperPolygon(Cluster* c, double reheight);

public:

	ERClusterDecomp* getERClusterDecomp();
	bool setERClusterDecomp(ERClusterDecomp* decomp);
	void clearERClusterDecomp();


	



	// Heat Map drawing
private:

	void DrawHeatMap();


  
signals:
	void changeFocusedVis(VISTYPE mtype);
	void selectedElementsChanged(VISTYPE mtype);

	void mouseLocChanged(double x, double y);
	void visSizeChanged(double x, double y);

public slots:

	void turnoffToolTip();
	void showSelectedElementInfo();
	void Highlight_selectedElementsOnce();
	void Highlight_selectedElementsOneFrame(double scale);
};

#endif 