#include "pbr/sensor/kinect2.h"

#include <iostream>

namespace pbr
{
Kinect2::Kinect2()
  : ColorSensor(), DepthSensor()
{
  rotation_ << 0.999971346110444, -0.00756469826720314, 0.000286876615262462,
    0.00756522677983534, 0.999969600410407, -0.00188827932806725,
    -0.000272583630970059, 0.00189039550817223, 0.999998176049830;
  //rotation_.setIdentity();
}

Kinect2::~Kinect2() = default;

void Kinect2::Open()
{
  if (FAILED(GetDefaultKinectSensor(&sensor_)))
  {
    return;
  }

  if (sensor_)
  {
    sensor_->get_CoordinateMapper(&mapper_);

    sensor_->Open();
    sensor_->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color, &reader_);
    if (reader_)
      opened_ = true;
  }
}

bool Kinect2::Update(Image& color_image, Image& depth_image)
{
  if (!opened_)
    Open();
  if (!opened_)
    return false;

  IMultiSourceFrame* frame = NULL;
  if (SUCCEEDED(reader_->AcquireLatestFrame(&frame)))
  {
    IColorFrame* color_frame = NULL;
    IColorFrameReference* color_frameref = NULL;
    frame->get_ColorFrameReference(&color_frameref);
    color_frameref->AcquireFrame(&color_frame);
    if (color_frameref)
      color_frameref->Release();
    if (color_frame)
      color_frame->CopyConvertedFrameDataToArray(ColorWidth() * ColorHeight() * 4, color_image.Data<unsigned char>(), ColorImageFormat_Rgba);

    IDepthFrame* depth_frame = NULL;
    IDepthFrameReference* depth_frameref = NULL;
    frame->get_DepthFrameReference(&depth_frameref);
    depth_frameref->AcquireFrame(&depth_frame);
    if (depth_frameref)
      depth_frameref->Release();
    if (depth_frame)
      depth_frame->CopyFrameDataToArray(DepthWidth() * DepthHeight(), depth_image.Data<unsigned short>());

    if (color_frame)
      color_frame->Release();
    if (depth_frame)
      depth_frame->Release();

    if (frame)
      frame->Release();
    return true;
  }

  return false;
}

bool Kinect2::Update(Image& color_image, Image& depth_image, Pointcloud& pointcloud)
{
  if (!opened_)
    Open();
  if (!opened_)
    return false;
   
  IMultiSourceFrame* frame = NULL;
  if (SUCCEEDED(reader_->AcquireLatestFrame(&frame)))
  {
    IColorFrame* color_frame = NULL;
    IColorFrameReference* color_frameref = NULL;
    frame->get_ColorFrameReference(&color_frameref);
    color_frameref->AcquireFrame(&color_frame);
    if (color_frameref)
      color_frameref->Release();
    if (color_frame)
      color_frame->CopyConvertedFrameDataToArray(ColorWidth() * ColorHeight() * 4, color_image.Data<unsigned char>(), ColorImageFormat_Rgba);

    IDepthFrame* depth_frame = NULL;
    IDepthFrameReference* depth_frameref = NULL;
    frame->get_DepthFrameReference(&depth_frameref);
    depth_frameref->AcquireFrame(&depth_frame);
    if (depth_frameref)
      depth_frameref->Release();
    if (depth_frame)
      depth_frame->CopyFrameDataToArray(DepthWidth() * DepthHeight(), depth_image.Data<unsigned short>());

    if (color_frame && depth_frame)
    {
      // Coordinate mapping from underlying buffer
      /*
      unsigned int sz;
      unsigned short* buf;
      depth_frame->AccessUnderlyingBuffer(&sz, &buf);

      mapper_->MapDepthFrameToCameraSpace(
        sz, buf,        // Depth frame data and size of depth frame
        sz, depth_to_xyz_); // Output CameraSpacePoint array and size
      mapper_->MapDepthFrameToColorSpace(
        sz, buf,        // Depth frame data and size of depth frame
        sz, depth_to_rgb_); // Output ColorSpacePoint array and size3
      */

      // Coordinate mapping from depth image buffer
      const int num_points = DepthWidth() * DepthHeight();
      mapper_->MapDepthFrameToCameraSpace(num_points, depth_image.Data<unsigned short>(), num_points, depth_to_xyz_);

      pointcloud.Resize(num_points);

      // Position
      for (int i = 0; i < num_points; i++)
      {
        if (depth_to_xyz_[i].X == -std::numeric_limits<float>::infinity() ||
          depth_to_xyz_[i].Y == -std::numeric_limits<float>::infinity() ||
          depth_to_xyz_[i].Z == -std::numeric_limits<float>::infinity())
        {
          pointcloud.Position(i)[0] = 0.f;
          pointcloud.Position(i)[1] = 0.f;
          pointcloud.Position(i)[2] = 0.f;
        }
        else
        {
          pointcloud.Position(i)[0] = depth_to_xyz_[i].X;
          pointcloud.Position(i)[1] = depth_to_xyz_[i].Y;
          pointcloud.Position(i)[2] = depth_to_xyz_[i].Z;
        }
      }

      mapper_->MapCameraPointsToColorSpace(num_points, depth_to_xyz_, num_points, depth_to_rgb_);

      // Color
      for (int i = 0; i < num_points; i++)
      {
        const auto& color_space = depth_to_rgb_[i];

        auto px = static_cast<int>(color_space.X + 0.5f);
        auto py = static_cast<int>(color_space.Y + 0.5f);

        // Check if color pixel coordinates are in bounds
        if (px < 0 || py < 0 || px >= ColorWidth() || py >= ColorHeight())
        {
          pointcloud.Color(i)[0] = 0.f;
          pointcloud.Color(i)[1] = 0.f;
          pointcloud.Color(i)[2] = 0.f;
        }
        else
        {
          int idx = px + ColorWidth() * py;
          pointcloud.Color(i)[0] = color_image.Data<unsigned char>()[4 * idx + 0] / 255.f;
          pointcloud.Color(i)[1] = color_image.Data<unsigned char>()[4 * idx + 1] / 255.f;
          pointcloud.Color(i)[2] = color_image.Data<unsigned char>()[4 * idx + 2] / 255.f;
        }
      }
    }
    if (color_frame)
      color_frame->Release();
    if (depth_frame)
      depth_frame->Release();

    if (frame)
      frame->Release();
    return true;
  }

  return false;
}

bool Kinect2::GeneratePointcloudWithCoordinateMapper(const Image& color_image, const Image& depth_image, Pointcloud& pointcloud)
{
  if (!opened_)
    Open();
  if (!opened_)
    return false;

  const int num_points = DepthWidth() * DepthHeight();

  mapper_->MapDepthFrameToCameraSpace(num_points, depth_image.Data<unsigned short>(), num_points, depth_to_xyz_);

  pointcloud.Resize(num_points);

  // Position
  for (int i = 0; i < num_points; i++)
  {
    if (depth_to_xyz_[i].X == -std::numeric_limits<float>::infinity() ||
      depth_to_xyz_[i].Y == -std::numeric_limits<float>::infinity() ||
      depth_to_xyz_[i].Z == -std::numeric_limits<float>::infinity())
    {
      pointcloud.Position(i)[0] = 0.f;
      pointcloud.Position(i)[1] = 0.f;
      pointcloud.Position(i)[2] = 0.f;
    }
    else
    {
      pointcloud.Position(i)[0] = depth_to_xyz_[i].X;
      pointcloud.Position(i)[1] = depth_to_xyz_[i].Y;
      pointcloud.Position(i)[2] = depth_to_xyz_[i].Z;
    }
  }


  mapper_->MapCameraPointsToColorSpace(num_points, depth_to_xyz_, num_points, depth_to_rgb_);

  // Color
  auto num_components = color_image.NumComponents();
  for (int i = 0; i < num_points; i++)
  {
    const auto& color_space = depth_to_rgb_[i];

    auto px = static_cast<int>(color_space.X + 0.5f);
    auto py = static_cast<int>(color_space.Y + 0.5f);

    // Check if color pixel coordinates are in bounds
    if (px < 0 || py < 0 || px >= ColorWidth() || py >= ColorHeight())
    {
      pointcloud.Color(i)[0] = 0.f;
      pointcloud.Color(i)[1] = 0.f;
      pointcloud.Color(i)[2] = 0.f;
    }
    else
    {
      int idx = px + ColorWidth() * py;
      pointcloud.Color(i)[0] = color_image.Data<unsigned char>()[num_components * idx + 0] / 255.f;
      pointcloud.Color(i)[1] = color_image.Data<unsigned char>()[num_components * idx + 1] / 255.f;
      pointcloud.Color(i)[2] = color_image.Data<unsigned char>()[num_components * idx + 2] / 255.f;
    }
  }

  /*
  // Color
  auto num_components = color_image.NumComponents();
  for (int i = 0; i < num_points; i++)
  {
    auto p = depth_to_rgb_[i];

    // Check if color pixel coordinates are in bounds
    if (p.X < 0 || p.Y < 0 || p.X >= ColorWidth() || p.Y >= ColorHeight())
    {
      pointcloud.Color(i)[0] = 0.f;
      pointcloud.Color(i)[1] = 0.f;
      pointcloud.Color(i)[2] = 0.f;
    }
    else
    {
      int idx = (int)p.X + ColorWidth() * (int)p.Y;
      pointcloud.Color(i)[0] = color_image.Data<unsigned char>()[num_components * idx + 0] / 255.f;
      pointcloud.Color(i)[1] = color_image.Data<unsigned char>()[num_components * idx + 1] / 255.f;
      pointcloud.Color(i)[2] = color_image.Data<unsigned char>()[num_components * idx + 2] / 255.f;
    }
  }
  */

  return true;
}

bool Kinect2::GeneratePointcloudWithCameraParameters(const Image& color_image, const Image& depth_image, Pointcloud& pointcloud)
{
  const int num_points = DepthWidth() * DepthHeight();

  pointcloud.Resize(num_points);

  auto num_components = color_image.NumComponents();

  for (int y = 0; y < DepthHeight(); y++)
  {
    for (int x = 0; x < DepthWidth(); x++)
    {
      int idx = x + y * DepthWidth();
      auto depth = static_cast<float>(depth_image.Data<unsigned short>()[idx]);

      // World space
      Vector3f p;
      p(0) = (x + 1 - depth_param_.Cx()) * depth_image.Data<unsigned short>()[idx] / depth_param_.Fx();
      p(1) = (y + 1 - depth_param_.Cy()) * depth_image.Data<unsigned short>()[idx] / depth_param_.Fy();
      p(2) = depth_image.Data<unsigned short>()[idx];

      p /= 1000.f;

      pointcloud.Position(idx)[0] = p(0);
      pointcloud.Position(idx)[1] = p(1);
      pointcloud.Position(idx)[2] = p(2);

      // Color space
      p = rotation_ * p + translation_ / 1000.f;

      // Color plane
      Vector2i c;
      c(0) = p(0) * color_param_.Fx() / p(2) + color_param_.Cx();
      c(1) = p(1) * color_param_.Fy() / p(2) + color_param_.Cy();

      // Check if color pixel coordinates are in bounds
      if (c(0) < 0 || c(1) < 0 || c(0) >= ColorWidth() || c(1) >= ColorHeight())
      {
        pointcloud.Color(idx)[0] = 0.f;
        pointcloud.Color(idx)[1] = 0.f;
        pointcloud.Color(idx)[2] = 0.f;
      }
      else
      {
        int color_idx = c(0) + ColorWidth() * c(1);
        pointcloud.Color(idx)[0] = color_image.Data<unsigned char>()[num_components * color_idx + 0] / 255.f;
        pointcloud.Color(idx)[1] = color_image.Data<unsigned char>()[num_components * color_idx + 1] / 255.f;
        pointcloud.Color(idx)[2] = color_image.Data<unsigned char>()[num_components * color_idx + 2] / 255.f;
      }
    }
  }

  return true;
}

bool Kinect2::UpdateColorImage(Image& image)
{
  if (!opened_)
    Open();
  if (!opened_)
    return false;

  return false;

  /*
  IColorFrame* colorframe;
  IColorFrameReference* frameref = NULL;
  frame->get_ColorFrameReference(&frameref);

  bool succeed = false;
  IColorFrame* frame = NULL;
  if (SUCCEEDED(color_reader_->AcquireLatestFrame(&frame)))
  {
    frame->CopyFrameDataToArray(ColorWidth() * ColorHeight(), image.Data<unsigned char>());
    succeed = true;
  }
  if (frame)
    frame->Release();

  bool succeed = false;
  return succeed;
    */
}

bool Kinect2::UpdateDepthImage(Image& image)
{
  if (!opened_)
    Open();
  if (!opened_)
    return false;

  return false;

  /*
  IDepthFrame* depthframe;
  IDepthFrameReference* frameref = NULL;
  frame->get_DepthFrameReference(&frameref);

  bool succeed = false;
  IDepthFrame* frame = NULL;
  if (SUCCEEDED(depth_reader_->AcquireLatestFrame(&frame)))
  {
    frame->CopyFrameDataToArray(DepthWidth() * DepthHeight(), image.Data<unsigned short>());
    succeed = true;
  }
  if (frame)
    frame->Release();

  return succeed;
  */
}
}
