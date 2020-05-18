#include "pbr/data/dataset_wnp.h"

#include <mat.h>

#include <iostream>
#include <filesystem>
#include <future>
#include <thread>

namespace pbr
{
namespace fs = std::filesystem;

DatasetWnp::DatasetWnp(const std::string& directory)
  : Dataset(), directory_(directory)
{
  for (auto base : { "office", "kitchen1", "kitchen2" })
  {
    for (auto& it : fs::directory_iterator(directory_ + "\\" + base))
    {
      const auto& path = it.path();
      const auto& subdir = path.filename().string();
      auto sequence = std::string(base) + "\\" + subdir;

      sequences_.emplace_back(sequence);
      frames_.emplace_back();

      for (auto& it : fs::directory_iterator(directory_ + "\\" + sequence + "\\depth"))
      {
        const auto& path = it.path();
        const auto& filename = path.filename().string();

        frames_.back().emplace_back(filename.substr(0, 4));
      }
    }
  }

  /*
  sequences_.emplace_back("office\\data_03-18-03");
  frames_.emplace_back();
  */

  SequenceChanged();
}

void DatasetWnp::SequenceChanged()
{
  LoadCurrentSequenceImages();
}

void DatasetWnp::LoadCurrentSequenceImages()
{
  std::vector<std::future<Image>> color_futures_(frames_[Sequence()].size());
  std::vector<std::future<Image>> depth_futures_(frames_[Sequence()].size());

  color_images_.clear();
  depth_images_.clear();

  for (int i = 0; i < frames_[Sequence()].size(); i++)
  {
    color_futures_[i] = std::async(std::launch::async, [this, i] { return LoadColorImage(i); });
    depth_futures_[i] = std::async(std::launch::async, [this, i] { return LoadDepthImage(i); });
  }
  for (int i = 0; i < frames_[Sequence()].size(); i++)
  {
    color_futures_[i].wait();
    color_images_.emplace_back(color_futures_[i].get());
    depth_futures_[i].wait();
    depth_images_.emplace_back(depth_futures_[i].get());
  }
}

DatasetWnp::~DatasetWnp() = default;

int DatasetWnp::NumSequences()
{
  return sequences_.size();
}

int DatasetWnp::NumFrames()
{
  return frames_[Sequence()].size();
}

Image DatasetWnp::LoadColorImage(int frame)
{
  auto filename = directory_ + "\\" + sequences_[Sequence()] + "\\rgbjpg\\" + frames_[Sequence()][frame] + ".jpg";
  Image image;
  image.Load(filename);
  return image;
}

Image DatasetWnp::GetColorImage()
{
  return color_images_[Frame()];
}

Image DatasetWnp::LoadDepthImage(int frame)
{
  auto filename = directory_ + "\\" + sequences_[Sequence()] + "\\depth\\" + frames_[Sequence()][frame] + ".mat";

  Image result(8, 8, 1, (unsigned short)2000);

  auto pmat = matOpen(filename.c_str(), "r");
  if (pmat == NULL)
  {
    std::cout << "Error opening .mat file: " << filename << std::endl;
    return result;
  }

  auto depth = matGetVariable(pmat, "depth");
  if (depth == NULL)
  {
    std::cout << "Error reading existing matrix \"depth\"" << filename << std::endl;
    matClose(pmat);
    return result;
  }

  if (mxGetNumberOfDimensions(depth) != 2)
  {
    std::cout << "Error reading matrix: not have two dimensions" << std::endl;
    matClose(pmat);
    return result;
  }

  // Column-major mat array
  double* ptr = mxGetPr(depth);

  // Row-major gl texture format
  result = Image(DepthWidth(), DepthHeight(), 1, (unsigned short)0);
  auto buffer = result.Data<unsigned short>();
  for (int i = 0; i < DepthWidth(); i++)
  {
    for (int j = 0; j < DepthHeight(); j++)
    {
      const int mat_idx = i * DepthHeight() + j;
      const int buffer_idx = i + j * DepthWidth();

      buffer[buffer_idx] = ptr[mat_idx];
    }
  }

  matClose(pmat);

  return result;
}

Image DatasetWnp::GetDepthImage()
{
  return depth_images_[Frame()];
}

Pointcloud DatasetWnp::GetPointcloud()
{
  return Pointcloud();
}
}
