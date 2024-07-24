#include "LightSettingDialog.h"
#include "LogConsole.h"
#include <QDoubleSpinBox>

LightSettingDialog::LightSettingDialog(OGLWidget * vis)
	: QDialog(NULL)
{
	ui.setupUi(this);

	if (vis == NULL) {
		LogConsole::showError("invalide vis!");
	}
	focusedVis = vis;
	syncUI();

	QList<QDoubleSpinBox*> spinBoxes = findChildren<QDoubleSpinBox*>();
	for(auto spinBox : spinBoxes)
		connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLight(double)));

}

LightSettingDialog::~LightSettingDialog()
{
}

void LightSettingDialog::syncUI()
{
	OGLLight l0 = focusedVis->light0;
	OGLLight l1 = focusedVis->light1;

	ui.checkBox_UseLIght0->setChecked(l0.enabled);
	ui.checkBox_UseLIght1->setChecked(l1.enabled);
	ui.checkBox_Draw_Lights->setChecked(focusedVis->flag_ShowLight);

	ui.doubleSpinBox_lightGlyphSize->setValue(focusedVis->lightsize);
	ui.doubleSpinBox_CylinderShininess->setValue(focusedVis->cylinderShininess);
	ui.doubleSpinBox_SphereShininess->setValue(focusedVis->SphereShininess);


	setDoubleBoxValue(ui.doubleSpinBox_LightX,l0.position[0]);
	setDoubleBoxValue(ui.doubleSpinBox_LightY,l0.position[1]);
	setDoubleBoxValue(ui.doubleSpinBox_LightZ,l0.position[2]);
	setDoubleBoxValue(ui.doubleSpinBox_LightX_2,l1.position[0]);
	setDoubleBoxValue(ui.doubleSpinBox_LightY_2,l1.position[1]);
	setDoubleBoxValue(ui.doubleSpinBox_LightZ_2,l1.position[2]);

	setDoubleBoxValue(ui.doubleSpinBox_AmbientR, l0.ambient[0]);
	setDoubleBoxValue(ui.doubleSpinBox_AmbientG, l0.ambient[1]);
	setDoubleBoxValue(ui.doubleSpinBox_AmbientB, l0.ambient[2]);
	setDoubleBoxValue(ui.doubleSpinBox_AmbientA, l0.ambient[2]);

	setDoubleBoxValue(ui.doubleSpinBox_DiffuseR, l0.diffuse[0]);
	setDoubleBoxValue(ui.doubleSpinBox_DiffuseG, l0.diffuse[1]);
	setDoubleBoxValue(ui.doubleSpinBox_DiffuseB, l0.diffuse[2]);
	setDoubleBoxValue(ui.doubleSpinBox_DiffuseA, l0.diffuse[2]);

	setDoubleBoxValue(ui.doubleSpinBox_SpecularR, l0.specular[0]);
	setDoubleBoxValue(ui.doubleSpinBox_SpecularG, l0.specular[1]);
	setDoubleBoxValue(ui.doubleSpinBox_SpecularB, l0.specular[2]);
	setDoubleBoxValue(ui.doubleSpinBox_SpecularA, l0.specular[2]);

	setDoubleBoxValue(ui.doubleSpinBox_AmbientR_2, l1.ambient[0]);
	setDoubleBoxValue(ui.doubleSpinBox_AmbientG_2, l1.ambient[1]);
	setDoubleBoxValue(ui.doubleSpinBox_AmbientB_2, l1.ambient[2]);
	setDoubleBoxValue(ui.doubleSpinBox_AmbientA_2, l1.ambient[2]);

	setDoubleBoxValue(ui.doubleSpinBox_DiffuseR_2, l1.diffuse[0]);
	setDoubleBoxValue(ui.doubleSpinBox_DiffuseG_2, l1.diffuse[1]);
	setDoubleBoxValue(ui.doubleSpinBox_DiffuseB_2, l1.diffuse[2]);
	setDoubleBoxValue(ui.doubleSpinBox_DiffuseA_2, l1.diffuse[2]);

	setDoubleBoxValue(ui.doubleSpinBox_SpecularR_2, l1.specular[0]);
	setDoubleBoxValue(ui.doubleSpinBox_SpecularG_2, l1.specular[1]);
	setDoubleBoxValue(ui.doubleSpinBox_SpecularB_2, l1.specular[2]);
	setDoubleBoxValue(ui.doubleSpinBox_SpecularA_2, l1.specular[2]);

}

void LightSettingDialog::setDoubleBoxValue(QDoubleSpinBox *box, double value)
{
	box->setValue(value);
}

void LightSettingDialog::getDoubleBoxValue(QDoubleSpinBox * box, GLfloat & value)
{
	value = box->value();
}

void LightSettingDialog::updateLight(double)
{

	focusedVis->light0.enabled = ui.checkBox_UseLIght0->isChecked();
	focusedVis->light1.enabled = ui.checkBox_UseLIght1->isChecked();

	/*light 0*/
	getDoubleBoxValue(ui.doubleSpinBox_LightX, focusedVis->light0.position[0]);
	getDoubleBoxValue(ui.doubleSpinBox_LightY, focusedVis->light0.position[1]);
	getDoubleBoxValue(ui.doubleSpinBox_LightZ, focusedVis->light0.position[2]);

	getDoubleBoxValue(ui.doubleSpinBox_AmbientR, focusedVis->light0.ambient[0]);
	getDoubleBoxValue(ui.doubleSpinBox_AmbientG, focusedVis->light0.ambient[1]);
	getDoubleBoxValue(ui.doubleSpinBox_AmbientB, focusedVis->light0.ambient[2]);
	getDoubleBoxValue(ui.doubleSpinBox_AmbientA, focusedVis->light0.ambient[3]);
	
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseR, focusedVis->light0.diffuse[0]);
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseG, focusedVis->light0.diffuse[1]);
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseB, focusedVis->light0.diffuse[2]);
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseA, focusedVis->light0.diffuse[3]);
	
	getDoubleBoxValue(ui.doubleSpinBox_SpecularR, focusedVis->light0.specular[0]);
	getDoubleBoxValue(ui.doubleSpinBox_SpecularG, focusedVis->light0.specular[1]);
	getDoubleBoxValue(ui.doubleSpinBox_SpecularB, focusedVis->light0.specular[2]);
	getDoubleBoxValue(ui.doubleSpinBox_SpecularA, focusedVis->light0.specular[3]);

	/*light 1*/
	getDoubleBoxValue(ui.doubleSpinBox_LightX_2, focusedVis->light1.position[0]);
	getDoubleBoxValue(ui.doubleSpinBox_LightY_2, focusedVis->light1.position[1]);
	getDoubleBoxValue(ui.doubleSpinBox_LightZ_2, focusedVis->light1.position[2]);
	
	getDoubleBoxValue(ui.doubleSpinBox_AmbientR_2, focusedVis->light1.ambient[0]);
	getDoubleBoxValue(ui.doubleSpinBox_AmbientG_2, focusedVis->light1.ambient[1]);
	getDoubleBoxValue(ui.doubleSpinBox_AmbientB_2, focusedVis->light1.ambient[2]);
	getDoubleBoxValue(ui.doubleSpinBox_AmbientA_2, focusedVis->light1.ambient[3]);
	
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseR_2, focusedVis->light1.diffuse[0]);
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseG_2, focusedVis->light1.diffuse[1]);
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseB_2, focusedVis->light1.diffuse[2]);
	getDoubleBoxValue(ui.doubleSpinBox_DiffuseA_2, focusedVis->light1.diffuse[3]);
	
	getDoubleBoxValue(ui.doubleSpinBox_SpecularR_2, focusedVis->light1.specular[0]);
	getDoubleBoxValue(ui.doubleSpinBox_SpecularG_2, focusedVis->light1.specular[1]);
	getDoubleBoxValue(ui.doubleSpinBox_SpecularB_2, focusedVis->light1.specular[2]);
	getDoubleBoxValue(ui.doubleSpinBox_SpecularA_2, focusedVis->light1.specular[3]);

	focusedVis->updateLights();
	focusedVis->update();
}

void LightSettingDialog::on_checkBox_UseLIght1_clicked(bool checked)
{
	focusedVis->light1.enabled = checked;
	focusedVis->updateLights();
	focusedVis->update();
}

void LightSettingDialog::on_checkBox_UseLIght0_clicked(bool checked)
{
	focusedVis->light0.enabled = checked;
	focusedVis->updateLights();
	focusedVis->update();
}

void LightSettingDialog::on_checkBox_Draw_Lights_clicked(bool checked)
{
	focusedVis->flag_ShowLight = checked;
	focusedVis->update();
}

void LightSettingDialog::on_doubleSpinBox_lightGlyphSize_valueChanged(double arg)
{
	focusedVis->lightsize = arg;
	focusedVis->update();
}

void LightSettingDialog::on_doubleSpinBox_CylinderShininess_valueChanged(double arg)
{
	focusedVis->cylinderShininess = arg;
	focusedVis->update();
}

void LightSettingDialog::on_doubleSpinBox_SphereShininess_valueChanged(double arg)
{
	focusedVis->SphereShininess = arg;
	focusedVis->update();
}

void LightSettingDialog::on_pushButton_ResetLightSources_clicked()
{
	focusedVis->initLights();
	syncUI();
	focusedVis->update();
}
