#ifndef ZZNREMARK_H
#define ZZNREMARK_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class ZZNRemark; }
QT_END_NAMESPACE

class ZZNRemark : public QMainWindow
{
    Q_OBJECT

private:
    static const int m_itemId = 1;
    QStringList m_imageFileList;
    QStringList m_settingInfo;
    QGraphicsScene *m_scene;
    QGraphicsItem *m_selectItem;
    QPointF m_pressPoint;
    QPixmap m_selectImage;
    QString m_defaultPath;
    int m_selectIndex = 0;
    int m_frontZValue = 0;
    int m_backZValue = 0;

    void drawRectangle(QPointF start,QPointF end,QString itemID,QColor color);
    bool checkConfigFile();
    void initUI();
    void getRemarkList();
    void selectRectPosToZero();
    void graphicsItemsReduce();
    void saveOldRemarkInfo(QString imageFilePath);
    void loadNewRemarkInfo(QString imageFilePath);
    void deleteSeleteItem();

protected:
    void closeEvent(QCloseEvent *event);

public:
    ZZNRemark(QWidget *parent = nullptr);
    ~ZZNRemark();

private slots:
    void on_actionOpen_triggered();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_mousePressPoint(QPointF point);
    void on_mouseReleasePoint(QPointF point);
    void on_mouseDoubleClicked(QPointF point);
    void on_keyPress(QKeyEvent* event);
    void on_keyRelease(QKeyEvent* event);
    void on_focusItemChanged(QGraphicsItem *newFocus, QGraphicsItem *oldFocus, Qt::FocusReason reason);
    void on_wheelSlideUp(bool isUp);

    void on_actionSettingType_triggered();
    void on_actionDefautPath_triggered();
    void on_comboBoxType_currentTextChanged(const QString &arg1);
    void on_comboBoxType_activated(const QString &arg1);

    void on_spinBoxX_valueChanged(int arg1);
    void on_spinBoxY_valueChanged(int arg1);
    void on_spinBoxWidth_valueChanged(int arg1);
    void on_spinBoxHeight_valueChanged(int arg1);

    void on_actionFront_triggered();
    void on_actionBack_triggered();
    void on_actionDelete_triggered();

    void on_actionReduce_triggered();
    void on_actionOriginalSize_triggered();

    void on_actionAbout_triggered();

private:
    Ui::ZZNRemark *ui;
};
#endif // ZZNREMARK_H
