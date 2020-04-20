#ifndef SETUPGUI_H
#define SETUPGUI_H

#include <QDialog>
#include <QLineEdit>
#include <QIntValidator>

class setupGui : public QDialog
{
    Q_OBJECT

public:
    setupGui(QString *host,quint16 *port);
    ~setupGui();

private:
    QLineEdit *hostedit;
    QLineEdit *portedit;
    QIntValidator *portval;
    QString *setuphost;
    quint16 *setupport;

private slots:
    void slotTestInputs();

};

#endif // SETUPGUI_H
