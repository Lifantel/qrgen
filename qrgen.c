#include "qrgen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
static unsigned char *qrgen_build_rgb_buffer(const QRcode *qr, int scale, int margin,
                                              int *out_width, int *out_height)
{
    if (qr == NULL || scale <= 0 || margin < 0) {
        return NULL;
    }

    int qr_size = qr->width;                         
    int img_size = (qr_size + margin * 2) * scale;    

    unsigned char *buffer = (unsigned char *)malloc((size_t)img_size * (size_t)img_size * 3);
    if (buffer == NULL) {
        return NULL;
    }
    memset(buffer, 0xFF, (size_t)img_size * (size_t)img_size * 3);
    for (int y = 0; y < qr_size; y++) {
        for (int x = 0; x < qr_size; x++) {
            unsigned char module = qr->data[y * qr_size + x];
            int is_black = module & 0x01;

            if (!is_black) {
                continue; 
            }

            int px_start_x = (x + margin) * scale;
            int px_start_y = (y + margin) * scale;

            for (int dy = 0; dy < scale; dy++) {
                for (int dx = 0; dx < scale; dx++) {
                    int px = px_start_x + dx;
                    int py = px_start_y + dy;
                    size_t idx = ((size_t)py * img_size + px) * 3;
                    buffer[idx + 0] = 0x00; /* R */
                    buffer[idx + 1] = 0x00; /* G */
                    buffer[idx + 2] = 0x00; /* B */
                }
            }
        }
    }

    *out_width = img_size;
    *out_height = img_size;
    return buffer;
}
QRcode *qrgen_encode(const char *text, QRecLevel level)
{
    if (text == NULL || text[0] == '\0') {
        return NULL;
    }
    QRcode *qr = QRcode_encodeString(text, 0, level, QR_MODE_8, 1);

    return qr; 
}

void qrgen_free(QRcode *qr)
{
    if (qr != NULL) {
        QRcode_free(qr);
    }
}
int qrgen_save_png(const QRcode *qr, const char *path, int scale, int margin)
{
    if (qr == NULL) {
        return QRGEN_ERR_NULL_QR;
    }
    if (path == NULL || scale <= 0 || margin < 0) {
        return QRGEN_ERR_PARAM;
    }

    int width = 0, height = 0;
    unsigned char *buffer = qrgen_build_rgb_buffer(qr, scale, margin, &width, &height);
    if (buffer == NULL) {
        return QRGEN_ERR_ENCODE;
    }
    int result = stbi_write_png(path, width, height, 3, buffer, width * 3);

    free(buffer);

    return (result != 0) ? QRGEN_OK : QRGEN_ERR_IMAGE_WRITE;
}
int qrgen_save_jpg(const QRcode *qr, const char *path, int scale, int margin, int quality)
{
    if (qr == NULL) {
        return QRGEN_ERR_NULL_QR;
    }
    if (path == NULL || scale <= 0 || margin < 0 || quality < 1 || quality > 100) {
        return QRGEN_ERR_PARAM;
    }

    int width = 0, height = 0;
    unsigned char *buffer = qrgen_build_rgb_buffer(qr, scale, margin, &width, &height);
    if (buffer == NULL) {
        return QRGEN_ERR_ENCODE;
    }

    int result = stbi_write_jpg(path, width, height, 3, buffer, quality);

    free(buffer);

    return (result != 0) ? QRGEN_OK : QRGEN_ERR_IMAGE_WRITE;
}
