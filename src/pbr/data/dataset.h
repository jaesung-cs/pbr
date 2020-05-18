#ifndef PBR_DATA_DATASET_H_
#define PBR_DATA_DATASET_H_

#include "pbr/data/pointcloud.h"
#include "pbr/data/image.h"

namespace pbr
{
class Dataset
{
public:
  Dataset();
  virtual ~Dataset();

  virtual int ColorWidth() const noexcept { return 0; }
  virtual int ColorHeight() const noexcept { return 0; }
  virtual int DepthWidth() const noexcept { return 0; }
  virtual int DepthHeight() const noexcept { return 0; }

  virtual int NumSequences() const { return 0; }
  virtual int NumFrames() const { return 0; }
  virtual double FramesPerSecond() const { return 30.; }

  virtual std::shared_ptr<Image> GetColorImage();
  virtual std::shared_ptr<Image> GetDepthImage();
  virtual Pointcloud GetPointcloud();

  auto Sequence() const noexcept { return sequence_idx_; }
  auto Frame() const noexcept { return frame_idx_; }

  void SetTime(double time);

  void PreviousSequence();
  void NextSequence();
  void PreviousFrame();
  void NextFrame();
  void SelectFrame(int frame);
  void Rewind();

  virtual void SequenceChanged() {}

private:
  int sequence_idx_ = 0;
  int frame_idx_ = 0;
};
}

#endif // PBR_DATA_DATASET_H_
