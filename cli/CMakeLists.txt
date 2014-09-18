set (CLI_SOURCES
./runtime-source.cxx
./man.cxx
./runtime-header.cxx
./source.cxx
./inline.cxx
./runtime-inline.cxx
./context.cxx
./traversal/option.cxx
./traversal/unit.cxx
./traversal/class.cxx
./traversal/namespace.cxx
./traversal/elements.cxx
./cli.cxx
./semantics/option.cxx
./semantics/unit.cxx
./semantics/class.cxx
./semantics/namespace.cxx
./semantics/elements.cxx
./semantics/expression.cxx
./header.cxx
./name-processor.cxx
./generator.cxx
./html.cxx
./lexer.cxx
./parser.cxx
./options.cxx
)

add_executable(cli ${CLI_SOURCES})
include_directories(${CMAKE_SOURCE_DIR}/cli)
target_link_libraries(cli cutl)

