file(GLOB SOURCES *.c)
idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS include
    REQUIRES driver

    # EMBED_TXTFILES html/code.js html/index.html html/style.css
)
