#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString *getVersion();

signals:
    void sendingText(QString);

public slots:
    void setLabelToBob();
    void onOkClicked();
    void onImport();
    void onImagePathChanged(QString path);
    void onTextChanged();
    void saveImage();

protected:
private:
    Ui::MainWindow *m_pUi; //this is Qt magic. It takes our Form (MainWindow.ui) and tunrs it into a variable that we can use!

    QString m_version;
    bool m_useHex;
    bool m_useBase64;
};
#endif // MAINWINDOW_HPP
