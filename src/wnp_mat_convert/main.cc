#include <iostream>
#include <fstream>
#include <filesystem>

#include <mat.h>

namespace fs = std::filesystem;

struct Frame
{
  std::string scene_name;
  std::string sequence_name;
  std::string frame_name;
};

const std::vector<std::string> scene_names =
{
  "office",
  "kitchen1",
  "kitchen2",
};

const std::string directory = "..\\..\\dataset\\watch-n-patch";
const int width = 512;
const int height = 424;

std::vector<Frame> frames_;


void ConvertFile(const Frame& frame)
{
  const auto& scene_name = frame.scene_name;
  const auto& sequence_name = frame.sequence_name;
  const auto& frame_name = frame.frame_name;

  auto filename = directory + "\\" + scene_name + "\\" + sequence_name + "\\depth\\" + frame_name + ".mat";

  auto pmat = matOpen(filename.c_str(), "r");
  if (pmat == NULL)
  {
    std::cout << "Error opening .mat file: " << filename << std::endl;
  }

  auto depth = matGetVariable(pmat, "depth");
  if (depth == NULL)
  {
    std::cout << "Error reading existing matrix \"depth\"" << filename << std::endl;
    matClose(pmat);
  }

  if (mxGetNumberOfDimensions(depth) != 2)
  {
    std::cout << "Error reading matrix: not have two dimensions" << std::endl;
    matClose(pmat);
  }

  // Column-major mat array
  double* ptr = mxGetPr(depth);

  // Save in row-major gl texture format
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      const int mat_idx = i * height + j;
      const int buffer_idx = i + j * width;

      //buffer[buffer_idx] = ptr[mat_idx];
    }
  }

  matClose(pmat);
}



int main()
{
  /*
  constexpr bool depth_conversion = false;
  constexpr bool body_conversion = false;
  constexpr bool overwrite = false;
  constexpr bool clean = true;

  std::string directory = "..\\..\\dataset\\watch-n-patch";
  constexpr char directory_character = '\\';

  // Get sequence names
  std::vector<std::string> sequence_names;
  for (const auto& scene_name : scene_names)
  {
    for (auto& it : fs::directory_iterator(directory + directory_character + scene_name + directory_character))
    {
      const auto& path = it.path();
      sequence_names.push_back(scene_name + directory_character + path.filename().string());
    }
  }
  std::sort(sequence_names.begin(), sequence_names.end());

  // Depth file conversion
  if (depth_conversion)
  {
    std::cout << "Converting depth files..." << std::endl;

    std::vector<unsigned short> buffer(512 * 424);
    for (const auto& sequence_name : sequence_names)
    {
      std::cout << sequence_name << ":" << std::endl;

      // Create new depth folder
      fs::create_directory(directory + directory_character + sequence_name + directory_character + "depth_raw");

      for (auto& it : fs::directory_iterator(directory + directory_character + sequence_name + directory_character + "depth"))
      {
        const auto& path = it.path();
        const auto& filename = path.string();
        const auto& short_filename = path.filename().string();

        if (short_filename.substr(short_filename.length() - 4) != ".mat")
          continue;

        // To overwrite raw file, comment these lines
        if (!overwrite && fs::exists(directory + directory_character + sequence_name + directory_character + "depth_raw\\" + short_filename.substr(0, short_filename.length() - 4) + ".raw"))
          continue;

        matlab->eval(u"load " + matlab::engine::convertUTF8StringToUTF16String(filename));

        matlab::data::TypedArray<double> depth = matlab->getVariable("depth");
        auto dim = depth.getDimensions();

        // 424x512 matrix
        // Save to raw data in row-major from top-left to bottom-right
        int buffer_idx = 0;
        for (int i = 0; i < dim[0]; i++)
          for (int j = 0; j < dim[1]; j++)
            buffer[buffer_idx++] = static_cast<unsigned short>(depth[i][j]);

        std::cout << "output file path: " << directory + directory_character + sequence_name + directory_character + "depth_raw\\" + short_filename.substr(0, short_filename.length() - 4) + ".raw" << std::endl;
        std::ofstream file(directory + directory_character + sequence_name + directory_character + "depth_raw\\" + short_filename.substr(0, short_filename.length() - 4) + ".raw", std::ios::out | std::ios::binary);
        file.write((const char*)buffer.data(), buffer.size() * sizeof(unsigned short));
        file.close();
      }
    }

    std::cout << "Finished converting depth files" << std::endl;
  }

  // Cleaning converted depth files
  if (clean)
  {
    std::cout << "Cleaning depth files..." << std::endl;

    for (const auto& sequence_name : sequence_names)
    {
      std::cout << sequence_name << ":" << std::endl;

      // Create new depth folder
      std::cout << "deleting directory: " << directory + directory_character + sequence_name + directory_character + "depth_raw" << std::endl;
      fs::remove_all(directory + directory_character + sequence_name + directory_character + "depth_raw");
    }

    std::cout << "Finished cleaning depth files" << std::endl;
  }

  // Body files
  if (body_conversion)
  {
    std::cout << "Converting body files..." << std::endl;
    for (const auto& sequence_name : sequence_names)
    {
      std::cout << sequence_name << ":" << std::endl;

      std::string mat_filename = directory + directory_character + sequence_name + directory_character + "body.mat";
      std::string struct_filename = directory + directory_character + sequence_name + directory_character + "body.struct";

      // To overwrite raw file, comment these lines
      if (!overwrite && fs::exists(struct_filename))
        continue;

      matlab->eval(u"load " + matlab::engine::convertUTF8StringToUTF16String(mat_filename));

      matlab::data::Array body = matlab->getVariable("body");
      auto dim = body.getDimensions();

      std::ofstream out(struct_filename);

      out << "frames " << dim[0] << std::endl;

      for (int i = 0; i < dim[0]; i++)
      {
        bool body_found = false;

        for (int j = 0; j < dim[1]; j++)
        {
          matlab::data::StructArray body_ij = body[i][j];
          matlab::data::TypedArray<double> is_body_tracked = body_ij[0]["isBodyTracked"];

          if (is_body_tracked[0] == 1)
          {
            if (body_found)
            {
              std::cout << "Multiple bodies at frame " << i << std::endl;
              continue;
            }

            matlab::data::Array joints = body_ij[0]["joints"];

            if (joints.getDimensions()[1] != 25)
            {
              std::cout << "Number of joints at frame " << i << " is not 25 (is "
                << joints.getDimensions()[1] << ")" << std::endl;
              continue;
            }

            // Body found
            out << "frame " << i << " isBodyTracked: " << 1 << std::endl;

            for (int k = 0; k < joints.getDimensions()[1]; k++)
            {
              matlab::data::StructArray joint = joints[0][k];

              matlab::data::TypedArray<double> tracking_state = joint[0]["trackingState"];
              matlab::data::TypedArray<double> camera = joint[0]["camera"];
              matlab::data::TypedArray<double> color = joint[0]["color"];
              matlab::data::TypedArray<double> depth = joint[0]["depth"];
              matlab::data::TypedArray<double> rotation = joint[0]["rotation"];
              matlab::data::TypedArray<double> pcloud = joint[0]["pcloud"];

              out << "joint " << k << std::endl;
              out << " trackingState: " << tracking_state[0] << std::endl
                << " camera: " << camera[0] << ' ' << camera[1] << ' ' << camera[2] << std::endl
                << " color: " << color[0] << ' ' << color[1] << std::endl
                << " depth: " << depth[0] << ' ' << depth[1] << std::endl
                << " rotation: " << rotation[0] << ' ' << rotation[1] << ' ' << rotation[2] << ' ' << rotation[3] << std::endl
                << " pcloud: " << pcloud[0] << ' ' << pcloud[1] << ' ' << pcloud[2] << std::endl;
            }

            body_found = true;
          }
        }

        if (!body_found)
        {
          // Body not found
          out << "frame " << i << " isBodyTracked: " << 0 << std::endl;
        }
      }

      out.close();

      matlab->eval(u"clear body");
    }
  }
  */

  return 0;
}
