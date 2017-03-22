#include <iostream>
#include <fstream>
#include "Mesh.pb.h"
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <random>

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

  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

  std::uniform_int_distribution<int> uniform_dist(1, 2000);
  std::uniform_real_distribution<float> float_dist(-20.0f, 20.0f);

  std::vector<float> data(uniform_dist(gen));
  float n={0};
  std::generate(std::begin(data),std::end(data),
                [&float_dist,&gen, &n]()
                {
                  return float_dist(gen);
                });
  auto writer=meshes.add_mesh();
  char meshName[]="Mesh_XXXXXX";
  mkstemp(meshName);
  writer->set_name(meshName);
  writer->set_startoffset(0);
  writer->set_endoffset(data.size()*sizeof(float));

  google::protobuf::RepeatedField<float> d(std::begin(data), std::end(data));
  writer->mutable_data()->Swap(&d);

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
