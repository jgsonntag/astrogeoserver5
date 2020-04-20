#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QSettings>
#include <QVariant>
#include <QDebug>
#include <QTime>

#include "genericclient.h"
#include "genericserver.h"
#include "utility.h"
#include "setupgui.h"
#include "querytopo.h"
#include "sunangle.h"

#include <stdio.h>

//FILE *fptr;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void createActions();
    void createMenus();
    void writeSettings();
    void readSettings();
    void closeEvent(QCloseEvent *event);
    bool received10;
    bool received11;
    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *exitAct;
    QAction *aboutAct;
    QString host;
    quint16 port;
    GenericClient *myClient;
    GenericServer *myServer;
    QTimer *timer;
    QTimer *astrotimer;
    QLabel *netlabel;
    QLabel *datelabel;
    QLabel *timelabel;
    QLabel *latlabel;
    QLabel *lonlabel;
    QLabel *sunazlabel;
    QLabel *sunellabel;
    QLabel *ellaltlabel;
    QLabel *ghtlabel;
    QLabel *thtlabel;
    QLabel *galtlabel;
    QLabel *agllabel;
    QLabel *serverlabel;
    QLabel *msglabel;
    QVBoxLayout *mainlayout;
    QString stemp;
    QString string12;
    QString string13;
    double lat;
    double lon;
    double h_ell;
    double ymd;
    double hms;
    double crs;
    double speed;
    double roc;
    int year;
    int month;
    int day;
    int hour;
    int min;
    double sec;
    double wks;
    char geoidid[10],demid[10];
    double geoid;
    double topo;
    double galt;
    double agl;
    double sunazac,sunelac;
    QGroupBox *netbox;
    QGroupBox *solarbox;
    FILE *fptr;
    char ctemp[200];

private slots:
    void slotStatus();
    void slotNewData(QString);
    void slotAbout();
    void slotAstroUpdate();

};

#endif // MAINWINDOW_H
