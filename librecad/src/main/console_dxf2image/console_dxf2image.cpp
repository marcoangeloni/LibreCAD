#include <QtCore>
#include <QCoreApplication>
#include <QApplication>

#include "rs_debug.h"
#include "rs_fontlist.h"
#include "rs_patternlist.h"
#include "rs_settings.h"
#include "rs_system.h"

#include "main.h"

#include "console_dxf2image.h"
#include "image_print.h"


int console_dxf2image(int argc, char* argv[]){

    QApplication app(argc, argv);
    DxfImagePrinter *imagePrinter = new DxfImagePrinter(&app);
    if(argc > 2){
        QString actOpt = QString::fromStdString(argv[2]);
        QString sourcePath = QString::fromStdString(argv[3]);
        QString destPath = QString::fromStdString(argv[4]);

        if(actOpt == "-single"){
            imagePrinter->exportDxfAsImage(sourcePath,destPath,"jpg",false,true);
        }
        if(actOpt == "-all"){
            QDir directory(sourcePath);
            QStringList dxfs = directory.entryList(QStringList() << "*.dxf" << "*.DXF",QDir::Files);
            foreach(QString filename, dxfs) {
                //imagePrinter->exportDxfAsImage(filename,,"jpg",false,true);
            }
        }
    }

    delete imagePrinter;

}
