MPACK_RELEASE="0.8.2"
MPACK_GIT="https://github.com/ludocode/mpack"
MPACK_ARCHIVE="mpack-amalgamation-${MPACK_RELEASE}.tar.gz"

cd ./external
wget ${MPACK_GIT}/releases/download/v${MPACK_RELEASE}/${MPACK_ARCHIVE}
tar xf ${MPACK_ARCHIVE}
rm ${MPACK_ARCHIVE}

