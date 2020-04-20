#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fptr = fopen("astrogeoserver.log","w");

    // Set up the styles
    qApp->setStyleSheet("QGroupBox { background: white }");

    // Initialize the geoid and topo databases
    initquerygeoid();
    initquerytopo();

    // Retrieve the saved settings
    readSettings();

    // Get starting info from user
    setupGui *setup = new setupGui(&host,&port);
    while (setup->exec() == QDialog::Rejected)
    {
        QMessageBox msgBox;
        msgBox.setText("Invalid input");
        msgBox.exec();
    }

    // Create the connection to the GPS server
    myClient = new GenericClient(this,host,port);
    connect(myClient,SIGNAL(newLine(QString)),this,SLOT(slotNewData(QString)));

    // Create the GPS server
    myServer = new GenericServer(this,4070);

    // Periodically check the connection to the server
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotStatus()));
    timer->start(500);

    // Build a timer solely for computing astronomical quantities
    astrotimer = new QTimer(this);
    connect(astrotimer,SIGNAL(timeout()),this,SLOT(slotAstroUpdate()));
    astrotimer->start(1000); // Once per second

    // Build the menu bar
    createActions();
    createMenus();

    // Window management
    setWindowTitle("AstroGeoServer5");
    setMinimumSize(160,160);
    resize(300,820);
    QWidget *window = new QWidget(this);

    // Build the network status area
    netbox = new QGroupBox("GPS status");
    QHBoxLayout *netlayout = new QHBoxLayout();
    netlabel = new QLabel();
    netlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    netlabel->setAlignment(Qt::AlignCenter);
    netlayout->addWidget(netlabel);
    netbox->setLayout(netlayout);

    // Build the date and time area
    QGroupBox *datetimebox = new QGroupBox("Date and time (UTC)");
    QHBoxLayout *datetimelayout = new QHBoxLayout();
    datelabel = new QLabel();
    timelabel = new QLabel();
    datelabel->setFont(QFont("Helvetica",14,QFont::Bold));
    timelabel->setFont(QFont("Helvetica",14,QFont::Bold));
    datetimelayout->addWidget(datelabel);
    datetimelayout->addWidget(timelabel);
    datetimebox->setLayout(datetimelayout);

    // Build the lat and lon area
    QGroupBox *latlonbox = new QGroupBox("Latitude and longitude (deg)");
    QHBoxLayout *latlonlayout = new QHBoxLayout();
    latlabel = new QLabel();
    lonlabel = new QLabel();
    latlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    lonlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    latlonlayout->addWidget(latlabel);
    latlonlayout->addWidget(lonlabel);
    latlonbox->setLayout(latlonlayout);

    // Build the solar data area
    solarbox = new QGroupBox("Sun azimuth and elevation (deg)");
    QHBoxLayout *solarlayout = new QHBoxLayout();
    sunazlabel = new QLabel();
    sunellabel = new QLabel();
    sunazlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    sunellabel->setFont(QFont("Helvetica",14,QFont::Bold));
    solarlayout->addWidget(sunazlabel);
    solarlayout->addWidget(sunellabel);
    solarbox->setLayout(solarlayout);

    // Build the ellipsoid altitude area
    QGroupBox *ellaltbox = new QGroupBox("Altitude relative to ellipsoid (ft)");
    QHBoxLayout *ellaltlayout = new QHBoxLayout();
    ellaltlabel = new QLabel();
    ellaltlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    ellaltlabel->setAlignment(Qt::AlignCenter);
    ellaltlayout->addWidget(ellaltlabel);
    ellaltbox->setLayout(ellaltlayout);

    // Build the geoid height area
    QGroupBox *ghtbox = new QGroupBox("Geoid height relative to ellipsoid (ft)");
    QHBoxLayout *ghtlayout = new QHBoxLayout();
    ghtlabel = new QLabel();
    ghtlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    ghtlabel->setAlignment(Qt::AlignCenter);
    ghtlayout->addWidget(ghtlabel);
    ghtbox->setLayout(ghtlayout);

    // Build the topo height area
    QGroupBox *thtbox = new QGroupBox("Topo height relative to geoid (ft)");
    QHBoxLayout *thtlayout = new QHBoxLayout();
    thtlabel = new QLabel();
    thtlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    thtlabel->setAlignment(Qt::AlignCenter);
    thtlayout->addWidget(thtlabel);
    thtbox->setLayout(thtlayout);

    // Build the geoid altitude area
    QGroupBox *galtbox = new QGroupBox("Altitude relative to geoid (ft)");
    QHBoxLayout *galtlayout = new QHBoxLayout();
    galtlabel = new QLabel();
    galtlabel->setFont(QFont("Helvetica",14,QFont::Bold));
    galtlabel->setAlignment(Qt::AlignCenter);
    galtlayout->addWidget(galtlabel);
    galtbox->setLayout(galtlayout);

    // Build the AGL area
    QGroupBox *aglbox = new QGroupBox("AGL Altitude (ft)");
    QHBoxLayout *agllayout = new QHBoxLayout();
    agllabel = new QLabel();
    agllabel->setFont(QFont("Helvetica",14,QFont::Bold));
    agllabel->setAlignment(Qt::AlignCenter);
    agllayout->addWidget(agllabel);
    aglbox->setLayout(agllayout);

    // Build the network server area
    QFont msgfont("Courier",8);
    QGroupBox *serverbox = new QGroupBox("Network server");
    QVBoxLayout *serverlayout = new QVBoxLayout();
    serverlabel = new QLabel();
    msglabel = new QLabel();
    msglabel->setFont(msgfont);
    msglabel->setWordWrap(true);
    serverlayout->addWidget(serverlabel);
    serverlayout->addWidget(msglabel);
    serverbox->setLayout(serverlayout);

    // Finalize the overall GUI
    QVBoxLayout *mainlayout = new QVBoxLayout;
    mainlayout->addWidget(netbox);
    mainlayout->addWidget(datetimebox);
    mainlayout->addWidget(latlonbox);
    mainlayout->addWidget(solarbox);
    mainlayout->addWidget(ellaltbox);
    mainlayout->addWidget(ghtbox);
    mainlayout->addWidget(thtbox);
    mainlayout->addWidget(galtbox);
    mainlayout->addWidget(aglbox);
    mainlayout->addWidget(serverbox);
    window->setLayout(mainlayout);
    setCentralWidget(window);

}


MainWindow::~MainWindow()
{

    delete ui;
    closequerygeoid();
    closequerytopo();

    fclose(fptr);

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();
}


void MainWindow::writeSettings()
{

    QSettings settings("NASA","AstroGeoServer5");
    settings.setValue("host",host);
    settings.setValue("port",port);

}


void MainWindow::readSettings()
{

    QSettings settings("NASA","AstroGeoServer5");
    host = settings.value("host","localhost").toString();
    port = settings.value("port",4062).toInt();

}


void MainWindow::createMenus()
{

    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction(aboutAct);

}


void MainWindow::createActions()
{

    exitAct = new QAction("Exit",this);
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

    aboutAct = new QAction("About",this);
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(slotAbout()));

}


void MainWindow::slotAbout()
{
    QMessageBox msgBox;
    msgBox.setText("AstroGeoServer5\n"
                   "Qt5-Based Astronomical and Geophysical Server\n"
                   "Author: John G. Sonntag\n"
                   "Released: 1 April 2018");
    msgBox.exec();
}


void MainWindow::slotStatus()
{

    if (myClient->slotConnectStatus())
    {
        netlabel->setText("Connected to server");
        netbox->setStyleSheet("background-color:white;");
    }
    else
    {
        netlabel->setText("Disconnected from server");
        netbox->setStyleSheet("background-color:red;");
    }
}


void MainWindow::slotAstroUpdate()
{

    // Date info not available, skipping
    if (year==9999)
    {
      stemp.sprintf("---.--");
      sunazlabel->setText(stemp);
      stemp.sprintf("--.--");
      sunellabel->setText(stemp);
      solarbox->setStyleSheet("background-color:red;");
    }
    else
    {
      getsunangle(lat*DEG2RAD,lon*DEG2RAD,h_ell,year,month,day,hour,min,sec,
                  &sunazac,&sunelac);
      stemp.sprintf("%06.2lf",sunazac/DEG2RAD);
      sunazlabel->setText(stemp);
      stemp.sprintf("%6.2lf",sunelac/DEG2RAD);
      sunellabel->setText(stemp);
      solarbox->setStyleSheet("background-color:white;");
    }

}

void MainWindow::slotNewData(QString lineQstring)
{

    QTime *currtime = new QTime(QTime::currentTime());
    double sod = currtime->hour()*3600.0+currtime->minute()*60.0
                 +currtime->second()+currtime->msec()/1000.0;

    std::string str = lineQstring.toStdString();
    const char* p = str.c_str();
    fprintf(fptr,"%09.3lf,%s",sod,p);
    //fprintf(fptr,"%02d:%02d:%02d.%03d\n",currtime->hour(),currtime->minute(),
    //        currtime->second(),currtime->msec());

    // Determine the message type and parse as appropriate
    received10 = false;
    received11 = false;
    stemp = lineQstring.section(',',0,0);
    if (stemp=="11")
    {
      received11 = true;
      stemp = lineQstring.section(',',1,1);
      ymd = stemp.toDouble();
      parseymd(ymd,&year,&month,&day);
      stemp = lineQstring.section(',',2,2);
      hms = stemp.toDouble();
      parsehms(hms,&hour,&min,&sec);
      stemp = lineQstring.section(',',3,3);
      lat = stemp.toDouble();
      stemp = lineQstring.section(',',4,4);
      lon = stemp.toDouble();
      stemp = lineQstring.section(',',5,5);
      h_ell = stemp.toDouble();
      stemp = lineQstring.section(',',6,6);
      crs = stemp.toDouble();
      stemp = lineQstring.section(',',7,7);
      speed = stemp.toDouble();
      stemp = lineQstring.section(',',8,8);
      roc = stemp.toDouble();
    }
    else if (stemp=="10")
    {
      received10 = true;
      stemp = lineQstring.section(',',1,1);
      wks = stemp.toDouble();
      sec2hms(wks,&hour,&min,&sec);
      year = 9999;
      month = 99;
      day = 99;
      stemp = lineQstring.section(',',2,2);
      lat = stemp.toDouble();
      stemp = lineQstring.section(',',3,3);
      lon = stemp.toDouble();
      stemp = lineQstring.section(',',4,4);
      h_ell = stemp.toDouble();
      stemp = lineQstring.section(',',6,6);
      crs = stemp.toDouble();
      stemp = lineQstring.section(',',7,7);
      speed = stemp.toDouble();
      stemp = lineQstring.section(',',8,8);
      roc = stemp.toDouble();
    }
    else
    {
      qDebug() << "Unrecognized network message type\n";
      fprintf(fptr,"Unrecognized network message type\n");
    }

    // Update the date and time fields
    if (year==9999)
        stemp.sprintf("-----------");
    else
        stemp.sprintf("%04d-%02d-%02d",year,month,day);
    datelabel->setText(stemp);
    stemp.sprintf("%02d:%02d:%04.1lf",hour,min,sec);
    timelabel->setText(stemp);

    // Update the lat and lon fields
    stemp.sprintf("%8.4lf",lat);
    latlabel->setText(stemp);
    stemp.sprintf("%8.4lf",lon);
    lonlabel->setText(stemp);

    // Update the ht-ellipsoid field
    stemp.sprintf("%5.0lf",h_ell);
    ellaltlabel->setText(stemp);

    // Update height of geoid relative to height of ellipsoid
    while (lon<=-180.0) lon+=360.0;
    while (lon>180.0) lon-=360.0;
    geoid = querygeoid(lat,lon,geoidid)*3.28;
    stemp.sprintf("%4.0lf\t(%3s)",geoid,geoidid);
    ghtlabel->setText(stemp);

    // Update height of topography, relative to geoid
    topo = querytopo(lat,lon,demid)*3.28;
    if (!strncmp(demid,"G90\0",3)) topo=topo-geoid;
    stemp.sprintf("%5.0lf\t(%3s)",topo,demid);
    thtlabel->setText(stemp);

    // Update the altitude relative to geoid
    galt = h_ell-geoid;
    stemp.sprintf("%5.0lf",galt);
    galtlabel->setText(stemp);

    // Update the AGL
    double agl = galt-topo;
    stemp.sprintf("%5.0lf",agl);
    agllabel->setText(stemp);

    // Send the new network message, format depends on what was received
    stemp.sprintf("Serving on port 4070\nActive network listeners: %02d\nCurrent broadcast text:\n",
                     myServer->numActiveConnections());
    serverlabel->setText(stemp);
    if (received10) // send message12 format
    {
        string12.sprintf("12,%8.1lf,%10.6lf,%10.6lf,%8.2lf,%8.2lf,%6.1lf,%5.0lf,",
                         wks,lat,lon,h_ell,crs,speed,roc);
        stemp.sprintf("%8.2lf,%8.2lf\n",galt,agl);
        string12.append(stemp);
        myServer->sendText(string12);
        msglabel->setText(string12);
    }
    else if (received11) // send message13 format
    {
        string13.sprintf("13,%04d%02d%02d,%02d%02d%04.1lf,%10.6lf,%10.6lf,%8.2lf,",
                           year,month,day,hour,min,sec,lat,lon,h_ell);
        stemp.sprintf("%8.2lf,%6.1lf,%5.0lf,%8.2lf,%8.2lf,%06.2lf,%6.2lf\n",
                      crs,speed,roc,galt,agl,sunazac/DEG2RAD,sunelac/DEG2RAD);
        string13.append(stemp);
        myServer->sendText(string13);
        msglabel->setText(string13);
    }

}
