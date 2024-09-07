#include "Viewer.hpp"

#include <QFile>
#include <QtGlobal>
#include <QtCore>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QKeyEvent>

#include <QHBoxLayout>
#include <QLabel>

#include <QBrush>
#include <QPainter>
#include <QPixmap>

Viewer::Viewer(QWidget *parent): QWidget(parent),
    m_pLabel1(new QLabel("Test")),
    m_pLabel2(new QLabel("123")),
    m_pLabel3(new QLabel("ABC")),
    m_blue(false),
    m_loadFromFile(false),
    m_useBase64(false),
    m_loadFailed(false),
    m_loadSuccess(false)
{
    QWidget * pBob = new QWidget();
    QHBoxLayout * pBobLayout = new QHBoxLayout(pBob);

    pBobLayout->addWidget(m_pLabel1);
    pBobLayout->addWidget(new QLabel(":"));
    pBobLayout->addWidget(m_pLabel2);
    pBobLayout->addWidget(new QLabel(":"));
    pBobLayout->addWidget(m_pLabel3);
    pBobLayout->addWidget(new QLabel(";"),1);

    m_pLabel1->setAlignment(Qt::AlignLeft);
    m_pLabel2->setAlignment(Qt::AlignCenter);
    m_pLabel3->setAlignment(Qt::AlignRight);

    //connect(m_pUi->updateButton, SIGNAL(clicked()), m_pUi->viewer, SLOT(onUpdateClicked()));

    QHBoxLayout * pLayout = new QHBoxLayout(this);
    pLayout->addWidget(pBob);

    setFocusPolicy(Qt::StrongFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setAcceptDrops(true);
}

Viewer::~Viewer() {
    delete m_pLabel1;
    delete m_pLabel2;
    delete m_pLabel3;
}

void Viewer::setUseBase64(bool base64)
{
    m_useBase64 = base64;
}

void Viewer::setImage(QString path)
{
    m_imagePath = path;
    emit pathChanged(m_imagePath);
    if (m_loadFromFile) {
        m_pixmap = QPixmap(m_imagePath);
    }
}

void Viewer::setPixmapData(QByteArray data)
{
    qDebug("Loading from data");
    m_loadFailed = false;
    if (!m_loadFromFile) {
        if (m_useBase64) {
            if (!m_pixmap.loadFromData(QByteArray::fromBase64(data))) {
                qDebug("Failed to load data");
                m_loadFailed = true;
            }
        } else {
            if (!m_pixmap.loadFromData(data)) {
                qDebug("Failed to load data");
                m_loadFailed = true;
            }
        }
        update();
    }
}

void Viewer::saveImage(QString filePath)
{
    m_pixmap.save(filePath, nullptr, 100); //C++ is not as smart as python. If you want to use the 3rd input, you need to use the 2nd as well.
}



void Viewer::onUpdateClicked()
{
    qDebug("Update buddon pressed");
    return;
    update();
}

void Viewer::setLabelOneNumber(int number)
{
    m_pLabel1->setText(QString::number(number,10));
}

void Viewer::setLabelTwoNumber(int number)
{
    QString numberStr = QString::number(number,2);

    while (numberStr.size() < 8)
    {
        numberStr = QString("0")+numberStr;
    }

    m_pLabel2->setText(numberStr);
}

void Viewer::setLabelThreeNumber(int number)
{
    QString numberStr = QString::number(number,16);

    while (numberStr.size() < 2)
    {
        numberStr = QString("0")+numberStr;
    }
    m_pLabel3->setText(QString("0x")+numberStr);
}

void Viewer::setLabelTwo(QString text)
{
    m_pLabel2->setText(text);
}

void Viewer::setLabelThree(QString text)
{
    m_pLabel3->setText(text);
}

void Viewer::keyPressEvent(QKeyEvent *pEvent)
{
    switch (pEvent->key())
    {
    case Qt::Key_B:
        m_blue = true;
        update();
        break;
    case Qt::Key_A:
        m_pLabel1->setText("Apple");
    case Qt::Key_R:
        update();
        m_blue = false;
        break;
    }
}

void Viewer::dragEnterEvent(QDragEnterEvent *event)
{
    for (int i=0; i<event->mimeData()->formats().size(); i++) {
        qDebug("    %s",event->mimeData()->formats().at(i).toUtf8().data());
    }

    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void Viewer::dropEvent(QDropEvent *event)
{

    QString path = event->mimeData()->text();

#ifdef Q_OS_WIN
    path = path.remove("file:///");
#else
    path = path.remove("file://");
#endif

    qDebug("%s",path.toUtf8().data());
    if (QFile::exists(path) && (
            path.endsWith(".jpg") || path.endsWith(".png") || path.endsWith(".JPG"))) {
        event->acceptProposedAction();
        m_imagePath = path;
        if (m_loadFromFile) {
            m_pixmap = QPixmap(m_imagePath);
        }
        emit pathChanged(m_imagePath);
        update();
    }
}

void Viewer::paintEvent(QPaintEvent * pEvent)
{
    int border = 5;
    QPainter painter(this);

    float widthScale = m_pixmap.width()/width();
    float heightScale = m_pixmap.height()/height();

    if (m_imagePath != "") {
        if (heightScale > widthScale) {
            m_pixmap = m_pixmap.scaledToHeight(height()-11);
        } else {
            m_pixmap = m_pixmap.scaledToWidth(width()-11);
        }
    }

    painter.setPen(Qt::black);
    if (hasFocus()) {
        if (m_blue) {
            painter.setBrush(QBrush(QColor(Qt::blue)));
        } else {
            painter.setBrush(QBrush(QColor(Qt::black)));
        }

    } else {
        painter.setBrush(QBrush(QColor(217,217,217)));
    }

    if (m_loadFailed) {
        painter.setBrush(QBrush(QColor(Qt::red)));
    }
    painter.drawRect(0,0, width()-1, height()-1);
    if (m_imagePath != "") {
        painter.drawPixmap(border/2+width()/2-m_pixmap.width()/2-1, border/2+height()/2-m_pixmap.height()/2-1, m_pixmap);
    }

    QWidget::paintEvent(pEvent);
}
