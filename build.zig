const std = @import("std");

pub fn build(b: *std.Build) void {
    const target: std.Build.ResolvedTarget = b.standardTargetOptions(.{});
    const optimize: std.builtin.OptimizeMode = b.standardOptimizeOption(.{});

    const libgff = b.dependency("libgff", .{
        .target = target,
        .optimize = optimize,
    });

    const libsoloscuro = b.addSharedLibrary(.{
        .name = "soloscuro",
        .target = target,
        .optimize = optimize,
        .version = .{.major = 0, .minor = 6, .patch = 0},
    });

    libsoloscuro.addIncludePath(b.path("include"));

    libsoloscuro.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &.{
            "alignment.c",
            "class.c",
            "entity.c",
            "race.c",
            "stats.c",
        },
    });

    libsoloscuro.addIncludePath(b.path("inc"));
    libsoloscuro.addIncludePath(b.path("ext/libgff/include"));
    //libsoloscuro.linkLibrary(libxmi2mid);
    libsoloscuro.linkLibrary(libgff.artifact("gff"));
    //libsoloscuro.linkLibrary(libgff.artifact("libxmi2mid"));

    b.installArtifact(libsoloscuro);
}
