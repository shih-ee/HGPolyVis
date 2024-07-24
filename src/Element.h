#pragma once
#include <string>
#include "JPMath/Vector3.h"
#include "globalVariables.h"
#include "ColorPalette.h"
#include "OperationRecord.h"

class OperationRecord;

class Element
{
protected:

	std::string m_label = "NULL";
	int m_index = -1;
	int m_opidx = -1;
	ELEMENTTYPE m_type;

	OperationRecord* m_opRemoval = NULL;
	vector<OperationRecord*> m_opMerges;

	/*Visual data*/
	bool m_LabelVisibility = true;
	bool m_Selected = false;
	bool m_Visibility = true;	//decided by the filters
	bool m_DrawFlag = true;
	Color4_F m_Color;

	//double m_weight = 1.0;
	int m_SimpFactor = 0;

public:
	Element();
	~Element();

	void setLabel(std::string label);
	std::string getLabel();

	virtual void setLabelWithIndex();

	void setIndex(int index);
	int getIndex() const;

	void setVisibility(bool res);
	bool getVisibility();

	void setSelected(bool res);
	bool getSelected();

	void setDrawFlag(bool flag);
	bool getDrawFlag();

	void setColor(Color4_F color);
	Color4_F getColor();
	
	/*Color Scheme*/
	static Color4_8BIT polyColor[COLORCOUNT];	//current color scheme
	static Color4_8BIT polyFontColor[COLORCOUNT];
	static Color4_F getDiscreteColor(int card);

	static void initPolyColors(const char*file);
	static void initPolyColors(const Color4_8BIT ColorBrewer1[], int n = COLORCOUNT);
	static void initPolyFontColors(const Color4_8BIT ColorBrewer1[], int n = COLORCOUNT);

	static double m_DigonLensAngle;
	static double m_VertexSize;
	static double m_TriangleThickness;
	static double m_MonogonSegments;
	static double m_MonogonLength;
	static double m_MonogonSize;
	static double m_MultipleDigonOffset;

	inline bool getLabelVisibility() { return m_LabelVisibility; };
	void setLabelVisibility(bool res);

	// Multiscale related
	void setOpidx(int opidx);
	int getOpidx() const;
	void setRemovalOp(OperationRecord* op_remvoal);
	void addMergerOp(OperationRecord* op_merger);
	void setMergerOps(vector<OperationRecord*> merger_list);
	OperationRecord* getRemovalOp();
	double getRemovalCost();
	double getRemovalImportance();
	double getRemovalPriority();
	vector<OperationRecord*> getMergerOps();
	void deleteOperations();
	//double getWeight();
	//void setWeight(double w);

	ELEMENTTYPE getType();

	int getSimpFactor();
	void incrementSimpFactor();
	void decrementSimpFactor();
};

