#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "setupgui.h"

setupGui::setupGui(QString *host,quint16 *port)
{

    // Set the local host and port pointers to the parent's
    setuphost = host;
    setupport = port;

    // Set up the layout
    QWidget *dialog = new QWidget(this);
    QGridLayout *grid = new QGridLayout();

    // Create the dialog's component widgets
    QString stemp;
    stemp = *host;
    hostedit = new QLineEdit;
    QLabel *hosteditlabel = new QLabel("GPS network host");
    hostedit->setText(stemp);
    stemp.setNum (*port);
    portedit = new QLineEdit;
    QLabel *porteditlabel = new QLabel("GPS network port");
    portedit->setText(stemp);
    QPushButton *contbutton = new QPushButton("Continue");
    connect(contbutton,SIGNAL(clicked(bool)),this,SLOT(slotTestInputs()));

    // Create the validators
    portval = new QIntValidator;
    portval->setRange(4040,4070);
    portedit->setValidator(portval);

    // Populate the grid layout
    grid->addWidget(hostedit,1,1);
    grid->addWidget(hosteditlabel,1,2);
    grid->addWidget(portedit,2,1);
    grid->addWidget(porteditlabel,2,2);
    grid->addWidget(contbutton,3,1,1,-1);

    // Make it all happen
    dialog->setLayout(grid);
    dialog->show();

}


setupGui::~setupGui()
{

}


void setupGui::slotTestInputs()
{

    int pos;
    QString stemp = portedit->text();
    bool portisvalid = (portval->validate(stemp,pos)==QValidator::Acceptable);

    if (portisvalid)
    {
        *setuphost = hostedit->text();
        *setupport = stemp.toUShort();
        accept();
    }
    else
        reject();

}
