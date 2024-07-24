#pragma once

#include <QString>
#include <QColor>

class LogConsole
{
public:
	LogConsole();
	~LogConsole();

	/*[0 - 255]*/
	struct LOGCOLOR {
		LOGCOLOR(int mr, int mg, int mb) {
			R = mr;
			G = mg;
			B = mb;
		}
		union {
			struct {
				int R;
				int G;
				int B;
			};
			int element[3];
		};
	};

	static void writeGreenConsole(const char * msg, ...);
	static void writeRedConsole(const char * msg, ...);
	static void writeBlueSeparator();

	static void writeBlueConsole(const char * msg, ...);
	static void writeColorConsole(LOGCOLOR color, const char * msg, ...);
	static void writeColorConsole(QColor color, const char * msg, ...);
	static void writeColorConsole(float R, float G, float B, const char * msg, ...);
	static void writeConsole(const char * msg, ...);
	static void writeConsole(QString str);

	static void writeError(const char * msg, ...);
	static void showError(const char * msg, ...);
	static bool showConfirm(const char * msg, ...);
	static void showWarning(const char * msg, ...);
};

class DockTextEdit;
extern DockTextEdit *gconsole;



