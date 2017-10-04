/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserve.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#include "paddle/operators/strided_memcpy.h"
#include "gtest/gtest.h"
#include "paddle/memory/memory.h"

namespace paddle {
namespace operators {

TEST(StridedMemcpy, CPUCrop) {
  // clang-format off
  int src[] = {
      0, 1, 2, 0, 0,
      0, 3, 4, 0, 0,
      0, 0, 0, 0, 0,
  };
  // clang-format on

  framework::DDim src_stride({5, 1});

  int dst[4];
  framework::DDim dst_dim({2, 2});
  framework::DDim dst_stride({2, 1});

  platform::CPUDeviceContext ctx;
  StridedMemcpy<int>(ctx, src + 1, src_stride, dst_dim, dst_stride, dst);

  ASSERT_EQ(1, dst[0]);
  ASSERT_EQ(2, dst[1]);
  ASSERT_EQ(3, dst[2]);
  ASSERT_EQ(4, dst[3]);
}

TEST(StridedMemcpy, CPUConcat) {
  // clang-format off
  int src[] = {
      1, 2,
      3, 4
  };
  // clang-format on

  int dst[8];

  framework::DDim src_stride({2, 1});
  framework::DDim dst_dim({2, 2});
  framework::DDim dst_stride({4, 1});
  platform::CPUDeviceContext ctx;

  StridedMemcpy<int>(ctx, src, src_stride, dst_dim, dst_stride, dst);
  StridedMemcpy<int>(ctx, src, src_stride, dst_dim, dst_stride, dst + 2);

  // clang-format off
  int expect_dst[] = {
      1, 2, 1, 2,
      3, 4, 3, 4
  };
  // clang-format on
  for (size_t i = 0; i < sizeof(expect_dst) / sizeof(int); ++i) {
    ASSERT_EQ(expect_dst[i], dst[i]);
  }
}

#ifdef PADDLE_WITH_GPU
TEST(StridedMemcpy, GPUCrop) {
  // clang-format off
  int src[] = {
      0, 1, 2, 0, 0,
      0, 3, 4, 0, 0,
      0, 0, 0, 0, 0,
  };
  // clang-format on

  platform::GPUPlace gpu0(0);
  platform::CPUPlace cpu;

  int* gpu_src = reinterpret_cast<int*>(memory::Alloc(gpu0, sizeof(src)));
  memory::Copy(gpu0, gpu_src, cpu, src, sizeof(src));

  framework::DDim src_stride({5, 1});

  int dst[4];
  int* gpu_dst = reinterpret_cast<int*>(memory::Alloc(gpu0, sizeof(dst)));

  framework::DDim dst_dim({2, 2});
  framework::DDim dst_stride({2, 1});

  platform::CUDADeviceContext ctx(gpu0);
  StridedMemcpy<int>(ctx, gpu_src + 1, src_stride, dst_dim, dst_stride,
                     gpu_dst);

  memory::Copy(cpu, dst, gpu0, gpu_dst, sizeof(dst), ctx.stream());
  ctx.Wait();

  ASSERT_EQ(1, dst[0]);
  ASSERT_EQ(2, dst[1]);
  ASSERT_EQ(3, dst[2]);
  ASSERT_EQ(4, dst[3]);

  memory::Free(gpu0, gpu_dst);
  memory::Free(gpu0, gpu_src);
}

TEST(StridedMemcpy, GPUConcat) {
  // clang-format off
  int src[] = {
      1, 2,
      3, 4
  };
  // clang-format on

  platform::GPUPlace gpu0(0);
  platform::CPUPlace cpu;

  int* gpu_src = reinterpret_cast<int*>(memory::Alloc(gpu0, sizeof(src)));
  memory::Copy(gpu0, gpu_src, cpu, src, sizeof(src));

  int dst[8];
  int* gpu_dst = reinterpret_cast<int*>(memory::Alloc(gpu0, sizeof(dst)));

  framework::DDim src_stride({2, 1});
  framework::DDim dst_dim({2, 2});
  framework::DDim dst_stride({4, 1});
  platform::CUDADeviceContext ctx(gpu0);

  StridedMemcpy<int>(ctx, gpu_src, src_stride, dst_dim, dst_stride, gpu_dst);
  StridedMemcpy<int>(ctx, gpu_src, src_stride, dst_dim, dst_stride,
                     gpu_dst + 2);

  memory::Copy(cpu, dst, gpu0, gpu_dst, sizeof(dst), ctx.stream());
  ctx.Wait();

  // clang-format off
  int expect_dst[] = {
      1, 2, 1, 2,
      3, 4, 3, 4
  };
  // clang-format on
  for (size_t i = 0; i < sizeof(expect_dst) / sizeof(int); ++i) {
    ASSERT_EQ(expect_dst[i], dst[i]);
  }

  memory::Free(gpu0, gpu_dst);
  memory::Free(gpu0, gpu_src);
}

#endif
}  // namespace operators
}  // namespace paddle