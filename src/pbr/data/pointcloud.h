#ifndef PBR_DATA_POINTCLOUD_H_
#define PBR_DATA_POINTCLOUD_H_

#include <vector>

namespace pbr
{
class Pointcloud
{
private:
  class Accessor
  {
  public:
    Accessor() = delete;

    explicit Accessor(float* arr)
      : array_(arr)
    {
    }

    float& operator [] (int i)
    {
      return array_[i];
    }

    const float operator [] (int i) const
    {
      return array_[i];
    }

  private:
    float* const array_;
  };

public:
  Pointcloud();
  ~Pointcloud();

  auto NumPoints() const { return num_points_; }

  void Resize(int num_points);

  auto Position(int i)
  {
    return Accessor(&buffer_[6 * i]);
  }

  auto Color(int i)
  {
    return Accessor(&buffer_[6 * i + 3]);
  }

  const auto& Buffer() const { return buffer_; }

private:
  int num_points_ = 0;
  std::vector<float> buffer_;
};
}

#endif // PBR_DATA_POINTCLOUD_H_
