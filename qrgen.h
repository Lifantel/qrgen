
#ifndef QRGEN_H
#define QRGEN_H

#include <qrencode.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    QRGEN_OK = 0,
    QRGEN_ERR_ENCODE = -1,      
    QRGEN_ERR_NULL_QR = -2,     
    QRGEN_ERR_IMAGE_WRITE = -3, 
    QRGEN_ERR_PARAM = -4        
} qrgen_status_t;
QRcode *qrgen_encode(const char *text, QRecLevel level);
void qrgen_free(QRcode *qr);
int qrgen_save_png(const QRcode *qr, const char *path, int scale, int margin);


int qrgen_save_jpg(const QRcode *qr, const char *path, int scale, int margin, int quality);

#ifdef __cplusplus
}
#endif
#endif 
