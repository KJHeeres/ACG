#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    qDebug() << "✓✓ MainWindow constructor";
    ui->setupUi(this);

    ui->controlNet->setChecked(true);
    ui->curvePoints->setChecked(false);

    // Input restrictions for the Mask
    ui->subdivMask->setValidator(new QRegularExpressionValidator(QRegularExpression("(-?\\d+\\s)+(-?\\d+\\s?)")));

    // Initialise mask
    ui->mainView->subCurve.setMask(ui->subdivMask->text());

}

MainWindow::~MainWindow() {
    qDebug() << "✗✗ MainWindow destructor";
    delete ui;
}

void MainWindow::on_controlNet_toggled(bool checked) {
    ui->mainView->settings.showNet = checked;
    ui->mainView->update();
}

void MainWindow::on_curvePoints_toggled(bool checked) {
    ui->mainView->subCurve.subdivide();
    ui->mainView->settings.showCurvePts = checked;
    ui->mainView->update();
    ui->mainView->updateBuffers();
}

void MainWindow::on_netPresets_currentIndexChanged(int index) {
    if (ui->mainView->isValid()) {
        ui->mainView->subCurve.presetNet(index);
    }
    ui->mainView->updateBuffers();
}

void MainWindow::on_subdivMask_returnPressed() {
    ui->mainView->subCurve.setMask(ui->subdivMask->text());
    ui->mainView->update();
}

void MainWindow::on_subdivSteps_valueChanged(int arg1) {
    ui->mainView->subCurve.setSubdivSteps(arg1);
    ui->mainView->subCurve.subdivide();
    ui->mainView->update();
    ui->mainView->updateBuffers();
}

void MainWindow::on_subdivisionType_currentTextChanged(QString subdivisionMethod)
{
    ui->mainView->subCurve.setSubdivMethod(subdivisionMethod);
    ui->mainView->update();
    ui->mainView->updateBuffers();
}
