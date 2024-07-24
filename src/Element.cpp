#include "Element.h"
#include "LogConsole.h"
#include "FileIO.h"

using namespace std;

double Element::m_VertexSize(0.05);
double Element::m_TriangleThickness(0.005);
double Element::m_MonogonSegments(50);
double Element::m_MonogonLength(0.27);
double Element::m_MonogonSize(0.05);
double Element::m_DigonLensAngle(15);
double Element::m_MultipleDigonOffset(20);
Color4_8BIT Element::polyColor[COLORCOUNT];	//current color scheme
Color4_8BIT Element::polyFontColor[COLORCOUNT];

Element::Element()
{

}

Element::~Element()
{
}

ELEMENTTYPE Element::getType()
{
	return m_type;
}

void Element::setLabel(string label) {

	{
		string oldLabel = label;
		label.erase(std::remove_if(label.begin(), label.end(),
			[](char c) { return (c < 0 || c > 255); /* return std::isspace(c) || std::isalpha(c);*/ }),
			label.end());

		if (oldLabel != label)
			LogConsole::writeRedConsole("trimmed label %s", label.c_str());

	}

	m_label = label;
}

string Element::getLabel() {
	return m_label;
}

void Element::setLabelWithIndex()
{
	m_label = to_string(m_index);
}

void Element::setIndex(int index) {
	m_index = index;
}

int Element::getIndex() const{
	return m_index;
}

void Element::setVisibility(bool vis) {
	m_Visibility = vis;
}

bool Element::getVisibility() {
	return m_Visibility;
}

void Element::setDrawFlag(bool flag) {
	m_DrawFlag = flag;
}

bool Element::getDrawFlag() {
	return m_DrawFlag;
}

bool Element::getSelected() {
 return m_Selected;
}

void Element::setSelected(bool res) {
 m_Selected = res;
}

void Element::setColor(Color4_F color) {
 m_Color = color;
}

Color4_F Element::getColor() {
 return m_Color;
}

Color4_F Element::getDiscreteColor(int card) {

 if (card <= COLORCOUNT) {
     return polyColor[card - 1] / 255.0;
 }
 else {
     return polyColor[COLORCOUNT - 1] / 255.0;
 }
}

void Element::initPolyColors(const char * file)
{
 vector<Color4_8BIT>colors = FileIO::readPolygonColorsFile(file);
 for (int i = 0; i < colors.size(); i++) {
     polyColor[i] = colors[i];
 }
}

void Element::initPolyColors(const Color4_8BIT colors[], int n)
{
 for (int i = 0; i < n; i++) {
     polyColor[i] = colors[i];
 }
}

void Element::initPolyFontColors(const Color4_8BIT colors[], int n)
{
 for (int i = 0; i < n; i++) {
     polyFontColor[i] = colors[i];
 }
}

void Element::setLabelVisibility(bool res)
{
 m_LabelVisibility = res;
}


// Multiscale related

void Element::setOpidx(int opidx)
{
	m_opidx = opidx;
}

int Element::getOpidx() const
{
	return m_opidx;
}

void Element::setRemovalOp(OperationRecord* op_removal)
{
	m_opRemoval = op_removal;
}

void Element::addMergerOp(OperationRecord* op_merger)
{
	m_opMerges.push_back(op_merger);
}

void Element::setMergerOps(vector<OperationRecord*> merger_list)
{
	m_opMerges = merger_list;
}

OperationRecord* Element::getRemovalOp()
{
	return m_opRemoval;
}

double Element::getRemovalCost()
{
	if (m_opRemoval)
		return m_opRemoval->getCost();
	else
		return 0.0;
}

double Element::getRemovalImportance()
{
	if (m_opRemoval)
		return m_opRemoval->getImportance();
	else
		return 0.0;
}

double Element::getRemovalPriority()
{
	if (m_opRemoval)
		return m_opRemoval->getPriority();
	else
		return 0.0;
}

vector<OperationRecord*> Element::getMergerOps()
{
	return m_opMerges;
}

void Element::deleteOperations()
{
	if (m_opRemoval)
		delete(m_opRemoval);
	for (OperationRecord* merger : m_opMerges)
		delete(merger);
	m_opRemoval = NULL;
	m_opMerges.clear();
}

//double Element::getWeight()
//{
//	return m_weight;
//}
//
//void Element::setWeight(double w)
//{
//	m_weight = w;
//}

int Element::getSimpFactor()
{
	return m_SimpFactor;
}

void Element::incrementSimpFactor()
{
	m_SimpFactor = m_SimpFactor + 1;
}

void Element::decrementSimpFactor()
{
	m_SimpFactor = m_SimpFactor - 1;
}