#include "oglwidget.h"
#include <stdio.h>
#include <set>
#include <math.h>
#include "trackball.h"
#include "tmatrix.h"
#include "mainwindow.h"
#include "LogConsole.h"
#include "ElementUtils.h"
#include "ERData.h"
#include "lodepng.h"
#include "FileIO.h"
#include <QDesktopWidget>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLPaintDevice>
#include <QMimeData>
#include <QThread>
#include <QToolTip>
#include <QTimer>
#include <QFileInfo>
#include <QPainterPath>
#include <QMenu>

#ifdef _WIN32
#include <gl\GLU.h>
#else
#include <glu.h>
#endif

#define BUFSIZE 512

void multmatrix(const Matrixt m)
{
	int i, j, index = 0;

	GLfloat mat[16];

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			mat[index++] = m[i][j];

	glMultMatrixf(mat);
}

OGLWidget::OGLWidget(QWidget *parent): QOpenGLWidget(parent)
{
	QSurfaceFormat format;
	format.setSamples(16);
	setFormat(format);
	translation[0] = translation[1] = 0.0;
	zoom = 1.0;
	win_width = win_height = 0;
	mat_ident(rotmat);
	m_selectState = SELECTSTATE::FINISHED;
	toolTipTimer = new QTimer(this);
	connect(toolTipTimer, SIGNAL(timeout()), this, SLOT(turnoffToolTip()));
	connect(this, SIGNAL(selectedElementsChanged(VISTYPE)), this, SLOT(showSelectedElementInfo()));
	setMouseTracking(true);
	toolTipFont = QFont("Times", 9, QFont::Bold);
	OGLFunctions.reset(new QOpenGLFunctions_2_1());
}

OGLWidget::~OGLWidget()
{
	cleanup();
	delete(m_bcdecomp);
}

void MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

void OGLWidget::cleanup()
{
	makeCurrent();
	doneCurrent();
}

void OGLWidget::initializeGL()
{
	CHECK_GL_ERROR();

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &OGLWidget::cleanup);

	gl = OGLFunctions.get();
	gl->initializeOpenGLFunctions();

	makeCurrent();
	CHECK_GL_ERROR();
	buildMonogonPipe(boundaryWidth);
	CHECK_GL_ERROR();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	glShadeModel(GL_FLAT);

	CHECK_GL_ERROR();
	initLights();
}

bool OGLWidget::saveTransformation(const char* fName)
{
	FILE *outFile;

	outFile = fopen(fName, "w");
	if (outFile == NULL)
	{
		return false;
	}

	fprintf(outFile, "%f\n", zoom);
	fprintf(outFile, "%f %f\n", translation[0], translation[1]);
	fprintf(outFile, "%f %f %f %f\n", rotmat[0][0], rotmat[0][1], rotmat[0][2], rotmat[0][3]);
	fprintf(outFile, "%f %f %f %f\n", rotmat[1][0], rotmat[1][1], rotmat[1][2], rotmat[1][3]);
	fprintf(outFile, "%f %f %f %f\n", rotmat[2][0], rotmat[2][1], rotmat[2][2], rotmat[2][3]);
	fprintf(outFile, "%f %f %f %f\n", rotmat[3][0], rotmat[3][1], rotmat[3][2], rotmat[3][3]);

	fclose(outFile);

	return true;
}

bool OGLWidget::openTransformation(const char* fName)
{
	FILE *inFile;

	inFile = fopen(fName, "r");
	if (inFile == NULL)
	{
		return false;
	}

	fscanf(inFile, "%f\n", &zoom);

	fscanf(inFile, "%f %f\n", &(translation[0]), &(translation[1]));

	float mat[4][4];

	fscanf(inFile, "%f %f %f %f\n", &(mat[0][0]), &(mat[0][1]), &(mat[0][2]), &(mat[0][3]));
	fscanf(inFile, "%f %f %f %f\n", &(mat[1][0]), &(mat[1][1]), &(mat[1][2]), &(mat[1][3]));
	fscanf(inFile, "%f %f %f %f\n", &(mat[2][0]), &(mat[2][1]), &(mat[2][2]), &(mat[2][3]));
	fscanf(inFile, "%f %f %f %f\n", &(mat[3][0]), &(mat[3][1]), &(mat[3][2]), &(mat[3][3]));

	rotmat[0][0] = mat[0][0]; rotmat[0][1] = mat[0][1]; rotmat[0][2] = mat[0][2]; rotmat[0][3] = mat[0][3];
	rotmat[1][0] = mat[1][0]; rotmat[1][1] = mat[1][1]; rotmat[1][2] = mat[1][2]; rotmat[1][3] = mat[1][3];
	rotmat[2][0] = mat[2][0]; rotmat[2][1] = mat[2][1]; rotmat[2][2] = mat[2][2]; rotmat[2][3] = mat[2][3];
	rotmat[3][0] = mat[3][0]; rotmat[3][1] = mat[3][1]; rotmat[3][2] = mat[3][2]; rotmat[3][3] = mat[3][3];

	fclose(inFile);

	return true;
}

void OGLWidget::testText(int fontSize, QPoint winloc, string str, Vec3 color, bool bold, Qt::Alignment flags, int maxPixelWidth)
{
	GLint viewport[4];					//var to hold the viewport info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	QOpenGLPaintDevice fboPaintDev(viewport[2], viewport[3]);
	QPainter painter(&fboPaintDev);
	painter.beginNativePainting();
	
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	painter.endNativePainting();

	QString name = QString::fromStdString(str);

	if (bold)
		textFont = QFont("Times", fontSize, QFont::Bold);
	else
		textFont = QFont("Times", fontSize);

	const qreal size = viewport[3];

	QPointF corner(winloc.x(), winloc.y() - size);

	if (flags & Qt::AlignHCenter)
		corner.rx() -= size / 2.0;
	else if (flags & Qt::AlignRight)
		corner.rx() -= size;
	if (flags & Qt::AlignVCenter)
		corner.ry() += size / 2.0;
	else if (flags & Qt::AlignTop)
		corner.ry() += size;
	else flags |= Qt::AlignBottom;

	QRectF rect{ corner.x(), corner.y(), size, size };

	if (maxPixelWidth != -1) {
		QFontMetrics fm(textFont);
		int pixelsWidth = fm.width(name);
		int pixelsHigh = fm.height();

		if (pixelsWidth > maxPixelWidth) {
			fontSize = fontSize* double(maxPixelWidth) / double(pixelsWidth);
		}
	}

	textPen = QPen(QColor(color.R * 255, color.G * 255, color.B * 255));

	painter.begin(this);
	painter.setPen(textPen);
	painter.setFont(textFont);
	painter.drawText(rect, flags, name, 0);
	painter.end();
}

void OGLWidget::renderText(int fontSize, double x, double y, double z, string str, Vec3 color, bool bold, Qt::Alignment flags, int maxPixelWidth) {

	QPoint winloc = GLCoordToWindowCoord(x, y, z);

	renderText(fontSize, winloc, str, color, bold, flags, maxPixelWidth);
}

void OGLWidget::renderText(int fontSize, QPoint winloc, string str, Vec3 color, bool bold, Qt::Alignment flags, int maxPixelWidth) {

	GLint viewport[4];					//var to hold the viewport info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	QOpenGLPaintDevice fboPaintDev(viewport[2], viewport[3]);
	QPainter painter(&fboPaintDev);
	QString name = QString::fromStdString(str);

	if (bold)
		textFont = QFont("Times", fontSize, QFont::Bold);
	else
		textFont = QFont("Times", fontSize);

	const qreal size = viewport[2];

	QPointF corner(winloc.x(), winloc.y() - size);
	
	if (flags & Qt::AlignHCenter) 
		corner.rx() -= size / 2.0;
	else if (flags & Qt::AlignRight) 
		corner.rx() -= size;
	if (flags & Qt::AlignVCenter) 
		corner.ry() += size / 2.0;
	else if (flags & Qt::AlignTop) 
		corner.ry() += size;
	else flags |= Qt::AlignBottom;

	QRectF rect{ corner.x(), corner.y(), size, size };

	if (maxPixelWidth != -1) {
		QFontMetrics fm(textFont);
		int pixelsWidth = fm.width(name);
		int pixelsHigh = fm.height();

		if (pixelsWidth > maxPixelWidth) {
			fontSize = fontSize* double(maxPixelWidth) / double(pixelsWidth);
		}
	}

	textPen = QPen(QColor(color.R * 255, color.G * 255, color.B * 255));

	painter.begin(this);
	painter.setPen(textPen);
	painter.setFont(textFont);
	painter.drawText(rect, flags, name, 0);
	painter.end();

}

void OGLWidget::renderTextWithOutline(int fontSize, double x, double y, double z, string str, Color4_F color, Color4_F bcolor, bool bold)
{
	QPoint winloc = GLCoordToWindowCoord(x, y, z);

	QPainter painter(this);
	painter.begin(this);

	QString name = QString::fromStdString(str);

	int size = fontSize;

	if (bold)
		textFont = QFont("Times", size, QFont::Bold);
	else
		textFont = QFont("Times", size);

	QColor fqcolor = QColor(color.R * 255, color.G * 255, color.B * 255, color.Alpha * 255);
	QColor bqcolor = QColor(bcolor.R * 255, bcolor.G * 255, bcolor.B * 255, bcolor.Alpha * 255);

	QPainterPath myPath;
	myPath.addText(winloc, textFont, name);

	painter.setBrush(fqcolor);
	painter.setPen(QPen(bqcolor));
	painter.drawPath(myPath);
	painter.end();
}

/*size of the font is auto adjusted based on the size of the dot radius*/
void OGLWidget::renderTextWithOutline(int fontSize, QPoint loc, string str, Color4_F color, Color4_F bcolor, bool bold)
{
	QPainter painter(this);
	painter.begin(this);

	QString name = QString::fromStdString(str);

	int size = fontSize;

	if (bold)
		textFont = QFont("Times", size, QFont::Bold);
	else
		textFont = QFont("Times", size);

	QColor fqcolor = QColor(color.R * 255, color.G * 255, color.B * 255, color.Alpha * 255);
	QColor bqcolor = QColor(bcolor.R * 255, bcolor.G * 255, bcolor.B * 255, bcolor.Alpha * 255);

	QPainterPath myPath;
	myPath.addText(loc, textFont, name);

	painter.setBrush(fqcolor);
	painter.setPen(QPen(bqcolor));
	painter.drawPath(myPath);
	painter.end();
}

QPoint OGLWidget::GLCoordToWindowCoord(double x, double y, double z)
{
	GLint viewport[4];					//var to hold the viewport info
	GLdouble modelview[16];				//var to hold the modelview info
	GLdouble projection[16];			//var to hold the projection matrix info
	GLdouble objx, objy, objz;			//variables to hold world x,y,z coordinates
	GLdouble winx, winy, winz;			// variables to hold screen x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	objx = x;
	objy = y;
	objz = z;

	//get the world coordinates from the screen coordinates
	gluProject(objx, objy, objz, modelview, projection, viewport, &winx, &winy, &winz);

	winy = (float)viewport[3] - (float)winy;

	QPoint baseline(winx, winy);

	return baseline;
}

Vec3 OGLWidget::WindowCoordToGLCoord(int x, int y)
{
	makeCurrent();

	GLint viewport[4];					//var to hold the viewport info
	GLdouble modelview[16];				//var to hold the modelview info
	GLdouble projection[16];			//var to hold the projection matrix info
	GLdouble objx, objy, objz;			//variables to hold world x,y,z coordinates
	GLfloat winx, winy, winz;			// variables to hold screen x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	winx = (float)x;
	winy = (float)viewport[3] - (float)y;
	winz = 0;

	//get the world coordinates from the screen coordinates
	gluUnProject(winx, winy, winz, modelview, projection, viewport, &objx, &objy, &objz);

	return Vec3(objx, objy, objz);
}

/*return the gl width of each pixel*/
double OGLWidget::getPixelWidth()
{
	/*get canvas pixels*/
	GLint viewport[4];					//var to hold the viewport info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info
	double ratio = double(viewport[2]) / double(viewport[3]);
	
	/*get openGL range*/
	double w = 2.0 * zoom*ratio;
	double pixelWidth = w / viewport[2];
	return pixelWidth;
}

int OGLWidget::GLSizeToPointSize(double glwidth, double dpi)
{
	int px = GLSizeToPixel(glwidth);
	return PixelToPointSize(px, dpi);
}

int OGLWidget::GLSizeToPixel(double glsize)
{
	double w = getPixelWidth();
	return glsize / w;
}

int OGLWidget::PixelToPointSize(int pixel, double dpi)
{
	return pixel * 72 / dpi;
}


/*Using window coordinates*/
int OGLWidget::renderText(int fontSize, int worldX, int worldY, string str, Vec3 color, bool bold)
{
	QPainter painter(this);
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
    int size = fontSize;

    if(bold)
        textFont = QFont("Times", size, QFont::Bold);
    else
        textFont = QFont("Times", size);

    int r = color.a * 255;
    int g = color.b * 255;
    int b = color.c * 255;

    textPen = QPen(QColor(r, g, b));

	painter.setPen(textPen);
    painter.setFont(textFont);

	GLint viewport[4];					//var to hold the viewport info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	QString name = QString::fromStdString(str);
	const QRect rectangle = QRect(worldX, worldY, viewport[2] - worldX, viewport[3] - worldY);
	QRect boundingRect;
	painter.drawText(rectangle, Qt::AlignLeft, name, &boundingRect);
    painter.end();

	return boundingRect.height();
}

void OGLWidget::turnoffToolTip() {
	flag_toopTipsShown = false;
	update();
}

void OGLWidget::startToolTips(QString str, QPoint p)
{
	tooltipContent = str;
	toolTipPos = p;
	flag_toopTipsShown = true;
	toolTipTimer->start(3000);
}

void OGLWidget::drawToolTips() {

	if (!flag_toopTipsShown)
		return;

	QPainter painter(this);
	painter.begin(this);

	painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath path;

	QFontMetrics fm(toolTipFont);
	QRect box = fm.boundingRect(QRect(0, 0, 100, 100), Qt::TextWordWrap, tooltipContent);

	int margin = 3;

	box = box.translated(toolTipPos);
	QRect boxWithMargin = box.marginsAdded(QMargins(margin, margin, margin, margin));
	path.addRoundedRect(boxWithMargin, boxWithMargin.width()*0.05, boxWithMargin.width()*0.05);

	QPen pen(Qt::black, 2);
	painter.setPen(pen);
	painter.fillPath(path, Qt::red);
	painter.drawPath(path);

	int size = 17 * 0.6;
	int r = 0 * 255;
	int g = 0 * 255;
	int b = 0 * 255;
	textPen = QPen(QColor(r, g, b));
	painter.setPen(textPen);
	painter.setFont(toolTipFont);
	painter.drawText(box, Qt::AlignLeft, tooltipContent);
	painter.end();

}

void OGLWidget::addCaption(QString str)
{
	addCaption(str.toStdString());
}

void OGLWidget::addCaption(string str)
{
	m_captions.push_back(str);
	if (m_captions.size() > MaximumCaptionLines)
	{
		m_captions.erase(m_captions.begin(), m_captions.begin() + (MaximumCaptionLines - m_captions.size()));
	}
}

void OGLWidget::DrawCaptions()
{
    int fontSize = CfontSize;

    int xstart = 0.01*win_width;
	int ystart = 0.01*win_height;
    Vec3 color(0.0,0.0,0.0);

	int offset = 0;
    for (int i = 0; i < m_captions.size(); i++) {
		ystart = ystart + offset*1.1;
        offset = renderText(fontSize, xstart, ystart, m_captions[i], color, CfontBold);
    }

}

void OGLWidget::clearCaptions()
{
	m_captions.clear();
}

void OGLWidget::DrawEntityLabels() 
{

	for (int i = 0; i < currentERData->getEntityNum(); i++) {
		Entity * tempe = currentERData->getEntity(i);

		if (!tempe->getVisibility() || !tempe->getLabelVisibility())
			continue;

		DrawEntityLabel(tempe);
	}
}

void OGLWidget::DrawRelationshipLabels() {

	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
		Relationship * tempre = currentERData->getRelationship(i);

		if (!tempre->getVisibility())
			continue;

		if (!tempre->getLabelVisibility())
			continue;

		DrawRelationshipLabel(tempre);
	}

}

void OGLWidget::DrawEntityLabel(Entity *entity) {

	bool res = entity->getVisibility();
	if (!res)
		return;

	Vec3 CurrentLocation = entity->getLocation();
	double Radius = entity->m_Size;

	int fontSize = EfontSize;
	QPoint fontloc;

	if (centerELabel)
		fontloc = GLCoordToWindowCoord(CurrentLocation.x, CurrentLocation.y, CurrentLocation.z);
	else
		fontloc = GLCoordToWindowCoord(CurrentLocation.x + Radius * 1.2, CurrentLocation.y, CurrentLocation.z);

	if (autoresizeELabel) 
	{
		double dpi = qApp->desktop()->logicalDpiX();
		int maxPixel = GLSizeToPixel(Radius * 2.0 * EfontScale);
		int dynamicfontSize = PixelToPointSize(maxPixel * 0.9, dpi);
		fontSize = dynamicfontSize;
	}
	
	bool fontBold = EfontBold;
	Vec3 labelcolor = entity->getColor();
	labelcolor = 0.5*labelcolor;

	if (autoresizeELabel)
		labelcolor = Vec3(1.0, 1.0, 1.0);
	
	if (blackELable)
		labelcolor = Vec3(0.0, 0.0, 0.0);

    string name;
	switch (entityLabels)
	{
	case LABEL_TYPE::LABEL_NONE:
		name = "";
		break;
	case LABEL_TYPE::LABEL_NAME:
		name = entity->getLabel();
		break;
	case LABEL_TYPE::LABEL_INDEX:
		name = to_string(entity->getIndex());
		break;
	case LABEL_TYPE::LABEL_OPIDX:
		name = to_string(entity->getOpidx());
		break;
	case LABEL_TYPE::LABEL_INCIDENCE:
		name = to_string(currentERData->getEntityIncidence(entity));
		break;
	case LABEL_TYPE::LABEL_ADJACENCY:
		name = to_string(currentERData->getEntityAdjacency(entity));
		break;
	case LABEL_TYPE::LABEL_ADJACENCY_FACTOR:
		name = to_string(currentERData->getEntityAdjacencyFactor(entity));
		break;
	case LABEL_TYPE::LABEL_BETWEENNESS:
		name = to_string(currentERData->getEntityBetweennessCentrality(entity));
		break;
	case LABEL_TYPE::LABEL_ECCENTRICITY:
		name = to_string(currentERData->getEntityEccentricity(entity));
		break;
	case LABEL_TYPE::LABEL_CLOSENESS:
		name = to_string(currentERData->getEntityClosenessCentrality(entity));
		break;
	case LABEL_TYPE::LABEL_RESISTANCE:
		name = to_string(currentERData->getEntityEffectiveResistance(entity));
		break;
	case LABEL_TYPE::LABEL_COST:
		name = to_string(entity->getRemovalCost());
		break;
	case LABEL_TYPE::LABEL_IMPORTANCE:
		name = to_string(entity->getRemovalImportance());
		break;
	case LABEL_TYPE::LABEL_PRIORITY:
		name = to_string(entity->getRemovalPriority());
		break;
	default:
		name = "";
		break;
	}

	if (centerELabel)
		renderText(fontSize, fontloc, name, labelcolor, fontBold, Qt::AlignVCenter | Qt::AlignHCenter);
	else
		renderText(fontSize, fontloc, name, labelcolor, fontBold, Qt::AlignLeft | Qt::AlignVCenter);
}

void OGLWidget::DrawRelationshipLabel(Relationship *re) {

	bool res = re->getVisibility();
	if (!res)
		return;

	int fontSize = RfontSize;
    bool fontbold = RfontBold;

	Vec3 color = re->getColor();
	color =  0.5*color;

	if (blackRLable)
		color = Vec3(0.0, 0.0, 0.0);

	Vec3 center = calculateCenter(re->getIncidentEntities());

    string name;
	switch (relationshipLabels)
	{
	case LABEL_TYPE::LABEL_NONE:
		name = "";
		break;
	case LABEL_TYPE::LABEL_NAME:
		name = re->getLabel();
		break;
	case LABEL_TYPE::LABEL_INDEX:
		name = to_string(re->getIndex());
		break;
	case LABEL_TYPE::LABEL_OPIDX:
		name = to_string(re->getOpidx());
		break;
	case LABEL_TYPE::LABEL_INCIDENCE:
		name = to_string(currentERData->getRelationshipIncidence(re));
		break;
	case LABEL_TYPE::LABEL_ADJACENCY:
		name = to_string(currentERData->getRelationshipAdjacency(re));
		break;
	case LABEL_TYPE::LABEL_ADJACENCY_FACTOR:
		name = to_string(currentERData->getRelationshipAdjacencyFactor(re));
		break;
	case LABEL_TYPE::LABEL_BETWEENNESS:
		name = to_string(currentERData->getRelationshipBetweennessCentrality(re));
		break;
	case LABEL_TYPE::LABEL_ECCENTRICITY:
		name = to_string(currentERData->getRelationshipEccentricity(re));
		break;
	case LABEL_TYPE::LABEL_CLOSENESS:
		name = to_string(currentERData->getRelationshipClosenessCentrality(re));
		break;
	case LABEL_TYPE::LABEL_RESISTANCE:
		name = to_string(currentERData->getRelationshipEffectiveResistance(re));
		break;
	case LABEL_TYPE::LABEL_COST:
		name = to_string(re->getRemovalCost());
		break;
	case LABEL_TYPE::LABEL_IMPORTANCE:
		name = to_string(re->getRemovalImportance());
		break;
	case LABEL_TYPE::LABEL_PRIORITY:
		name = to_string(re->getRemovalPriority());
		break;
	default:
		name = "";
		break;
	}

	renderText(fontSize, re->getObserver().x, re->getObserver().y, 0, name, color, fontbold);
}

void OGLWidget::updateVisibility() {

	updateEntitiesVisibility();

	updateRelationshipsVisibility();

	if (flag_inheritVisibility) {
		updateEntitiesInheiredVisibility();
		updateRelationshipsInheiredVisibility();
	}
}

void OGLWidget::updateEntitiesInheiredVisibility() {

	for (int i = 0; i < currentERData->getEntityNum(); i++) {
		Entity *temp_e = currentERData->getEntity(i);
		
		bool oldvis = temp_e->getVisibility();
		bool inheritedVis = getEntityInheiredVisibility(temp_e);
		temp_e->setVisibility(oldvis && inheritedVis);
	}
}

void OGLWidget::updateRelationshipsInheiredVisibility() {

	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
		Relationship *temp_e = currentERData->getRelationship(i);
		
		bool oldvis = temp_e->getVisibility();
		bool inheritedVis = getRelationshipInheiredVisibility(temp_e);
		temp_e->setVisibility(oldvis && inheritedVis);
	}
}

bool OGLWidget::getEntityInheiredVisibility(Entity *e) {

	for (int j = 0; j < e->getIncidentRelationships().size(); j++) {
		if (e->getIncidentRelationship(j)->getVisibility())
			return true;
	}

	return false;
}

bool OGLWidget::getRelationshipInheiredVisibility(Relationship *r) {

	for (int j = 0; j < r->getIncidentEntities().size(); j++) {
		if (!r->getIncidentEntity(j)->getVisibility())
			return false;
	}
	return true;
}

void OGLWidget::updateEntitiesVisibility() {
	for (int i = 0; i < currentERData->getEntityNum(); i++) {

		Entity *temp_e = currentERData->getEntity(i);
		updateEntityVisibility(temp_e);
	}
}

void OGLWidget::updateRelationshipsVisibility() {
	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {

		Relationship *temp_r = currentERData->getRelationship(i);
		updateRelationshipVisibility(temp_r);
	}
}

void OGLWidget::updateEntityVisibility(Entity *temp_e) {
	
	if (checkEntityVisibility(temp_e))
		temp_e->setVisibility(true);
	else
		temp_e->setVisibility(false);
}

void OGLWidget::updateRelationshipVisibility(Relationship *temp_r) {

	if (checkRelationshipVisibility(temp_r))
		temp_r->setVisibility(true);
	else
		temp_r->setVisibility(false);
}

bool OGLWidget::checkEntityVisibility(Entity *en) {

	if (!flag_showEntities)
		return false;

	if (flag_entityDegreeFilter) {
		int d = en->getDegree();

		if (d > FilterEntityDegreeTop || d < FilterEntityDegreeBottom){
			return false;
		}
	}

	if (flag_entityCardinalityFilter) {
		int c = en->getCardinality();

		if (c > FilterEntityCardTop || c < FilterEntityCardBottom) {
			return false;
		}
	}

    if (flag_entityIndexFilter) {
        int index = en->getIndex();
        if (index > FilterEntityIndexTop || index < FilterEntityIndexBottom) {
            return false;
        }
    }

	return true;
}

bool OGLWidget::checkRelationshipVisibility(Relationship* re) {

	if (!flag_showRelationships)
		return false;

	if (flag_relationshipDegreeFilter) {
		int d = re->getDegree();

		if (d > FilterRelationshipDegreeTop || d < FilterRelationshipDegreeBottom) {
			return false;
		}
	}

	if (flag_relationshipCardinalityFilter) {
		int c = re->getCardinality();

		if (c > FilterRelationshipCardTop || c < FilterRelationshipCardBottom) {
			return false;
		}
	}

    if (flag_relationshipIndexFilter) {
        int index = re->getIndex();
        if (index > FilterRelationshipIndexTop || index < FilterRelationshipIndexBottom) {
            return false;
        }
    }

	return true;
}

void OGLWidget::DrawLabels()
{
	if (entityLabels != LABEL_TYPE::LABEL_NONE)
		DrawEntityLabels();
	if (relationshipLabels != LABEL_TYPE::LABEL_NONE)
		DrawRelationshipLabels();
}

void OGLWidget::DrawERData2D() {

    CHECK_GL_ERROR();

	if (!currentERData)
		return;

	// render the polygons
	vector<Relationship*> selected;
	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {

		Relationship *temp_r = currentERData->getRelationship(i);

		if (!temp_r->getVisibility())
			continue;

		if (m_bcdecomp && flag_drawClusterSuperPolygons)
			if (m_bcdecomp->getERData() == currentERData &&
				m_bcdecomp->getElemCluster(temp_r->getIndex(), RELATIONSHIP))
				continue;

		if (flag_renderSelectedPolysLast && temp_r->getSelected())
			selected.push_back(temp_r);
		else
			draw2D(temp_r, m_reheight);

        CHECK_GL_ERROR();
	}	
	
	//render selected RE at top
	for (int i = 0; i < selected.size(); i++) {
		Relationship *temp_r = selected[i];
		draw2D(temp_r, m_reheight);
	}
	
	// render cluster information
	glEnable(GL_DEPTH_TEST);
	if (flag_drawClusters)
		drawClusters(m_reheight, m_enheight, m_clheight);

	//double layer = currentERData->getRelationshipNum();
	//double height = layer * boundaryWidth*0.05*Element::m_VertexSize;

	//render the nodes
	for (int i = 0; i < currentERData->getEntityNum(); i++) {

		Entity *temp_e = currentERData->getEntity(i);

		if (!temp_e->getVisibility())
			continue;

		draw2D(temp_e, m_enheight);

        CHECK_GL_ERROR();
	}
	glDisable(GL_DEPTH_TEST);

	if (gmainwindow->flag_showGradientDirs || gmainwindow->flag_showLSearchDirs)
		drawGradentDirectionsForEntities();

	//vector<Vec3> locs;
	//for (Entity* en : currentERData->getEntities())
	//{
	//	Vec3 loc = en->getLocation();
	//	loc.z = m_clheight;
	//	locs.push_back(loc);
	//}
	//DrawBubbleLine(locs, Element::m_VertexSize * 2, Vec4(0, 0, 0, 0.5), 0.2, true);
}

//void OGLWidget::DrawERDataComposedDual()
//{
//
//	if (!currentERData)
//		return;
//
//	CHECK_GL_ERROR();
//
//	glDisable(GL_DEPTH_TEST);
//	glShadeModel(GL_FLAT);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	auto drawRe = [](Relationship *re, bool drawBoundary, Vec4 color)
//	{
//		Vec4 polygonColor = color;
//		Vec4 boundaryColor = color*0.8;
//		boundaryColor.Alpha = 1.0;
//
//		glEnable(GL_POLYGON_OFFSET_FILL);
//		glPolygonOffset(1., 1.);
//
//		CHECK_GL_ERROR();
//
//		/*draw filled color*/
//		glBegin(GL_TRIANGLES);
//
//		vector<Scaffold>* scaffoldlist = re->getScaffoldList();
//		for (int i = 0; i<scaffoldlist->size(); i++) {
//			for (int j = 0; j < re->getScaffold(i).m_CurvedLocation.size() - 1; j++) {
//
//				Vec3 loc1 = re->getScaffold(i).m_CurvedLocation[j];
//				Vec3 loc2 = re->getScaffold(i).m_CurvedLocation[j + 1];
//
//				glColor4f(polygonColor.R, polygonColor.G, polygonColor.B, polygonColor.Alpha);
//				glVertex3f(re->getObserver().a, re->getObserver().b, 0);
//				glVertex3f(loc1.a, loc1.b, 0);
//				glVertex3f(loc2.a, loc2.b, 0);
//			}
//		}
//		glEnd();
//
//		CHECK_GL_ERROR();
//
//		if (drawBoundary) {
//
//			glColor4f(boundaryColor.R, boundaryColor.G, boundaryColor.B, boundaryColor.Alpha);
//			glLineWidth(2.0);
//			glBegin(GL_LINES);
//
//			for (int i = 0; i < scaffoldlist->size(); i++) {
//				for (int j = 0; j <re->getScaffold(i).m_CurvedLocation.size() - 1; j++) {
//
//					Vec3 loc1 = re->getScaffold(i).m_CurvedLocation[j];
//					Vec3 loc2 = re->getScaffold(i).m_CurvedLocation[j + 1];
//
//					glVertex3f(loc1.a, loc1.b, 0);
//					glVertex3f(loc2.a, loc2.b, 0);
//				}
//			}
//			glEnd();
//		}
//
//		CHECK_GL_ERROR();
//
//		glFlush();
//
//		CHECK_GL_ERROR();
//	};
//
//	/*draw background*/
//	float maxCard = (float)currentERData->maxRelationshipCardinality;
//	float minCard = (float)currentERData->minRelationshipCardinality;
//
//	float maxDualCard = (float)currentERData->maxEntityDegree;
//	float minDualCard = (float)currentERData->minEntityDegree;
//
//	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
//
//		Relationship *re = currentERData->getRelationship(i);
//
//		if (!re->getVisibility())
//			continue;
//
//		/*draw background polys*/
//		float card = (float)re->getCardinality();
//		float r, g, b;
//		float v = card / (maxCard - minCard);
//		composedDualBackgroundColors.getColorAtValue(v, r, g, b);
//		drawRe(re, false, Vec4(r, g, b, 0.8));
//
//		CHECK_GL_ERROR();
//	}
//
//	for (int i = 0; i < currentERData->getEntityNum(); i++) {
//
//		Entity *en = currentERData->getEntity(i);
//
//		if (!en->getVisibility())
//			continue;
//
//		/*draw background nodes*/
//		{
//			double Radius = en->m_Size;
//			Vec4 color = en->getColor();
//			Vec3 CurrentLocation = en->getLocation();
//			DrawDisk(CurrentLocation, Radius, Vec4(1.0, 1.0, 1.0, 1.0));
//			DrawCircle(CurrentLocation, Radius, color);
//		}
//		CHECK_GL_ERROR();
//	}
//
//	for (int i = 0; i < currentERData->getEntityNum(); i++) {
//
//		Entity *en = currentERData->getEntity(i);
//
//		if (!en->getVisibility())
//			continue;
//
//		/*draw foreground polys*/
//		Relationship *re = en->getDualRelationship();
//
//		if (flag_hideMonogon && re && re->getCardinality() == 1) {
//			continue;
//		}
//
//		if (re) {
//
//			float card = (float)re->getCardinality();
//			float r, g, b;
//			float v = card / (maxDualCard - minDualCard);
//			composedDualFrontgroundColors.getColorAtValue(v, r, g, b);
//			drawRe(re, flag_drawComposedDual_showDualPolyBoundary, Vec4(r, g, b, 0.8));
//		}
//
//		CHECK_GL_ERROR();
//	}
//
//
//	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
//
//		Relationship *re = currentERData->getRelationship(i);
//
//		if (!re->getVisibility())
//			continue;
//
//		Entity *en = re->getDualEntity();
//
//		/*draw foreground nodes*/
//		if (en) {
//			double Radius = en->m_Size;
//			Vec4 color = en->getColor();
//			Vec3 CurrentLocation = en->getLocation();
//			DrawDisk(CurrentLocation, Radius, color);
//		}
//
//		CHECK_GL_ERROR();
//	}
//}
//
//void OGLWidget::DrawERDataComposedNoChange()
//{
//
//	if (!currentERData)
//		return;
//
//	CHECK_GL_ERROR();
//
//	glDisable(GL_DEPTH_TEST);
//	glShadeModel(GL_FLAT);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	auto drawRe = [](Relationship *re, bool drawBoundary, Vec4 color)
//	{
//		Vec4 polygonColor = color;
//		Vec4 boundaryColor = color*0.8;
//		boundaryColor.Alpha = 1.0;
//
//		glEnable(GL_POLYGON_OFFSET_FILL);
//		glPolygonOffset(1., 1.);
//
//		CHECK_GL_ERROR();
//
//		/*draw filled color*/
//		glBegin(GL_TRIANGLES);
//
//		vector<Scaffold>* scaffoldlist = re->getScaffoldList();
//		for (int i = 0; i < scaffoldlist->size(); i++) {
//			for (int j = 0; j < re->getScaffold(i).m_CurvedLocation.size() - 1; j++) {
//
//				Vec3 loc1 = re->getScaffold(i).m_CurvedLocation[j];
//				Vec3 loc2 = re->getScaffold(i).m_CurvedLocation[j + 1];
//
//				glColor4f(polygonColor.R, polygonColor.G, polygonColor.B, polygonColor.Alpha);
//				glVertex3f(re->getObserver().a, re->getObserver().b, 0);
//				glVertex3f(loc1.a, loc1.b, 0);
//				glVertex3f(loc2.a, loc2.b, 0);
//			}
//		}
//		glEnd();
//
//		CHECK_GL_ERROR();
//
//		if (drawBoundary) {
//
//			glColor4f(boundaryColor.R, boundaryColor.G, boundaryColor.B, boundaryColor.Alpha);
//			glLineWidth(2.0);
//			glBegin(GL_LINES);
//
//			for (int i = 0; i < scaffoldlist->size(); i++) {
//				for (int j = 0; j < re->getScaffold(i).m_CurvedLocation.size() - 1; j++) {
//
//					Vec3 loc1 = re->getScaffold(i).m_CurvedLocation[j];
//					Vec3 loc2 = re->getScaffold(i).m_CurvedLocation[j + 1];
//
//					glVertex3f(loc1.a, loc1.b, 0);
//					glVertex3f(loc2.a, loc2.b, 0);
//				}
//			}
//			glEnd();
//		}
//
//		CHECK_GL_ERROR();
//
//		glFlush();
//
//		CHECK_GL_ERROR();
//	};
//
//	/*draw background*/
//	float maxCard = (float)currentERData->maxRelationshipCardinality;
//	float minCard = (float)currentERData->minRelationshipCardinality;
//
//	float maxDualCard = (float)currentERData->maxEntityDegree;
//	float minDualCard = (float)currentERData->minEntityDegree;
//
//	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
//
//		Relationship *re = currentERData->getRelationship(i);
//
//		if (!re->getVisibility())
//			continue;
//
//		draw2D(re);
//
//		CHECK_GL_ERROR();
//	}
//
//	for (int i = 0; i < currentERData->getEntityNum(); i++) {
//
//		Entity *en = currentERData->getEntity(i);
//
//		if (!en->getVisibility())
//			continue;
//
//		draw2D(en);
//
//		CHECK_GL_ERROR();
//	}
//
//	for (int i = 0; i < currentERData->getEntityNum(); i++) {
//
//		Entity *en = currentERData->getEntity(i);
//
//		if (!en->getVisibility())
//			continue;
//
//		/*draw foreground polys*/
//		Relationship *re = en->getDualRelationship();
//
//		if (flag_hideMonogon && re && re->getCardinality() == 1) {
//			continue;
//		}
//
//		draw2D(re);
//
//		CHECK_GL_ERROR();
//	}
//
//	double layer = currentERData->getEntityNum();
//	double height = layer * boundaryWidth*0.05*Element::m_VertexSize;
//
//	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
//
//		Relationship *re = currentERData->getRelationship(i);
//
//		if (!re->getVisibility())
//			continue;
//
//		Entity *en = re->getDualEntity();
//
//		draw2D(en, height);
//
//		CHECK_GL_ERROR();
//	}
//
//}

void OGLWidget::DrawCircleTexture(GLuint texId, float x, float y, float radius, int num_vertex)
{
	GLfloat vertex[2];
	GLfloat texcoord[2];

	const GLfloat delta_angle = 2.0*M_PI / num_vertex;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBegin(GL_TRIANGLE_FAN);

	//draw the vertex at the center of the circle
	texcoord[0] = 0.5;
	texcoord[1] = 0.5;
	glTexCoord2fv(texcoord);

	vertex[0] = x;
	vertex[1] = y;

	glVertex2fv(vertex);

	for (int i = 0; i < num_vertex; i++)
	{
		texcoord[0] = (std::cos(delta_angle*i) + 1.0)*0.5;
		texcoord[1] = (std::sin(delta_angle*i) + 1.0)*0.5;
		glTexCoord2fv(texcoord);

		vertex[0] = x + std::cos(delta_angle*i) * radius;
		vertex[1] = y + std::sin(delta_angle*i) * radius;
		glVertex2fv(vertex);
	}

	texcoord[0] = (1.0 + 1.0)*0.5;
	texcoord[1] = (0.0 + 1.0)*0.5;
	glTexCoord2fv(texcoord);

	vertex[0] = x + 1.0 * radius;
	vertex[1] = y + 0.0 * radius;
	glVertex2fv(vertex);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void OGLWidget::createDecalTexture(const char *textureFileName)
{
	unsigned char *texDat;
	unsigned height, width;
	decodeOneStep("../Resources/textures/friendship.png", &texDat, width, height);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texDat);
	glBindTexture(GL_TEXTURE_2D, 0);

	decaltextures.push_back(tex);
}

void OGLWidget::DrawRelationshipBoundingBox() {

	for (auto r : currentERData->getRelationships()) {

		Vec3 p1 = Vec3(r->maxX, r->maxY, 0);
		Vec3 p2 = Vec3(r->maxX, r->minY, 0);
		Vec3 p3 = Vec3(r->minX, r->minY, 0);
		Vec3 p4 = Vec3(r->minX, r->maxY, 0);

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);

		glColor3f(0, 1, 0);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);

		glColor3f(1, 0, 0);
		glVertex3f(p3.x, p3.y, p3.z);
		glVertex3f(p4.x, p4.y, p4.z);

		glColor3f(0, 1, 0);
		glVertex3f(p4.x, p4.y, p4.z);
		glVertex3f(p1.x, p1.y, p1.z);

		glEnd();

		glFlush();
	}
}

void OGLWidget::DrawLayoutBoundingBox()
{
	glColor3f(1, 0, 0);

	double maxX = currentERData->maxX;
	double maxY = currentERData->maxY;

	double minX = currentERData->minX;
	double minY = currentERData->minY;


	glBegin(GL_LINES);

	
	glVertex3f(minX, minY, 0);
	glVertex3f(maxX, minY, 0);

	//glColor3f(0, 1, 0);
	glVertex3f(minX, minY, 0);
	glVertex3f(minX, maxY, 0);

	//glColor3f(1, 0, 0);
	glVertex3f(maxX, maxY, 0);
	glVertex3f(maxX, minY, 0);

	//glColor3f(0, 1, 0);
	glVertex3f(maxX, maxY, 0);
	glVertex3f(minX, maxY, 0);

	glVertex3f(minX, minY, 0);
	glVertex3f(maxX, maxY, 0);

	glVertex3f(minX, maxY, 0);
	glVertex3f(maxX, minY, 0);

	glEnd();

	glFlush();
}

void OGLWidget::DrawBoundingCircle()
{
	double x = currentERData->BC_center.x;
	double y = currentERData->BC_center.y;
	double r = currentERData->BC_radius;
	Vec4 color(1, 0, 0, 1);
    DrawCircle(x, y, r, color);
}

void OGLWidget::DrawPlaneAxes()
{
	Vec3 min = WindowCoordToGLCoord(0, 0);
	Vec3 max = WindowCoordToGLCoord(win_width, win_height);

	DrawLine(Vec2(0, min.y), Vec2(0, max.y), Vec4(0.0, 1.0, 0.0, 1.0), 1.0);
	DrawLine(Vec2(min.x, 0), Vec2(max.x, 0), Vec4(1.0, 0.0, 0.0, 1.0), 1.0);
}

void OGLWidget::DrawPlaneGrid(int numlines)
{
	float dim = float(numlines);
	Vec4 color(0.9, 0.9, 0.9, 1.0);
	for (int i = 1; i < numlines + 1; i++)
	{
		float val = float(i);
		DrawLine(Vec2(-dim, val), Vec2(dim, val), color, 1.0);
		DrawLine(Vec2(-dim, -val), Vec2(dim, -val), color, 1.0);
		DrawLine(Vec2(val, -dim), Vec2(val, dim), color, 1.0);
		DrawLine(Vec2(-val, -dim), Vec2(-val, dim), color, 1.0);
	}
}

void OGLWidget::DrawCursor()
{
	QPoint windowPos = GLCoordToWindowCoord(m_cursorPos.x, m_cursorPos.y, m_cursorPos.z);
	int winx = windowPos.x();
	int winy = windowPos.y();

	if (winx > -16 && winy > -16 && winx < win_width - 16 && winy < win_height - 16)
	{
		Vec3 notchL1 = WindowCoordToGLCoord(winx - 4, winy);
		Vec3 notchL2 = WindowCoordToGLCoord(winx - 16, winy);
		Vec3 notchR1 = WindowCoordToGLCoord(winx + 4, winy);
		Vec3 notchR2 = WindowCoordToGLCoord(winx + 16, winy);
		Vec3 notchD1 = WindowCoordToGLCoord(winx, winy - 4);
		Vec3 notchD2 = WindowCoordToGLCoord(winx, winy - 16);
		Vec3 notchU1 = WindowCoordToGLCoord(winx, winy + 4);
		Vec3 notchU2 = WindowCoordToGLCoord(winx, winy + 16);

		DrawCircle(m_cursorPos, (notchR2.x - m_cursorPos.x)*0.8, Vec4(0.5, 0.5, 0.5, 1.0), 3.0);
		DrawLine(notchL1, notchL2, Vec4(0.0, 0.0, 0.0, 1.0), 2.0);
		DrawLine(notchR1, notchR2, Vec4(0.0, 0.0, 0.0, 1.0), 2.0);
		DrawLine(notchD1, notchD2, Vec4(0.0, 0.0, 0.0, 1.0), 2.0);
		DrawLine(notchU1, notchU2, Vec4(0.0, 0.0, 0.0, 1.0), 2.0);
	}
}

void OGLWidget::displayScene(int w, int h) {

	set_scene(GL_RENDER, w, h);
	glClearColor(m_backgroundColor.R, m_backgroundColor.G, m_backgroundColor.B, m_backgroundColor.Alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CHECK_GL_ERROR();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	glShadeModel(GL_FLAT);

	CHECK_GL_ERROR();

	if (gmainwindow->flag_drawNodeSphere || gmainwindow->flag_drawEdgeCylinder)
		updateLights();

	if (gmainwindow->flag_drawPlaneGrid)
		DrawPlaneGrid(10);

	if (gmainwindow->flag_drawPlaneAxes)
		DrawPlaneAxes();

	switch (viewStyle)
	{
		case VIEWSTYLE::POLYGON:
		{
			DrawERData2D();
			CHECK_GL_ERROR();
			if (m_selectType == SELECTTYPE::SELECTREGION)
				drawSelectRegion();
			CHECK_GL_ERROR();
			DrawLabels();
			break;
		}
		case VIEWSTYLE::COMPOSED:
		{
			DrawComposed2D();
			break;
		}
		case VIEWSTYLE::BIPARTITE:
		{
			DrawBipartite2D();
			break;
		}
		default:
			break;
	}
	
	if (flag_ShowLight)
		drawLights(lightsize);

	if (gmainwindow->flag_drawCursor)
		DrawCursor();

	CHECK_GL_ERROR();
}

void OGLWidget::getFitWindowSize(int &w, int &h, int mimdim) {

	currentERData->calculateBoundingCirle();
	double layout_w = currentERData->maxX - currentERData->minX;
	double layout_h = currentERData->maxY - currentERData->minY;
	double layout_aspectRatio = layout_w / layout_h;

	if (layout_aspectRatio > 1)
	{
		h = mimdim;
		w = h*layout_aspectRatio;
	}
	else {
		w = mimdim;
		h = w / layout_aspectRatio;
	}

	if (w % 8 != 0)
		w -= w % 8;
	if (h % 8 != 0)
		h -= h % 8;

	if (std::isinf(double(w)) || std::isinf(double(h)) || std::isnan(layout_aspectRatio)) {
		w = mimdim;
		h = mimdim;
	}
}

void OGLWidget::paintGL()
{
	CHECK_GL_ERROR();

	makeCurrent();

	CHECK_GL_ERROR();

	glEnable(GL_MULTISAMPLE);

	CHECK_GL_ERROR();

	displayScene(win_width, win_height);
}

void OGLWidget::saveToImage(const char * fName)
{
	QImage fimage = grabFramebuffer();
	QString imagePath(fName);
	QImage image(fimage.constBits(), fimage.width(), fimage.height(), QImage::Format_RGB32);
	image.save(imagePath); // writes image into path in PNG format

	if (gmainwindow->flag_saveImageAlpha)
	{
		image = QImage(fimage.constBits(), fimage.width(), fimage.height(), QImage::Format_ARGB32);
		for (int i = 0; i < image.width(); i++)
		{
			for (int j = 0; j < image.height(); j++)
			{
				if (i == 512 && j == 512)
					LogConsole::writeConsole("reached pixel");
				QColor c = image.pixelColor(i, j);
				Vec4 col = Vec4(c.red(), c.green(), c.blue(), c.alpha());
				if (col.Alpha > 56 && col.Alpha < 72)
				{
					col = (2.0 * col) - (m_backgroundColor * 255.0);
					c.setRed(col.x);
					c.setGreen(col.y);
					c.setBlue(col.z);
					c.setAlpha(col.w);
					image.setPixelColor(i, j, c);
				}
			}
		}
		imagePath.chop(4);
		imagePath.append("_transparent.png");
		image.save(imagePath);
	}

}

void OGLWidget::saveImageHtmlMap(std::string fname)
{
	std::ofstream file(fname);
	if (file.is_open())
	{
		GLdouble gl_mv_mat[16];
		GLdouble gl_pr_mat[16];
		glGetDoublev(GL_MODELVIEW_MATRIX, gl_mv_mat);
		glGetDoublev(GL_PROJECTION_MATRIX, gl_pr_mat);
		Mat4 mv_mat(gl_mv_mat);
		Mat4 pr_mat(gl_pr_mat);
		Mat4 mvp_mat = pr_mat * mv_mat;
		string name = fname.substr(fname.find_last_of("/") + 1);
		name = name.substr(0, name.find_last_of('.'));
		
		file << "<map name=\"" + name + "\">" << std::endl;

		for (Entity* en : currentERData->getEntities())
		{
			double rad = en->m_VertexSize;
			Vec4 pos(en->getLocation(), 1.0);
			Vec4 off = pos + Vec4(rad, 0.0, 0.0, 0.0);
			pos = mvp_mat * pos;
			off = mvp_mat * off;
			Vec2 win_pos = ((Vec2(pos.x, -pos.y) + 1.0) * 0.5);
			Vec2 win_off = ((Vec2(off.x, -off.y) + 1.0) * 0.5);
			Vec2 win_size(win_width, win_height);
			win_pos *= win_size;
			win_off *= win_size;
			double win_rad = length(win_off - win_pos);
			int pix_x = floor(win_pos.x);
			int pix_y = floor(win_pos.y);
			int pix_r = ceil(win_rad);

			file << "\t<area alt=\"" << en->getLabel() << "\" ";
			file << "title=\"" << en->getLabel() << "\" ";
			file << "coords=\"" << pix_x << "," << pix_y << "," << pix_r << "\" ";
			file << "shape=\"circle\">" << endl;
		}

		file << "</map>" << endl;
	}
}

void OGLWidget::encodeOneStep(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
	/*Encode the image*/
	unsigned error = lodepng_encode32_file(filename, image, width, height);

	/*if there's an error, display it*/
	if (error) 
		LogConsole::showError("error %u: %s\n", error, lodepng_error_text(error));
}

void OGLWidget::decodeOneStep(const char* filename, unsigned char **data, unsigned &width, unsigned &height)
{
	//decode
	std::vector<unsigned char> image;

	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error) 
		LogConsole::showError("error %u: %s\n", error, lodepng_error_text(error));

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

	*data = new unsigned char[width * height * 3];

	int idx;
	int idx_upsidedown;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			idx = j + i * width;
			idx_upsidedown = j + (height - 1 - i)*width;

			(*data)[3 * idx + 0] = image[4 * idx_upsidedown + 0];
			(*data)[3 * idx + 1] = image[4 * idx_upsidedown + 1];
			(*data)[3 * idx + 2] = image[4 * idx_upsidedown + 2];
		}
	}
}

void OGLWidget::initAntiA() {
	antiA_radius = .004;
	int gridWidth;
	double cellSize;
	double halfCellSize;
	int i;

	antiA_4[0] = (double *)malloc(4 * sizeof(double));
	antiA_4[1] = (double *)malloc(4 * sizeof(double));

	gridWidth = (int)sqrt((double)4);
	cellSize = 1.0 / gridWidth;
	halfCellSize = .5*cellSize;
	i = 0;
	for (int x = 0; x < gridWidth; x++) {
		double xSamp0 = ((double)x) / ((double)gridWidth);
		for (int y = 0; y < gridWidth; y++) {
			double ySamp = ((double)y) / ((double)gridWidth);
			double xSamp = xSamp0;

			double rndx = ((double)rand() / (double)RAND_MAX);
			double rndy = ((double)rand() / (double)RAND_MAX);
			rndx *= cellSize;
			rndy *= cellSize;

			xSamp -= .5;
			ySamp -= .5;

			xSamp += rndx;
			ySamp += rndy;

			antiA_4[0][i] = xSamp;
			antiA_4[1][i] = ySamp;

			i++;
		}
	}


	antiA_16[0] = (double *)malloc(16 * sizeof(double));
	antiA_16[1] = (double *)malloc(16 * sizeof(double));

	gridWidth = (int)sqrt((double)16);
	cellSize = 1.0 / gridWidth;
	halfCellSize = .5*cellSize;
	i = 0;
	for (int x = 0; x < gridWidth; x++) {
		double xSamp0 = ((double)x) / ((double)gridWidth);
		for (int y = 0; y < gridWidth; y++) {
			double ySamp = ((double)y) / ((double)gridWidth);
			double xSamp = xSamp0;

			double rndx = ((double)rand() / (double)RAND_MAX);
			double rndy = ((double)rand() / (double)RAND_MAX);
			rndx *= cellSize;
			rndy *= cellSize;

			xSamp -= .5;
			ySamp -= .5;

			xSamp += rndx;
			ySamp += rndy;

			antiA_16[0][i] = xSamp;
			antiA_16[1][i] = ySamp;

			i++;
		}
	}

	antiA_64[0] = (double *)malloc(64 * sizeof(double));
	antiA_64[1] = (double *)malloc(64 * sizeof(double));

	gridWidth = (int)sqrt((double)64);
	cellSize = 1.0 / gridWidth;
	halfCellSize = .5*cellSize;
	i = 0;
	for (int x = 0; x < gridWidth; x++) {
		double xSamp0 = ((double)x) / ((double)gridWidth);
		for (int y = 0; y < gridWidth; y++) {
			double ySamp = ((double)y) / ((double)gridWidth);
			double xSamp = xSamp0;

			double rndx = ((double)rand() / (double)RAND_MAX);
			double rndy = ((double)rand() / (double)RAND_MAX);
			rndx *= cellSize;
			rndy *= cellSize;

			xSamp -= .5;
			ySamp -= .5;

			xSamp += rndx;
			ySamp += rndy;

			antiA_64[0][i] = xSamp;
			antiA_64[1][i] = ySamp;

			i++;
		}
	}
}

void OGLWidget::freeAntiA() {
	free(antiA_4[0]);
	free(antiA_4[1]);

	free(antiA_16[0]);
	free(antiA_16[1]);

	free(antiA_64[0]);
	free(antiA_64[1]);
}

void OGLWidget::savePNGImage_fitAspectRatio(const char * fName, int mimdim)
{
	int w, h;
	getFitWindowSize(w, h, mimdim);

	makeCurrent();

	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
	format.setSamples(16);
	QOpenGLFramebufferObject * fbo = new QOpenGLFramebufferObject(w, h, format);
	fbo->bind();

	float old_translation[2];
	old_translation[0] = translation[0];
	old_translation[1] = translation[1];
	float old_zoom = zoom;
	
	double midX = currentERData->midX;
	double midY = currentERData->midY;
	
	double maxX = currentERData->maxX;
	double maxY = currentERData->maxY;
	
	double minX = currentERData->minX;
	double minY = currentERData->minY;
	
	translation[0] = -midX;
	translation[1] = -midY;
	
	double layout_aspectRatio = double(w) / double(h);
	double zoom1 = (maxX - midX) / layout_aspectRatio;
	double zoom2 = maxY - midY;
	
	zoom = max(zoom1, zoom2);
	if (zoom == 0)
		zoom = 1.0;

	displayScene(w, h);

	translation[0] = old_translation[0];
	translation[1] = old_translation[1];
	zoom = old_zoom;
	
	QImage fboImage(fbo->toImage());
	QImage image(fboImage.constBits(), fboImage.width(), fboImage.height(), QImage::Format_RGB32);
	
	image.save(fName);
	fbo->release();
	delete(fbo);

}

void OGLWidget::resizeGL(int w, int h)
{
	makeCurrent();

	glViewport(0, 0, w, h);

	CHECK_GL_ERROR();

	win_width = w;
	win_height = h;

	emit visSizeChanged(win_width, win_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double ratio = double(win_width) / double(win_height);
    setViewProjection(view_dimension, ratio, zoom);

    glTranslatef(translation[0], translation[1], -zoffset);
	multmatrix(rotmat);
}

void OGLWidget::setViewProjection(VIEWDIMENSION view_dimension, double ratio, double zoom)
{
    if (view_dimension == VIEWDIMENSION::TWO_D)
        glOrtho(-zoom*ratio, zoom*ratio, -zoom*1.0, zoom*1.0, -zoffset, zoffset);
    else if (view_dimension == VIEWDIMENSION::THREE_D)
        glOrtho(-zoom*ratio, zoom*ratio, -zoom*1.0, zoom*1.0, -1000, 1000);
}

void OGLWidget::drawGradentDirectionsForEntities()
{
	if (currentERData->getEntities().size() != gmainwindow->gradientDirections.size())
		return;

	if (gmainwindow->gradientDirections.empty())
		return;

	double scale = m_gradientArrowScale;

	for (int i = 0; i < currentERData->getEntities().size(); i++) {

		float x = currentERData->getEntity(i)->getLocation().x;
		float y = currentERData->getEntity(i)->getLocation().y;

		Vec2 dir = gmainwindow->gradientDirections[i];
		double len = length(dir);
		
		Vec2 target = Vec2(x, y) + dir*scale;
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(x, y, 0);
		glVertex3f(target.x, target.y, 0);
		glEnd();
	}
}

void OGLWidget::drawTestCircleLineIntersections() {

	if (circleLineIntersectionsTestPoints.size() == 0)
		return;
	CHECK_GL_ERROR();
	Vec4 color(1, 0, 0, 1);
	float x = circleLineIntersectionsTestPoints[0].x;
	float y = circleLineIntersectionsTestPoints[0].y;

	float r = length(circleLineIntersectionsTestPoints[0] - circleLineIntersectionsTestPoints[1]);

	DrawCircle(x, y, r, color);
	CHECK_GL_ERROR();
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(circleLineIntersectionsTestPoints[2].x, circleLineIntersectionsTestPoints[2].y, 0.0);
	glVertex3f(circleLineIntersectionsTestPoints[3].x, circleLineIntersectionsTestPoints[3].y, 0.0);
	glEnd();
	CHECK_GL_ERROR();
	Vec3 bc(0, 1, 0);
	glColor3f(bc.R, bc.G, bc.B);

	if (circleLineIntersectionsTestPoints.size() >= 5)
		DrawCross(circleLineIntersectionsTestPoints[4].x, circleLineIntersectionsTestPoints[4].y, r*0.05, bc);

	if(circleLineIntersectionsTestPoints.size()>=6)
		DrawCross(circleLineIntersectionsTestPoints[5].x, circleLineIntersectionsTestPoints[5].y, r*0.05, bc);

	CHECK_GL_ERROR();
}

void OGLWidget::initializeSelection()
{
	clearSelectEntityList();
	clearSelectRegion();
	clearSelectRelationshipList();
	clearSwapList();
	clearScaffoldList();
	selectedScaffoldIndex = -1;
}

void OGLWidget::showSelectedElementInfo()
{

	if (!infoDock)
		return;

	infoDock->clearConsole();

	for (auto e : selectedEntityList) {
		showEntityInformation(e);
		infoDock->appendSeparator();
	}

	for (auto r : selectedRelationshipList) {
		showRelationshipInformation(r);
		infoDock->appendSeparator();
	}
}

void OGLWidget::showEntityInformation(Entity* entity) {

	char text[1000];

	int index = entity->getIndex();
	string label = entity->getLabel();
	int degree = entity->getDegree();
	int cardinality = entity->getCardinality();
	double energy = 0;
	Vec3 loc = entity->getLocation();

	sprintf(text, "Entity: %d\nLabel: %s\nDegree: %d\nCardinality: %d\nloc:(%.8f, %.8f)\n",
		index, label.c_str(), degree, cardinality, loc.x, loc.y);

	infoDock->appendLine(text);

	if (entity->getDualRelationship()) {

		Relationship *dualElement = entity->getDualRelationship();

		int index = dualElement->getIndex();
		string label = dualElement->getLabel();
		int degree = dualElement->getDegree();
		int cardinality = dualElement->getCardinality();

		sprintf(text, "Dual: %d\nLabel: %s\nDegree: %d\nCardinality: %d\n",
			index, label.c_str(), degree, cardinality);
		infoDock->appendLine(text);

	}
	else {
		infoDock->appendLine("Dual: NULL");
	}

    int eNum = currentERData->getEntityNum();
    int rNum = currentERData->getRelationshipNum();
	infoDock->appendLine("|E|:%d\n|R|:%d\n|R|/|E|:%f\n", eNum, rNum, (double)rNum/ (double)eNum);

    vector<string>attr = entity->getAttributes();
    if (attr.size() > 0) {
        for (int i = 0; i < attr.size(); i++) {
			infoDock->appendLine("attr%d:%s", i, attr[i].c_str());
        }
    }

	infoDock->appendLine("--Adjacent Entities:");
	for (int i = 0; i < entity->getAdjacentEntities().size(); i++) {
		int cindex = entity->getAdjacentEntity(i)->getIndex();
		string label = entity->getAdjacentEntity(i)->getLabel();
		infoDock->appendLine("--index:%d, Label:%s", cindex, label.c_str());
	}

	infoDock->appendLine("\n--Inicdent Relationships:");
	for (int i = 0; i < entity->getIncidentRelationships().size(); i++) {
		int cindex = entity->getIncidentRelationship(i)->getIndex();
		string label = entity->getIncidentRelationship(i)->getLabel();
		if (label.size() > 200) {
			label = label.substr(0, 10);
			label += "...";
		}
		infoDock->appendLine("--index:%d, Label:%s", cindex, label.c_str());
	}
}

void OGLWidget::showRelationshipInformation(Relationship* relationship) {
	char text[10000];

	int index = relationship->getIndex();
	string label = relationship->getLabel();
	const char *name = label.c_str();
    Vec3 center = relationship->getCenter(relationship->getMonogonLength());
    Vec3 observer = relationship->getObserver();
	int degree = relationship->getDegree();
	int cardinality = relationship->getCardinality();
    int sumEDegree = relationship->getSumEntityDegree();
    double entanglement = relationship->entangleCoefficient;
	double energy = 0;
    double area = relationship->area();
    double perimeter = relationship->perimeter();
	double energy_area = relationship->energy_Area;             
	double energy_aspectratio = relationship->energy_AspectRatio;

	sprintf(text, "Relationship:%d\nlabel:%s\nDegree:%d\nCardinality:%d\nSum Entity Degree:%d\nArea:%f\nPerimeter:%f\nEnergy(Area):%.16f\nEnergy(Aspect Ratio):%.16f\n", 
		index, name, degree, cardinality, sumEDegree, area, perimeter ,energy_area, energy_aspectratio);
	infoDock->appendLine(text);
	infoDock->appendLine("center: %f, %f, %f", center.x, center.y, center.z);
	infoDock->appendLine("observer: %f, %f, %f\n", observer.x, observer.y, observer.z);

	if (relationship->getCardinality() == 1) {
		infoDock->appendLine("monogon: %dth", currentERData->getMonogonIndex(relationship));
		infoDock->appendLine("monogonSize: %f", relationship->getMonogonSize());
		infoDock->appendLine("monogonRotation: %f", relationship->getMonogonRotation());
		infoDock->appendLine("monogonInclined: %f\n", relationship->getMonogonInclinedAngle());
	}

	if (relationship->getDualEntity()) {

		Entity *dualElement = relationship->getDualEntity();

		int index = dualElement->getIndex();
		string label = dualElement->getLabel();
		const char *name = label.c_str();
		int degree = dualElement->getDegree();
		int cardinality = dualElement->getCardinality();

		sprintf(text, "Dual: %d\nLabel: %s\nDegree: %d\nCardinality: %d\n",
			index, name, degree, cardinality);
		infoDock->appendLine(text);

	}
	else {
		infoDock->appendLine("Dual: NULL");
	}

    int eNum = currentERData->getEntityNum();
    int rNum = currentERData->getRelationshipNum();
	infoDock->appendLine("|E|:%d\n|R|:%d\n|R|/|E|:%f\n", eNum, rNum, (double)rNum / (double)eNum);

	infoDock->appendLine("Entanglement:%f\n", entanglement);

	infoDock->appendLine("--Incident Entities:");
	for (int i = 0; i < relationship->getIncidentEntities().size(); i++) {
		int cindex = relationship->getIncidentEntity(i)->getIndex();
		string label = relationship->getIncidentEntity(i)->getLabel();
        Vec3 loc = relationship->getIncidentEntity(i)->getLocation();
		const char *cname = label.c_str();
		infoDock->appendLine("--index:%d, Label:%s", cindex, cname);
		infoDock->appendLine("(%f, %f)", loc.x, loc.y);
	}

	infoDock->appendLine("\n--Adjacent Relationships:");
	for (int i = 0; i < relationship->getAdjacentRelationships().size(); i++) {
		int cindex = relationship->getAdjacentRelationship(i)->getIndex();
		string label = relationship->getAdjacentRelationship(i)->getLabel();
		const char *cname = label.c_str();
		infoDock->appendLine("--index:%d, Label:%s", cindex, cname);
	}

	if (relationship->getCardinality() < 3) {
		infoDock->appendLine("\n--Scaffold Vertices:");
		for (int i = 0; i < relationship->getScaffoldList()->size(); i++) {
			infoDock->appendLine("\n--Scaffold:%d:", i);
			for (int j = 0; j < relationship->getScaffoldList()->at(i).m_CurvedLocation.size(); j++) {
				infoDock->appendLine("--(%.8f, %.8f, %.8f)", relationship->getScaffoldList()->at(i).m_CurvedLocation[j].a, relationship->getScaffoldList()->at(i).m_CurvedLocation[j].b, relationship->getScaffoldList()->at(i).m_CurvedLocation[j].c);
			}
		}
	}
}

QString OGLWidget::createTipContent(Entity * en)
{
	return QString("Node:%1\nLabel:%2\nDegree:%3\nCard:%4").arg(en->getIndex()).arg(en->getLabel().c_str()).arg(en->getDegree()).arg(en->getCardinality());
}

QString OGLWidget::createTipContent(Relationship * re)
{
	return QString("Relationship:%1\nLabel:%2\nDegree:%3\nCard:%4").arg(re->getIndex()).arg(re->getLabel().c_str()).arg(re->getDegree()).arg(re->getCardinality());
}

void OGLWidget::updateMouseOpenGLCoordinates(int x, int y)
{
	makeCurrent();

	GLint viewport[4];					//var to hold the viewport info
	GLdouble modelview[16];				//var to hold the modelview info
	GLdouble projection[16];			//var to hold the projection matrix info
	GLfloat winX, winY, winZ;			//variables to hold screen x,y,z coordinates
	GLdouble worldX, worldY, worldZ;	//variables to hold world x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	winZ = 0;

	//get the world coordinates from the screen coordinates
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);

	emit mouseLocChanged(worldX, worldY);
}

void OGLWidget::contextMenuEvent(QContextMenuEvent* event)
{
	QPoint wpos = event->pos();
	QPoint gpos = event->globalPos();
	QMenu menu;
	menu.addAction("Move Cursor Here");
	if (!selectedEntityList.empty())
		menu.addAction("Fix Entities");
	if (!selectedRelationshipList.empty())
	{
		menu.addAction("Fix Relationships");
		menu.addAction("Create Cluster");
	}

	QAction* action = menu.exec(gpos);
	if (action)
	{
		if (action->text() == "Move Cursor Here")
			m_cursorPos = WindowCoordToGLCoord(wpos.x(), wpos.y());
		else if (action->text() == "Fix Entities")
			gmainwindow->fixSelectedVerts();
		else if (action->text() == "Fix Relationhsips")
			LogConsole::writeConsole("Implement Fixing Relationships");
		else if (action->text() == "Create Cluster")
			gmainwindow->createCluster(selectedEntityList,selectedRelationshipList);
		
		if (action->text() != "Move Cursor Here")
		{
			removeSelectedEntities();
			removeSelectedRelationships();
			emit selectedElementsChanged(widgetType);
		}
	}
	
	update();
}

void OGLWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
	if (e->button() == Qt::LeftButton)
	{

		if (m_mouseAction == MOUSEACTION::SELECT) {
			if (m_selectType == SELECTTYPE::SELECTREGION) {
				m_selectState = SELECTSTATE::FINISHED;
				pick_EntitiesInsideSelectRegion();
				emit selectedElementsChanged(widgetType);
			}
			else if (m_selectType == SELECTTYPE::SELECTENTITY || 
					 m_selectType == SELECTTYPE::SELECTRELATIONSHIP) 
			{
				removeSelectedEntities();
				removeSelectedRelationships();
				emit selectedElementsChanged(widgetType);
			}
		}
		if (m_mouseAction == MOUSEACTION::MOVEELEMENTS) {
			m_selectState = SELECTSTATE::FINISHED;
			clearSelectRegion();
			removeSelectedEntities();
			removeSelectedRelationships();
		}
	}

	if (e->button() == Qt::RightButton)
	{
        m_selectState = SELECTSTATE::FINISHED;
        clearSelectRegion();
        removeSelectedEntities();
        removeSelectedRelationships();
        clearSwapList();
        clearConvexHull();
        emit selectedElementsChanged(widgetType);
	}
	update();
}

void OGLWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
	m_pressPos = event->pos();
	m_pressGlobalPos = event->globalPos();
	record_mouse_pos(m_lastPos.x(), m_lastPos.y());

	float clickx = (float)m_lastPos.x() / win_width - 0.5;
	float clicky = (win_height - 1 - (float)m_lastPos.y()) / win_height - 0.5;

	double ratio = double(win_width) / double(win_height);
	clickx = 2.0*clickx*zoom*ratio;
	clicky = 2.0*clicky*zoom;

	clickx -= translation[0];
	clicky -= translation[1];

	if (event->buttons() == Qt::LeftButton && (view_dimension == VIEWDIMENSION::TWO_D))
	{
		//control + select to multiple select elements
		if ((QGuiApplication::keyboardModifiers() & Qt::ControlModifier) && m_mouseAction == MOUSEACTION::SELECT)
		{
			if (m_selectType == SELECTTYPE::SELECTREGION) {

				if (m_selectState == SELECTSTATE::FINISHED) {
					clearSelectRegion();
					m_selectState = SELECTSTATE::SELECTING;
				}

				Vec3 v = Vec3(clickx, clicky, 0);
				selectionRegion.push_back(v);
			}
			else if (m_selectType == SELECTTYPE::SELECTENTITY) {
				int cur_selectedEnitity = pick_Entity(m_lastPos.x(), m_lastPos.y());
				if (cur_selectedEnitity != -1) {
					addSelectEntityList(currentERData->getEntity(cur_selectedEnitity));
					emit selectedElementsChanged(widgetType);
				}

			}
			else if (m_selectType == SELECTTYPE::SELECTRELATIONSHIP) {
				int cur_selectedRelationship = pick_Relationship(m_lastPos.x(), m_lastPos.y());
				if (cur_selectedRelationship != -1) {
					addSelectRelationshipList(currentERData->getRelationship(cur_selectedRelationship));
					emit selectedElementsChanged(widgetType);
				}
			}
		}
		//alt + select to select entities for special actions
		else if ((QGuiApplication::keyboardModifiers() & Qt::AltModifier) && m_mouseAction == MOUSEACTION::SELECT)
		{
			if (m_selectType == SELECTTYPE::SELECTRELATIONSHIP) {

				if (getSelectedRelationships().size() != 1) {
					return;
				}
				int cur_selectedEnitity = pick_Entity(m_lastPos.x(), m_lastPos.y());

				if (cur_selectedEnitity != -1) {
					addEntityToSwapList(currentERData->getEntity(cur_selectedEnitity));

				}
			}
		}
		//Unique selection and move
		else if (m_mouseAction == MOUSEACTION::SELECT) {

			if (m_selectType == SELECTTYPE::SELECTREGION && (QGuiApplication::keyboardModifiers() != Qt::ShiftModifier)) {

				if (m_selectState == SELECTSTATE::FINISHED ) {
					clearSelectRegion();
					removeSelectedEntities();
					removeSelectedRelationships();
					m_selectState = SELECTSTATE::SELECTING;
				}

				Vec3 v = Vec3(clickx, clicky, 0);
				selectionRegion.push_back(v);
			}
			else if (m_selectType == SELECTTYPE::SELECTENTITY) {
				int cur_selectedEnitity = pick_Entity(m_lastPos.x(), m_lastPos.y());
				if (cur_selectedEnitity != -1) {
					selectSingleEntity(currentERData->getEntity(cur_selectedEnitity));
					emit selectedElementsChanged(widgetType);
				}

			}
			else if (m_selectType == SELECTTYPE::SELECTRELATIONSHIP) {
				int cur_selectedRelationship = pick_Relationship(m_lastPos.x(), m_lastPos.y());
				if (cur_selectedRelationship != -1) {
					selectSingleRelationship(currentERData->getRelationship(cur_selectedRelationship));
					emit selectedElementsChanged(widgetType);
				}
			}
			else if (m_selectType == SELECTTYPE::SELECT_SCAFFOLD) {
				selectedScaffoldIndex = pick_Scaffold(m_lastPos.x(), m_lastPos.y());
			}
		}

		if (m_mouseAction == MOUSEACTION::ADDENTITY) {
			Entity* newEn = currentERData->addEntity(Vec3(clickx, clicky, 0));
			newEn->setLabelWithIndex();
			gmainwindow->ERDataChanged();
		}
	}

	updateMouseOpenGLCoordinates(m_lastPos.x(), m_lastPos.y());

	update();
}

void OGLWidget::mouseReleaseEvent(QMouseEvent * event) {

	m_lastPos = event->pos();

	float clickx = (float)m_lastPos.x() / win_width - 0.5;
	float clicky = (win_height - 1 - (float)m_lastPos.y()) / win_height - 0.5;

	double ratio = double(win_width) / double(win_height);
	clickx = 2.0*clickx*zoom*ratio;
	clicky = 2.0*clicky*zoom;

	clickx -= translation[0];
	clicky -= translation[1];

	update();
}

void OGLWidget::mouseMoveEvent(QMouseEvent *event)
{

	if (flag_showEntityTips || flag_showRelationshipTips) {
		if (flag_showRelationshipTips) {
			int cur_selectedRelationship = pick_Relationship(event->pos().x(), event->pos().y(), false);
			if (cur_selectedRelationship != -1) {
				startToolTips(createTipContent(currentERData->getRelationship(cur_selectedRelationship)), event->pos());
				update();
			}
		}

		if (flag_showEntityTips) {
			int cur_selectedEnitity = pick_Entity(event->pos().x(), event->pos().y(), false);
			if (cur_selectedEnitity != -1) {
				startToolTips(createTipContent(currentERData->getEntity(cur_selectedEnitity)), event->pos());
				update();
			}
		}
	}

	if (event->buttons() == Qt::MiddleButton)
	{
		int dx = event->pos().x() - m_lastPos.x();
		int dy = event->pos().y() - m_lastPos.y();
		int ddx = event->pos().x() - m_pressPos.x();
		int ddy = event->pos().y() - m_pressPos.y();

		m_lastPos = event->pos();

		updateMouseOpenGLCoordinates(event->pos().x(), event->pos().y());

		update_translation(m_lastPos.x(), m_lastPos.y());
		update();

	}
	else if (event->buttons() == Qt::LeftButton) {
		int dx = event->pos().x() - m_lastPos.x();
		int dy = event->pos().y() - m_lastPos.y();
		int ddx = event->pos().x() - m_pressPos.x();
		int ddy = event->pos().y() - m_pressPos.y();

		m_lastPos = event->pos();

		float clickx = (float)m_lastPos.x() / win_width - 0.5;
		float clicky = (win_height - 1 - (float)m_lastPos.y()) / win_height - 0.5;

		double ratio = double(win_width) / double(win_height);
		clickx = 2.0*clickx*zoom*ratio;
		clicky = 2.0*clicky*zoom;

		clickx -= translation[0];
		clicky -= translation[1];

		updateMouseOpenGLCoordinates(event->pos().x(), event->pos().y());

		if (view_dimension == VIEWDIMENSION::TWO_D) {
			if (m_mouseAction == MOUSEACTION::SELECT) {

				int newx = event->pos().x();
				int newy = event->pos().y();

				if (m_pressPos.x() == newx && m_pressPos.y() == newy)
					return;

				if ((m_selectType == SELECTTYPE::SELECTENTITY || m_selectType == SELECTTYPE::SELECTREGION) && !m_selectState == SELECTSTATE::SELECTING)
				{
					for (Entity* en : selectedEntityList)
						moveEntity(en, dx, dy);
				}
				
				if (m_selectType == SELECTTYPE::SELECTRELATIONSHIP)
				{
					for (Relationship* re : selectedRelationshipList)
						moveRelationship(re, dx, dy);
				}

				if (m_selectType == SELECTTYPE::SELECT_SCAFFOLD) 
				{
					if (selectedScaffoldIndex != -1)
						moveScaffold(scaffoldList[selectedScaffoldIndex], m_lastPos.x(), m_lastPos.y());
				}

			}
			else if (m_mouseAction == MOUSEACTION::MOVEELEMENTS ||
					(m_mouseAction == MOUSEACTION::SELECT &&
					m_selectType == SELECTTYPE::SELECTREGION &&
					QGuiApplication::keyboardModifiers() & Qt::ShiftModifier))
			{

				if (view_dimension == VIEWDIMENSION::TWO_D) {
					int newx = event->pos().x();
					int newy = event->pos().y();

					if (m_pressPos.x() == newx && m_pressPos.y() == newy)
						return;

					set<Entity*> enlis;
					set<Relationship*> relis;
					for (unsigned int i = 0; i < selectedEntityList.size(); i++) {
						Entity *en = selectedEntityList[i];
						enlis.insert(en);

						for (unsigned int j = 0; j < en->getIncidentRelationships().size(); j++) {
							Relationship *temp_r = en->getIncidentRelationship(j);
							relis.insert(temp_r);
						}
					}

					for (unsigned int i = 0; i < selectedRelationshipList.size(); i++) {
						Relationship *temp_r = selectedRelationshipList[i];

						relis.insert(temp_r);
						for (unsigned int j = 0; j < temp_r->getAdjacentRelationships().size(); j++) {
							relis.insert(temp_r->getAdjacentRelationship(j));
						}

						for (unsigned int j = 0; j < temp_r->getIncidentEntities().size(); j++) {
							Entity *en = temp_r->getIncidentEntity(j);
							enlis.insert(en);
						}
					}

					if (selectedEntityList.size() == 0 && selectedRelationshipList.size() == 0) {
						for (auto en : currentERData->getEntities()) {
							enlis.insert(en);
						}

						for (auto re : currentERData->getRelationships()) {
							relis.insert(re);
						}
					}

					for (set<Entity*>::iterator it = enlis.begin(); it != enlis.end(); ++it) {
						setEntityLocation(*it, dx, dy);
					}

					for (set<Relationship*>::iterator it = relis.begin(); it != relis.end(); ++it) {
						Relationship *r = *it;
						r->update2D();
					}
				}
			}
		}
		else if (view_dimension == VIEWDIMENSION::THREE_D) {
			//rotation in 3D
			update_orientation(m_lastPos.x(), m_lastPos.y());
		}

		update();
	}

}

void OGLWidget::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void OGLWidget::dropEvent(QDropEvent *event)
{
	QString pathOftext(event->mimeData()->text());

	QStringList outList = pathOftext.split(QChar('\n'));

	for (int i = 0; i < outList.size(); i++)
	{
		QString currentPath = outList.at(i);

		currentPath.remove(0, 8);

		string str = currentPath.toStdString();

		if (!str.size())
			return;

		const char* path = str.c_str();

		QFileInfo fi(currentPath);
		QString folder_name = fi.path();
		gmainwindow->m_PATH = folder_name.toStdString();

		if (currentPath.contains(QString(".er"), Qt::CaseSensitivity::CaseInsensitive))
		{
			if (widgetType == VISTYPE::PRIMEVIS) {
				if (gmainwindow->OpenERFile(path, this))
				{
					initializeSelection();
					reset_sceneToCenterVisualization();
				}
			}
			else if (widgetType == VISTYPE::DUALVIS) 
			{	
				if (FileIO::readERFileLayout(path, currentERData))
					reset_sceneToCenterVisualization();
			}

			update();
		}
        if (currentPath.contains(QString(".txt"), Qt::CaseSensitivity::CaseInsensitive) || currentPath.contains(QString(".view"), Qt::CaseSensitivity::CaseInsensitive))
        {
            bool res =openTransformation(path);
            if (res)
                LogConsole::writeConsole("view point is loaded");
            update();
        }

		if (currentPath.contains(QString(".PolyColors"), Qt::CaseSensitivity::CaseInsensitive)) {
			gmainwindow->OpenColorScheme(path);
			update();
		}
		
	}


	event->acceptProposedAction();
}

void OGLWidget::focusOutEvent(QFocusEvent * event)
{
}

void OGLWidget::focusInEvent(QFocusEvent * event)
{
	emit changeFocusedVis(widgetType);
}

void OGLWidget::addEntityToSwapList(Entity *e) {

	if (getSelectedRelationships().size() != 1) {
		LogConsole::writeConsole("please select one relationship at first");
		return;
	}

	int incident = isInsideList<Entity>(e, getSelectedRelationship(0)->getIncidentEntities());
	if(incident == -1){
		LogConsole::writeConsole("can only select the entities inside the selected relationship");
		return;
	}
	int inside = isInsideList<Entity>(e, swapList);
	if (inside != -1) {
		swapList[inside]->setSwapFlag(false);
		swapList.erase(swapList.begin() + inside);
	}
	else {
		e->setSwapFlag(true);
		swapList.push_back(e);
	}
}

void OGLWidget::clearScaffoldList()
{
	scaffoldList.clear();
}

void OGLWidget::clearConvexHull()
{
	convexHull.clear();
}

void OGLWidget::setConvexHull(vector<Vec3>Poitlist)
{
	convexHull = Poitlist;
}

int OGLWidget::pick_Entity(int x, int y, bool showInfo) {

	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	glViewport(0, 0, win_width, win_height);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuf);
	(void)glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 10.0, 10.0, viewport);

	double ratio = double(win_width) / double(win_height);
    setViewProjection(view_dimension, ratio, zoom);
	glTranslatef(translation[0], translation[1], -zoffset);

	displayEntities(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFlush();

	hits = glRenderMode(GL_RENDER);

	int seed_id = processHits(hits, selectBuf);

	if (seed_id != -1 && showInfo) {
		LogConsole::writeConsole("Selected Entity %d", seed_id);
	}

	return seed_id;

}

int OGLWidget::pick_Relationship(int x, int y, bool showInfo) {

	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	glViewport(0, 0, win_width, win_height);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuf);
	(void)glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 10.0, 10.0, viewport);

	double ratio = double(win_width) / double(win_height);
    setViewProjection(view_dimension, ratio, zoom);

	glTranslatef(translation[0], translation[1], -zoffset);

	displayRelationships(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFlush();

	hits = glRenderMode(GL_RENDER);

	int seed_id = processHits(hits, selectBuf);

	if (seed_id != -1 && showInfo) {
		LogConsole::writeConsole("Selected Relationship %d", seed_id);
	}

	return seed_id;
}

void OGLWidget::pick_EntitiesInsideSelectRegion() {

	for (int i = 0; i < currentERData->getEntityNum(); i++) {

		Entity * en = currentERData->getEntity(i);
		Vec3 loc = en->getLocation();
		bool res = isPointInsidePolygon(loc, selectionRegion);
		if (res) {
			addSelectEntityList(en);
		}
	}
}

int OGLWidget::pick_Scaffold(int x, int y)
{
	GLuint selectBuf[BUFSIZE];
	GLint hits;
	GLint viewport[4];

	glViewport(0, 0, win_width, win_height);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glSelectBuffer(BUFSIZE, selectBuf);
	(void)glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 10.0, 10.0, viewport);

	double ratio = double(win_width) / double(win_height);
    setViewProjection(view_dimension, ratio, zoom);

	glTranslatef(translation[0], translation[1], -zoffset);

	displayScaffolds(GL_SELECT);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glFlush();

	hits = glRenderMode(GL_RENDER);

	int seed_id = processHits(hits, selectBuf);

	if (seed_id != -1) {
		LogConsole::writeConsole("Selected Scaffold %d", seed_id);
	}

	return seed_id;
}

void OGLWidget::displayEntities(GLenum mode) {

	if (mode == GL_SELECT) {

		glColor3f(1.0, 0.0, 0.0);
		glPointSize(10);
		for (int i = 0; i < currentERData->getEntityNum(); i++)
		{
			Entity* en = currentERData->getEntity(i);

			if (!en->getVisibility())
				continue;

			glLoadName(i);

            draw2D(en);
		}
		glFinish();
	}
}

void OGLWidget::displayRelationships(GLenum mode) {

	if (mode == GL_SELECT) {

		for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
			Relationship *temp_r = currentERData->getRelationship(i);

			if (!temp_r->getVisibility())
				continue;

			glLoadName(i);

			draw2D(temp_r);
			glFinish();
		}
	}
}

void OGLWidget::displayScaffolds(GLenum mode)
{
	if (mode == GL_SELECT) {

		for (int i = 0; i < scaffoldList.size(); i++) {
			Scaffold *s = scaffoldList[i];

			glLoadName(i);
			drawScaffold(s, Vec4(1.0f, 0.0f, 1.0f, 1.0), 2);
			glFinish();
		}

	}else{
	
		for (int i = 0; i < scaffoldList.size(); i++) {
			Scaffold *s = scaffoldList[i];
			bool selected = (i == selectedScaffoldIndex ? true : false);
			drawScaffold(s, Vec4(1.0f, 0.0f, 1.0f, 1.0), 2);
			glFinish();
		}
	}
}

void OGLWidget::updateScaffoldList()
{
	for (int i = 0; i < currentERData->getRelationshipNum(); i++) {
		Relationship *temp_r = currentERData->getRelationship(i);

		if (!temp_r->getVisibility())
			continue;

		vector<Scaffold>* scaffoldlist = temp_r->getScaffoldList();
		for (int i = 0; i < (*scaffoldlist).size(); i++) {
			scaffoldList.push_back(&(*scaffoldlist)[i]);
		}
	}
}

int OGLWidget::processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint names, *ptr, minZ, *ptrNames, numberOfNames;
	numberOfNames = 0;
	int seed_id = -1;
	ptr = (GLuint *)buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) {
		names = *ptr;
		ptr++;
		if (*ptr <= minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr + 2;
		}

		ptr += names + 2;
	}

	ptr = ptrNames;
	for (j = 0; j < numberOfNames; j++, ptr++) {
		seed_id = *ptr;
	}

	return seed_id;
}

void OGLWidget::wheelEvent(QWheelEvent *event) {
	QPoint numDegrees = event->angleDelta();
	double d = -0.1*numDegrees.ry()/360.0;
	zoom *= exp(d);
	update();
}

void OGLWidget::moveEntity(Entity *en, int dx, int dy) {

	makeCurrent();

	GLint viewport[4];					//var to hold the viewport info
	GLdouble modelview[16];				//var to hold the modelview info
	GLdouble projection[16];			//var to hold the projection matrix info
	GLfloat winX, winY, winZ;			//variables to hold screen x,y,z coordinates
	GLdouble worldX, worldY, worldZ;	//variables to hold world x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	winX = (float)100 + dx;
	winY = (float)viewport[3] - (100 + (float)dy);
	winZ = 0;

	GLfloat oldwinx = 100;
	GLfloat oldwiny = (float)viewport[3] - 100;
	GLdouble oldworldX, oldworldY, oldworldZ;	//variables to hold world x,y,z coordinates

												////get the world coordinates from the screen coordinates
	gluUnProject(oldwinx, oldwiny, 0, modelview, projection, viewport, &oldworldX, &oldworldY, &oldworldZ);
	gluUnProject(winX, winY, 0, modelview, projection, viewport, &worldX, &worldY, &worldZ);

	double ddx = worldX - oldworldX;
	double ddy = worldY - oldworldY;

	Vec3 new_loc = en->getLocation() + Vec3(ddx, ddy, 0);
	en->moveToLocation(new_loc.x, new_loc.y);

	if (flag_drawClusters && m_bcdecomp)
	{
		for (Relationship* re : en->getIncidentRelationships())
		{
			Cluster* cluster = m_bcdecomp->getElemCluster(re->getIndex(), RELATIONSHIP);
			if (!cluster)
				continue;
			m_bcdecomp->updateClusterLayout(cluster);
			
			//ClusterLayout* layout = c->getLayout();
			//if (!layout)
			//	continue;

			//Vec3 center = layout->center;
			//double new_radius = length(new_loc - center);
			//m_bcdecomp->scaleClusterLayout(c, new_radius);
		}
	}
}

void OGLWidget::setEntityLocation(Entity *en, int dx, int dy) {

    makeCurrent();

    GLint viewport[4];					//var to hold the viewport info
    GLdouble modelview[16];				//var to hold the modelview info
    GLdouble projection[16];			//var to hold the projection matrix info
    GLfloat winX, winY, winZ;			//variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ;	//variables to hold world x,y,z coordinates

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
    glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    winX = (float)100 + dx;
    winY = (float)viewport[3] - (100 + (float)dy);
    winZ = 0;

    GLfloat oldwinx = 100;
    GLfloat oldwiny = (float)viewport[3] - 100;
    GLdouble oldworldX, oldworldY, oldworldZ;	//variables to hold world x,y,z coordinates

                                                ////get the world coordinates from the screen coordinates
    gluUnProject(oldwinx, oldwiny, 0, modelview, projection, viewport, &oldworldX, &oldworldY, &oldworldZ);
    gluUnProject(winX, winY, 0, modelview, projection, viewport, &worldX, &worldY, &worldZ);

    double ddx = worldX - oldworldX;
    double ddy = worldY - oldworldY;

    en->setLocation(en->getLocation().a + ddx, en->getLocation().b + ddy);
}

void OGLWidget::moveScaffold(Scaffold * s, int x, int y)
{
	makeCurrent();

	GLint viewport[4];					//var to hold the viewport info
	GLdouble modelview[16];				//var to hold the modelview info
	GLdouble projection[16];			//var to hold the projection matrix info
	GLfloat winX, winY, winZ;			//variables to hold screen x,y,z coordinates
	GLdouble worldX, worldY, worldZ;	//variables to hold world x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	winZ = 0;

	//get the world coordinates from the screen coordinates
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
	s->ControlPoint = Vec3(worldX, worldY, 0.0);

	vector<Vec3> currentListToAdd;
	currentListToAdd.push_back(s->m_CurvedLocation.front());
	currentListToAdd.push_back(s->ControlPoint);
	currentListToAdd.push_back(s->m_CurvedLocation.back());
	UpdateBezierLocation(currentListToAdd);
	s->m_CurvedLocation = currentListToAdd;
}

void OGLWidget::moveRelationship(Relationship* re, int dx, int dy) {

	makeCurrent();

	GLint viewport[4];					//var to hold the viewport info
	GLdouble modelview[16];				//var to hold the modelview info
	GLdouble projection[16];			//var to hold the projection matrix info
	GLfloat winX, winY, winZ;			//variables to hold screen x,y,z coordinates
	GLdouble worldX, worldY, worldZ;	//variables to hold world x,y,z coordinates

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
	glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
	glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

	winX = (float)100 + dx;
	winY = (float)viewport[3] - (100 + (float)dy);
	winZ = 0;

	GLfloat oldwinx = 100;
	GLfloat oldwiny = (float)viewport[3] - 100;
	GLdouble oldworldX, oldworldY, oldworldZ;	//variables to hold world x,y,z coordinates

												////get the world coordinates from the screen coordinates
	gluUnProject(oldwinx, oldwiny, 0, modelview, projection, viewport, &oldworldX, &oldworldY, &oldworldZ);
	gluUnProject(winX, winY, 0, modelview, projection, viewport, &worldX, &worldY, &worldZ);

	double ddx = worldX - oldworldX;
	double ddy = worldY - oldworldY;

	unordered_set<Entity*> enlist;
	unordered_set<Relationship*> relist;

	// check if re is in a cluster
	ClusterLayout* layout = NULL;
	Cluster* cluster = NULL;
	if (flag_drawClusters && m_bcdecomp)
	{
		cluster = m_bcdecomp->getElemCluster(re->getIndex(), RELATIONSHIP);
		if (cluster)
			layout = cluster->getLayout();
	}

	// if re is in a cluster that has a layout, move the whole cluster
	if (cluster && layout)
	{
		for (Relationship* c_re : m_bcdecomp->getSubgraphRelationships(cluster, ELEM_NONE))
		{
			relist.insert(c_re);
			for (Relationship* c_adj : c_re->getAdjacentRelationships())
				relist.insert(c_adj);

			for (Entity* en : c_re->getIncidentEntities())
				enlist.insert(en);
		}

		layout->center += Vec3(ddx, ddy, 0);
	}
	else	// otherwise, just move the relationship
	{
		relist.insert(re);
		for (Relationship* adj : re->getAdjacentRelationships())
			relist.insert(adj);

		for (Entity* en : re->getIncidentEntities())
			enlist.insert(en);
	}

	for (Entity* en : enlist)
	{
		Vec3 loc = en->getLocation();
		loc.x += ddx; loc.y += ddy;
		en->setLocation(loc);
	}

	for (Relationship* re : relist)
		re->update2D();
}

void OGLWidget::record_mouse_pos(int x, int y)
{
	last_s = (2.0 * x - win_width) / win_width;
	last_t = (2.0 * (win_height - y) - win_height) / win_height;
}

void OGLWidget::update_translation(int x, int y)
{
	float s, t;

	s = (2.0 * x - win_width) / win_width;
	t = (2.0 * (win_height - y) - win_height) / win_height;

	if (s == last_s && t == last_t)
		return;

	translation[0] += (s - last_s)*zoom;
	translation[1] += (t - last_t)*zoom;

	last_s = s;
	last_t = t;
}

void OGLWidget::update_orientation(int x, int y) {

	float s, t;
	float r[4];
	Quaterniont rvec;

	s = (2.0 * x - win_width) / win_width;
	t = (2.0 * (win_height - y) - win_height) / win_height;

	if (s == last_s && t == last_t)
		return;

	mat_to_quat(rotmat, rvec);
	trackball(r, last_s, last_t, s, t);
	add_quats(r, rvec, rvec);
	quat_to_mat(rvec, rotmat);

	last_s = s;
	last_t = t;

}

void OGLWidget::update_zoom(int x, int y)
{
	float s, t;

	s = (2.0 * x - win_width) / win_width;
	t = (2.0 * (win_height - y) - win_height) / win_height;

	if (s == last_s && t == last_t)
		return;

	zoom *= exp(2.0*(t - last_t));

	last_s = s;
	last_t = t;
}

void OGLWidget::set_scene(GLenum mode, int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	if (mode == GL_RENDER)
		glLoadIdentity();

	double ratio = double(w) / double(h);

    setViewProjection(view_dimension, ratio, zoom);

	glMatrixMode(GL_MODELVIEW);
	if (mode == GL_RENDER)
		gl->glLoadIdentity();

	glTranslatef(translation[0], translation[1], -zoffset);
	multmatrix(rotmat);
}

void OGLWidget::reset_scene() {
	translation[0] = translation[1] = 0.0;
	mat_ident(rotmat);
	zoom = 1.0;
}

/*align the right bottom of the vis to the right bottom of the window*/
void OGLWidget::reset_sceneToCenterVisualization(bool alignRightBottom)
{
	currentERData->calculateBoundingCirle();

	double lengthx = currentERData->maxX - currentERData->minX;
	double lengthy = currentERData->maxY - currentERData->minY;

	double midX = currentERData->midX;
	double midY = currentERData->midY;

	double maxX = currentERData->maxX;
	double maxY = currentERData->maxY;

	double minX = currentERData->minX;
	double minY = currentERData->minY;

	translation[0] = -midX;
	translation[1] = -midY;

	mat_ident(rotmat);

    double ratio = double(win_width) / double(win_height);
    double zoom1 = (maxX - midX) / ratio;
	double zoom2 = maxY - midY;

	zoom = max(zoom1, zoom2);

	if (alignRightBottom) {

		/*make maxx zoom*ratio, make miny -zoom*/
		double offsetX =  zoom*ratio - (maxX - midX);
		double offsetY =  -zoom - (minY - midY);

		translation[0] += offsetX;
		translation[1] += offsetY;
	}
}

void OGLWidget::reset_sceneToContent(double minX, double minY, double maxX, double maxY)
{
	double midX, midY;

	maxX += 2 * Element::m_VertexSize;
	maxY += 2 * Element::m_VertexSize;
	minX -= 2 * Element::m_VertexSize;
	minY -= 2 * Element::m_VertexSize;

	midX = (maxX + minX) / 2;
	midY = (maxY + minY) / 2;

	translation[0] = -midX;
	translation[1] = -midY;

	mat_ident(rotmat);
    
    double ratio = double(win_width) / double(win_height);
	double zoom1 = (maxX - midX)/ratio;
	double zoom2 = maxY - midY;

	zoom = max(zoom1, zoom2);
}

void OGLWidget::updateLights()
{
	if (light0.enabled)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	
	if (light1.enabled)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0.diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0.ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0.specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0.position);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1.diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1.ambient);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1.specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1.position);


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void OGLWidget::initLights()
{

	/*light 0*/
	light0.enabled = true;
	light0.diffuse[0] = 0.3f;
	light0.diffuse[1] = 0.3f;
	light0.diffuse[2] = 0.3f;
	light0.diffuse[3] = 0.3f;
		 
	light0.ambient[0] = 0.3f;
	light0.ambient[1] = 0.3f;
	light0.ambient[2] = 0.3f;
	light0.ambient[3] = 0.3f;
		 
	light0.specular[0] = .65f;
	light0.specular[1] = .65f;
	light0.specular[2] = .65f;
	light0.specular[3] = .65f;
		 
	light0.position[0] = 10.00;
	light0.position[1] = 10.00;
	light0.position[2] = 10.00;
	light0.position[3] = 0.0f;


	/*light 1*/
	light1.enabled = true;
	light1.diffuse[0] = 0.3f;
	light1.diffuse[1] = 0.3f;
	light1.diffuse[2] = 0.3f;
	light1.diffuse[3] = 0.2f;

	light1.ambient[0] = 0.2f;
	light1.ambient[1] = 0.2f;
	light1.ambient[2] = 0.2f;
	light1.ambient[3] = 0.2f;

	light1.specular[0] = 0.3f;
	light1.specular[1] = 0.3f;
	light1.specular[2] = 0.3f;
	light1.specular[3] = 0.3f;

	light1.position[0] = 10.00;
	light1.position[1] = -10.00;
	light1.position[2] = 10.00;
	light1.position[3] = 0.0f;

	updateLights();
}

void OGLWidget::drawLights(double radius)
{
	if(light0.enabled)
		DrawSphere(Vec3(light0.position[0], light0.position[1], light0.position[2]), Vec4(1.0, 0.0, 0.0, 1.0) , radius);

	if (light1.enabled)
		DrawSphere(Vec3(light1.position[0], light1.position[1], light1.position[2]), Vec4(1.0, 0.5, 0.0, 1.0), radius);
}

vector<Entity*> OGLWidget::getSwapList() {
	return swapList;
}

void OGLWidget::clearSwapList() {
	for (int i = 0; i < swapList.size(); i++) {
		swapList[i]->setSwapFlag(false);
	}
	swapList.clear();
}

void OGLWidget::clearSelectRegion() {
	selectionRegion.clear();
}

void OGLWidget::addVertexToSelectRegion(Vec3 v) {
	selectionRegion.push_back(v);
}

void OGLWidget::drawSelectRegion() {

	if (m_selectState == SELECTSTATE::SELECTING) {
		glColor3f(0.5, 0.5, 0.5);
		glLineWidth(1.0);
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i<selectionRegion.size(); i++) {
			glVertex3f(selectionRegion[i].a, selectionRegion[i].b, 0.0);
		}
		glEnd();

	}
	else if (m_selectState == SELECTSTATE::FINISHED) {
		glColor3f(0.0, 0.0, 0.0);
		glLineWidth(2.0);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i<selectionRegion.size(); i++) {
			glVertex3f(selectionRegion[i].a, selectionRegion[i].b, 0.0);
		}
		glEnd();
		glFlush();
	}

}

void OGLWidget::drawConvexHull()
{
	glColor3f(0.0, 1.0, 1.0);
	glLineWidth(2.0);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i<convexHull.size(); i++) {
		glVertex3f(convexHull[i].a, convexHull[i].b, 0.0);
	}
	glEnd();
	glFlush();	
}


// drawing primitives

void OGLWidget::DrawArrow(Vec2 start, Vec2 end, double thickness, Color4_F color, double headlen, double headtailraitio, double angle)
{
	glLineWidth(thickness);
	glColor4dv(color.data());

	/*draw head*/
	Vec2 dir = end - start;
	Vec2 headtip = dir*headtailraitio + start;

	double len = length(dir);

	double anglerad = angle / 180.0*M_PI;
	Vec2 dir1 = dir.rotate(-anglerad);
	Vec2 dir2 = dir.rotate(anglerad);

	dir1.normalize();
	dir2.normalize();

	Vec2 headend1 = headtip - dir1*headlen;
	Vec2 headend2 = headtip - dir2*headlen;

	/*draw body*/
	glBegin(GL_LINES);

	glVertex3f(start.x, start.y, 0);
	glVertex3f(end.x, end.y, 0);

	glVertex3f(headend1.x, headend1.y, 0);
	glVertex3f(headtip.x, headtip.y, 0);

	glVertex3f(headend2.x, headend2.y, 0);
	glVertex3f(headtip.x, headtip.y, 0);

	glEnd();
	glFlush();
}

void OGLWidget::DrawCircle(double x, double y, double radius, Vec4 color, double lw, int segments)
{
	DrawCircle(Vec3(x, y, 0), radius, color, lw, segments);
}

void OGLWidget::DrawCircle(Vec3 loc, double radius, Vec4 color, double lw, int segments)
{
	glLineWidth(lw);
	double r = radius;

	glColor4f(color.R, color.G, color.B, color.Alpha);

	glBegin(GL_LINE_LOOP);
	for (int n = 0; n <= segments; ++n) {
		float const t = 2 * M_PI * (float)n / (float)segments;
		glVertex3f(loc.x + cos(t) * r, loc.y + sin(t) * r, loc.z);
	}

	glEnd();
}

void OGLWidget::DrawArc(Vec3 center, double radius, double startAngle, double endAngle, Vec4 color, double lw, double radian_step)
{
	glLineWidth(lw);
	glColor4f(color.R, color.G, color.B, color.Alpha);

	double angleRange = endAngle - startAngle;
	int segments = (int)(angleRange / radian_step) + 1;
	double step = angleRange / (double)segments;

	glBegin(GL_LINES);
	for (int n = 0; n < segments; ++n) {
		double const t = startAngle + (double)n * step;
		double const tn = startAngle + (double)(n + 1) * step;

		glVertex3f(center.x + cos(t) * radius, center.y + sin(t) * radius, center.z);
		glVertex3f(center.x + cos(tn) * radius, center.y + sin(tn) * radius, center.z);
	}

	glEnd();
}

void OGLWidget::DrawLine(Vec3 start, Vec3 end, Vec4 color, double width)
{
	DrawLine(Vec2(start), Vec2(end), color, width);
}

void OGLWidget::DrawLine(Vec2 start, Vec2 end, Vec4 color, double width)
{
	glLineWidth(width);
	glColor3f(color.R, color.G, color.B);

	glBegin(GL_LINES);
	glVertex2f(start.x, start.y);
	glVertex2f(end.x, end.y);
	glEnd();

	CHECK_GL_ERROR();
}

void OGLWidget::DrawLine3D(Vec3 start, Vec3 end, Vec4 color, double width)
{
	glLineWidth(width);
	glColor3f(color.R, color.G, color.B);

	glBegin(GL_LINES);
	glVertex3f(start.x, start.y, start.z);
	glVertex3f(end.x, end.y, end.z);
	glEnd();

	CHECK_GL_ERROR();
}

void OGLWidget::DrawDisk(double x, double y, double radius, Vec4 color, int segments, bool do_shading)
{
	DrawDisk(Vec3(x, y, 0), radius, color, segments, do_shading);
}

void OGLWidget::DrawDisk(Vec3 loc, double r, Vec4 color, int segments, bool do_shading)
{
	glColor4f(color.R, color.G, color.B, color.Alpha);

	if (do_shading)
	{
		GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		float shininess = cylinderShininess;
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	glBegin(GL_TRIANGLE_FAN);
	if (do_shading)
		glNormal3f(0, 0, 1.0);
	glVertex3f(loc.x, loc.y, loc.z);
	for (int n = 0; n <= segments; ++n) 
	{
		float const t = 2 * M_PI * (float)n / (float)segments;
		if (do_shading)
			glNormal3f(0, 0, 1.0);
		glVertex3f(loc.x + cos(t) * r, loc.y + sin(t) * r, loc.z);
	}
	glEnd();

	if (do_shading)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}
}

void OGLWidget::DrawPartialDisk(Vec3 center, double radius, double startAngle, double endAngle, Vec4 color, double radian_step, bool do_shading)
{
	glColor4f(color.R, color.G, color.B, color.Alpha);

	double angleRange = endAngle - startAngle;
	int segments = int(angleRange / radian_step) + 1;
	double step = angleRange / (double)segments;

	if (do_shading)
	{
		GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		float shininess = cylinderShininess;
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	glBegin(GL_TRIANGLE_FAN);
	if (do_shading)
		glNormal3f(0, 0, 1.0);
	glVertex3f(center.x, center.y, center.z);
	for (int n = 0; n <= segments; ++n)
	{
		double a = startAngle + (double)n * step;
		if (do_shading)
			glNormal3f(0, 0, 1.0);
		glVertex3f(center.x + cos(a) * radius, center.y + sin(a) * radius, center.z);
	}
	glEnd();

	if (do_shading)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}
}

void OGLWidget::DrawCross(double x, double y, double r, Vec3 color, double angle)
{

	double radian = angle / 180.0 * M_PI;

    glColor3f(color.R, color.G, color.B);
    glBegin(GL_LINES);
    glVertex3f(x - r*cos(radian), y - r*sin(radian), 0);
    glVertex3f(x + r*cos(radian), y + r*sin(radian), 0);

    glVertex3f(x - r*sin(radian), y + r*cos(radian), 0);
    glVertex3f(x + r*sin(radian), y - r*cos(radian), 0);

    glEnd();
}

void OGLWidget::DrawCross(Vec3 loc, double radius, Vec4 color, double rotation)
{
	double a = rotation / (180.0 * M_PI);
	glColor4f(color.R, color.G, color.B, color.Alpha);

	glBegin(GL_LINES);
	glVertex3f(loc.x - radius * cos(a), loc.y - radius * sin(a), loc.z);
	glVertex3f(loc.x + radius * cos(a), loc.y + radius * sin(a), loc.z);

	glVertex3f(loc.x - radius * sin(a), loc.y + radius * cos(a), loc.z);
	glVertex3f(loc.x + radius * sin(a), loc.y - radius * cos(a), loc.z);
	glEnd();
}

void OGLWidget::DrawSquare(double x, double y, double r, Vec4 color)
{
	const double sa = r;
	const double ca = r;

	glColor4f(color.R, color.G, color.B, color.Alpha);

	glBegin(GL_QUADS);

	glVertex3f(x + sa, y + ca, 0);
	glVertex3f(x + sa, y - ca, 0);
	glVertex3f(x - sa, y - ca, 0);
	glVertex3f(x - sa, y + ca, 0);

	glEnd();
}

void OGLWidget::DrawSphere(Vec3 Point0, Vec4 Color, double radius) {

	GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &SphereShininess);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);

	GLUquadric* quad = gluNewQuadric();

	glColor4f(Color.R, Color.G, Color.B, Color.Alpha);

	glPushMatrix();
	glTranslatef(Point0.x, Point0.y, Point0.z);
	gluSphere(quad, radius, 50, 50);
	glPopMatrix();
	gluDeleteQuadric(quad);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);

	CHECK_GL_ERROR();

}

void OGLWidget::DrawCylinder(Vec3 start, Vec3 end, Vec4 Color, double radius)
{
	GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &cylinderShininess);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);


	glColor4f(Color.R, Color.G, Color.B, Color.Alpha);

	GLUquadric* quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);

	Vec3 dir = end - start;
	float vx = dir.x;
	float vy = dir.y;
	float vz = dir.z;

	//handle the degenerate case of z1 == z2 with an approximation
	if (vz == 0)
		vz = .0001;

	float v = length(dir);
	float ax = 57.2957795*acos(vz / v);
	if (vz < 0.0)
		ax = -ax;

	float rx = -vy*vz;
	float ry = vx*vz;

	glPushMatrix();

	//draw the cylinder body
	glTranslatef(start.x, start.y, start.z);
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluCylinder(quad, radius, radius, v, 50, 1);

	//draw the first cap
	gluQuadricOrientation(quad, GLU_INSIDE);
	gluDisk(quad, 0.0, radius, 50, 1);
	glTranslatef(0, 0, v);

	//draw the second cap
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluDisk(quad, 0.0, radius, 50, 1);
	
	glPopMatrix();

	gluDeleteQuadric(quad);

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);

	CHECK_GL_ERROR();

}

void OGLWidget::DrawCylidnerCircle(Vec3 loc, double radius, Vec4 color, double width, int segments)
{
	double angle_step = 2 * M_PI / (double)segments;
	Vec3 prev = Vec3(loc.x + cos(0) * radius, loc.y + sin(0) * radius, loc.z);
	for (int i = 1; i <= segments; i++)
	{
		float a = (double)i * angle_step;
		Vec3 next = Vec3(loc.x + cos(a) * radius, loc.y + sin(a) * radius, loc.z);
		DrawCylinder(prev, next, color, width * 0.0025);
		prev = next;
	}
}

void OGLWidget::DrawCylinderArc(Vec3 center, double radius, double startangle, double endangle, Vec4 color, double width, int segments)
{
	double angle_step = (endangle - startangle) / (double)segments;
	Vec3 prev = Vec3(center.x + cos(startangle) * radius, center.y + sin(startangle) * radius, center.z);
	for (int i = 1; i <= segments; i++)
	{
		float a = (double)i * angle_step;
		Vec3 next = Vec3(center.x + cos(a) * radius, center.y + sin(a) * radius, center.z);
		DrawCylinder(prev, next, color, width * 0.0025);
		prev = next;
	}
}

void OGLWidget::DrawPolygonFill(ENList locs, Vec3 center, Vec4 color, double height, bool do_shading)
{
	glColor4f(color.R, color.G, color.B, color.Alpha);

	if (do_shading)
	{
		GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		float shininess = cylinderShininess;
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	glBegin(GL_TRIANGLE_FAN);
	if (do_shading)
		glNormal3f(0, 0, 1.0);
	glVertex3f(center.x, center.y, height);
	for (int i = 0; i <= locs.size(); i++)
	{
		int j = i % locs.size();
		Vec3 loc = locs[j]->getLocation();
		if (do_shading)
		{
			Vec3 dir = loc - center;
			normalize(dir);
			glNormal3f(dir.x, dir.y, dir.z);
		}
		glVertex3f(loc.x, loc.y, height);
	}
	glEnd();

	if (do_shading)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}
}

void OGLWidget::DrawPolygonLine(ENList locs, Vec4 color, double height, bool drawCylinder)
{
	for (int i = 0; i < locs.size(); i++)
	{
		int j = (i + 1) % locs.size();
		Vec3 loci = locs[i]->getLocation();
		loci.z = height;
		Vec3 locj = locs[j]->getLocation();
		locj.z = height;

		if (drawCylinder)
			DrawCylinder(loci, locj, color, boundaryWidth * 0.0025);
		else
			DrawLine(loci, locj, color, boundaryWidth);
	}
}

void OGLWidget::DrawPillShape(Vec3 start, Vec3 end, double radius, Vec4 color, double radian_step, bool do_shading)
{
	Vec3 dir = end - start;
	Vec3 offset = Vec3(-dir.y, dir.x, 0);
	normalize(offset);
	double a = angle(Vec2(offset));
	Vec3 pos;

	glColor4f(color.R, color.G, color.B, color.Alpha);
	
	if (do_shading)
	{
		GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		float shininess = cylinderShininess;
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	glBegin(GL_QUADS);
	if (do_shading)
		glNormal3f(-offset.x, -offset.y, -offset.z);
	pos = start + offset * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(-offset.x, -offset.y, -offset.z);
	pos = end + offset * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(offset.x, offset.y, offset.z);
	pos = end - offset * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(offset.x, offset.y, offset.z);
	pos = start - offset * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();

	if (do_shading)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}

	DrawPartialDisk(start, radius, a, a + M_PI, color, radian_step, do_shading);
	DrawPartialDisk(end, radius, a + M_PI, a + 2 * M_PI, color, radian_step, do_shading);
}

void OGLWidget::DrawBubbleLine(vector<Vec3> locs, double radius, Vec4 color, double radian_step, bool do_shading)
{
	if (locs.size() == 1)
		return DrawDisk(locs[0], radius, color, (int)(2 * M_PI / radian_step), do_shading);
	else if (locs.size() == 2)
		return DrawPillShape(locs[0], locs[1], radius, color, radian_step, do_shading);

	glColor4f(color.R, color.G, color.B, color.Alpha);

	if (do_shading)
	{
		GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		float shininess = cylinderShininess;
		glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	Vec3 pos;
	for (int i = 0; i < locs.size() - 2; i++)
	{
		Vec3 p1 = locs[i];
		Vec3 p2 = locs[i + 1];
		Vec3 p3 = locs[i + 2];

		Vec3 dir12 = p2 - p1;
		Vec3 dir23 = p3 - p2;
		Vec3 off12 = Vec3(dir12.y, -dir12.x, 0);
		normalize(off12);
		Vec3 off23 = Vec3(dir23.y, -dir23.x, 0);
		normalize(off23);
		Vec3 mid12 = p1 + 0.5 * dir12;
		Vec3 mid23 = p2 + 0.5 * dir23;

		double a12 = angle(Vec2(dir12.x, dir12.y));
		double a23 = angle(Vec2(dir23.x, dir23.y));
		if (a23 < a12)
			a23 = a23 + 2 * M_PI;
		double diff = a23 - a12;
		
		if (diff < M_PI)
		{
			double aoff12 = a12 + 1.5 * M_PI;
			double aoff23 = a23 + 1.5 * M_PI;
			DrawPartialDisk(p2, radius, aoff12, aoff23, color, radian_step, do_shading);

			double aoff_diff = (aoff23 - aoff12) * 0.5;
			Vec3 elbow = -(off12 + off23) * 0.5 ;
			normalize(elbow);
			double elbow_dist = radius / cos(aoff_diff);

			glBegin(GL_POLYGON);

			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid12 + off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 + off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 + elbow * elbow_dist;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid12 - off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);

			glEnd();

			CHECK_GL_ERROR();

			glBegin(GL_POLYGON);

			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid23 + off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 + off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 + elbow * elbow_dist;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid23 - off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);

			glEnd();

			CHECK_GL_ERROR();
		}
		else if (diff > M_PI)
		{
			double aoff12 = a12 + 2.5 * M_PI;
			double aoff23 = a23 + 0.5 * M_PI;
			DrawPartialDisk(p2, radius, aoff23, aoff12, color, radian_step, do_shading);

			double aoff_diff = (aoff12 - aoff23) * 0.5;
			Vec3 elbow = (off12 + off23) * 0.5;
			normalize(elbow);
			double elbow_dist = radius / cos(aoff_diff);

			glBegin(GL_POLYGON);

			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid12 - off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 - off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 + elbow * elbow_dist;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid12 + off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);

			glEnd();

			CHECK_GL_ERROR();

			glBegin(GL_POLYGON);

			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid23 - off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 - off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = p2 + elbow * elbow_dist;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(0.0, 0.0, 1.0);
			pos = mid23 + off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);

			glEnd();

			CHECK_GL_ERROR();
		}
		else
		{
			glBegin(GL_QUADS);

			if (do_shading)
				glNormal3f(-off12.x, -off12.y, -off12.z);
			pos = mid12 + off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(-off12.x, -off12.y, -off12.z);
			pos = p2 + off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(off12.x, off12.y, off12.z);
			pos = p2 - off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(off12.x, off12.y, off12.z);
			pos = mid12 - off12 * radius;
			glVertex3f(pos.x, pos.y, pos.z);

			if (do_shading)
				glNormal3f(-off23.x, -off23.y, -off23.z);
			pos = mid23 + off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(-off23.x, -off23.y, -off23.z);
			pos = p2 + off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(off23.x, off23.y, off23.z);
			pos = p2 - off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);
			if (do_shading)
				glNormal3f(off23.x, off23.y, off23.z);
			pos = mid23 - off23 * radius;
			glVertex3f(pos.x, pos.y, pos.z);

			glEnd();

			CHECK_GL_ERROR();
		}
	}

	int n = locs.size();
	Vec3 start = locs[0];
	Vec3 start_dir = locs[1] - start;
	Vec3 start_mid = start + start_dir * 0.5;
	Vec3 start_off = Vec3(-start_dir.y, start_dir.x, 0);
	normalize(start_off);
	double start_a = angle(start_off);

	glBegin(GL_QUADS);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = start + start_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = start_mid + start_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = start_mid - start_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = start - start_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();

	CHECK_GL_ERROR();

	DrawPartialDisk(start, radius, start_a, start_a + M_PI, color, radian_step, do_shading);

	CHECK_GL_ERROR();

	Vec3 end = locs[n - 1];
	Vec3 end_dir = locs[n - 2] - end;
	Vec3 end_mid = end + end_dir * 0.5;
	Vec3 end_off = Vec3(-end_dir.y, end_dir.x, 0);
	normalize(end_off);
	double end_a = angle(end_off);

	glBegin(GL_QUADS);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = end + end_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = end_mid + end_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = end_mid - end_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	if (do_shading)
		glNormal3f(0.0, 0.0, 1.0);
	pos = end - end_off * radius;
	glVertex3f(pos.x, pos.y, pos.z);
	glEnd();

	CHECK_GL_ERROR();

	DrawPartialDisk(end, radius, end_a, end_a + M_PI, color, radian_step, do_shading);

	CHECK_GL_ERROR();

	if (do_shading)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}
}



void OGLWidget::updateSelectionFromData()
{
	clearSelectEntityList();
	clearSelectRelationshipList();

	for (Entity* en : currentERData->getEntities())
		if (en->getSelected())
			selectedEntityList.push_back(en);

	for (Relationship* re : currentERData->getRelationships())
		if (re->getSelected())
			selectedRelationshipList.push_back(re);

	repaint();
}

void OGLWidget::addSelectEntityList(Entity *e) {
	if (!e)
		return;

	int inside = isInsideList<Entity>(e, selectedEntityList);
	if (inside != -1) {
		selectedEntityList[inside]->setSelected(false);
		selectedEntityList.erase(selectedEntityList.begin() + inside);
	}
	else {
		e->setSelected(true);
		selectedEntityList.push_back(e);
	}
}

void OGLWidget::addEntitiesToSelectList(const vector<Entity*>& enlist)
{
	for (auto &en : enlist) {
		addSelectEntityList(en);
	}
}

void OGLWidget::addSelectRelationshipList(Relationship *e) {

	if (!e)
		return;

	bool oldselected = e->getSelected();

	int inside = isInsideList<Relationship>(e, selectedRelationshipList);
	if (inside != -1) {
		selectedRelationshipList[inside]->setSelected(false);
		selectedRelationshipList.erase(selectedRelationshipList.begin() + inside);
	}
	else {
		e->setSelected(true);
		selectedRelationshipList.push_back(e);
	}
}

void OGLWidget::addRelationsToSelectList(const vector<Relationship*>& relist)
{
	for (auto &re : relist) {
		addSelectRelationshipList(re);
	}
}

void OGLWidget::selectSingleEntity(Entity *e) {

	removeSelectedEntities();
	e->setSelected(true);
	selectedEntityList.push_back(e);
}

void OGLWidget::selectSingleRelationship(Relationship *e) {

	removeSelectedRelationships();
	e->setSelected(true);
	selectedRelationshipList.push_back(e);
}

void OGLWidget::clearSelectEntityList() {
	selectedEntityList.clear();
}

void OGLWidget::clearSelectRelationshipList() {
	selectedRelationshipList.clear();
}

void OGLWidget::removeSelectedEntities() {

	for (int i = 0; i < selectedEntityList.size(); i++) {
		Entity *e = selectedEntityList[i];
		e->setSelected(false);
	}
	selectedEntityList.clear();
}

void OGLWidget::removeSelectedRelationships() {
	for (int i = 0; i < selectedRelationshipList.size(); i++) {
		Relationship *e = selectedRelationshipList[i];
		e->setSelected(false);
	}
	selectedRelationshipList.clear();
}

vector<Entity*> OGLWidget::getSelectedEntities(bool get_incident)
{
	if (!get_incident)
		return selectedEntityList;

	ENList selection = selectedEntityList;
	for (Relationship* re : selectedRelationshipList)
		for (Entity* en : re->getIncidentEntities())
			if (!isEntityInsideEntityList(en, selection))
				selection.push_back(en);

	return selection;
}

vector<Relationship*> OGLWidget::getSelectedRelationships(bool get_incident)
{
	if (!get_incident)
		return selectedRelationshipList;

	REList selection = selectedRelationshipList;
	for (Entity* en : selectedEntityList)
		for (Relationship* re : en->getIncidentRelationships())
			if (!isRelationshipInsideRelationshipList(re, selection))
				selection.push_back(re);
	for (Relationship* re : selectedRelationshipList)
		for (Relationship* adj : re->getAdjacentRelationships())
			if (adj->getCardinality() == 1)
				selection.push_back(adj);

	return selection;
}

void OGLWidget::Highlight_selectedElementsOnce()
{
	double scale = 0;

	if (scale < 1)
		scale = 1.25;

	vector<Entity*> incidents = getUniqueIncidentEntities(selectedRelationshipList);

	for (int frame = 0; frame < 12; frame++) {

		QCoreApplication::processEvents();

		for (int i = 0; i < selectedRelationshipList.size(); i++) {
			Relationship *temp_r = selectedRelationshipList[i];
			temp_r->ScalePolygonWithoutUpdateAdjcent(scale);
		}

		for (int i = 0; i < selectedEntityList.size(); i++) {
			Entity *en = selectedEntityList[i];
			en->m_Size = en->m_Size*scale;
		}

		for (int i = 0; i < incidents.size(); i++) {
			Entity *en = incidents[i];
			en->m_Size = en->m_Size*scale;
		}

		repaint();
	}

	for (int frame = 0; frame < 12; frame++) {

		QCoreApplication::processEvents();

		for (int i = 0; i < selectedRelationshipList.size(); i++) {
			Relationship *temp_r = selectedRelationshipList[i];
			temp_r->ScalePolygonWithoutUpdateAdjcent(1/ scale);
		}

		for (int i = 0; i < selectedEntityList.size(); i++) {
			Entity *en = selectedEntityList[i];
			en->m_Size = en->m_Size/ scale;
		}

		for (int i = 0; i < incidents.size(); i++) {
			Entity *en = incidents[i];
			en->m_Size = en->m_Size / scale;
		}

		repaint();
	}
}

void OGLWidget::Highlight_selectedElementsOneFrame(double scale)
{
	vector<Entity*> incidents = getUniqueIncidentEntities(selectedRelationshipList);

	{

		for (int i = 0; i < selectedRelationshipList.size(); i++) {
			Relationship *temp_r = selectedRelationshipList[i];
			temp_r->ScalePolygonWithoutUpdateAdjcent(scale);
		}

		for (int i = 0; i < selectedEntityList.size(); i++) {
			Entity *en = selectedEntityList[i];
			en->m_Size = en->m_Size*scale;
		}

		for (int i = 0; i < incidents.size(); i++) {
			Entity *en = incidents[i];
			en->m_Size = en->m_Size*scale;
		}

	}
}

void OGLWidget::Highlight_selectedElements()
{
	for (int i = 0; i < 4; i++) {
		QTimer::singleShot(1000*i, this, SLOT(Highlight_selectedElementsOnce()));
	}
}

Relationship* OGLWidget::getSelectedRelationship(int i) {
	if (i >= selectedRelationshipList.size() || i < 0)
		return NULL;
	return selectedRelationshipList[i];
}

Entity* OGLWidget::getSelectedEntity(int i) {
	if (i >= selectedEntityList.size() || i < 0)
		return NULL;
	return selectedEntityList[i];
}



// ER element drawing

void OGLWidget::draw2D(Entity *en, double z) {

	double Radius = en->m_Size;
	bool m_Selected = en->getSelected();
	bool m_SelectedForSwap = en->getSwapFlag();
	Vec4 color = en->getColor();
	//color *= Vec4(0.8, 0.8, 0.8, 1.0);
	Vec4 boundaryColor = EntityBoundaryColor;
	boundaryColor.Alpha = color.Alpha;
	Vec3 CurrentLocation = en->getLocation();

	if (m_Selected)
	{
		color = Vec4(0.0, 0.0, 1.0, color.Alpha);
		Radius = Radius*1.5;
	}
	else if (m_SelectedForSwap) 
	{
		color.set(0.0f, 1.0f, 0.0f, 1.0);
	}

	if (z != 0) {
		CurrentLocation.z = z;
	}

	int segments = 32;
	if (widgetType == VISTYPE::DUALVIS || widgetType == VISTYPE::BIPARTVIS)
	{
		Radius *= 1.25;
		segments = 4;
		boundaryColor = Vec4(0.0, 0.0, 0.0, 1.0);
	}

	if(gmainwindow->flag_drawNodeSphere)
		DrawSphere(CurrentLocation, color, Radius);
	else
		DrawDisk(CurrentLocation, Radius, color, segments);

	DrawCircle(CurrentLocation, Radius, boundaryColor, nodeBoundaryWidth, segments);

	if (gmainwindow->flag_drawCrossOnFixedElements && !en->flag_Optimizable)
		DrawCross(CurrentLocation, Radius*1.5, Vec4(1.0, 0, 0, 1.0), 45);

	glFlush();

	CHECK_GL_ERROR();

}

void OGLWidget::buildMonogonPipe(double width)
{
	double b = Element::m_MonogonSize;
	double a = Element::m_MonogonLength;

	double inclinedAngle = 0;

	PipeMesh::Vector3 nodeLoc = PipeMesh::Vector3(0, 0, 0);

	PipeMesh::Vector3 m_observer = PipeMesh::Vector3(nodeLoc.x + a*cos(inclinedAngle), nodeLoc.y + a*sin(inclinedAngle), 0.0);

	double x = m_observer.x;
	double y = m_observer.y;

	vector<PipeMesh::Vector3> scaffoldsPoints;
	scaffoldsPoints.push_back(nodeLoc);

	double startAngle = -M_PI*0.5 - asin(b / a);
	double endAngle = M_PI*0.5 + asin(b / a);
	double angleRange = endAngle - startAngle;

	for (int n = 0; n <= Element::m_MonogonSegments; ++n) {
		double const t = startAngle + angleRange * (double)n / (double)Element::m_MonogonSegments;

		double ex = cos(t) * b;
		double ey = sin(t) * b;

		Eigen::Vector2d loc = Eigen::Rotation2D<double>(inclinedAngle) * Eigen::Vector2d(ex, ey);

		scaffoldsPoints.push_back(PipeMesh::Vector3(x + loc.x(), y + loc.y(), 0));
	}
	scaffoldsPoints.push_back(nodeLoc);


	// generate points of a circle on xy plane
	auto buildCircle = [](float radius, int steps)
	{
		std::vector<PipeMesh::Vector3> points;
		if (steps < 2) return points;

		const float PI2 = acos(-1) * 2.0f;
		float x, y, a;
		for (int i = 0; i <= steps; ++i)
		{
			a = PI2 / steps * i;
			x = radius * cosf(a);
			y = radius * sinf(a);
			points.push_back(PipeMesh::Vector3(x, y, 0));
		}
		return points;
	};

	std::vector<PipeMesh::Vector3> circle = buildCircle(width*0.05*Element::m_VertexSize, 48); // radius, segments

	mMonogonPipe.set(scaffoldsPoints, circle);
}

void OGLWidget::drawMonogonScaffold(Vec3 loc, double rotation, Vec4 Color, double width, double z)
{
	drawReMonogonScaffold(&mMonogonPipe, loc, rotation, Color, width, z);
}

void OGLWidget::drawReMonogonScaffold(PipeMesh::Pipe* mMonogonPipe, Vec3 loc, double rotation, Vec4 Color, double width, double z)
{
	if (mMonogonPipe == NULL)
		return;

	if (gmainwindow->flag_drawEdgeCylinder)
	{
		GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &cylinderShininess);

		glShadeModel(GL_SMOOTH);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();

		glTranslatef(loc.x, loc.y, z);
		glRotatef(rotation / M_PI*180.0, 0, 0, 1);

		glColor4f(Color.R, Color.G, Color.B, Color.Alpha);

		int count = mMonogonPipe->getContourCount();
		for (int i = 0; i < count - 1; ++i)
		{
			std::vector<PipeMesh::Vector3> c1 = mMonogonPipe->getContour(i);
			std::vector<PipeMesh::Vector3> c2 = mMonogonPipe->getContour(i + 1);
			std::vector<PipeMesh::Vector3> n1 = mMonogonPipe->getNormal(i);
			std::vector<PipeMesh::Vector3> n2 = mMonogonPipe->getNormal(i + 1);

			glBegin(GL_TRIANGLE_STRIP);
			for (int j = 0; j < (int)c2.size(); ++j)
			{
				glNormal3f(n2[j].x, n2[j].y, n2[j].z);
				glVertex3f(c2[j].x, c2[j].y, c2[j].z);
				glNormal3f(n1[j].x, n1[j].y, n1[j].z);
				glVertex3f(c1[j].x, c1[j].y, c2[j].z);
			}
			glEnd();
		}

		glPopMatrix();

		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_DEPTH_TEST);

		CHECK_GL_ERROR();
	}
	else
	{

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef(loc.x, loc.y, z);

		glRotatef(rotation / M_PI * 180.0, 0, 0, 1);
		glLineWidth(width);
		glColor4dv(Color.data());
		glBegin(GL_LINES);

		for (int j = 0; j < mMonogonPipe->getPathCount() - 1; j++) {
			glVertex3f(mMonogonPipe->getPathPoint(j).x, mMonogonPipe->getPathPoint(j).y, 0.0);
			glVertex3f(mMonogonPipe->getPathPoint(j + 1).x, mMonogonPipe->getPathPoint(j + 1).y, 0.0);
		}

		glEnd();
		glPopMatrix();
		glFlush();
	}
}

void OGLWidget::drawEdges(Relationship *re, double z) {
	/*line*/

	Vec4 color = re->getColor();
	double width = boundaryWidth;

	if (re->getSelected())
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		glLineWidth(width + 3);
	}
	else
	{
		glColor4f(0.0, 0.0, 0.0, 0.7);
	}

	glBegin(GL_LINES);

	const Scaffold &m_scaffolds = re->getScaffold(0);

	if (m_scaffolds.m_Curved)
	{
		for (int i = 0; i < m_scaffolds.m_CurvedLocation.size() - 1; i++)
		{
			glVertex3f(m_scaffolds.m_CurvedLocation[i].a, m_scaffolds.m_CurvedLocation[i].b, 0);
			glVertex3f(m_scaffolds.m_CurvedLocation[i + 1].a, m_scaffolds.m_CurvedLocation[i + 1].b, 0);
		}
	}
	else
	{
		for (int i = 0; i < re->getIncidentEntities().size(); i++) {
			glVertex3f(re->getIncidentEntity(i)->getLocation().a, re->getIncidentEntity(i)->getLocation().b, 0);
		}
	}
	glEnd();
	glFlush();

	CHECK_GL_ERROR();
}

void OGLWidget::drawScaffold(Scaffold*scaffold, Vec4 color, double width, double z)
{
	if (gmainwindow->flag_drawEdgeCylinder) {
		for (int j = 0; j < scaffold->m_CurvedLocation.size() - 1; j++) {
			Vec3 loc_c = scaffold->m_CurvedLocation[j];
			Vec3 loc_n = scaffold->m_CurvedLocation[j + 1];
			loc_c.z = z;
			loc_n.z = z;
			DrawCylinder(loc_c, loc_n, color, width*0.05*0.05);
		}
	}
	else {
		glLineWidth(width);
		glColor4dv(color.data());
		glBegin(GL_LINES);

		for (int j = 0; j < scaffold->m_CurvedLocation.size() - 1; j++) {
			glVertex3f(scaffold->m_CurvedLocation[j].a, scaffold->m_CurvedLocation[j].b, z);
			glVertex3f(scaffold->m_CurvedLocation[j + 1].a, scaffold->m_CurvedLocation[j + 1].b, z);
		}

		glEnd();
		glFlush();
	}
}

void OGLWidget::draw2D(Relationship* re, double z) {

	if (flag_hideMonogon && re->getCardinality() == 1)
		return;

	if (viewStyle == VIEWSTYLE::POLYGON)
	{
		if (re->getCardinality() == 2 && flag_drawDigonAsLine)
			drawEdges(re, z);
		else
			drawNary2D(re, z);
	}
	else if (viewStyle == VIEWSTYLE::BIPARTITE)
	{
		drawBipart2D(re, z);
	}
	else if (viewStyle == VIEWSTYLE::COMPOSED)
	{
		drawNary2D(re, z);
		drawBipart2D(re, z);
	}

	CHECK_GL_ERROR();

	if (flag_drawPolygonObserver && (viewStyle == VIEWSTYLE::POLYGON || viewStyle == VIEWSTYLE::COMPOSED))
		drawObserver(re, z);

	CHECK_GL_ERROR();

	if (flag_showRelationshipDecal && decaltextures.size() > 0) 
	{
		if (re->getCardinality() == 1) {
			DrawCircleTexture(decaltextures[0], re->getObserver().x, re->getObserver().y, re->getMonogonSize());
		}
		else if (re->getCardinality() == 2) {
			float r = re->getDiagonInnerCircle();
			DrawCircleTexture(decaltextures[0], re->getCenter(0.0).x, re->getCenter(0.0).y, r);
		}
		else {
			DrawCircleTexture(decaltextures[0], re->getCenter(0.0).x, re->getCenter(0.0).y, calculateMinimumDis(re->getCenter(0.0), re->getIncidentEntities()));
		}
	}

	CHECK_GL_ERROR();
}

void OGLWidget::drawNary2D(Relationship *re, bool flag_Opt, double z) {
	CHECK_GL_ERROR();

	Vec4 color = re->getColor();
	Vec4 polygonColor = color;
	Vec4 boundaryColor = color*0.8;
	boundaryColor.Alpha = 1.0;

	if (re->getSelected())
	{
		double R = polygonColor.R * 0.1;
		double G = polygonColor.G * 0.1; 
		double B = polygonColor.B * 0.1 + 0.5;

		B = B > 1.0 ? 1.0 : B;

		polygonColor = Vec4(R, G, B, 0.6);
		boundaryColor = Vec4(0.0, 0.0, 1.0, 1.0);
	}

	if (flag_Opt)
		polygonColor = color;

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);

	CHECK_GL_ERROR();

	if (gmainwindow->flag_showPolyFill) 
	{
		if (gmainwindow->flag_drawPolygonWithShading) 
		{
			GLfloat matSpecular[] = { 1.f, 1.f, 1.f, 1.f };
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
			float shininess = cylinderShininess;
			glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
			glShadeModel(GL_SMOOTH);
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glBegin(GL_TRIANGLES);

			for (int i = 0; i < re->getScaffoldNum(); i++) {
				for (int j = 0; j < re->getScaffold(i).m_CurvedLocation.size() - 1; j++) {
					
					Vec3 cent = re->getObserver();
					Vec3 loc1 = re->getScaffold(i).m_CurvedLocation[j];
					Vec3 loc2 = re->getScaffold(i).m_CurvedLocation[j + 1];

					glColor4f(polygonColor.R, polygonColor.G, polygonColor.B, polygonColor.Alpha);
					glNormal3f(0, 0, 1.0);
					glVertex3f(cent.a, cent.b, z);

					Vec3 dir1 = loc1 - cent;
					normalize(dir1);
					glNormal3f(dir1.a, dir1.b, dir1.c);
					glVertex3f(loc1.a, loc1.b, z);

					Vec3 dir2 = loc2 - cent;
					normalize(dir2);
					glNormal3f(dir2.a, dir2.b, dir2.c);
					glVertex3f(loc2.a, loc2.b, z);
				}
			}

			glEnd();

			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
		}
		else 
		{
			glBegin(GL_TRIANGLES);

			for (int i = 0; i < re->getScaffoldNum(); i++) {
				for (int j = 0; j < re->getScaffold(i).m_CurvedLocation.size() - 1; j++) {

					Vec3 loc1 = re->getScaffold(i).m_CurvedLocation[j];
					Vec3 loc2 = re->getScaffold(i).m_CurvedLocation[j + 1];

					glColor4f(polygonColor.R, polygonColor.G, polygonColor.B, polygonColor.Alpha);

					glVertex3f(re->getObserver().a, re->getObserver().b, z);
					glVertex3f(loc1.a, loc1.b, z);
					glVertex3f(loc2.a, loc2.b, z);
				}
			}

			glEnd();
		}
	}

	CHECK_GL_ERROR();

	if (flag_Opt) {
		glFlush();
		return;
	}

	if (gmainwindow->flag_showPolyBoundary) {
		int swidth = boundaryWidth;
		if (re->getSelected())
			swidth = swidth*1.5;

		//if (gmainwindow->flag_drawEdgeCylinder)
		//	height = (double)re->getIndex() * boundaryWidth * 0.05 * Element::m_VertexSize;
		//if (height > zoffset) {
		//	zoffset *= 2.0;
		//	LogConsole::showError("out of z range, and z offset is changed %f", zoffset);
		//}

		if (re->getCardinality() == 1) {
			Vec3 loc = re->getIncidentEntity(0)->getLocation();
			double inclinedAngle = re->getMonogonRotation();
			//TODO:since the monogon pipe is pre-generated, so it cannot change the width
			if (abs(re->getMonogonSize() - Element::m_MonogonSize) > 1e-8) {

				if (!re->monogonPipeReconstructed)
					re->createMonogonPipe(boundaryWidth);

				drawReMonogonScaffold(re->getMonogonPipe(), loc, inclinedAngle, boundaryColor, swidth, z);
			}
			else
				drawMonogonScaffold(loc,inclinedAngle, boundaryColor, swidth, z);
		}
		else
		{
			for (int i = 0; i < re->getScaffoldNum(); i++) {
				Scaffold *s = re->getScaffoldPointer(i);
				drawScaffold(s, boundaryColor, swidth, z);
			}
		}
	}

	CHECK_GL_ERROR();

	glFlush();

	CHECK_GL_ERROR();
}

void OGLWidget::drawBipart2D(Relationship* re, double z)
{
	double radius = Element::m_VertexSize; 
	Vec3 location = re->getCenter(Element::m_MonogonLength);
	Vec4 color = re->getColor();
	color *= Vec4(0.8, 0.8, 0.8, 1.0);

	Entity* dual = re->getDualEntity();
	if (dual)
	{
		radius = dual->m_Size;
		location = dual->getLocation();
		color = dual->getColor();
	}
	
	bool selected = re->getSelected();
	Vec4 bcolor = EntityBoundaryColor;
	bcolor.Alpha = color.Alpha;

	if (selected)
	{
		color = Vec4(0.0, 0.0, 1.0, color.Alpha);
		radius = radius * 1.5;
	}
	if (z != 0)
		location.z = z;

	if (gmainwindow->flag_drawNodeSphere)
		DrawSphere(location, color, radius);
	else
		DrawDisk(location, radius, color);

	DrawCircle(location, radius, bcolor, nodeBoundaryWidth);

	/*if (gmainwindow->flag_drawCrossOnFixedElements && !re->flag_Optimizable)
		DrawCross(location, radius * 1.5, Vec4(1.0, 0, 0, 1.0), 45);*/

	glFlush();

	CHECK_GL_ERROR();
}

void OGLWidget::drawLinks(Relationship* re, double z)
{
	Vec3 location = re->getCenter(Element::m_MonogonLength);
	Entity* dual = re->getDualEntity();
	if (dual)
		location = dual->getLocation();
	location.z = z;

	for (Entity* en : re->getIncidentEntities())
	{
		Vec3 incident = en->getLocation();
		incident.z = z;
		Vec4 col = Vec4(0.5, 0.5, 0.5, 1.0);
		if (re->getSelected() && en->getSelected())
			col = Vec4(0.0, 0.0, 1.0, 1.0);
		DrawLine(location, incident, col, boundaryWidth);

		CHECK_GL_ERROR();
	}

	glFlush();

	CHECK_GL_ERROR();
}

void OGLWidget::drawObserver(Relationship *re, double z)
{
	double Radius = 0.5*Element::m_VertexSize;
	Vec3 CurrentLocation = re->getObserver();
	CurrentLocation.z = z;
	DrawDisk(CurrentLocation, Radius, Vec4(1.0, 1.0, 0.0, 1.0));
	glFlush();
}

void OGLWidget::drawCenter(Relationship *re, double z)
{
	double Radius = 0.5*Element::m_VertexSize;
	Vec3 CurrentLocation = re->getCenter(re->getMonogonLength());
	CurrentLocation.z = z;
	DrawDisk(CurrentLocation, Radius, Vec4(1.0, 0.0, 1.0, 1.0));

	glFlush();
}



// Graph Drawing

void OGLWidget::DrawBipartite2D()
{
	CHECK_GL_ERROR();

	if (!currentERData)
		return;

	for (Relationship* re : currentERData->getRelationships())
	{
		if (!re->getVisibility())
			continue;

		drawLinks(re, m_reheight);

		CHECK_GL_ERROR();
	}

	for (Relationship* re : currentERData->getRelationships())
	{
		if (!re->getVisibility())
			continue;

		draw2D(re, m_enheight);

		CHECK_GL_ERROR();
	}
	
	for (Entity* en : currentERData->getEntities())
	{
		if (!en->getVisibility())
			continue;

		draw2D(en, m_enheight);
		
		CHECK_GL_ERROR();
	}

	glDisable(GL_DEPTH_TEST);

	CHECK_GL_ERROR();
}

void OGLWidget::DrawComposed2D()
{

}



// Decomposition Drawing

ERClusterDecomp* OGLWidget::getERClusterDecomp()
{
	return m_bcdecomp;
}

bool OGLWidget::setERClusterDecomp(ERClusterDecomp* decomp)
{
	if (decomp->getERData() == currentERData)
	{
		if (m_bcdecomp)
			delete(m_bcdecomp);

		m_bcdecomp = decomp;
		return true;
	}
	else
	{
		LogConsole::writeError("Decomposition data does not match display data.");
		return false;
	}
}

void OGLWidget::clearERClusterDecomp()
{
	if (m_bcdecomp)
	{
		delete m_bcdecomp;
		m_bcdecomp = NULL;
	}
}

void OGLWidget::drawClusters(double reheight, double enheight, double clheight)
{
	if (!m_bcdecomp)
		return;

	if (m_bcdecomp->getERData() != currentERData)
		return;

	for (Cluster* c : m_bcdecomp->getClusters())
	{
		if (flag_drawClusterSuperPolygons)
			drawClusterSuperPolygon(c, reheight);

		if (flag_drawClusterBarcodes)
			drawClusterBarcode(c, enheight, clheight);
	}
}

void OGLWidget::drawClusterBarcode(Cluster* c, double enheight, double clheight)
{
	ClusterLayout* layout = c->getLayout();
	Vec3* c_temp = new Vec3(0, 0, 0);
	ENList enlist = m_bcdecomp->getSubgraphIncidentEntities(c, ELEM_INTERIOR_INCIDENCE, false, c_temp);
	if (!layout)
	{
		layout = new ClusterLayout();
		layout->center = *c_temp;
	}
	delete(c_temp);

	double target_step = 2 * M_PI / (double)enlist.size();
	std::unordered_map<Entity*, double> en_angles(enlist.size());
	std::vector<std::pair<int, double>> anchors;
	for (int i = 0; i < enlist.size(); i++)
	{
		Entity* en = enlist[i];
		double a = layout->barcode_rotation + (double)i * target_step;

		if ((m_bcdecomp->entityHasExteriorConnection(en, c) || !flag_hideClusterInteralEntities)
			&& flag_alignClusterBarcodeWithEntities)
		{
			Vec3 dir = en->getLocation() - layout->center;
			a = angle(dir);
			
			anchors.push_back(make_pair(i,a));
		}

		en_angles.insert(std::make_pair(en, a));
	}

	// if there are no exterior connections, or if internal entities are not hidden, 
	// keep default angle orders
	if (anchors.size() == 1)
	{
		int start_idx = anchors[0].first;
		double start_angle = anchors[0].second;

		for (int i = 1; i < enlist.size(); i++)
		{
			int idx = (start_idx + i) % enlist.size();
			double angle = start_angle + target_step * (double)i;
			if (angle > 2 * M_PI)
				angle -= 2 * M_PI;
			en_angles.at(enlist[idx]) = angle;
		}
	}
	else if (anchors.size() > 1 && anchors.size() < enlist.size())
	{
		for (int i = 1; i < anchors.size(); i++)
			if (anchors[i].second <= anchors[0].second)
				anchors[i].second += 2.0 * M_PI;
		std::sort(anchors.begin(), anchors.end(), [](auto& left, auto& right) { return left.second < right.second;});

		int idx = anchors[0].first;
		for (int i = 0; i < anchors.size(); i++)
		{
			int curr_anchor = i;
			int next_anchor = (i + 1) % anchors.size();
			double angle_start = anchors[curr_anchor].second;
			double angle_end = anchors[next_anchor].second;
			if (angle_end < angle_start)
				angle_end += 2 * M_PI;
			int num_steps = ((angle_end - angle_start) / target_step) + 0.5;
			double curr_step = (angle_end - angle_start) / (double)num_steps;
			double curr_angle = angle_start;
			for (int j = 1; j < num_steps; j++)
			{
				idx = (idx + 1) % enlist.size();
				if (std::find_if(anchors.begin(), anchors.end(), [&idx](auto element) { return element.first == idx; }) != anchors.end())
				{
					j = j - 1;
					continue;
				}
				curr_angle += curr_step;
				while (curr_angle > 2 * M_PI)
					curr_angle -= 2 * M_PI;
				en_angles.at(enlist[idx]) = curr_angle;
			}
		}
	}

	glEnable(GL_DEPTH_TEST);

	double radius = layout->barcode_radius;
	std::unordered_map<Entity*, std::pair<Vec3,Vec3>> en_bubbles;
	for (Relationship* re : m_bcdecomp->getSubgraphRelationships(c, gmainwindow->CLUSTER_RESORT))
	{
		Vec4 color = re->getColor() * 0.8;
		color.Alpha = 1.0;

		vector<double> angles;
		for (Entity* en : re->getIncidentEntities())
		{
			double a = en_angles.at(en);
			Vec3 dir = Vec3(cos(a), sin(a), 0);
			Vec3 loc = layout->center + radius * dir;
			loc.z = enheight;

			DrawDisk(loc, en->m_Size, color);

			CHECK_GL_ERROR();

			if (en_bubbles.find(en) == en_bubbles.end())
				en_bubbles.insert(std::make_pair(en, std::make_pair(loc, loc)));
			else
				en_bubbles.at(en).second = loc;

			angles.push_back(a);
		}

		// find biggest gap
		double start = 0;
		double end = 0;
		double max_diff = 0;
		std::sort(angles.begin(), angles.end());
		for (int i = 0; i < angles.size(); i++)
		{
			int j = (i + 1) % angles.size();
			double diff = angles[j] - angles[i];
			if (j == 0)
				diff += 2 * M_PI;
			if (diff > max_diff)
			{
				max_diff = diff;
				start = angles[j];
				if (j == 0)
					end = angles[i];
				else
					end = angles[i] + 2 * M_PI;
			}
		}

		DrawArc(Vec3(layout->center.x, layout->center.y, enheight), radius, start, end, color, boundaryWidth*0.5, Element::m_VertexSize);

		CHECK_GL_ERROR();
		
		radius += layout->barcode_step;
	}
	
	if (flag_drawClusterEntityBubbles)
	{
		for (auto bubble : en_bubbles)
		{
			Entity* en = bubble.first;
			Vec4 color = en->getColor();
			color.Alpha = 0.1;

			vector<Vec3> locs;
			Vec3 dir = bubble.second.first - layout->center;
			dir.z = 0; normalize(dir);
			if (m_bcdecomp->entityHasExteriorConnection(en, c) || !flag_hideClusterInteralEntities)
			{
				locs.push_back(bubble.second.first);
				if (!flag_alignClusterBarcodeWithEntities)
				{
					Vec3 mid = layout->center + dir * (radius - layout->barcode_step);
					if (length(mid - bubble.second.first) < layout->barcode_step)
						locs.push_back(mid);
				}
				locs.push_back(en->getLocation());
			}
			else
			{
				en->setVisibility(false);
				if (bubble.second.second != bubble.second.first)
				{
					locs.push_back(bubble.second.first);
					locs.push_back(bubble.second.second);
				}
				else
				{
					continue;
					/*Vec3 start = bubble.second.first - dir * layout->barcode_step * 0.5;
					Vec3 end = bubble.second.first + dir * layout->barcode_step * 0.5;
					locs.push_back(start);
					locs.push_back(end);*/
				}
			}
				
			for (int i = 0; i < locs.size(); i++)
				locs[i].z = clheight;

			DrawBubbleLine(locs, Element::m_VertexSize * 1.5, color);

			CHECK_GL_ERROR();
		}
	}

	glDisable(GL_DEPTH_TEST);
}

void OGLWidget::drawClusterSuperPolygon(Cluster* c, double reheight)
{
	CHECK_GL_ERROR();

	Vec4 fillcolor = c->getColor();
	Vec4 linecolor = fillcolor * 0.8;
	linecolor.Alpha = 1.0;
	ClusterLayout* layout = c->getLayout();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1., 1.);

	CHECK_GL_ERROR();

	if (flag_drawClusterCircleLayout && layout)
	{
		Vec3 center = layout->center;
		center.z = reheight;

		if (gmainwindow->flag_showPolyFill)
			DrawDisk(center, layout->radius, fillcolor, 128, gmainwindow->flag_drawPolygonWithShading);
		
		CHECK_GL_ERROR();

		if (gmainwindow->flag_showPolyBoundary/* && false*/)
		{
			if (gmainwindow->flag_drawEdgeCylinder)
				DrawCylidnerCircle(center, layout->radius, linecolor, boundaryWidth, 128);
			else
				DrawCircle(center, layout->radius, linecolor, boundaryWidth, 128);
		}
	}
	else
	{
		Vec3* c_temp = new Vec3(0, 0, 0);
		ENList enlist = m_bcdecomp->getSubgraphIncidentEntities(c, ELEM_REL_ANGLE, true, c_temp);
		Vec3 center = *c_temp;
		delete(c_temp);

		if (gmainwindow->flag_showPolyFill)
			DrawPolygonFill(enlist, center, fillcolor, reheight, gmainwindow->flag_drawPolygonWithShading);

		if (gmainwindow->flag_showPolyBoundary/* && false*/)
			DrawPolygonLine(enlist, linecolor, reheight, gmainwindow->flag_drawEdgeCylinder);
	}

	CHECK_GL_ERROR();

	glFlush();

	CHECK_GL_ERROR();
}