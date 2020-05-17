#include "pbr/widget/widget.h"

namespace pbr
{
Widget::Widget()
{
}

Widget::Widget(int width, int height)
  : width_(width), height_(height)
{
}

Widget::Widget(int x, int y, int width, int height)
  : x_(x), y_(y), width_(width), height_(height)
{
}

Widget::~Widget()
{
}
}
