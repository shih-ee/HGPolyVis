#pragma once
#include "JPMath/Vector4.h"

using namespace std;

const static int COLORCOUNT = 11;

const static Color4_8BIT TolColor_Bright[COLORCOUNT] = {
	Color4_8BIT(68,  119, 170, 127),
	Color4_8BIT(102, 204, 238, 127),
	Color4_8BIT(34,  136, 51,  127),
	Color4_8BIT(204, 187, 68,  127),
	Color4_8BIT(238, 102, 119, 127),
	Color4_8BIT(179, 51,  119, 127),
	Color4_8BIT(187, 187, 187, 127),
	// dummy
	Color4_8BIT(187, 187, 187, 127),
	Color4_8BIT(187, 187, 187, 127),
	Color4_8BIT(187, 187, 187, 127),
	Color4_8BIT(187, 187, 187, 127)
};

const static Color4_8BIT TolColor_Vibrant[COLORCOUNT] = {
	Color4_8BIT(0,   119, 187, 127),
	Color4_8BIT(51,  187, 238, 127),
	Color4_8BIT(0,   153, 136, 127),
	Color4_8BIT(238, 119, 51,  127),
	Color4_8BIT(204, 51,  17,  127),
	Color4_8BIT(238, 51,  119, 127),
	Color4_8BIT(187, 187, 187, 127),
	// dummy
	Color4_8BIT(187, 187, 187, 127),
	Color4_8BIT(187, 187, 187, 127),
	Color4_8BIT(187, 187, 187, 127),
	Color4_8BIT(187, 187, 187, 127)
};

const static Color4_8BIT TolColor_Muted[COLORCOUNT] = {
	Color4_8BIT(51,  34,  136, 127),
	Color4_8BIT(136, 204, 238, 127),
	Color4_8BIT(68,  170, 153, 127),
	Color4_8BIT(17,  119, 51,  127),
	Color4_8BIT(153, 153, 51,  127),
	Color4_8BIT(221, 204, 119, 127),
	Color4_8BIT(204, 102, 119, 127),
	Color4_8BIT(136, 34,  85,  127),
	Color4_8BIT(170, 68,  153, 127),
	Color4_8BIT(221, 221, 221, 127),
	// dummy
	Color4_8BIT(221, 221, 221, 127)
};

const static Color4_8BIT TolColor_Light[COLORCOUNT] = {
	Color4_8BIT(119, 170, 221, 127),
	Color4_8BIT(153, 221, 255, 127),
	Color4_8BIT(68,  187, 153, 127),
	Color4_8BIT(187, 204, 51,  127),
	Color4_8BIT(170, 170, 0,   127),
	Color4_8BIT(238, 221, 136, 127),
	Color4_8BIT(238, 136, 102, 127),
	Color4_8BIT(255, 170, 187, 127),
	Color4_8BIT(221, 221, 221, 127),
	// dummy
	Color4_8BIT(221, 221, 221, 127),
	Color4_8BIT(221, 221, 221, 127)
};

const static Color4_8BIT Predefined_1_PolyColor[COLORCOUNT] = {
	Color4_8BIT(155,	87,		38,		127),	//1
	Color4_8BIT(25,		153,	0,		127),	//2
	Color4_8BIT(255,	127,	0,		127),	//3
	Color4_8BIT(0,		127,	255,	127),	//4
	Color4_8BIT(153,	51,		255,	127),	//5
	Color4_8BIT(153,	255,	127,	127),	//6
	Color4_8BIT(255,	153,	153,	127),	//7
	Color4_8BIT(0,		230,	255,	127),	//8
	Color4_8BIT(0,		255,	255,	127),	//9
	Color4_8BIT(0,		255,	230,	127),	//10
	Color4_8BIT(150,	150,	150,	127)	//else
};

const static Color4_8BIT Predefined_1_PolyFontColor[COLORCOUNT] = {
	Color4_8BIT(155.0,	87.0,	38.0,	255),
	Color4_8BIT(0.0,	175,	0,		255),
	Color4_8BIT(179,	86,		0,		255),
	Color4_8BIT(0.0,	74.0,	153.0,	255),
	Color4_8BIT(98,	0,		179,	255),
	Color4_8BIT(31,	153,	0,		255),
	Color4_8BIT(153,	0.0,	0,		255),
	Color4_8BIT(0.0,	115,	128,	255),
	Color4_8BIT(0.0,	153,	153,	255),
	Color4_8BIT(0.0,	153,	153,	255),
	Color4_8BIT(0.0,	0,		0,		255),
};

//ColorBrewer https://colorbrewer2.org/#type=qualitative&scheme=Paired&n=11
const static Color4_8BIT ColorBrewer1[COLORCOUNT] = {
Color4_8BIT(166,206,227,127),
Color4_8BIT(31,120,180, 127),
Color4_8BIT(178,223,138, 127),
Color4_8BIT(51,160,44, 127),
Color4_8BIT(251,154,153, 127),
Color4_8BIT(227,26,28, 127),
Color4_8BIT(253,191,111, 127),
Color4_8BIT(255,127,0, 127),
Color4_8BIT(202,178,214, 127),
Color4_8BIT(106,61,154, 127),
Color4_8BIT(255,255,153, 127)
};

//ColorBrewer https://colorbrewer2.org/#type=qualitative&scheme=Set3&n=11
const static Color4_8BIT ColorBrewer2[COLORCOUNT] = {
//ColorBrewer2_sort_lightness_ace
Color4_8BIT(188,128,189, 127),
Color4_8BIT(179,222,105, 127),
Color4_8BIT(128,177,211, 127),
Color4_8BIT(141,211,199, 127),
Color4_8BIT(253,180,98 ,127 ),
Color4_8BIT(251,128,114, 127),
Color4_8BIT(190,186,218, 127),
Color4_8BIT(204,235,197, 127),
Color4_8BIT(217,217,217, 127),
Color4_8BIT(255,255,179, 127),
Color4_8BIT(252,205,229, 127),
};

//https://colorbrewer2.org/#type=sequential&scheme=PuRd&n=9
//sequential with 9 colors multiple hue
const static Color4_8BIT ColorBrewer_sequential_multihue[12][COLORCOUNT] = {
	{
	Color4_8BIT(247,252,253,127),
	Color4_8BIT(229,245,249,127),
	Color4_8BIT(204,236,230,127),
	Color4_8BIT(153,216,201,127),
	Color4_8BIT(102,194,164,127),
	Color4_8BIT(65,174,118,127),
	Color4_8BIT(35,139,69,127),
	Color4_8BIT(0,109,44,127),
	Color4_8BIT(0,68,27,127),
	//dummy
	Color4_8BIT(0,68,27,127),
	Color4_8BIT(0,68,27,127),
	},

	{
	Color4_8BIT(247,252,253	,127),
	Color4_8BIT(224,236,244	,127),
	Color4_8BIT(191,211,230	,127),
	Color4_8BIT(158,188,218	,127),
	Color4_8BIT(140,150,198	,127),
	Color4_8BIT(140,107,177	,127),
	Color4_8BIT(136,65,157	,127),
	Color4_8BIT(129,15,124	,127),
	Color4_8BIT(77,0,75		,127),
	//dummy
	Color4_8BIT(77,0,75		,127),
	Color4_8BIT(77,0,75		,127)
	},
{
	Color4_8BIT(247,252,240	,127),
	Color4_8BIT(224,243,219	,127),
	Color4_8BIT(204,235,197	,127),
	Color4_8BIT(168,221,181	,127),
	Color4_8BIT(123,204,196	,127),
	Color4_8BIT(78,179,211 	,127),
	Color4_8BIT(43,140,190 	,127),
	Color4_8BIT(8,104,172  	,127),
	Color4_8BIT(8,64,129   	,127),
	//dummy
	Color4_8BIT(8,64,129   	,127),
	Color4_8BIT(8,64,129   	,127),
	},
	{
	Color4_8BIT(255,247,236	,127),
	Color4_8BIT(254,232,200	,127),
	Color4_8BIT(253,212,158	,127),
	Color4_8BIT(253,187,132	,127),
	Color4_8BIT(252,141,89 	,127),
	Color4_8BIT(239,101,72 	,127),
	Color4_8BIT(215,48,31  	,127),
	Color4_8BIT(179,0,0	,127),
	Color4_8BIT(127,0,0	,127),
	//dummy
	Color4_8BIT(127,0,0	,127),
	Color4_8BIT(127,0,0	,127)
	},
	{
	Color4_8BIT(255,247,251,127),
	Color4_8BIT(236,231,242,127),
	Color4_8BIT(208,209,230,127),
	Color4_8BIT(166,189,219,127),
	Color4_8BIT(116,169,207,127),
	Color4_8BIT(54,144,192 ,127),
	Color4_8BIT(5,112,176  ,127),
	Color4_8BIT(4,90,141   ,127),
	Color4_8BIT(2,56,88,127),
	//dummy
	Color4_8BIT(2,56,88,127),
	Color4_8BIT(2,56,88,127)
	},
	{
	Color4_8BIT(255,247,251,127),
	Color4_8BIT(236,226,240,127),
	Color4_8BIT(208,209,230,127),
	Color4_8BIT(166,189,219,127),
	Color4_8BIT(103,169,207,127),
	Color4_8BIT(54,144,192 ,127),
	Color4_8BIT(2,129,138  ,127),
	Color4_8BIT(1,108,89   ,127),
	Color4_8BIT(1,70,54,127),
	//dummy
	Color4_8BIT(1,70,54,127),
	Color4_8BIT(1,70,54,127)
	},
	{
	Color4_8BIT(247,244,249,127),
	Color4_8BIT(231,225,239,127),
	Color4_8BIT(212,185,218,127),
	Color4_8BIT(201,148,199,127),
	Color4_8BIT(223,101,176,127),
	Color4_8BIT(231,41,138 ,127),
	Color4_8BIT(206,18,86  ,127),
	Color4_8BIT(152,0,67   ,127),
	Color4_8BIT(103,0,31   ,127),
	//dummy
	Color4_8BIT(103,0,31   ,127),
	Color4_8BIT(103,0,31   ,127)
	},
	{
	Color4_8BIT(255,247,243,127),
	Color4_8BIT(253,224,221,127),
	Color4_8BIT(252,197,192,127),
	Color4_8BIT(250,159,181,127),
	Color4_8BIT(247,104,161,127),
	Color4_8BIT(221,52,151 ,127),
	Color4_8BIT(174,1,126  ,127),
	Color4_8BIT(122,1,119  ,127),
	Color4_8BIT(73,0,106   ,127),
	//dummy
	Color4_8BIT(73,0,106   ,127),
	Color4_8BIT(73,0,106   ,127)
	},
	{
	Color4_8BIT(255,255,229,127),
	Color4_8BIT(247,252,185,127),
	Color4_8BIT(217,240,163,127),
	Color4_8BIT(173,221,142,127),
	Color4_8BIT(120,198,121,127),
	Color4_8BIT(65,171,93  ,127),
	Color4_8BIT(35,132,67  ,127),
	Color4_8BIT(0,104,55   ,127),
	Color4_8BIT(0,69,41,127),
	//dummy
	Color4_8BIT(0,69,41,127),
	Color4_8BIT(0,69,41,127)
	},
	{
	Color4_8BIT(255,255,217,127),
	Color4_8BIT(237,248,177,127),
	Color4_8BIT(199,233,180,127),
	Color4_8BIT(127,205,187,127),
	Color4_8BIT(65,182,196 ,127),
	Color4_8BIT(29,145,192 ,127),
	Color4_8BIT(34,94,168  ,127),
	Color4_8BIT(37,52,148  ,127),
	Color4_8BIT(8,29,88,127),
	//dummy
	Color4_8BIT(8,29,88,127),
	Color4_8BIT(8,29,88,127)
	},
	{
		Color4_8BIT(255, 255, 229,127),
		Color4_8BIT(255, 247, 188,127),
		Color4_8BIT(254, 227, 145,127),
		Color4_8BIT(254, 196, 79 ,127),
		Color4_8BIT(254, 153, 41 ,127),
		Color4_8BIT(236, 112, 20 ,127),
		Color4_8BIT(204, 76, 2	 ,127),
		Color4_8BIT(153, 52, 4	 ,127),
		Color4_8BIT(102, 37, 6	 ,127),
		//dummy
		Color4_8BIT(102, 37, 6	 ,127),
		Color4_8BIT(102, 37, 6	 ,127)
	},
	{
	Color4_8BIT(255, 255, 204,127),
	Color4_8BIT(255, 237, 160,127),
	Color4_8BIT(254, 217, 118,127),
	Color4_8BIT(254, 178, 76 ,127),
	Color4_8BIT(253, 141, 60 ,127),
	Color4_8BIT(252, 78, 42	 ,127),
	Color4_8BIT(227, 26, 28	 ,127),
	Color4_8BIT(189, 0, 38	 ,127),
	Color4_8BIT(128, 0, 38	 ,127),
	//dummy
	Color4_8BIT(128, 0, 38	 ,127),
	Color4_8BIT(128, 0, 38	 ,127)
	//Color4_8BIT(150,	150,	150,	127),
	//Color4_8BIT(150,	150,	150,	127)
	}
};

//https://colorbrewer2.org/#type=sequential&scheme=PuRd&n=9
//sequential with 9 colors single hue
const static Color4_8BIT ColorBrewer_sequential_singleHue[6][COLORCOUNT] = {
	{
	Color4_8BIT(247,251,255,127),
	Color4_8BIT(222,235,247,127),
	Color4_8BIT(198,219,239,127),
	Color4_8BIT(158,202,225,127),
	Color4_8BIT(107,174,214,127),
	Color4_8BIT(66,146,198 ,127),
	Color4_8BIT(33,113,181 ,127),
	Color4_8BIT(8,81,156   ,127),
	Color4_8BIT(8,48,107   ,127),
	//dummy
	Color4_8BIT(8,48,107   ,127),
	Color4_8BIT(8,48,107   ,127)
	},

	{
	Color4_8BIT(247,252,245,127),
	Color4_8BIT(229,245,224,127),
	Color4_8BIT(199,233,192,127),
	Color4_8BIT(161,217,155,127),
	Color4_8BIT(116,196,118,127),
	Color4_8BIT(65,171,93,127),
	Color4_8BIT(35,139,69,127),
	Color4_8BIT(0,109,44,127),
	Color4_8BIT(0,68,27 ,127),
	//dummy
	Color4_8BIT(0,68,27 ,127),
	Color4_8BIT(0,68,27 ,127)
	},
	{
	Color4_8BIT(255,255,255,127),
	Color4_8BIT(240,240,240,127),
	Color4_8BIT(217,217,217,127),
	Color4_8BIT(189,189,189,127),
	Color4_8BIT(150,150,150,127),
	Color4_8BIT(115,115,115,127),
	Color4_8BIT(82,82,82,127),
	Color4_8BIT(37,37,37,127),
	Color4_8BIT(0,0,0,127),
	//dummy
	Color4_8BIT(0,0,0,127),
	Color4_8BIT(0,0,0,127)
	},

	{
	Color4_8BIT(255,245,235,127),
	Color4_8BIT(254,230,206,127),
	Color4_8BIT(253,208,162,127),
	Color4_8BIT(253,174,107,127),
	Color4_8BIT(253,141,60,127),
	Color4_8BIT(241,105,19,127),
	Color4_8BIT(217,72,1,127),
	Color4_8BIT(166,54,3,127),
	Color4_8BIT(127,39,4,127),
	//dummy
	Color4_8BIT(127,39,4,127),
	Color4_8BIT(127,39,4,127)
	},

	{
	Color4_8BIT(252,251,253,127),
	Color4_8BIT(239,237,245,127),
	Color4_8BIT(218,218,235,127),
	Color4_8BIT(188,189,220,127),
	Color4_8BIT(158,154,200,127),
	Color4_8BIT(128,125,186,127),
	Color4_8BIT(106,81,163,127),
	Color4_8BIT(84,39,143,127),
	Color4_8BIT(63,0,125,127),
	//dummy
	Color4_8BIT(63,0,125,127),
	Color4_8BIT(63,0,125,127)
	},

	{
	Color4_8BIT(255,245,240,127),
	Color4_8BIT(254,224,210,127),
	Color4_8BIT(252,187,161,127),
	Color4_8BIT(252,146,114,127),
	Color4_8BIT(251,106,74,127),
	Color4_8BIT(239,59,44,127),
	Color4_8BIT(203,24,29,127),
	Color4_8BIT(165,15,21,127),
	Color4_8BIT(103,0,13,127),
	//dummy
	Color4_8BIT(103,0,13,127),
	Color4_8BIT(103,0,13,127)
	},
};

//https://colorbrewer2.org/#type=diverging&scheme=Spectral&n=11
const static Color4_8BIT ColorBrewer_diverging[9][COLORCOUNT] = {
{
	Color4_8BIT(84,48,5	   ,127),
	Color4_8BIT(140,81,10  ,127),
	Color4_8BIT(191,129,45 ,127),
	Color4_8BIT(223,194,125,127),
	Color4_8BIT(246,232,195,127),
	Color4_8BIT(245,245,245,127),
	Color4_8BIT(199,234,229,127),
	Color4_8BIT(128,205,193,127),
	Color4_8BIT(53,151,143 ,127),
	Color4_8BIT(1,102,94   ,127),
	Color4_8BIT(0,60,48	   ,127),
},
{
	Color4_8BIT(142,1,82   ,127),
	Color4_8BIT(197,27,125 ,127),
	Color4_8BIT(222,119,174,127),
	Color4_8BIT(241,182,218,127),
	Color4_8BIT(253,224,239,127),
	Color4_8BIT(247,247,247,127),
	Color4_8BIT(230,245,208,127),
	Color4_8BIT(184,225,134,127),
	Color4_8BIT(127,188,65 ,127),
	Color4_8BIT(77,146,33  ,127),
	Color4_8BIT(39,100,25  ,127),
},
{
	Color4_8BIT(64,0,75	   ,127),
	Color4_8BIT(118,42,131 ,127),
	Color4_8BIT(153,112,171,127),
	Color4_8BIT(194,165,207,127),
	Color4_8BIT(231,212,232,127),
	Color4_8BIT(247,247,247,127),
	Color4_8BIT(217,240,211,127),
	Color4_8BIT(166,219,160,127),
	Color4_8BIT(90,174,97  ,127),
	Color4_8BIT(27,120,55  ,127),
	Color4_8BIT(0,68,27	   ,127),
},
{
	Color4_8BIT(127,59,8   ,127),
	Color4_8BIT(179,88,6   ,127),
	Color4_8BIT(224,130,20 ,127),
	Color4_8BIT(253,184,99 ,127),
	Color4_8BIT(254,224,182,127),
	Color4_8BIT(247,247,247,127),
	Color4_8BIT(216,218,235,127),
	Color4_8BIT(178,171,210,127),
	Color4_8BIT(128,115,172,127),
	Color4_8BIT(84,39,136  ,127),
	Color4_8BIT(45,0,75	   ,127),
},
{
	Color4_8BIT(103,0,31   ,127),
	Color4_8BIT(178,24,43  ,127),
	Color4_8BIT(214,96,77  ,127),
	Color4_8BIT(244,165,130,127),
	Color4_8BIT(253,219,199,127),
	Color4_8BIT(247,247,247,127),
	Color4_8BIT(209,229,240,127),
	Color4_8BIT(146,197,222,127),
	Color4_8BIT(67,147,195 ,127),
	Color4_8BIT(33,102,172 ,127),
	Color4_8BIT(5,48,97	   ,127),
},
{
	Color4_8BIT(103,0,31   ,127),
	Color4_8BIT(178,24,43  ,127),
	Color4_8BIT(214,96,77  ,127),
	Color4_8BIT(244,165,130,127),
	Color4_8BIT(253,219,199,127),
	Color4_8BIT(255,255,255,127),
	Color4_8BIT(224,224,224,127),
	Color4_8BIT(186,186,186,127),
	Color4_8BIT(135,135,135,127),
	Color4_8BIT(77,77,77   ,127),
	Color4_8BIT(26,26,26   ,127),
},
{
	Color4_8BIT(165,0,38   ,127),
	Color4_8BIT(215,48,39  ,127),
	Color4_8BIT(244,109,67 ,127),
	Color4_8BIT(253,174,97 ,127),
	Color4_8BIT(254,224,144,127),
	Color4_8BIT(255,255,191,127),
	Color4_8BIT(224,243,248,127),
	Color4_8BIT(171,217,233,127),
	Color4_8BIT(116,173,209,127),
	Color4_8BIT(69,117,180 ,127),
	Color4_8BIT(49,54,149  ,127),
},
{
	Color4_8BIT(165,0,38   ,127),
	Color4_8BIT(215,48,39  ,127),
	Color4_8BIT(244,109,67 ,127),
	Color4_8BIT(253,174,97 ,127),
	Color4_8BIT(254,224,139,127),
	Color4_8BIT(255,255,191,127),
	Color4_8BIT(217,239,139,127),
	Color4_8BIT(166,217,106,127),
	Color4_8BIT(102,189,99 ,127),
	Color4_8BIT(26,152,80  ,127),
	Color4_8BIT(0,104,55   ,127),
},
{
	Color4_8BIT(158,1,66   ,127),
	Color4_8BIT(213,62,79  ,127),
	Color4_8BIT(244,109,67 ,127),
	Color4_8BIT(253,174,97 ,127),
	Color4_8BIT(254,224,139,127),
	Color4_8BIT(255,255,191,127),
	Color4_8BIT(230,245,152,127),
	Color4_8BIT(171,221,164,127),
	Color4_8BIT(102,194,165,127),
	Color4_8BIT(50,136,189 ,127),
	Color4_8BIT(94,79,162  ,127),
}
};