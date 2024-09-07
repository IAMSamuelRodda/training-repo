#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <QHBoxLayout>
#include <QWidget>

class QLabel;

class Viewer : public QWidget
{
    Q_OBJECT

public:
    Viewer(QWidget *parent = nullptr);
    ~Viewer();

    void setUseBase64(bool base64);
    void imageChanged(QString path);
    void setLabelTwo(QString text);

signals:
    void pathChanged(QString path);

public slots:
    void setImage(QString path);
    void setPixmapData(QByteArray data);
    void onUpdateClicked();
    void setLabelOneNumber(int number);
    void setLabelTwoNumber(int number);
    void setLabelThreeNumber(int number);

    void setLabelThree(QString text);
    void saveImage(QString filePath);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent * pEvent);
    void paintEvent(QPaintEvent * pEvent);

private:
    QString m_imagePath;
    QPixmap m_pixmap;

    QLabel * m_pLabel1;
    QLabel * m_pLabel2;
    QLabel * m_pLabel3;

    bool m_blue;
    bool m_loadFromFile;
    bool m_useBase64;
    bool m_loadFailed;
    bool m_loadSuccess;
};

#endif // VIEWER_HPP
