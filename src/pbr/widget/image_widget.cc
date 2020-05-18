#include "pbr/widget/image_widget.h"

namespace pbr
{
ImageWidget::ImageWidget()
{
}

ImageWidget::ImageWidget(int width, int height)
  : Widget(width, height)
{
}

ImageWidget::ImageWidget(int x, int y, int width, int height)
  : Widget(x, y, width, height)
{
}

ImageWidget::~ImageWidget()
{
}

void ImageWidget::UpdateImage(std::shared_ptr<pbr::Image> image)
{
  image_ = image;
  need_update_ = true;
}

void ImageWidget::FinishedUpdate()
{
  need_update_ = false;
}
}
