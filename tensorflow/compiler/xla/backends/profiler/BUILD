load("//tensorflow/tsl:tsl.bzl", "if_with_tpu_support", "tsl_gpu_library")

# copybara:uncomment package(default_applicable_licenses = ["//tensorflow:license"])

package_group(
    name = "friends",
    packages = [
        "//tensorflow/compiler/xla/backends/profiler/cpu",
        "//tensorflow/compiler/xla/backends/profiler/gpu",
        "//tensorflow/compiler/xla/backends/profiler/tpu",
    ],
)

tsl_gpu_library(
    name = "profiler_backends",
    cuda_deps = [
        "//tensorflow/compiler/xla/backends/profiler/gpu:device_tracer",
    ],
    visibility = ["//tensorflow/compiler/xla:internal"],
    deps = [
        "//tensorflow/compiler/xla/backends/profiler/cpu:host_tracer",
        "//tensorflow/compiler/xla/backends/profiler/cpu:metadata_collector",
    ] + if_with_tpu_support(
        [
            "//tensorflow/compiler/xla/backends/profiler/tpu:tpu_tracer",
        ],
    ),
    alwayslink = True,
)
