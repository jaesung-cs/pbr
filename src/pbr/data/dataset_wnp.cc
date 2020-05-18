#include "pbr/data/dataset_wnp.h"

#include <mat.h>

#include <iostream>
#include <filesystem>
#include <future>
#include <thread>
#include <fstream>

namespace pbr
{
namespace fs = std::filesystem;

DatasetWnp::DatasetWnp(const std::string& directory)
  : Dataset(), directory_(directory)
{
  for (auto base : { "kitchen1", "kitchen2", "office" })
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

  SequenceChanged();
}

void DatasetWnp::SequenceChanged()
{
  LoadCurrentSequenceImages();
}

void DatasetWnp::LoadCurrentSequenceImages()
{
  std::vector<std::future<std::shared_ptr<Image>>> color_futures_(frames_[Sequence()].size());
  std::vector<std::future<std::shared_ptr<Image>>> depth_futures_(frames_[Sequence()].size());

  color_images_.clear();
  depth_images_.clear();

  for (int i = 0; i < frames_[Sequence()].size(); i++)
  {
    color_futures_[i] = std::async(std::launch::async, [this, i] { return LoadColorImage(i); });
    depth_futures_[i] = std::async(std::launch::async, [this, i] { return LoadDepthRawImage(i); });
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

std::shared_ptr<Image> DatasetWnp::LoadColorImage(int frame)
{
  auto filename = directory_ + "\\" + sequences_[Sequence()] + "\\rgbjpg\\" + frames_[Sequence()][frame] + ".jpg";
  auto image = std::make_shared<Image>();
  image->Load(filename);
  return image;
}

std::shared_ptr<Image> DatasetWnp::GetColorImage()
{
  return color_images_[Frame()];
}

std::shared_ptr<Image> DatasetWnp::LoadDepthRawImage(int frame)
{
  auto filename = directory_ + "\\" + sequences_[Sequence()] + "\\depth_raw\\" + frames_[Sequence()][frame] + ".raw";
  auto image = std::make_shared<Image>(DepthWidth(), DepthHeight(), 1, (unsigned short)0);

  std::ifstream in(filename, std::ios::binary);
  in.read(image->Data<char>(), sizeof(unsigned short) * DepthWidth() * DepthHeight());
  in.close();

  return image;
}

std::shared_ptr<Image> DatasetWnp::LoadDepthImage(int frame)
{
  auto filename = directory_ + "\\" + sequences_[Sequence()] + "\\depth\\" + frames_[Sequence()][frame] + ".mat";

  auto pmat = matOpen(filename.c_str(), "r");
  if (pmat == NULL)
  {
    std::cout << "Error opening .mat file: " << filename << std::endl;
    return nullptr;
  }

  auto depth = matGetVariable(pmat, "depth");
  if (depth == NULL)
  {
    std::cout << "Error reading existing matrix \"depth\"" << filename << std::endl;
    matClose(pmat);
    return nullptr;
  }

  if (mxGetNumberOfDimensions(depth) != 2)
  {
    std::cout << "Error reading matrix: not have two dimensions" << std::endl;
    matClose(pmat);
    return nullptr;
  }

  // Column-major mat array
  double* ptr = mxGetPr(depth);

  // Row-major gl texture format
  auto result = std::make_shared<Image>(DepthWidth(), DepthHeight(), 1, (unsigned short)0);
  auto buffer = result->Data<unsigned short>();
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

std::shared_ptr<Image> DatasetWnp::GetDepthImage()
{
  return depth_images_[Frame()];
}

Pointcloud DatasetWnp::GetPointcloud()
{
  return Pointcloud();
}
}
