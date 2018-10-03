#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QFontDatabase>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QQmlDebuggingEnabler>

#include "qmlbridge.h"

QT_CHARTS_USE_NAMESPACE

int main(int argc, char *argv[])
{

	QQmlDebuggingEnabler debug;

	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

	qmlRegisterType<QMLBridge>("hulaloop.qmlbridge", 1, 0, "QMLBridge");

	QFontDatabase::addApplicationFont(":/fonts/materialdesignicons-webfont.ttf");
	QQuickStyle::setStyle("Material");

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}
