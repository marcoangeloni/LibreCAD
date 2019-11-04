#include <QtCore>
#include <QtSvg>

#include "rs.h"
#include "rs_graphic.h"
#include "rs_painterqt.h"
#include "lc_printing.h"
#include "rs_staticgraphicview.h"
#include "rs_math.h"
#include "rs_vector.h"
#include "image_print.h"


/**
 * @brief MARCOA Adattamento per la conversione da DXF a JPG.
 * @param nameFileInputDxf
 * @param nameFileOutput
 * @param formatFileOutput
 * @param borders
 * @param black
 * @param bw
 * @return
 */

ERROR_TYPE DxfImagePrinter::exportDxfAsImage(const QString & nameFileInputDxf,
                                    const QString& nameFileOutput,
                                     const QString& formatFileOutput,
                                     bool black,
                                     bool bw){

    ERROR_TYPE err_type = ERROR_TYPE::UNDEFINED;
    RS_Document *doc;
    RS_Graphic *graphic;

    if(!openDocAndSetGraphic2(&doc,&graphic,nameFileInputDxf))
        return ERROR_TYPE::DOCOPENANDGRAPHICSET;

    if(graphic==nullptr)
        return ERROR_TYPE::UNDEFINED;

    touchGraphic(graphic); //ADATTA IL DISEGNO ALL'IMMAGINE
    RS_Vector qvector = graphic->getSize()*2.;
    double resolution = 1.0;
    int w = RS_Math::round(resolution * qvector.x);
    int h = RS_Math::round(resolution * qvector.y);

    QSize size = QSize(RS_Math::round(RS_Math::eval(QString("%1").arg(w))),
                       RS_Math::round(RS_Math::eval(QString("%1").arg(h))));

    QSize borders (5,5);
    // set vars for normal pictures and vectors (svg)
    QPixmap* picture = new QPixmap(size);

    QSvgGenerator* vector = new QSvgGenerator();

    // set buffer var
    QPaintDevice* buffer;

    if(formatFileOutput.toLower() != "svg") {
        buffer = picture;
    } else {
        vector->setSize(size);
        vector->setViewBox(QRectF(QPointF(0,0),size));
        vector->setFileName(nameFileOutput);
        buffer = vector;
    }

    // set painter with buffer
    RS_PainterQt painter(buffer);

    // black background:
    if (black) {
//RLZ        painter.setBackgroundColor(RS_Color(0,0,0));
        painter.setBackground(Qt::black);
    }
    // white background:
    else {
    //RLZ        painter.setBackgroundColor(RS_Color(255,255,255));
        painter.setBackground(Qt::white);
    }

    // black/white:
    if (bw) {
        painter.setDrawingMode(RS2::ModeBW);
    }

    painter.eraseRect(0,0, size.width(), size.height());

    RS_StaticGraphicView gv(size.width(), size.height(), &painter, &borders);
    if (black) {
        gv.setBackground(Qt::black);
    } else {
        gv.setBackground(Qt::white);
    }
    gv.setContainer(graphic);
    gv.zoomAuto(false);
    gv.drawEntity(&painter, gv.getContainer());

    // end the picture output
    if(formatFileOutput.toLower() != "svg")
    {
        // RVT_PORT QImageIO iio;
        QImageWriter iio;
        QImage img = picture->toImage();
        // RVT_PORT iio.setImage(img);
        iio.setFileName(nameFileOutput);
        iio.setFormat(formatFileOutput.toLatin1());
        // RVT_PORT if (iio.write()) {
        if (iio.write(img)) {
            err_type = ERROR_TYPE::OK;
        }
        //QString stringa = iio.errorString();
        //QString error=iio.errorString();
    }else{
        err_type = ERROR_TYPE::INCOMPATIBLE_FORMAT;
    }

    // GraphicView deletes painter
    painter.end();
    // delete vars
    delete picture;
    delete vector;
    delete doc;

    return err_type;
}

/**
 * @brief FIXME:Funzione duplicata! E' copiata da pdf_print_loop.cpp
 * @param doc
 * @param graphic
 * @param dxfFile
 * @return
 */
bool DxfImagePrinter::openDocAndSetGraphic2(RS_Document** doc, RS_Graphic** graphic,
    const QString& dxfFile)
{
    *doc = new RS_Graphic();

    if (!(*doc)->open(dxfFile, RS2::FormatUnknown)) {
        qDebug() << "ERROR: Failed to open document" << dxfFile;
        delete *doc;
        return false;
    }

    *graphic = (*doc)->getGraphic();
    if (*graphic == nullptr) {
        qDebug() << "ERROR: No graphic in" << dxfFile;
        delete *doc;
        return false;
    }

    return true;
}


void DxfImagePrinter::touchGraphic(RS_Graphic* graphic)
{
    graphic->calculateBorders();
    graphic->setMargins(0, 0,
                        0, 0);
    graphic->setPagesNum(1, 1);
    graphic->fitToPage(); // Adatta e centra

    //graphic->setPaperScale(1.0);

    //graphic->setPaperSize(graphic->getSize());


}
