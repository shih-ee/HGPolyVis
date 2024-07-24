#pragma once

#include <QDialog>
#include "Entity.h"
#include "Relationship.h"
#include "ui_ModifyElementDialog.h"

class Entity;
class Relationship;

class ModifyElementDialog : public QDialog
{
	Q_OBJECT

public:
	ModifyElementDialog();
	~ModifyElementDialog();

	void init(const std::vector<Entity*> &enlist, const std::vector<Relationship*> &srelist);

	struct ElementProperties {

		ElementProperties() {
			label = "";
			color = Vec4(0.0);
			loc = Vec3(0.0);
		}

		ElementProperties(Entity* en) {
			this->label = en->getLabel();
			this->color = en->getColor();
			this->loc = en->getLocation();
		};

		ElementProperties(Relationship* en) {
			this->label = en->getLabel();
			this->color = en->getColor();
			this->loc = en->getCenter(en->getMonogonLength());
		};

		string label;
		Vec4 color;
		Vec3 loc;
	};


	void updateUi(ElementProperties props);
	ElementProperties getCurrentElementProperties(int currentSelection);
	ElementProperties getPropertiesFomUi();

	void modifyElements(std::vector<Entity*> enlist, std::vector<Relationship*> relist, ElementProperties props);
	void getCurrentColor(int &r, int &g, int &b, int &alpha);

public slots:
	
	void on_pushButton_Save_clicked();
	void on_pushButton_Reset_clicked();
	void on_pushButton_ChangeColor_clicked();
	void on_comboBox_AllSelectedElements_currentIndexChanged(int index);
	void on_pushButton_ModifyAll_clicked();
	void change_pushButton_ChangeColor_color(QColor current_color);

private:
	QColor current_color;

	std::vector<Entity*> enlist;
	std::vector<Relationship*> relist;
	int currentSelection;

	ElementProperties backupProps;
	Ui::ModifyElementDialog ui;
};
