#include "ZZNRemark.h"
#include "ui_ZZNRemark.h"
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QBuffer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "DefaultPathDialog.h"
#include "ZZNGlobal.h"
#include "ZZNCommon.h"
#include <QSettings>
#include "SettingTypeDialog.h"
#include <QColorDialog>
#include <QKeyEvent>
#include <QtMath>
#include "ZZNGraphicsScene.h"
#include <QDateTime>

const int GraphicsViewWidth = 1308;
const int GraphicsViewHeight = 981;
ZZNRemark::ZZNRemark(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ZZNRemark)
{
    ui->setupUi(this);
    initUI();
}

void ZZNRemark::initUI()
{
    m_scene = new ZZNGraphicsScene();
    m_scene->setSceneRect(0,0,ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setCursor(Qt::CrossCursor);
    ui->graphicsView->setMouseTracking(true);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    ui->graphicsView->setScene(m_scene);
    ui->actionReduce->setEnabled(false);
    ui->actionOriginalSize->setEnabled(false);

    QObject::connect(m_scene,SIGNAL(sceneMousePressPoint(QPointF)),this,SLOT(on_mousePressPoint(QPointF)));
    QObject::connect(m_scene,SIGNAL(sceneMouseReleasePoint(QPointF)),this,SLOT(on_mouseReleasePoint(QPointF)));
    QObject::connect(m_scene,SIGNAL(sceneMouseDoubleClicked(QPointF)),this,SLOT(on_mouseDoubleClicked(QPointF)));
    QObject::connect(m_scene,SIGNAL(sceneKeyPress(QKeyEvent*)),this,SLOT(on_keyPress(QKeyEvent*)));
    QObject::connect(m_scene,SIGNAL(sceneKeyRelease(QKeyEvent*)),this,SLOT(on_keyRelease(QKeyEvent*)));
    QObject::connect(m_scene,SIGNAL(sceneWheelSlideUp(bool)),this,SLOT(on_wheelSlideUp(bool)));
    QObject::connect(m_scene,&QGraphicsScene::focusItemChanged,this,&ZZNRemark::on_focusItemChanged);
    getRemarkList();
}

void ZZNRemark::getRemarkList()
{
    QStringList remarkList;
    ui->comboBoxType->clear();
    foreach (QString infoStr, ZZNCommon::getRemarkInfo()) {
        QStringList item = infoStr.split(" ");
        if(item.count() == 3){
            remarkList.append(item.at(0));
        }
    }
    ui->comboBoxType->addItems(remarkList);
}

ZZNRemark::~ZZNRemark()
{
    delete ui;
}

void ZZNRemark::on_actionOpen_triggered()
{
    QString dlgTitle = "选择图片";
    QString fileFilter = "图片文件(*.jpg *.png *.bmp);;所有文件(*.*)";
    QStringList fileList = QFileDialog::getOpenFileNames(this,
                                                         dlgTitle,
                                                         ZZNCommon::getImageDefaultPath(),
                                                         fileFilter);
    if(fileList.count()<1) return;
    int curentCount = ui->listWidget->count();
    for (int i=0;i<fileList.count() ;i++ ) {
        QString aFile = fileList.at(i);
        m_imageFileList.append(aFile);
        QFileInfo fileInfo(aFile);
        ui->listWidget->addItem(QString::number(curentCount+i+1)+"."+fileInfo.fileName());
    }
}


void ZZNRemark::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow == -1) return;

    if(!(m_selectImage.load(m_imageFileList[currentRow]))){
        QMessageBox::information(this,
                                 tr("打开图像"),
                                 tr("打开图像失败!"));
        return;
    }

    if(m_selectIndex != currentRow){
        saveOldRemarkInfo(m_imageFileList.at(m_selectIndex));
        m_selectIndex = currentRow;
    }
    graphicsItemsReduce();
}

void ZZNRemark::on_mousePressPoint(QPointF point)
{
    if(m_imageFileList.empty()){
        QMessageBox::information(this,
                                 tr("打开图像"),
                                 tr("请先选择要标注的图像!"));
        return;
    }

    QGraphicsItem *item = NULL;
    item = m_scene->itemAt(point,ui->graphicsView->transform());
    if(item == NULL){
        m_pressPoint = point;
        m_scene->clearSelection();
        selectRectPosToZero();
        return;
    }
}

void ZZNRemark::on_mouseReleasePoint(QPointF point)
{
    QGraphicsItem *item = NULL;
    item = m_scene->itemAt(point,ui->graphicsView->transform());
    if(item == NULL){
        drawRectangle(m_pressPoint,
                      point,
                      ZZNCommon::searchInfoFromIndex(ui->comboBoxType->currentText(),
                                                      ZZNCommon::SearchRemark,
                                                      ZZNCommon::SearchRemarkNo),
                      ZZNCommon::nameToColor(ZZNCommon::searchInfoFromIndex(ui->comboBoxType->currentText(),
                                                                              ZZNCommon::SearchRemark,
                                                                              ZZNCommon::SearchRemarkColor)));
    }else{
        if(m_scene->selectedItems().count()==1 && m_scene->selectedItems().at(0)->pos().x()!=0){
            QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
            if(m_selectItem->x()<-rect.x()) m_selectItem->setX(-rect.x());
            if(m_selectItem->y()<-rect.y()) m_selectItem->setY(-rect.y());

            ui->spinBoxX->setValue(rect.x() + m_selectItem->x());
            ui->spinBoxY->setValue(rect.y() + m_selectItem->y());
        }
    }
}

template<class T> void setBrushColor(T *item)
{
    QColor color = item->brush().color();
    color = QColorDialog::getColor(color,NULL,"选择填充颜色",QColorDialog::ShowAlphaChannel);
    if(color.isValid()){
        item->setBrush(QBrush(color));
    }
}

void ZZNRemark::on_mouseDoubleClicked(QPointF point)
{
    QGraphicsItem *item = NULL;
    item = m_scene->itemAt(point,ui->graphicsView->transform());
    if(item == NULL) return;
    switch(item->type()){
        case QGraphicsRectItem::Type:
        {
            QGraphicsRectItem *theItem;
            theItem = qgraphicsitem_cast<QGraphicsRectItem*>(item);
            //setBrushColor(theItem);
            break;
        }
    }
}

void ZZNRemark::on_keyPress(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Delete:
        //on_actionDelete_triggered();
        deleteSeleteItem();
        break;
    case Qt::Key_Left:
        if(m_scene->selectedItems().count() >0){
            foreach (QGraphicsItem *item, m_scene->selectedItems()) {
                item->setX(-1+item->x());
            }
        }
        if(m_scene->selectedItems().count()>1){
            selectRectPosToZero();
        }
        break;
    case Qt::Key_Right:
        if(m_scene->selectedItems().count() >0){
            foreach (QGraphicsItem *item, m_scene->selectedItems()) {
                item->setX(1+item->x());
            }
        }
        if(m_scene->selectedItems().count()>1){
            selectRectPosToZero();
        }
        break;
    case Qt::Key_Up:
        if(m_scene->selectedItems().count() >0){
            foreach (QGraphicsItem *item, m_scene->selectedItems()) {
                item->setY(-1+item->y());
            }
        }
        if(m_scene->selectedItems().count()>1){
            selectRectPosToZero();
        }
        break;
    case Qt::Key_Down:
        if(m_scene->selectedItems().count() >0){
            foreach (QGraphicsItem *item, m_scene->selectedItems()) {
                item->setY(1+item->y());
            }
        }
        if(m_scene->selectedItems().count()>1){
            selectRectPosToZero();
        }
        break;
    case Qt::Key_Equal:
        if(ui->actionOriginalSize->isEnabled()){
            on_actionOriginalSize_triggered();
        }
        break;
    case Qt::Key_Minus:
        if(ui->actionReduce->isEnabled()){
            on_actionReduce_triggered();
        }
        break;
// 此处可以设置常用类别的快捷方式
    case Qt::Key_1://person
        ui->comboBoxType->setCurrentIndex(0);
        break;
/*
    case Qt::Key_2://
        ui->comboBoxType->setCurrentIndex(1);
        break;
    case Qt::Key_3://
        ui->comboBoxType->setCurrentIndex(2);
        break;
    case Qt::Key_4://
        ui->comboBoxType->setCurrentIndex(3);
        break;
    case Qt::Key_5://
        ui->comboBoxType->setCurrentIndex(4);
        break;
    case Qt::Key_6://
        ui->comboBoxType->setCurrentIndex(6);
        break;
    case Qt::Key_7://
        ui->comboBoxType->setCurrentIndex(7);
        break;
    case Qt::Key_8://
        ui->comboBoxType->setCurrentIndex(9);
        break;
    case Qt::Key_9://
        ui->comboBoxType->setCurrentIndex(14);
        break;
        */
    }
}

void ZZNRemark::on_keyRelease(QKeyEvent* event)
{
    if(m_scene->selectedItems().count()!=1) return;
    switch (event->key()) {
    case Qt::Key_Left:
    {
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        if(m_selectItem->x()<-rect.x()) m_selectItem->setX(-rect.x());
        ui->spinBoxX->setValue(rect.x() + m_selectItem->x());
        break;
    }
    case Qt::Key_Right:
    {
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        ui->spinBoxX->setValue(rect.x() + m_selectItem->x());
        break;
    }
    case Qt::Key_Up:
    {
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        if(m_selectItem->y()<-rect.y()) m_selectItem->setY(-rect.y());
        ui->spinBoxY->setValue(rect.y() + m_selectItem->y());
        break;
    }
    case Qt::Key_Down:
    {
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        ui->spinBoxY->setValue(rect.y() + m_selectItem->y());
        break;
    }
    }
}

void ZZNRemark::on_wheelSlideUp(bool isUp)
{
    if(isUp){
        if(ui->actionOriginalSize->isEnabled() && !ui->actionReduce->isEnabled()){
            on_actionOriginalSize_triggered();
        }
    }else{
        if(!ui->actionOriginalSize->isEnabled() && ui->actionReduce->isEnabled()){
            on_actionReduce_triggered();
        }
    }
}

void ZZNRemark::on_focusItemChanged(QGraphicsItem *newFocus, QGraphicsItem *oldFocus, Qt::FocusReason reason)
{
    Q_UNUSED(oldFocus)
    if(Qt::MouseFocusReason == reason && newFocus){
        m_selectItem = newFocus;
        QString itemId = newFocus->data(m_itemId).toString();
        ui->comboBoxType->setCurrentText(ZZNCommon::searchInfoFromIndex(itemId,
                                                                        ZZNCommon::SearchRemarkNo,
                                                                        ZZNCommon::SearchRemark));
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(newFocus)->rect();
        ui->spinBoxX->setValue(rect.x()+newFocus->x());
        ui->spinBoxY->setValue(rect.y()+newFocus->y());
        ui->spinBoxWidth->setValue(rect.width());
        ui->spinBoxHeight->setValue(rect.height());
        ui->groupBoxRect->setEnabled(true);
    }
}

void ZZNRemark::drawRectangle(QPointF start,QPointF end,QString itemID,QColor color)
{
    qreal posX = qMin(start.x(),end.x());
    qreal posY = qMin(start.y(),end.y());
    qreal width = qAbs(start.x()-end.x());
    qreal height = qAbs(start.y()-end.y());
    if(end.x()>m_scene->width()||end.y()>m_scene->height()||width<=3||height<=3) return;
    QGraphicsRectItem *item = new QGraphicsRectItem(posX,posY,width,height);
    item->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsFocusable);
    item->setPen(QPen(color));
    item->setZValue(++ m_frontZValue);
    item->setBrush(QBrush(Qt::transparent));
    item->setData(m_itemId,itemID);
    m_scene->addItem(item);
    m_scene->clearSelection();
}

bool ZZNRemark::checkConfigFile()
{
    QFileInfo file(JSBZGlobal::g_AppConfigFileName);
    return file.exists();
}

void ZZNRemark::on_actionSettingType_triggered()
{
    SettingTypeDialog *dlg = new SettingTypeDialog(this);
    Qt::WindowFlags flags=dlg->windowFlags();
    dlg->setWindowFlags(flags|Qt::MSWindowsFixedSizeDialogHint);
    int res = dlg->exec();
    if(res == 0 ){
        delete dlg;
        getRemarkList();
    }
}

void ZZNRemark::on_actionDefautPath_triggered()
{
    DefaultPathDialog *dlg = new DefaultPathDialog(this);
    dlg->setGeometry(this->x()+52,this->y()+42,dlg->width(),dlg->height());
    Qt::WindowFlags flags=dlg->windowFlags();
    dlg->setWindowFlags(flags|Qt::MSWindowsFixedSizeDialogHint);
    int res = dlg->exec();
    if(res == 0 ){
        m_defaultPath = ZZNCommon::getImageDefaultPath();
        delete dlg;
    }
}

void ZZNRemark::on_comboBoxType_currentTextChanged(const QString &arg1)
{

    QPalette palette;
    palette.setColor(QPalette::Background,
                     ZZNCommon::nameToColor(ZZNCommon::searchInfoFromIndex(arg1,
                                                                           ZZNCommon::SearchRemark,
                                                                           ZZNCommon::SearchRemarkColor)));
    ui->labelColorType->setAutoFillBackground(true);
    ui->labelColorType->setPalette(palette);
}


void ZZNRemark::on_comboBoxType_activated(const QString &arg1)
{
    QColor color= ZZNCommon::nameToColor(ZZNCommon::searchInfoFromIndex(arg1,
                                                                        ZZNCommon::SearchRemark,
                                                                        ZZNCommon::SearchRemarkColor));
    QPen currentPen = QPen(color);
    foreach (QGraphicsItem *item, m_scene->selectedItems()) {
        item->setData(m_itemId,
                      ZZNCommon::searchInfoFromIndex(ui->comboBoxType->currentText(),
                                                     ZZNCommon::SearchRemark,
                                                     ZZNCommon::SearchRemarkNo));
        qgraphicsitem_cast<QGraphicsRectItem*>(item)->setPen(currentPen);
        item->setSelected(false);
        item->clearFocus();
    }
    selectRectPosToZero();
}

void ZZNRemark::on_spinBoxX_valueChanged(int arg1)
{
    if(m_scene->selectedItems().count() == 1){
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        m_selectItem->setX(arg1-rect.x());
    }
}

void ZZNRemark::on_spinBoxY_valueChanged(int arg1)
{
    if(m_scene->selectedItems().count() == 1){
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        m_selectItem->setY(arg1-rect.y());
    }
}

void ZZNRemark::on_spinBoxWidth_valueChanged(int arg1)
{
    if(m_scene->selectedItems().count() == 1){
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        rect.setWidth(arg1);
        qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->setRect(rect);
    }
}

void ZZNRemark::on_spinBoxHeight_valueChanged(int arg1)
{
    if(m_scene->selectedItems().count() == 1){
        QRectF rect = qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->rect();
        rect.setHeight(arg1);
        qgraphicsitem_cast<QGraphicsRectItem*>(m_selectItem)->setRect(rect);
    }
}

void ZZNRemark::selectRectPosToZero()
{
    ui->spinBoxX->setValue(0);
    ui->spinBoxY->setValue(0);
    ui->spinBoxWidth->setValue(1);
    ui->spinBoxHeight->setValue(1);
    ui->groupBoxRect->setEnabled(false);
}

void ZZNRemark::saveOldRemarkInfo(QString imageFilePath)
{
    //if(m_scene->items().count()<=0) return;
    QFile aFile(imageFilePath.left(imageFilePath.length()-3)+"txt");
    if(!aFile.open(QIODevice::WriteOnly|QIODevice::Text)) return;
    QString saveStr;
    QRectF rect;
    foreach (QGraphicsItem *item, m_scene->items()) {
        rect = qgraphicsitem_cast<QGraphicsRectItem*>(item)->rect();
        saveStr+=item->data(m_itemId).toString() + " ";
        saveStr+=QString("%1").arg((rect.x()+item->x())*1.0/m_scene->sceneRect().width() + rect.width()/2.0/m_scene->sceneRect().width()) + " ";
        saveStr+=QString("%1").arg((rect.y()+item->y())*1.0/m_scene->sceneRect().height() + rect.height()/2.0/m_scene->sceneRect().height()) + " ";
        saveStr+=QString("%1").arg(rect.width()*1.0/m_scene->sceneRect().width()) + " ";
        saveStr+=QString("%1").arg(rect.height()*1.0/m_scene->sceneRect().height()) +"\n";
    }
    QByteArray strBytes = saveStr.left(saveStr.length()-1).toUtf8();
    aFile.write(strBytes,strBytes.length());
    aFile.close();
}

void ZZNRemark::loadNewRemarkInfo(QString imageFilePath)
{
    m_scene->clear();
    QFile aFile(imageFilePath.left(imageFilePath.length()-3)+"txt");
    if(!aFile.exists()) return;//判断是否在相同路径下有同样的txt文件
    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text)) return;
    foreach (QString strItem, QString(aFile.readAll()).split("\n")) {
        QStringList itemInfo = strItem.split(" ");
        if(itemInfo.count() == 5){
            QPoint start = QPoint(qRound(itemInfo.at(1).toFloat() * m_scene->sceneRect().width() - qRound(itemInfo.at(3).toFloat() / 2.0 * m_scene->sceneRect().width())),
                                  qRound(itemInfo.at(2).toFloat() * m_scene->sceneRect().height() - qRound(itemInfo.at(4).toFloat() / 2.0 * m_scene->sceneRect().height())));
            QPoint end = QPoint(qRound(itemInfo.at(1).toFloat() * m_scene->sceneRect().width() + qRound(itemInfo.at(3).toFloat() / 2.0 * m_scene->sceneRect().width())),
                                qRound(itemInfo.at(2).toFloat() * m_scene->sceneRect().height() + qRound(itemInfo.at(4).toFloat() / 2.0 * m_scene->sceneRect().height())));
            drawRectangle(start,
                          end,
                          itemInfo.at(0),
                          ZZNCommon::nameToColor(ZZNCommon::searchInfoFromIndex(itemInfo.at(0),
                                                                                ZZNCommon::SearchRemarkNo,
                                                                                ZZNCommon::SearchRemarkColor)));
        }
    }
    aFile.close();
}

void ZZNRemark::closeEvent(QCloseEvent *event)
{
    if(!m_imageFileList.isEmpty()){
        saveOldRemarkInfo(m_imageFileList.at(m_selectIndex));
    }
    event->accept();
}


void ZZNRemark::on_actionFront_triggered()
{
    if(m_scene->selectedItems().count() >0){
        QGraphicsItem *item = m_scene->selectedItems().at(0);
        item->setZValue(++m_frontZValue);
    }
}

void ZZNRemark::on_actionBack_triggered()
{
    if(m_scene->selectedItems().count() >0){
        QGraphicsItem *item = m_scene->selectedItems().at(0);
        item->setZValue(--m_backZValue);
    }
}

void ZZNRemark::on_actionDelete_triggered()
{
    if(m_scene->selectedItems().count() >0){
        foreach (QGraphicsItem *item, m_scene->selectedItems()) {
            m_scene->removeItem(item);
        }
        selectRectPosToZero();
    }else{
        if(m_imageFileList.count() >0){
            QString path = m_imageFileList.at(m_selectIndex);
            QFile::remove(path);
            QFile aFile(path.left(path.length()-3)+"txt");
            if(aFile.exists()){
                aFile.remove();
            }
            m_imageFileList.removeAt(m_selectIndex);
            if(m_selectIndex == m_imageFileList.count()){
                m_selectIndex = 1;
            }
            ui->listWidget->clear();
            for(int i=0; i< m_imageFileList.count(); ++i){
                QFileInfo fileInfo(m_imageFileList[i]);
                ui->listWidget->addItem(QString::number(i+1)+"."+fileInfo.fileName());
            }
            if(m_imageFileList.count() >0){
                graphicsItemsReduce();
            }
        }
    }
}

void ZZNRemark::deleteSeleteItem()
{
    if(m_scene->selectedItems().count() >0){
        foreach (QGraphicsItem *item, m_scene->selectedItems()) {
            m_scene->removeItem(item);
        }
        selectRectPosToZero();
    }
}

void ZZNRemark::graphicsItemsReduce()
{
    float widthRate = m_selectImage.width()*1.0/GraphicsViewWidth;
    float heightRate = m_selectImage.height()*1.0/GraphicsViewHeight;
    float maxRate = widthRate > heightRate ? widthRate : heightRate;
    if(maxRate > 1){
        QPixmap fitPixmap;
        if(widthRate > heightRate){
            fitPixmap = m_selectImage.scaled(GraphicsViewWidth,
                                             m_selectImage.height()/maxRate,
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
        }else{
            fitPixmap = m_selectImage.scaled(m_selectImage.width()/maxRate,
                                             GraphicsViewHeight,
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation);
        }
        ui->graphicsView->resize(fitPixmap.width(),fitPixmap.height());
        m_scene->setSceneRect(10,10,fitPixmap.width(), fitPixmap.height());
        m_scene->setBackgroundBrush(fitPixmap);
        ui->actionOriginalSize->setEnabled(true);
        ui->actionReduce->setEnabled(false);
    }else{
        ui->graphicsView->resize(m_selectImage.width(),m_selectImage.height());
        m_scene->setSceneRect(0,0,m_selectImage.width(),m_selectImage.height());
        m_scene->setBackgroundBrush(m_selectImage);
        ui->actionOriginalSize->setEnabled(false);
        ui->actionReduce->setEnabled(false);
    }
    loadNewRemarkInfo(m_imageFileList.at(m_selectIndex));
}

void ZZNRemark::on_actionReduce_triggered()
{
    saveOldRemarkInfo(m_imageFileList.at(m_selectIndex));
    graphicsItemsReduce();
}

void ZZNRemark::on_actionOriginalSize_triggered()
{
    saveOldRemarkInfo(m_imageFileList.at(m_selectIndex));
    float widthRate = m_selectImage.width()*1.0/GraphicsViewWidth;
    float heightRate = m_selectImage.height()*1.0/GraphicsViewHeight;
    float maxRate = widthRate > heightRate ? widthRate : heightRate;
    if(maxRate > 1){
        if(widthRate > heightRate){
            ui->graphicsView->resize(GraphicsViewWidth,m_selectImage.height()*1.0/widthRate);
        }else{
            ui->graphicsView->resize(m_selectImage.width()*1.0/heightRate,GraphicsViewHeight);
        }
        m_scene->setSceneRect(0,0,m_selectImage.width(), m_selectImage.height());
        m_scene->setBackgroundBrush(m_selectImage);
        loadNewRemarkInfo(m_imageFileList.at(m_selectIndex));
        ui->actionOriginalSize->setEnabled(false);
        ui->actionReduce->setEnabled(true);
    }
}


void ZZNRemark::on_actionAbout_triggered()
{
    QMessageBox::about(this,"版本信息","Version:1.4.4\nBuild:"+ZZNCommon::getBuildDate());
}

