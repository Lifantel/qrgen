#!/usr/bin/env bash
set -euo pipefail

info()  { printf '\033[1;34m[BILGI]\033[0m %s\n' "$1"; }
ok()    { printf '\033[1;32m[TAMAM]\033[0m %s\n' "$1"; }
warn()  { printf '\033[1;33m[UYARI]\033[0m %s\n' "$1"; }
error() { printf '\033[1;31m[HATA]\033[0m %s\n' "$1" >&2; }

DISTRO_ID=""
DISTRO_LIKE=""

if [ -f /etc/os-release ]; then
    . /etc/os-release
    DISTRO_ID="${ID:-}"
    DISTRO_LIKE="${ID_LIKE:-}"
else
    error "Dagitim tespit edilemedi."
    exit 1
fi

info "Dagitim: ${PRETTY_NAME:-bilinmiyor}"

install_pkg() {
    case "$DISTRO_ID" in
        fedora|rhel|centos|rocky|almalinux)
            sudo dnf install -y qrencode-devel
            ;;
        ubuntu|debian|linuxmint|pop)
            sudo apt update
            sudo apt install -y libqrencode-dev build-essential
            ;;
        arch|manjaro|endeavouros)
            sudo pacman -Sy --needed --noconfirm qrencode base-devel
            ;;
        opensuse*|sles)
            sudo zypper install -y libqrencode-devel gcc make
            ;;
        alpine)
            sudo apk add libqrencode-dev build-base
            ;;
        *)
            case "$DISTRO_LIKE" in
                *fedora*|*rhel*)
                    sudo dnf install -y qrencode-devel
                    ;;
                *debian*)
                    sudo apt update
                    sudo apt install -y libqrencode-dev build-essential
                    ;;
                *arch*)
                    sudo pacman -Sy --needed --noconfirm qrencode base-devel
                    ;;
                *suse*)
                    sudo zypper install -y libqrencode-devel gcc make
                    ;;
                *)
                    error "Desteklenmeyen dagitim: $DISTRO_ID"
                    exit 1
                    ;;
            esac
            ;;
    esac
}

if [ -f /usr/include/qrencode.h ] || [ -f /usr/local/include/qrencode.h ]; then
    ok "Kutuphane zaten kurulu."
else
    install_pkg
    ok "Kutuphane kuruldu."
fi

REPO_URL="https://github.com/Lifantel/qrgen.git"
CLONE_DIR="$(pwd)/qrgen"

if [ -d "$CLONE_DIR/.git" ]; then
    info "Repo guncelleniyor..."
    git -C "$CLONE_DIR" pull
else
    info "Repo klonlaniyor..."
    git clone "$REPO_URL" "$CLONE_DIR"
fi

cd "$CLONE_DIR"

info "Derleniyor..."
make clean >/dev/null 2>&1 || true
make
ok "Derlendi."

if [ "$#" -lt 2 ]; then
    warn "Kullanim: ./run.sh \"https://example.com\" qr.png"
    exit 0
fi

info "Calistiriliyor..."
./qrgen "$1" "$2"
