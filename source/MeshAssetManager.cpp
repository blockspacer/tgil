#include "IOUtils.h"
#include "FancyAssert.h"
#include "LogHelper.h"
#include "Mesh.h"
#include "MeshAssetManager.h"

#include <tinyobj/tiny_obj_loader.h>

#include <cstring>

namespace tinyobj {

class NullMaterialReader : public MaterialReader {
public:
   NullMaterialReader() {}
   virtual ~NullMaterialReader() {}
   virtual std::string operator()(const std::string &matId,
                                  std::vector<material_t> &materials,
                                  std::map<std::string, int> &matMap) {
      return std::string("");
   }
};

} // namespace tinyobj

namespace {

const char* CUBE_MESH_SOURCE = "v -0.500000 -0.500000 0.500000\nv 0.500000 -0.500000 0.500000\nv -0.500000 0.500000 0.500000\nv 0.500000 0.500000 0.500000\nv -0.500000 0.500000 -0.500000\nv 0.500000 0.500000 -0.500000\nv -0.500000 -0.500000 -0.500000\nv 0.500000 -0.500000 -0.500000\n\nvt 0.000000 0.000000\nvt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 1.000000 1.000000\n\nvn 0.000000 0.000000 1.000000\nvn 0.000000 1.000000 0.000000\nvn 0.000000 0.000000 -1.000000\nvn 0.000000 -1.000000 0.000000\nvn 1.000000 0.000000 0.000000\nvn -1.000000 0.000000 0.000000\n\ns 1\nf 1/1/1 2/2/1 3/3/1\nf 3/3/1 2/2/1 4/4/1\ns 2\nf 3/1/2 4/2/2 5/3/2\nf 5/3/2 4/2/2 6/4/2\ns 3\nf 5/4/3 6/3/3 7/2/3\nf 7/2/3 6/3/3 8/1/3\ns 4\nf 7/1/4 8/2/4 1/3/4\nf 1/3/4 8/2/4 2/4/4\ns 5\nf 2/1/5 8/2/5 4/3/5\nf 4/3/5 8/2/5 6/4/5\ns 6\nf 7/1/6 1/2/6 5/3/6\nf 5/3/6 1/2/6 3/4/6\n";

const char* XY_PLANE_MESH_SOURCE = "v -1.000000 -1.000000 -0.000000\nv 1.000000 -1.000000 -0.000000\nv -1.000000 1.000000 0.000000\nv 1.000000 1.000000 0.000000\nvt 1.000000 0.000000\nvt 1.000000 1.000000\nvt 0.000000 1.000000\nvt 0.000000 0.000000\nvn 0.000000 -0.000000 1.000000\ns off\nf 2/1/1 4/2/1 3/3/1\nf 1/4/1 2/1/1 3/3/1\n";

const char* OPEN_TOP_CUBE_MESH_SOURCE = "v -0.500000 -0.500000 0.500000\nv 0.500000 -0.500000 0.500000\nv -0.500000 0.500000 0.500000\nv 0.500000 0.500000 0.500000\nv -0.500000 0.500000 -0.500000\nv 0.500000 0.500000 -0.500000\nv -0.500000 -0.500000 -0.500000\nv 0.500000 -0.500000 -0.500000\nvt 0.000000 0.000000\nvt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 1.000000 1.000000\nvn 0.000000 -0.000000 1.000000\nvn 0.000000 0.000000 -1.000000\nvn 0.000000 -1.000000 -0.000000\nvn 1.000000 0.000000 0.000000\nvn -1.000000 0.000000 0.000000\ns 1\nf 1/1/1 2/2/1 3/3/1\nf 3/3/1 2/2/1 4/4/1\nf 5/4/2 6/3/2 7/2/2\nf 7/2/2 6/3/2 8/1/2\nf 8/2/3 1/3/3 7/1/3\nf 1/3/3 8/2/3 2/4/3\nf 2/1/4 8/2/4 4/3/4\nf 4/3/4 8/2/4 6/4/4\nf 7/1/5 1/2/5 5/3/5\nf 5/3/5 1/2/5 3/4/5\n";

SPtr<Mesh> meshFromStream(std::istream &in) {
   std::vector<tinyobj::shape_t> shapes;
   std::vector<tinyobj::material_t> materials;
   tinyobj::NullMaterialReader reader;

   std::string error = tinyobj::LoadObj(shapes, materials, in, reader);

   if (!error.empty()) {
      LOG_WARNING(error);
      return nullptr;
   }

   if (shapes.empty()) {
      LOG_WARNING("No shapes");
      return nullptr;
   }

   tinyobj::shape_t &shape = shapes.at(0);

   // Vertices
   unsigned int numVertices = shape.mesh.positions.size() / 3;
   UPtr<float[]> vertices(new float[3 * numVertices]);
   memcpy(vertices.get(), shape.mesh.positions.data(), sizeof(float) * 3 * numVertices);

   // Normals
   unsigned int numNormals = shape.mesh.normals.size() / 3;
   UPtr<float[]> normals(new float[3 * numNormals]);
   memcpy(normals.get(), shape.mesh.normals.data(), sizeof(float) * 3 * numNormals);

   // Indices
   unsigned int numIndices = shape.mesh.indices.size();
   UPtr<unsigned int[]> indices(new unsigned int[numIndices]);
   memcpy(indices.get(), shape.mesh.indices.data(), sizeof(unsigned int) * numIndices);

   // Texture coordinates
   unsigned int numTexCoords = shape.mesh.texcoords.size() / 2;
   UPtr<float[]> texCoords(new float[numTexCoords * 2]);
   memcpy(texCoords.get(), shape.mesh.texcoords.data(), sizeof(float) * 2 * numTexCoords);

   return std::make_shared<Mesh>(std::move(vertices), numVertices, std::move(normals), numNormals, std::move(indices), numIndices, std::move(texCoords), numTexCoords);
}

SPtr<Mesh> getMeshFromMemory(const char *data) {
   std::stringstream ss(data);
   return meshFromStream(ss);
}

} // namespace

MeshAssetManager::MeshAssetManager() {
}

MeshAssetManager::~MeshAssetManager() {
}

SPtr<Mesh> MeshAssetManager::loadMesh(const std::string &fileName) {
   if (meshMap.count(fileName) > 0) {
      return meshMap[fileName];
   }

   if (!IOUtils::canReadData(fileName)) {
      LOG_WARNING("Unable to load mesh from file \"" << fileName << "\", reverting to default mesh");
      return getMeshForShape(MeshShape::Cube);
   }

   std::ifstream in(IOUtils::dataPath(fileName));
   SPtr<Mesh> mesh(meshFromStream(in));

   if (!mesh) {
      LOG_WARNING("Unable to import mesh \"" << fileName << "\", reverting to default mesh");
      return getMeshForShape(MeshShape::Cube);
   }

   meshMap[fileName] = mesh;
   return mesh;
}

SPtr<Mesh> MeshAssetManager::getMeshForShape(MeshShape shape) {
   static SPtr<Mesh> cubeMesh = nullptr;
   static SPtr<Mesh> xyPlaneMesh = nullptr;
   static SPtr<Mesh> openTopCubeMesh = nullptr;

   switch (shape) {
      case MeshShape::Cube:
         if (!cubeMesh) {
            cubeMesh = getMeshFromMemory(CUBE_MESH_SOURCE);
         }
         return cubeMesh;
      case MeshShape::XYPlane:
         if (!xyPlaneMesh) {
            xyPlaneMesh = getMeshFromMemory(XY_PLANE_MESH_SOURCE);
         }
         return xyPlaneMesh;
      case MeshShape::OpenTopCube:
         if (!openTopCubeMesh) {
            openTopCubeMesh = getMeshFromMemory(OPEN_TOP_CUBE_MESH_SOURCE);
         }
         return openTopCubeMesh;
      default:
         ASSERT(false, "Invalid mesh shape");
         return nullptr;
   }
}
