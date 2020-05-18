#include "pbr/data/dataset.h"

namespace pbr
{
Dataset::Dataset() = default;

Dataset::~Dataset() = default;

std::shared_ptr<Image> Dataset::GetColorImage()
{
  return std::make_shared<Image>();
}

std::shared_ptr<Image> Dataset::GetDepthImage()
{
  return std::make_shared<Image>(1, 1, 1, (unsigned short)0);
}

Pointcloud Dataset::GetPointcloud()
{
  return Pointcloud();
}

void Dataset::SetTime(double time)
{
  int frame = static_cast<int>(time * FramesPerSecond());
  SelectFrame(frame);
}

void Dataset::PreviousSequence()
{
  if (sequence_idx_ > 0)
  {
    sequence_idx_--;
    frame_idx_ = 0;
    SequenceChanged();
  }
}

void Dataset::NextSequence()
{
  if (sequence_idx_ < NumSequences() - 1)
  {
    sequence_idx_++;
    frame_idx_ = 0;
    SequenceChanged();
  }
}

void Dataset::PreviousFrame()
{
  if (frame_idx_ > 0)
    frame_idx_--;
}

void Dataset::NextFrame()
{
  if (frame_idx_ < NumFrames() - 1)
    frame_idx_++;
}

void Dataset::SelectFrame(int frame)
{
  if (frame < 0)
    frame_idx_ = 0;

  else if (frame >= NumFrames())
    frame_idx_ = NumFrames() - 1;

  else
    frame_idx_ = frame;
}

void Dataset::Rewind()
{
  frame_idx_ = 0;
}
}
