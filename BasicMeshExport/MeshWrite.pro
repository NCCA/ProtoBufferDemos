TARGET=MeshWrite
SOURCES+=MeshWrite.cpp
HEADERS+=Mesh.pb.h
CONFIG+=c++11
CONFIG-=app_bundle
LIBS+=$$system("pkg-config protobuf --libs")
QMAKE_CXXFLAGS+=$$system("pkg-config protobuf --cflags")


# this is the source fed to the proto compiler
PROTOSOURCES+=$$PWD/Mesh.proto
# we then add this to the other files for editing
OTHER_FILES+=$$PROTOSOURCES

###########################################################################################
# now we are going to create a Qt compiler ojbect for the EXTRA_COMPILERS FLAG
# this basically has a number of attributes which we need to set as outlined below
###########################################################################################
# this flag list the sources we wish to input to this custom compiler, in this case listed above
# in the PROTOSOURCES variable
proto.input = PROTOSOURCES
# now we need to say what files to output, for flex we want to create the .c
proto.output = ${QMAKE_FILE_BASE}.pb.cc
# this is the actual compile command we wish to call in this case it wil be
# flex -o (output).c input.l
proto.commands =protoc -I=. --cpp_out=.   ${QMAKE_FILE_IN}
# once this compiler has been run we need to add the output files to the linker stage in this case
# as we are generating .c files we just add it to the SOURCES variable and it will be compiled
proto.variable_out = SOURCES
# this flag ensures that the extra compiler is run before the main c one (as we need to have the .c files
# generated by flex before we use the gcc compiler)
proto.CONFIG += target_predeps
# this tells the make clean command what files to remove
proto.clean =  ${QMAKE_FILE_IN_BASE}.c
# this is just the name of the extra compiler used in the make file (make flex for example)
proto.name = proto
# finally add this to the qmake project so it will have the makefile generated
QMAKE_EXTRA_COMPILERS += proto