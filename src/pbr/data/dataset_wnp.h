#ifndef PBR_DATA_DATASET_WNP_H_
#define PBR_DATA_DATASET_WNP_H_

#include "pbr/data/dataset.h"

namespace pbr
{
class DatasetWnp : public Dataset
{
public:
  DatasetWnp() = delete;

  explicit DatasetWnp(const std::string& directory);

  ~DatasetWnp() override;

  int ColorWidth() const noexcept override { return 1920; }
  int ColorHeight() const noexcept override { return 1080; }
  int DepthWidth() const noexcept override { return 512; }
  int DepthHeight() const noexcept override { return 424; }

  int NumSequences() override;
  int NumFrames() override;

  void SequenceChanged() override;

  std::shared_ptr<Image> GetColorImage() override;
  std::shared_ptr<Image> GetDepthImage() override;
  Pointcloud GetPointcloud() override;

  std::shared_ptr<Image> LoadColorImage(int frame);
  std::shared_ptr<Image> LoadDepthImage(int frame);

private:
  void LoadCurrentSequenceImages();

  std::string directory_;

  std::vector<std::string> sequences_;
  std::vector<std::vector<std::string>> frames_;

  std::vector<std::shared_ptr<Image>> color_images_;
  std::vector<std::shared_ptr<Image>> depth_images_;
};
}

#endif // PBR_DATA_DATASET_WNP_H_
