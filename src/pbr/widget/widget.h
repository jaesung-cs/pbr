#ifndef PBR_WIDGET_WIDGET_H_
#define PBR_WIDGET_WIDGET_H_

#include <memory>
#include <vector>

namespace pbr
{
class Widget
{
public:
  Widget();
  Widget(int width, int height);
  Widget(int x, int y, int width, int height);

  virtual ~Widget();

  virtual bool IsImage() const noexcept { return false; }
  virtual bool IsScene() const noexcept { return false; }

  void SetParent(std::shared_ptr<Widget> parent) { parent_ = parent; }
  void AddChild(std::shared_ptr<Widget> child) { children_.push_back(child); }

  const auto& Children() const { return children_; }

  auto& X() noexcept { return x_; }
  auto& Y() noexcept { return y_; }
  auto& Width() noexcept { return width_; }
  auto& Height() noexcept { return height_; }

  auto X() const noexcept { return x_; }
  auto Y() const noexcept { return y_; }
  auto Width() const noexcept { return width_; }
  auto Height() const noexcept { return height_; }

  void Resize(int width, int height) noexcept
  {
    width_ = width;
    height_ = height;
  }

private:
  std::weak_ptr<Widget> parent_;
  std::vector<std::shared_ptr<Widget>> children_;

  int x_ = 0;
  int y_ = 0;
  int width_ = 100;
  int height_ = 100;
};
}

#endif // PBR_WIDGET_WIDGET_H_
