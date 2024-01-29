const std = @import("std");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    const optimize = b.standardOptimizeOption(.{});

    const dll = b.addSharedLibrary(.{
        .name = "lightwm_dll",
        .root_source_file = .{ .path = "wm_dll.c" },
        .target = target,
        .optimize = optimize,
    });

    dll.linkLibC();
    dll.force_pic = true;
    b.installArtifact(dll);

    const exe = b.addExecutable(.{
        .name = "lightwm",
        .root_source_file = .{ .path = "wm.c" },
        .target = target,
        .optimize = optimize,
    });

    exe.addCSourceFiles(&.{
        "tiling.c",
        "error.c",
    }, &.{
        "-Wall",
        "-W",
        "-Wstrict-prototypes",
        "-Wwrite-strings",
        "-Wno-missing-field-initializers",
    });
    exe.linkLibC();
    b.installArtifact(exe);
}
