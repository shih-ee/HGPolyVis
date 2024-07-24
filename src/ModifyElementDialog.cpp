#include "ModifyElementDialog.h"
#include "mainwindow.h"
#include <qcolordialog.h>
#include <assert.h>
#include "LogConsole.h"

ModifyElementDialog::ModifyElementDialog()
	: QDialog(NULL)
{
	ui.setupUi(this);
}

ModifyElementDialog::~ModifyElementDialog()
{
}

void ModifyElementDialog::init(const std::vector<Entity*> &enlist, const std::vector<Relationship*> &relist)
{
	this->enlist = enlist;
	this->relist = relist;

	ui.comboBox_AllSelectedElements->clear();

	/*init Combo*/
	for (int i = 0; i < enlist.size(); i++) {
		ui.comboBox_AllSelectedElements->addItem(QString("Entity: index(%1) Label(%2)").arg(enlist[i]->getIndex()).arg(QString::fromStdString(enlist[i]->getLabel())));
	}

	for (int i = 0; i < relist.size(); i++) {
		ui.comboBox_AllSelectedElements->addItem(QString("Relationship: index(%1) Label(%2)").arg(relist[i]->getIndex()).arg(QString::fromStdString(relist[i]->getLabel())));
	}

}

void ModifyElementDialog::updateUi(ElementProperties props)
{
	ui.lineEdit_Label->setText(QString::fromStdString(props.label));

	ui.doubleSpinBox_X->setValue(props.loc.x);
	ui.doubleSpinBox_Y->setValue(props.loc.y);
	ui.doubleSpinBox_Z->setValue(props.loc.z);

	current_color.setRgbF(props.color.R, props.color.G, props.color.B, props.color.Alpha);
	change_pushButton_ChangeColor_color(current_color);
}

ModifyElementDialog::ElementProperties ModifyElementDialog::getCurrentElementProperties(int currentSelection)
{
	int enNum = enlist.size();
	int reNum = relist.size();

	assert(currentSelection < enNum + reNum && currentSelection>=0);

	if(currentSelection <enNum)
		return ElementProperties(enlist[currentSelection]);
	if (currentSelection >= enNum && currentSelection < enNum + reNum) {
		return ElementProperties(relist[currentSelection - enNum]);
	}
}

ModifyElementDialog::ElementProperties ModifyElementDialog::getPropertiesFomUi()
{
	ElementProperties res;

	res.color = Vec4(current_color.redF(), current_color.greenF(), current_color.blueF(), current_color.alphaF());
	res.loc = Vec3(ui.doubleSpinBox_X->value(), ui.doubleSpinBox_Y->value(), ui.doubleSpinBox_Z->value());
	res.label = ui.lineEdit_Label->text().toStdString();

	return res;
}

void ModifyElementDialog::modifyElements(std::vector<Entity*> enlist, std::vector<Relationship*> relist, ElementProperties props)
{
	for (auto &en : enlist) {
		en->setLabel(props.label);
		en->setColor(props.color);
		en->setLocation(props.loc);
	}

	for (auto &re : relist) {
		re->setLabel(props.label);
		re->setColor(props.color);
		re->moveToLocation(props.loc.x, props.loc.y, STARRIZATIONR_FLAG::NOTSTARRIZE, UPDATESCAFFOLD_FLAG::NOTUPDATESCAFFOLD);
	}

	set<Relationship*> allRelis;
	for (unsigned int i = 0; i < enlist.size(); i++) {
		Entity *en = enlist[i];
		for (unsigned int j = 0; j < en->getIncidentRelationships().size(); j++) {
			Relationship *temp_r = en->getIncidentRelationship(j);
			allRelis.insert(temp_r);
		}
	}

	for (unsigned int i = 0; i < relist.size(); i++) {
		Relationship *temp_r = relist[i];

		allRelis.insert(temp_r);
		for (unsigned int j = 0; j < temp_r->getAdjacentRelationships().size(); j++) {
			allRelis.insert(temp_r->getAdjacentRelationship(j));
		}
	}

	for (set<Relationship*>::iterator it = allRelis.begin(); it != allRelis.end(); ++it) {
		Relationship *r = *it;
		r->update2D();
	}
}

void ModifyElementDialog::getCurrentColor(int & r, int & g, int & b, int & alpha)
{
	if (enlist.size() + relist.size() != 1)
		return;

	Vec4 color;
	if (enlist.size() == 1) {
		color = enlist[0]->getColor();
	}
	else {
		color = relist[0]->getColor();
	}

	r = color.R * 255;
	g = color.G * 255;
	b = color.B * 255;
	alpha = color.Alpha * 255;
}


void ModifyElementDialog::on_pushButton_ChangeColor_clicked()
{
	int br; int bg; int bb; int alpha;
	getCurrentColor(br, bg, bb, alpha);
	QColorDialog cd(QColor(br, bg, bb, alpha), this);
	cd.setOption(QColorDialog::ColorDialogOption::ShowAlphaChannel);
	int dialogCode = cd.exec();

	if (dialogCode == QDialog::Accepted) {
		QColor col = cd.currentColor();
		change_pushButton_ChangeColor_color(col);
	}
}

void ModifyElementDialog::on_comboBox_AllSelectedElements_currentIndexChanged(int index)
{
	currentSelection = index;

	if (currentSelection != -1) {
		backupProps = getCurrentElementProperties(currentSelection);
		updateUi(backupProps);
	}
}

void ModifyElementDialog::on_pushButton_Save_clicked() {
	
	int enNum = enlist.size();
	int reNum = relist.size();

	assert(currentSelection < enNum + reNum && currentSelection >= 0);

	std::vector<Entity*> toModifyEnlist;
	std::vector<Relationship*> toModifyRelist;

	if (currentSelection <enNum)
		toModifyEnlist.push_back(enlist[currentSelection]);
	if (currentSelection >= enNum && currentSelection < enNum + reNum) {
		toModifyRelist.push_back(relist[currentSelection - enNum]);
	}

	ElementProperties curr_prop = getPropertiesFomUi();
	modifyElements(toModifyEnlist, toModifyRelist, curr_prop);
	gmainwindow->updateWidgets();
	
	this->close();
}

void ModifyElementDialog::on_pushButton_ModifyAll_clicked()
{
	bool res = LogConsole::showConfirm("Set to all elements cannot be revert, sure to proceed?");

	if (!res)
		return;

	ElementProperties curr_prop = getPropertiesFomUi();
	modifyElements(enlist, relist, curr_prop);

	gmainwindow->updateWidgets();
	this->close();
}

void ModifyElementDialog::change_pushButton_ChangeColor_color(QColor color)
{
	current_color = color;

	QString qss = QString("background-color: rgba(%1,%2,%3,%4);").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha());
	ui.pushButton_ChangeColor->setStyleSheet(qss);
	ui.pushButton_ChangeColor->setText("");
}

void ModifyElementDialog::on_pushButton_Reset_clicked()
{
	int enNum = enlist.size();
	int reNum = relist.size();

	assert(currentSelection < enNum + reNum && currentSelection >= 0);

	std::vector<Entity*> toModifyEnlist;
	std::vector<Relationship*> toModifyRelist;

	if (currentSelection <enNum)
		toModifyEnlist.push_back(enlist[currentSelection]);
	if (currentSelection >= enNum && currentSelection < enNum + reNum) {
		toModifyRelist.push_back(relist[currentSelection - enNum]);
	}

	modifyElements(toModifyEnlist, toModifyRelist, backupProps);
	updateUi(backupProps);

	gmainwindow->updateWidgets();
}