#ifndef PBR_WIDGET_IMAGE_WIDGET_H_
#define PBR_WIDGET_IMAGE_WIDGET_H_

#include "pbr/widget/widget.h"

#include "pbr/data/image.h"

namespace pbr
{
class ImageWidget : public Widget
{
public:
  ImageWidget();
  ImageWidget(int width, int height);
  ImageWidget(int x, int y, int width, int height);

  ~ImageWidget() override;

  bool IsImage() const noexcept override { return true; }

  void UpdateImage(std::shared_ptr<Image> texture);

  auto Image() const { return image_; }

  void NeedUpdate() { need_update_ = true; }
  bool ShouldUpdate() const noexcept { return need_update_; }
  void FinishedUpdate();

private:
  std::shared_ptr<pbr::Image> image_;
  bool need_update_ = false;
};
}

#endif // PBR_WIDGET_IMAGE_WIDGET_H_
