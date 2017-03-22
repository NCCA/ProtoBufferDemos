#include <iostream>
#include <fstream>
#include "Mesh.pb.h"
#include <algorithm>
#include <numeric>


int main(int argc, char **argv)
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

    {
      // Read the existing Meshes book.
      std::fstream input(argv[1], std::ios::in | std::ios::binary);
      if (!meshes.ParseFromIstream(&input))
      {
        std::cerr << "Failed to parse Meshes\n";
      return EXIT_FAILURE;
      }
    }
  std::cout<<"Numebr of meshes "<<meshes.mesh_size()<<'\n';
  for (int i = 0; i < meshes.mesh_size(); i++)
  {
    const meshExport::meshData& meshData = meshes.mesh(i);
    std::cout<<meshData.name()<<'\n';
    std::cout<<meshData.startoffset()<<'\n';
    std::cout<<meshData.endoffset()<<'\n';
    std::cout<<"Data size "<<meshData.data_size()<<"\n";
    std::vector<float> data(meshData.data_size());

    std::copy(meshData.data().begin(), meshData.data().end(), data.begin());
    for(auto p : data)
    {
      std::cout<<p<<"\n";
    }

  }
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

}
