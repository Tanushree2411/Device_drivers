DESCRIPTION = "Simple test application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://test.c"

S = "${WORKDIR}"

do_compile() {
        ${CC} test.c ${LDFLAGS} -o test1
}

do_install() {
        install -d ${D}${bindir}
        install -m 0755 test1 ${D}${bindir}
}                                
