#ifndef DXFPRINTER_H
#define DXFPRINTER_H

#include <QtCore>
#include <QtSvg>

#include "rs.h"
#include "rs_graphic.h"
#include "rs_painterqt.h"
#include "lc_printing.h"
#include "rs_staticgraphicview.h"

enum ERROR_TYPE{
    OK = 0,
    UNDEFINED,
    DOCOPENANDGRAPHICSET,
    INCOMPATIBLE_FORMAT
};
static ERROR_TYPE err_type;


class DxfImagePrinter: public QObject {

    Q_OBJECT


public:
    DxfImagePrinter(QObject* parent=0) : QObject(parent){}

    ERROR_TYPE exportDxfAsImage(const QString & nameFileInputDxf,
                                const QString& nameFileOutput,
                                 const QString& formatFileOutput,
                                 bool black,
                                 bool bw);

    /**
     * @brief FIXME:Funzione duplicata! E' copiata da pdf_print_loop.cpp
     * @param doc
     * @param graphic
     * @param dxfFile
     * @return
     */
    bool openDocAndSetGraphic2(RS_Document** doc, RS_Graphic** graphic,
        const QString& dxfFile);
    void touchGraphic(RS_Graphic* graphic);
public slots:
signals:

};

#endif // DXFPRINTER_H
