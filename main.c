#include <stdio.h>
#include <string.h>
#include "qrgen.h"

static int ends_with(const char *str, const char *suffix)
{
    size_t str_len = strlen(str);
    size_t suf_len = strlen(suffix);
    if (suf_len > str_len) {
        return 0;
    }
    return strcasecmp(str + (str_len - suf_len), suffix) == 0;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Kullanim: %s <link/metin> <cikti_dosyasi.png|.jpg>\n", argv[0]);
        fprintf(stderr, "Ornek   : %s \"https://example.com\" qr.png\n", argv[0]);
        return 1;
    }

    const char *text = argv[1];
    const char *out_path = argv[2];
    QRcode *qr = qrgen_encode(text, QR_ECLEVEL_M);
    if (qr == NULL) {
        fprintf(stderr, "Hata: QR kod uretilemedi.\n");
        return 1;
    }
    int status;
    int scale = 8;   
    int margin = 4;  

    if (ends_with(out_path, ".png")) {
        status = qrgen_save_png(qr, out_path, scale, margin);
    } else if (ends_with(out_path, ".jpg") || ends_with(out_path, ".jpeg")) {
        status = qrgen_save_jpg(qr, out_path, scale, margin, 90);
    } else {
        fprintf(stderr, "Hata: cikti dosyasi .png, .jpg veya .jpeg ile bitmeli.\n");
        qrgen_free(qr);
        return 1;
    }

    qrgen_free(qr);

    if (status != QRGEN_OK) {
        fprintf(stderr, "Hata: goruntu dosyasi yazilamadi (kod: %d).\n", status);
        return 1;
    }

    printf("QR kod basariyla olusturuldu: %s\n", out_path);
    return 0;
}
