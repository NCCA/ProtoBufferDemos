#include <iostream>
#include <fstream>
#include "Mesh.pb.h"
#include <algorithm>
#include <numeric>

int main(int argc, char* argv[])
{
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2)
  {
    std::cerr << "Usage:  " << argv[0] << " MeshFile\n";
    return EXIT_FAILURE;
  }

  meshExport::meshes meshes;

  // Read the existing address book.
  std::fstream input(argv[1], std::ios::in | std::ios::binary);
  if (!input)
  {
    std::cout << argv[1] << ": File not found.  Creating a new file.\n";
  }
  else if (!meshes.ParseFromIstream(&input))
  {
    std::cerr << "Failed to parse meshes\n";
  return EXIT_FAILURE;
  }
  std::vector<float> data(200);
  std::iota(std::begin(data),std::end(data),1);
  meshes.add_mesh()->set_name("Mesh1");
  meshes.add_mesh()->set_startoffset(0);
  meshes.add_mesh()->set_endoffset(data.size()*sizeof(float));

  google::protobuf::RepeatedField<float> d(std::begin(data), std::end(data));
  meshes.add_mesh()->mutable_data()->Swap(&d);

  {
  // Write the new address book back to disk.
  std::fstream output(argv[1], std::ios::out | std::ios::trunc | std::ios::binary);
  if (!meshes.SerializeToOstream(&output))
  {
    std::cerr << "Failed to write Meshes\n" ;
    return EXIT_FAILURE;
  }
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return EXIT_SUCCESS;
}
