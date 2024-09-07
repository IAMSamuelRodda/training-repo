#include "MainWindow.hpp"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QFile>
#include <QTemporaryFile>
#include <QUuid>

//Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pUi(new Ui::MainWindow)
    , m_useHex(false)
    , m_useBase64(true)
{
    m_pUi->setupUi(this);

    m_version = QString("0.0b1");
    qDebug("this 0x%X", this);

    m_pUi->viewer->setFocus();
    m_pUi->viewer->setUseBase64(m_useBase64);

    connect(m_pUi->okButton, SIGNAL(clicked()), this, SLOT(setLabelToBob()));
    connect(m_pUi->okButton, SIGNAL(clicked()), this, SLOT(onOkClicked()));
    //connect(m_pUi->updateButton, SIGNAL(clicked()), m_pUi->viewer, SLOT(onUpdateClicked()));
    connect(m_pUi->updateButton, SIGNAL(clicked()), this, SLOT(saveImage()));


    connect(this, SIGNAL(sendingText(QString)), m_pUi->viewer, SLOT(setLabelThree(QString)));
    connect(m_pUi->horizontalSlider, SIGNAL(valueChanged(int)), m_pUi->viewer, SLOT(setLabelOneNumber(int)));
    connect(m_pUi->horizontalSlider, SIGNAL(valueChanged(int)), m_pUi->viewer, SLOT(setLabelTwoNumber(int)));;
    connect(m_pUi->horizontalSlider, SIGNAL(valueChanged(int)), m_pUi->viewer, SLOT(setLabelThreeNumber(int)));;

    connect(m_pUi->actionImport, SIGNAL(triggered(bool)), this, SLOT(onImport()));
    connect(m_pUi->actionQuit, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(m_pUi->actionAbout_Qt, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(aboutQt()));

    connect(m_pUi->viewer, SIGNAL(pathChanged(QString)), this, SLOT(onImagePathChanged(QString)));
    connect(m_pUi->textEdit, SIGNAL(textChanged()),this, SLOT(onTextChanged()));

}

//Destructor
MainWindow::~MainWindow()
{
    delete m_pUi;
}


void MainWindow::saveImage()
{
    QString extension = "jpg";
    QUuid uuid = QUuid::createUuid();
    QString tempFileFullPath = QDir::toNativeSeparators(QDir::tempPath() + "/" + qApp->applicationName().replace(" ", "") + "_" + uuid.toString(QUuid::WithoutBraces) + "." +  extension);
    m_pUi->viewer->saveImage(tempFileFullPath);
}


void MainWindow::setLabelToBob()
{
    m_pUi->viewer->setLabelTwo("Bob");
}

void MainWindow::onOkClicked()
{
    emit sendingText(m_pUi->person->text());
}



void MainWindow::onImagePathChanged(QString path)
{
    QFile image(path);
    QByteArray data;

    if ( !image.exists()) {
        qDebug("Error: File doesn't exist: %s", path.toUtf8().data());
        return;
    }
    if (!image.open(QIODevice::ReadOnly)) {
        qDebug("Error: Couldn't open file: %s", path.toUtf8().data());
        return;
    }

    if (0 != 0) {
        //goto cleanup;
    }

    if (!m_useHex) {
        if (m_useBase64) {
            QByteArray read = image.readAll();
            data = read.toBase64();
        } else {
            data = image.readAll();
        }
    } else {
        while (!image.atEnd()) {
            QByteArray line = image.readLine();
            for (int i=0; i< line.size(); i++) {
                //QString byte = QString::number((quint64)line.at(i),64);
                //QString byte = line.toBase64();
                //data.append(byte);
            }
        }
    }
    m_pUi->textEdit->setPlainText(data);

    bool debugFile = false;
    if (debugFile) {
        QString extension = path.split(".").last();

        QUuid uuid = QUuid::createUuid();
        QString tempFileFullPath = QDir::toNativeSeparators(QDir::tempPath() + "/" + qApp->applicationName().replace(" ", "") + "_" + uuid.toString(QUuid::WithoutBraces) + "." +  extension);


        qDebug("Temp File: %s", tempFileFullPath.toUtf8().data());
        QFile tmpFile(tempFileFullPath);

        auto result = QByteArray::fromBase64Encoding(data);
        if (result.decodingStatus != QByteArray::Base64DecodingStatus::Ok) {
            qDebug("Decode Error %d", result.decodingStatus);
        }

        if (tmpFile.open(QIODevice::WriteOnly)) {
            tmpFile.write(result.decoded);
            tmpFile.flush();
            tmpFile.close();
        } else {
            qDebug("Couldn't open temp file");
        }
    }

cleanup:
    image.close();
}

void MainWindow::onTextChanged()
{
    m_pUi->viewer->setPixmapData(m_pUi->textEdit->toPlainText().toUtf8());
#if 0
    QTemporaryFile tmpFile;
    if (! tmpFile.open()) {
        qDebug("Couldn't open tmp file");
    } else {
        qDebug("TempFile: %s",tmpFile.fileName().toUtf8().data());
        QByteArray ba;
        ba.append(m_pUi->textEdit->toPlainText().toUtf8());
        tmpFile.write(QByteArray::fromBase64(ba));
        tmpFile.flush();
        tmpFile.close();
    }
#endif
}

void MainWindow::onImport()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/simon", tr("Image Files (*.png *.jpg *.bmp)"));
    m_pUi->viewer->setImage(fileName);
}

QString *MainWindow::getVersion()
{
    return &m_version;
}
