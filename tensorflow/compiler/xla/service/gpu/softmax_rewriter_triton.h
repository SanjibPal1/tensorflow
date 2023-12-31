/* Copyright 2023 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_COMPILER_XLA_SERVICE_GPU_SOFTMAX_REWRITER_TRITON_H_
#define TENSORFLOW_COMPILER_XLA_SERVICE_GPU_SOFTMAX_REWRITER_TRITON_H_

#include <vector>

#include "absl/container/flat_hash_set.h"
#include "absl/strings/string_view.h"
#include "tensorflow/compiler/xla/hlo/ir/hlo_instruction.h"
#include "tensorflow/compiler/xla/hlo/ir/hlo_module.h"
#include "tensorflow/compiler/xla/service/gpu/gpu_types.h"
#include "tensorflow/compiler/xla/service/hlo_pass_interface.h"
#include "tensorflow/compiler/xla/status.h"
#include "tensorflow/compiler/xla/statusor.h"

namespace xla {
namespace gpu {

struct DiamondChainDescriptor {
  HloInstruction* root = nullptr;
  HloInstruction* producer = nullptr;
};

// Rewrite compatible Softmax into a custom fusion region to be code-generated
// with the Triton-based Softmax emitter.
class SoftmaxRewriterTriton : public HloModulePass {
 public:
  explicit SoftmaxRewriterTriton(GpuVersion gpu_version)
      : gpu_version_(gpu_version) {}
  absl::string_view name() const override { return "triton-softmax-rewriter"; }

  using HloPassInterface::Run;
  StatusOr<bool> Run(
      HloModule* module,
      const absl::flat_hash_set<absl::string_view>& execution_threads) override;

  // Finds and returns all the fusible diamond chains in the module. The
  // resulting vector is sorted according to a post-order matching (i.e. within
  // the same computation, producer diamonds appear before consumer diamonds).
  std::vector<DiamondChainDescriptor> FindAllFusibleDiamondChains(
      HloModule& module,
      const absl::flat_hash_set<absl::string_view>& execution_threads) const;

  // Constructs a Softmax fusion containing all the instructions between the
  // root and the producer of a diamond chain. The producer is excluded from the
  // fusion.
  Status FuseDiamondChain(const DiamondChainDescriptor& diamond_chain);

 private:
  GpuVersion gpu_version_;
};

}  // namespace gpu
}  // namespace xla

#endif  // TENSORFLOW_COMPILER_XLA_SERVICE_GPU_SOFTMAX_REWRITER_TRITON_H_
